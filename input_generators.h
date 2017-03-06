#ifndef input_generators_h
#define input_generators_h

#include <string>
#include <vector>

#include "sorting.h"
#include "compare.h"

using namespace std;

template <class ValueType>
class InputGenerator {
 public:
  virtual vector<ValueType> operator()(size_t size) const = 0;
};

template <class ValueType>
class RandomInputGenerator : public InputGenerator<ValueType> {
  static ValueType ElementGenerator(size_t range);
 public:
  RandomInputGenerator() {}
  vector<ValueType> operator()(size_t size) const override;
};

template <class ValueType>
class ReversedInputGenerator : public InputGenerator<ValueType> {
 public:
  ReversedInputGenerator() {}
  vector<ValueType> operator()(size_t size) const override;
};

template <class ValueType>
class AlmostSortedInputGenerator : public InputGenerator<ValueType> {
 public:
  AlmostSortedInputGenerator() {}
  vector<ValueType> operator()(size_t size) const override;
};

template <class ValueType>
class SampleInputGenerator : public InputGenerator<ValueType> {
 private:
  vector<ValueType> data_;
 public:
  SampleInputGenerator(const vector<ValueType>& data) : data_{data} {}
  vector<ValueType> operator()(size_t size) const override;                     
};

template <class ValueType>
vector<ValueType> SampleInputGenerator<ValueType>::operator()(size_t size) const {
  vector<ValueType> input = data_;
  input.resize(size);
  return input;
}

template <class ValueType>
vector<ValueType> AlmostSortedInputGenerator<ValueType>::operator()(size_t size) const {
  RandomInputGenerator<ValueType> random_generator;
  vector<ValueType> input = random_generator(size);
  HeapSort<ValueType> heap_sort;
  Compare<ValueType> comparator;
  heap_sort(input, comparator);
  for (int i = 0; i < 2 + rand() % 3; ++i) {
    size_t pos_1 = rand() % size, pos_2 = rand() % size;
    swap(input[pos_1], input[pos_2]);
  }
  return input;
}

template <class ValueType>
vector<ValueType> ReversedInputGenerator<ValueType>::operator()(size_t size) const {
  RandomInputGenerator<ValueType> random_generator;
  vector<ValueType> input = random_generator(size);
  Reversed<ValueType> reversed_comparator;
  HeapSort<ValueType> heap_sort;
  heap_sort(input, reversed_comparator);
  return input;
}

template <class ValueType>
vector<ValueType> RandomInputGenerator<ValueType>::operator()(size_t size) const {
  vector<ValueType> array(size);
  for (auto& elem : array) {
    elem = ElementGenerator(size * 2);
  }
  return array;
}

template <>
int RandomInputGenerator<int>::ElementGenerator(size_t range) {
  return rand() % range;
}

template <>
string RandomInputGenerator<string>::ElementGenerator(size_t range) {
  size_t lenght = 5 + rand() % (range / 4);
  string element = "";
  for (size_t pos = 0; pos < lenght; ++pos) {
    char symbol = 'a' + rand() % ('z' - 'a' + 1);
    element += symbol;
  }
  return element;
}

#endif /* input_generators_h */
