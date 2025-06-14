#pragma once

#include <arpa/inet.h>   // inet_ntoa() to print ips
#include <netinet/in.h>  // sockaddr_in
#include <sys/socket.h>  // socket(), bind(), listen(), accept()
#include <unistd.h>      // close()

#include <atomic>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

#include "../async/ThreadPool.hpp"
#include "Command.hpp"
#include "TcpClient.hpp"  // Will implement after

class TcpGameServer {
 public:
  using CommandHandler = std::function<void(TcpClient&, const Command&)>;

  TcpGameServer(ThreadPool& pool) : _pool(pool) {}

  ~TcpGameServer() {
    stop();
  }

  void listen(int port, std::function<void()> onListen = {}) {
    _serverFd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd == -1) {
      perror("Socket failed");
      return;
    }

    int opt = 1;
    ::setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (::bind(_serverFd, (sockaddr*)&address, sizeof(address)) < 0) {
      perror("Bind failed");
      return;
    }

    if (::listen(_serverFd, SOMAXCONN) < 0) {
      perror("Listen failed");
      return;
    }

    if (onListen) onListen();

    _acceptThread = std::thread([this] { acceptLoop(); });
  }

  void route(const std::string& cmdPath, CommandHandler handler) {
    _routes[cmdPath] = std::move(handler);
  }

  void stop() {
    if (_serverFd != -1) {
      _shutdown.store(true);
      ::shutdown(_serverFd, SHUT_RDWR);
      ::close(_serverFd);
      _serverFd = -1;
    }
    if (_acceptThread.joinable()) _acceptThread.join();
  }

 private:
  void acceptLoop() {
    while (!_shutdown.load()) {
      sockaddr_in clientAddr{};
      socklen_t clientLen = sizeof(clientAddr);
      int clientFd = ::accept(_serverFd, (sockaddr*)&clientAddr, &clientLen);

      if (clientFd < 0) {
        if (_shutdown.load()) break;
        perror("Accept failed");
        continue;
      }

      _pool.submit([this, clientFd]() mutable {
        TcpClient client(clientFd);

        while (true) {
          auto msg = client.receiveLine();
          if (!msg.has_value()) break;

          std::string_view line = msg.value();
          if (line.empty()) continue;

          try {
            auto cmd = Command::parse(msg.value());
            auto it = _routes.find(cmd.path());
            if (it != _routes.end()) {
              it->second(client, cmd);
            } else {
              client.send("error: unknown command\n");
            }
          } catch (const std::exception& e) {
            client.send(std::string("error: ") + e.what() + "\n");
          }
        }
      });
    }
  }

  int _serverFd = -1;
  std::thread _acceptThread;
  ThreadPool& _pool;
  std::atomic<bool> _shutdown{false};

  std::unordered_map<std::string, CommandHandler> _routes;
};