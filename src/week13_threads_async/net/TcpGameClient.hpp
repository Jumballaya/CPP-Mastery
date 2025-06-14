#pragma once

#include <arpa/inet.h>   // inet_pton
#include <netinet/in.h>  // sockaddr_in
#include <sys/socket.h>  // socket, connect, send, recv
#include <unistd.h>      // close, shutdown

#include <atomic>
#include <cstring>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>

class TcpGameClient {
 public:
  TcpGameClient(std::string_view host, int port)
      : _host(host), _port(port) {}

  ~TcpGameClient() {
    disconnect();
  }

  bool connect() {
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1) {
      perror("Failed to create socket");
      return false;
    }

    sockaddr_in server{};
    std::memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(_port);
    if (inet_pton(AF_INET, _host.c_str(), &server.sin_addr) <= 0) {
      perror("Invalid Address");
      return false;
    }

    if (::connect(_fd, (sockaddr*)&server, sizeof(server)) < 0) {
      perror("Failed to connect");
      return false;
    }

    _running = true;
    _recvThread = std::thread(&TcpGameClient::_readLoop, this);
    return true;
  }

  void disconnect() {
    _running = false;
    if (_fd != -1) {
      ::shutdown(_fd, SHUT_RDWR);
      ::close(_fd);
      _fd = -1;
    }
    if (_recvThread.joinable()) _recvThread.join();
  }

  void send(std::string_view msg) {
    std::string msgWithNewline = std::string(msg) + "\n";
    ::send(_fd, msgWithNewline.data(), msgWithNewline.size(), 0);
  }

  void onMessage(std::function<void(std::string_view)> cb) {
    _onMessage = std::move(cb);
  }

 private:
  void _readLoop() {
    char buf[1024];

    while (_running.load()) {
      ssize_t n = ::recv(_fd, buf, sizeof(buf), 0);
      if (n <= 0) break;

      _buffer.append(buf, n);

      while (true) {
        auto newline = _buffer.find('\n');
        if (newline == std::string::npos) break;

        std::string line = _buffer.substr(0, newline);
        _buffer.erase(0, newline + 1);

        if (_onMessage) _onMessage(line);
      }
    }
  }

  std::string _host;
  int _port;
  int _fd = -1;

  std::thread _recvThread;
  std::atomic<bool> _running = false;

  std::function<void(std::string_view)> _onMessage;
  std::string _buffer;
};