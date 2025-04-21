#pragma once

#include "FieldRange.hpp"

template <typename TStruct>
class FieldProjection {
 public:
  FieldProjection(TStruct* base, size_t count) : _base(base), _count(count) {};

  template <typename TField>
  FieldRange<TStruct, TField> field(TField TStruct::* member) {
    return FieldRange<TStruct, TField>(_base, _count, member);
  }

 private:
  TStruct* _base = nullptr;
  size_t _count = 0;
};