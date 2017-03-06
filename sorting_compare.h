#ifndef sorting_compare_h
#define sorting_compare_h

#include <memory.h>
#include <vector>

#include "timer.h"
#include "input_generators.h"
#include "sorting.h"
#include "compare.h"

using namespace std;

template <class ValueType, class _Compare = Compare<ValueType>>
class CompareSorting {
 private:
  class CountedComparator : public _Compare {
   private:
    size_t* counter;
    
   public:
    explicit CountedComparator(size_t* where_count) : counter{where_count} {}
    bool operator()(const ValueType& fst, const ValueType& snd) const override;
  };
  
  vector<const Sort<ValueType>*> sorting_;
  
 public:
  CompareSorting(const initializer_list<const Sort<ValueType>*>& sorting)
  : sorting_(sorting) {}
  
  vector<vector<ValueType>> PureSort(const vector<ValueType>& data) const;
  vector<size_t> OperationsCount(size_t vector_size,
                                 InputGenerator<ValueType>* generator) const;
  vector<double> SecondsCount(size_t vector_size,
                              InputGenerator<ValueType>* generator) const;      
};

template <class ValueType, class _Compare>
vector<double> CompareSorting<ValueType, _Compare>::SecondsCount(
    size_t vector_size, InputGenerator<ValueType> *generator) const {
  vector<double> time_results;
  auto test_input = (*generator)(vector_size);
  
  for (auto sort_ptr : sorting_) {
    auto input_copy = test_input;
    Timer timer;
    (*sort_ptr)(input_copy);
    double time_exceed = timer.Stop();
    time_results.push_back(time_exceed);
  }
  return time_results;
}


template <class ValueType, class _Compare>
vector<size_t> CompareSorting<ValueType, _Compare>::OperationsCount(
    size_t test_size, InputGenerator<ValueType>* generator) const {
  vector<size_t> results;
  auto test_input = (*generator)(test_size);
  
  for (auto sort_ptr : sorting_) {
    auto input_copy = test_input;
    size_t counter = 0;
    CountedComparator counted_comparator(&counter);
    (*sort_ptr)(input_copy, counted_comparator);
    results.push_back(counter);
  }
  return results;
}

template <class ValueType, class _Compare>
vector<vector<ValueType>> CompareSorting<ValueType, _Compare>::PureSort(
    const vector<ValueType>& data) const {
  vector<vector<ValueType>> results;
  for (auto sort_ptr : sorting_) {
    results.push_back(data);
    (*sort_ptr)(results.back());
  }
  return results;
}

template <class ValueType, class _Compare>
bool CompareSorting<ValueType, _Compare>::CountedComparator::operator()(
    const ValueType &fst, const ValueType &snd) const {
  ++(*counter);
  return _Compare()(fst, snd);
}

#endif /* sorting_compare_h */
