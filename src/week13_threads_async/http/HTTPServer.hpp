#pragma once

#include <arpa/inet.h>   // inet_ntoa() to print ips
#include <netinet/in.h>  // sockaddr_in
#include <sys/socket.h>  // socket(), bind(), listen(), accept()
#include <unistd.h>      // close()

#include <atomic>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <thread>

#include "../async/ThreadPool.hpp"

class HTTPServer {
 public:
  HTTPServer(ThreadPool& pool) : _pool(pool) {}

  ~HTTPServer() {
    stop();
  }

  void listen(int port, std::function<void()> onListen = {}) {
    _serverFd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd == -1) {
      std::cerr << "Failed to create socket" << std::endl;
      return;
    }

    // Allow reuse of port after restarting
    int opt = 1;
    ::setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Server address
    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (::bind(_serverFd, (sockaddr*)&address, sizeof(address)) < 0) {
      perror("Bind failed");
      return;
    }

    if (::listen(_serverFd, SOMAXCONN) < 0) {
      perror("Listen failed");
      return;
    }

    if (onListen) onListen();

    _acceptThread = std::thread([this] { _accept(); });
  }

  void stop() {
    if (_serverFd != -1) {
      _shutdown.store(true);
      ::shutdown(_serverFd, SHUT_RDWR);
      ::close(_serverFd);
      _serverFd = -1;
    }
    if (_acceptThread.joinable()) {
      _acceptThread.join();
    }
  }

 private:
  void _accept() {
    while (true) {
      sockaddr_in clientaddr{};
      socklen_t clientLen = sizeof(clientaddr);
      int clientFd = ::accept(_serverFd, (sockaddr*)&clientaddr, &clientLen);
      if (clientFd < 0) {
        if (_shutdown.load()) break;
        perror("Failed to accept connection");
        continue;
      }
      _pool.submit([clientFd]() mutable {
        const char* response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 59\r\n"
            "\r\n"
            "<html><head></head><body><h1>Hello World</h1></body></html>";
        ::send(clientFd, response, std::strlen(response), 0);
        ::close(clientFd);
      });
    }
  }

  int _serverFd = -1;
  std::thread _acceptThread;
  ThreadPool& _pool;
  std::atomic<bool> _shutdown{false};
};
