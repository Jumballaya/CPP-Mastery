#pragma once

#include <arpa/inet.h>   // inet_ntoa() to print ips
#include <netinet/in.h>  // sockaddr_in
#include <sys/socket.h>  // socket(), bind(), listen(), accept()
#include <unistd.h>      // close()

#include <cstring>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

class TcpClient {
 public:
  explicit TcpClient(int fd) : _fd(fd) {}

  ~TcpClient() {
    if (_fd != -1) {
      ::close(_fd);
      _fd = -1;
    }
  }

  TcpClient(const TcpClient&) = delete;
  TcpClient& operator=(const TcpClient&) = delete;

  TcpClient(TcpClient&& other) noexcept : _fd(other._fd) {
    other._fd = -1;
  }

  TcpClient& operator=(TcpClient&& other) noexcept {
    if (this != &other) {
      if (_fd != -1) ::close(_fd);
      _fd = other._fd;
      other._fd = -1;
    }
    return *this;
  }

  void send(std::string_view msg) {
    ::send(_fd, msg.data(), msg.size(), 0);
  }

  std::optional<std::string> receiveLine() {
    while (true) {
      // Search for newline in the buffer
      auto newline = _buffer.find('\n');
      if (newline != std::string::npos) {
        std::string line = _buffer.substr(0, newline);
        _buffer.erase(0, newline + 1);  // Remove line from buffer
        return line;
      }

      // Read more data from socket
      char buf[1024];
      ssize_t n = ::recv(_fd, buf, sizeof(buf), 0);

      if (n <= 0) {
        return std::nullopt;  // Client disconnected or error
      }

      _buffer.append(buf, n);
    }
  }

 private:
  int _fd;
  std::string _buffer;
};