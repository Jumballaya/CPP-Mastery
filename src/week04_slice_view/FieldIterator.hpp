#pragma once

template <typename TStruct, typename TField>
class FieldIterator {
 public:
  FieldIterator(TStruct* ptr, TField TStruct::* fieldPtr)
      : _ptr(ptr), _fieldPtr(fieldPtr) {}

  TField& operator*() const { return _ptr->*_fieldPtr; }

  FieldIterator& operator++() {
    ++_ptr;
    return *this;
  }

  bool operator!=(const FieldIterator& other) const {
    return _ptr != other._ptr;
  }

 private:
  TStruct* _ptr = nullptr;
  TField TStruct::* _fieldPtr;
};