#pragma once

#include "FieldIterator.hpp"

template <typename TStruct, typename TField>
class FieldRange {
 public:
  FieldRange(TStruct* base, size_t count, TField TStruct::* fieldPtr)
      : _base(base), _count(count), _fieldPtr(fieldPtr) {};

  const TField& operator[](size_t index) const {
    return _base[index].*_fieldPtr;
  };

  size_t size() const {
    return _count;
  };

  FieldIterator<TStruct, TField> begin() { return FieldIterator<TStruct, TField>(_base, _fieldPtr); }
  FieldIterator<TStruct, TField> end() { return FieldIterator<TStruct, TField>(_base + _count, _fieldPtr); }

 private:
  TStruct* _base = nullptr;
  size_t _count = 0;
  TField TStruct::* _fieldPtr;
};