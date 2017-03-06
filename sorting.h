#ifndef sorting_h
#define sorting_h

#include <vector>
#include <random>

#include "compare.h"

using namespace std;

template <class ValueType>
class Sort {
 public:
  typedef Compare<ValueType> _Compare;
  static _Compare simple;
  virtual void operator()(vector<ValueType>&, _Compare& cmp = simple) const;
};

template <class ValueType>
Compare<ValueType> Sort<ValueType>::simple = Compare<ValueType>();

template <class ValueType>
class BubbleSort : public Sort<ValueType> {
 public:
  BubbleSort() {}
  typedef Compare<ValueType> _Compare;
  void operator()(vector<ValueType>&, _Compare& cmp = _Compare()) const override;
};

template <class ValueType>
class InsertSort : public Sort<ValueType> {
 public:
  InsertSort() {}
  typedef Compare<ValueType> _Compare;
  void operator()(vector<ValueType>&, _Compare& cmp = _Compare()) const override;
};

template <class ValueType>
class ShakerSort : public Sort<ValueType> {
 public:
  ShakerSort() {}
  typedef Compare<ValueType> _Compare;
  void operator()(vector<ValueType>&, _Compare& cmp = _Compare()) const override;

 private:
  static bool LeftToRight(int begin, int end, vector<ValueType>& data,
                          _Compare& cmp);
  static bool RightToLeft(int begin, int end, vector<ValueType>& data,
                          _Compare& cmp);
};

template <class ValueType>
class QuickSort : public Sort<ValueType> {
 public:
  QuickSort() {}
  typedef Compare<ValueType> _Compare;
  void operator()(vector<ValueType>&, _Compare& cmp = _Compare()) const override;
  
 private:
  static void SortSegment(size_t begin, size_t end,
                          vector<ValueType>& data, _Compare& cmp);
};

template <class ValueType>
class MergeSort : public Sort<ValueType> {
 public:
  MergeSort() {}
  typedef Compare<ValueType> _Compare;
  void operator()(vector<ValueType>&, _Compare& cmp = _Compare()) const override;

 private:
  static vector<ValueType> Merge(const vector<ValueType>& fst,
                                 const vector<ValueType>& snd,
                                 _Compare& cmp);
};

template <class ValueType>
class HeapSort : public Sort<ValueType> {
 public:
  HeapSort() {}
  typedef Compare<ValueType> _Compare;
  void operator()(vector<ValueType>&, _Compare& cmp = _Compare()) const override;

 private:
  class Negation : public Compare<ValueType> {
  private:
    _Compare& negation_;
  public:
    Negation(_Compare& negate) : negation_{negate} {}
    bool operator()(const ValueType& fst, const ValueType& snd) const override;
  };
  
  static size_t daddy(size_t node) { return (node - 1) / 2; }
  static size_t left_son(size_t node)  { return 2 * node + 1; }
  static size_t right_son(size_t node) { return 2 * node + 2; }
  
  static void SiftUp(size_t pos, vector<ValueType>& data, _Compare& cmp);
  static void SiftDown(size_t pos, size_t range, vector<ValueType>& data,
                       _Compare& cmp);
};

template <class ValueType>
vector<ValueType> MergeSort<ValueType>::Merge(const vector<ValueType> &fst,
                                              const vector<ValueType> &snd,
                                              _Compare& cmp) {
  vector<ValueType> merged;
  for (size_t pos_1 = 0, pos_2 = 0; pos_1 < fst.size() || pos_2 < snd.size();) {
    if (pos_1 == fst.size()) {
      merged.push_back(snd[pos_2]);
      ++pos_2;
    } else if (pos_2 == snd.size()) {
      merged.push_back(fst[pos_1]);
      ++pos_1;
    } else if (cmp(fst[pos_1], snd[pos_2])) {
      merged.push_back(fst[pos_1]);
      ++pos_1;
    } else {
      merged.push_back(snd[pos_2]);
      ++pos_2;
    }
  }
  return merged;
}

template <class ValueType>
void MergeSort<ValueType>::operator()(vector<ValueType> &data,
                                      _Compare& cmp) const {
  if (data.size() <= 1) {
    return;
  }
  
  vector<ValueType> left_half((data.size() + 1) / 2);
  vector<ValueType> right_half(data.size() / 2);

  for (size_t pos = 0; pos < data.size(); ++pos) {
    if (pos % 2 == 0) {
      left_half[pos / 2] = data[pos];
    } else {
      right_half[pos / 2] = data[pos];
    }
  }
  MergeSort merge_sort;
  merge_sort(left_half, cmp);
  merge_sort(right_half, cmp);
  data = Merge(left_half, right_half, cmp);
}

template <class ValueType>
void QuickSort<ValueType>::SortSegment(size_t begin, size_t end,
                                       vector<ValueType>& data, _Compare& cmp) {
  if (end <= 1 + begin) {
    return;
  }
  
  size_t separator = begin + rand() % (end - begin);
  size_t left = begin, right = end - 1;
 
  OrEqual<ValueType, _Compare> improved_cmp(&cmp);
  
  while (left < right) {
    if (!improved_cmp(data[left], data[right])) {
      swap(data[left], data[right]);
    }
    if (left == separator) {
      left = begin;
    } else if (right == separator) {
      right = end - 1;
    }
    while (left < separator && improved_cmp(data[left], data[separator])) {
      ++left;
    }
    while (separator < right && improved_cmp(data[separator], data[right])) {
      --right;
    }
  }
  SortSegment(begin, separator, data, cmp);
  SortSegment(separator + 1, end, data, cmp);
}

template <class ValueType>
void QuickSort<ValueType>::operator()(vector<ValueType> &data,
                                      _Compare& cmp) const {
  SortSegment(0, data.size(), data, cmp);
}

template <class ValueType>
bool HeapSort<ValueType>::Negation::operator()(const ValueType& fst,
                                                         const ValueType& snd) const {
  return !(negation_(fst, snd));
}

template <class ValueType>
void HeapSort<ValueType>::operator()(vector<ValueType> &data,
                                     _Compare& cmp)const {
  Negation negate_comparator(cmp);
  for (size_t pos = 0; pos < data.size(); ++pos) {
    SiftUp(pos, data, negate_comparator);
  }
  for (size_t last = data.size() - 1; last > 0; --last) {
    swap(data[0], data[last]);
    SiftDown(0, last, data, negate_comparator);
  }
}

template <class ValueType>
void HeapSort<ValueType>::SiftUp(size_t pos, vector<ValueType> &data,
                                 _Compare& cmp) {
  if (pos == 0) {
    return;
  }
  if (cmp(data[pos], data[daddy(pos)])) {
    swap(data[pos], data[daddy(pos)]);
    SiftUp(daddy(pos), data, cmp);
  }
}

template <class ValueType>
void HeapSort<ValueType>::SiftDown(size_t pos, size_t range,
                                   vector<ValueType> &data, _Compare& cmp) {
  if (left_son(pos) < range && right_son(pos) < range) {
    if (cmp(data[pos], data[left_son(pos)]) &&
        cmp(data[pos], data[right_son(pos)])) {
      return;
    }
    if (cmp(data[left_son(pos)], data[right_son(pos)])) {
      swap(data[pos], data[left_son(pos)]);
      SiftDown(left_son(pos), range, data, cmp);
    } else {
      swap(data[pos], data[right_son(pos)]);
      SiftDown(right_son(pos), range, data, cmp);
    }
    return;
  }
  
  if (left_son(pos) < range) {
    if (cmp(data[left_son(pos)], data[pos])) {
      swap(data[pos], data[left_son(pos)]);
      SiftDown(left_son(pos), range, data, cmp);
    }
    return;
  }
}

template <class ValueType>
void ShakerSort<ValueType>::operator()(vector<ValueType>& data,
                                       _Compare& cmp) const {
  bool direction = 0; // 0 -> left to right
                      // 1 -> right to left
  
  for (int left = 0, right = static_cast<int>(data.size()) - 1; left < right;) {
    bool switched = 0;
    if (direction == 0) {
      switched = LeftToRight(left, right + 1, data, cmp);
      --right;
    } else {
      switched = RightToLeft(right, left - 1, data, cmp);
      ++left;
    }
    if (switched == false) {
      return;
    }
  }
}

template <class ValueType>
bool ShakerSort<ValueType>::RightToLeft(int begin, int end,
                                        vector<ValueType>& data, _Compare& cmp) {
  bool switched = 0;
  for (int pos_1 = begin - 1, pos_2 = begin; pos_1 != end && pos_2 != end;
       --pos_1, --pos_2) {
    if (cmp(data[pos_2], data[pos_1])) {
      swap(data[pos_1], data[pos_2]);
      switched = 1;
    }
  }
  return switched;
}

template <class ValueType>
bool ShakerSort<ValueType>::LeftToRight(int begin, int end,
                                        vector<ValueType>& data, _Compare& cmp) {
  bool switched = 0;
  for (int pos_1 = begin, pos_2 = begin + 1; pos_1 != end && pos_2 != end;
       ++pos_1, ++pos_2) {
    if (cmp(data[pos_2], data[pos_1])) {
      swap(data[pos_1], data[pos_2]);
      switched = 1;
    }
  }
  return switched;
}

template <class ValueType>
void InsertSort<ValueType>::operator()(vector<ValueType>& data,
                                       _Compare& cmp) const {
  for (size_t current_pos = 0; current_pos < data.size(); ++current_pos) {
    size_t pos_min = current_pos;
    for (size_t pos = current_pos; pos < data.size(); ++pos) {
      if (cmp(data[pos], data[pos_min])) {      // <----
        pos_min = pos;
      }
    }
    swap(data[current_pos], data[pos_min]);
  }
}

template <class ValueType>
void BubbleSort<ValueType>::operator()(vector<ValueType>& data,
                                       _Compare& cmp) const {
  for (bool switched = 1; switched; ) {
    switched = 0;
    for (size_t index = 0; index + 1 < data.size(); ++index) {
      if (cmp(data[index + 1], data[index])) {
        swap(data[index], data[index + 1]);
        switched = 1;
      }
    }
  }
}

template <class ValueType>
void Sort<ValueType>::operator()(vector<ValueType>& data, _Compare& cmp) const {
  std::sort(data.begin(), data.end(), cmp);
}

#endif /* sorting_h */
