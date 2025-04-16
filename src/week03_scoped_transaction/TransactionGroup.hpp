#pragma once

#include <stdexcept>
#include <vector>

#include "Transaction.hpp"

template <typename T>
class TransactionGroup {
 public:
  TransactionGroup() = default;
  ~TransactionGroup() = default;

  TransactionGroup(const TransactionGroup&) = delete;
  TransactionGroup& operator=(const TransactionGroup&) = delete;

  TransactionGroup(TransactionGroup&& other) : _transactions(std::move(other._transactions)),
                                               _committed(other._committed) {}

  TransactionGroup& operator=(TransactionGroup&& other) {
    if (this == &other) return *this;
    _transactions = std::move(other._transactions);
    _committed = other._committed;
    return *this;
  };

  void add(T& value) {
    _transactions.push_back(value);
  }

  T& get(size_t index) {
    if (index >= _transactions.size()) {
      throw std::runtime_error("Index out of range");
    }
    return _transactions[index].edit();
  }

  void commitAll() {
    if (_committed) return;
    for (auto& tx : _transactions) {
      tx.commit();
    }
    _committed = true;
  };

  void rollbackAll() {
    for (auto& tx : _transactions) {
      tx.rollback();
    }
    _committed = false;
  };

  size_t size() const {
    return _transactions.size();
  };

  bool committed() const {
    return _committed;
  };

 private:
  std::vector<Transaction<T>> _transactions;
  bool _committed = false;
};