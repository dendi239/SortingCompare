#ifndef compare_h
#define compare_h

#include <cmath>
#include <algorithm>
#include <utility>

using namespace std;

template <class ValueType>
class Compare {
 public:
  Compare() {}
  virtual bool operator()(const ValueType& fst, const ValueType& snd) const;
};

template <class ValueType, class _Compare = Compare<ValueType>>
class OrEqual : public Compare<ValueType> {
 private:
  _Compare* base_cmp;
  
 public:
  OrEqual(_Compare* base_comparator) : base_cmp(base_comparator) {}
  bool operator() (const ValueType& fst, const ValueType& snd) const override;
};

template <class ValueType>
class Greahter : public Compare<ValueType> {
 public:
  Greahter() {}
  bool operator()(const ValueType& fst, const ValueType& snd) const override;
};

template <class ValueType>
class Reversed : public Compare<ValueType> {
 public:
  Reversed() {}
  bool operator()(const ValueType& fst, const ValueType& snd) const override;
};

class ClosureCompare : public Compare<int>  {
 private:
  int point_;

 public:
  explicit ClosureCompare(int point) : point_(point) {}
  bool operator()(const int& fst, const int& snd) const override;
};

template <class ValueType>
bool Greahter<ValueType>::operator()(const ValueType &fst,
                                    const ValueType &snd) const {
  return fst > snd;
}

template <class ValueType, class _Compare>
bool OrEqual<ValueType, _Compare>::operator()(const ValueType& fst,
                                              const ValueType& snd) const {
  _Compare comparator;
  
  if (fst == snd) {
    return true;
  }
  return (*base_cmp)(fst, snd);
}

int Abs(int x) {
  if (x >= 0) {
    return x;
  } else {
    return -x;
  }
}

bool ClosureCompare::operator()(const int &fst, const int &snd) const {
  return Abs(point_ - fst) < Abs(point_ - snd);
}

template <class ValueType>
bool Reversed<ValueType>::operator()(const ValueType &fst,
                                     const ValueType &snd) const {
  return snd < fst;
}

template <class ValueType>
bool Compare<ValueType>::operator()(const ValueType &fst,
                                    const ValueType &snd) const {
  return fst < snd;
}

#endif /* compare_h */
