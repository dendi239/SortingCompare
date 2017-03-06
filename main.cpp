#include <iostream>

#include "input_generators.h"
#include "sorting.h"
#include "sorting_compare.h"

using namespace std;

class PrimeGenerator {
 private:
  static bool TestPrime(int prime);
  
 public:
  PrimeGenerator() {}
  int operator()(int min_) const;
};

PrimeGenerator GetPrime;

template <class ValueType>
void Print(const vector<ValueType>& data);

class CompareSortingPrintTable {
 private:
  template <class ValueType>
  static void PrintColumn(string message, const vector<ValueType>& results,
                          bool alligned = 0, string step = "\t");
  template <class ValueType>
  static string ToString(ValueType element, bool alligned);
  
 public:
  CompareSortingPrintTable() {}
  void operator()(initializer_list<size_t> ranges,
                  initializer_list<const Sort<int>*>,
                  initializer_list<InputGenerator<int>*>,
                  initializer_list<string> sort_labels,
                  initializer_list<string> generator_labes) const;
};

CompareSortingPrintTable PrintTable;

void SomeFunction(Sort<int>& sorting);

int main() {
  srand(12345);
  
  ShakerSort<int> shaker_sort;
  BubbleSort<int> bubble_sort;
  InsertSort<int> insert_sort;
  HeapSort<int> heap_sort;
  QuickSort<int> quick_sort;
  MergeSort<int> merge_sort;
  Sort<int> stl_sort;
  
  SomeFunction(shaker_sort);
  
  ReversedInputGenerator<int> reversed_generator;
  RandomInputGenerator<int> random_generator;
  AlmostSortedInputGenerator<int> almost_sorted_generator;
  vector<int> sample_data{1, 6, 3, 2, 5, 4, 7, 9, 10};
  SampleInputGenerator<int> sample_generator(sample_data);
  
  PrintTable({1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7, 1<<8, 1<<9, 1<<10, 
              1<<11, 1<<12, 1<<13, 1<<14},
             {&bubble_sort, &insert_sort, &shaker_sort, &quick_sort, &heap_sort,
              &merge_sort, &stl_sort}, {&sample_generator, &reversed_generator,
              &almost_sorted_generator, &random_generator}, {"Bubble Sort",
                "Insert Sort", "Shaker Sort", "Quick Sort", "Heap Sort",
                "Merge Sort", "STL Sort"}, {"Sample Data Generator",
                "Reversed Data Generator", "Almost Sorted Data Generator",
                "Random Data Generator"});
  

  return 0;
}

void SomeFunction(Sort<int>& sorting) {
  auto input = vector<int>{1, 5, 2, 3, 4};
  sorting(input);
}

template <>
void CompareSortingPrintTable::PrintColumn(string message,
                                           const vector<double>& results,
                                           bool alligned, string step) {
  cout << message;
  for (auto result : results) {
    cout << step << fixed << result;
  }
  cout << endl;
}

template <class ValueType>
string CompareSortingPrintTable::ToString(ValueType element, bool alligned) {
  string result = "";
  while (element > 0) {
    result = static_cast<char>('0' + element % 10) + result;
    element /= 10;
  }
  while (result.length() < 8) {
    if (alligned) {
      result += " ";
    } else {
      result = " " + result;
    }
  }
  return result;
}

template <class ValueType>
void CompareSortingPrintTable::PrintColumn(string message,
                                           const vector<ValueType>& results,
                                           bool alligned, string step) {
  cout << message;
  for (auto result : results) {
    cout << step << ToString(result, alligned);
  }
  cout << endl;
}

void CompareSortingPrintTable::operator()(
    initializer_list<size_t> ranges_,
    initializer_list<const Sort<int>*> sort_ptrs_,
    initializer_list<InputGenerator<int>*> generator_ptrs_,
    initializer_list<string> sort_labels_,
    initializer_list<string> generator_labes_) const {

  vector<size_t> ranges(ranges_);
  vector<const Sort<int>*> sorts(sort_ptrs_);
  vector<InputGenerator<int>*> generators(generator_ptrs_);
  vector<string> sort_labels(sort_labels_);
  vector<string> generator_labels(generator_labes_);
  string empty_sort_label = "";
  
  CompareSorting<int> comparission(sort_ptrs_);

  size_t longest_label_lenght = 0;
  for (auto label : sort_labels) {
    longest_label_lenght = max(label.length(), longest_label_lenght);
  }
  longest_label_lenght += 2;
  
  for (auto& label : sort_labels) {
    while (label.length() < longest_label_lenght) {
      label = " " + label;
    }
  }
  
  while (empty_sort_label.length() < longest_label_lenght) {
    empty_sort_label += " ";
  }
  
  cout << empty_sort_label << "--- CORRECT SORT ----" << endl;
  string greeting = "Let's sort";
  while (greeting.size() < longest_label_lenght) {
    greeting = " " + greeting;
  }
  
  vector<int> sample_data{10, 1, 6, 3, 9, 5, 4, 7, 2, 8};
  PrintColumn<int>(greeting, sample_data, 0, "");
  auto sorted_results = comparission.PureSort(sample_data);
  for (size_t sort_number = 0; sort_number < sorts.size(); ++sort_number) {
    PrintColumn<int>(sort_labels[sort_number], sorted_results[sort_number], 0, "");
  }
  cout << endl;
  
  cout << empty_sort_label << "--- TIME EXCEED ----" << endl;
  
  size_t counter = 0;
  for (auto generator : generators) {
    cout << "Result of testing " << generator_labels[counter] << ":" << endl;
    PrintColumn(empty_sort_label, ranges);
    vector<vector<double>> results(sorts.size());
    for (auto range : ranges) {
      auto result = comparission.SecondsCount(range, generator);
      for (size_t i = 0; i < sorts.size(); ++i) {
        results[i].push_back(result[i]);
      }
    }
    for (size_t sort_number = 0; sort_number < sorts.size(); ++sort_number) {
      PrintColumn<double>(sort_labels[sort_number], results[sort_number]);
    }
    ++counter;
    cout << endl;
  }
 
  cout << empty_sort_label << "--- OPERATIONS EXCEED -----" << endl;
  
  counter = 0;
  for (auto generator : generators) {
    cout << "Result of testing " << generator_labels[counter] << ":\n" << endl;
    PrintColumn(empty_sort_label, ranges);
    cout << endl;
    vector<vector<size_t>> results(sorts.size());
    for (auto range : ranges) {
      auto result = comparission.OperationsCount(range, generator);
      for (size_t i = 0; i < sorts.size(); ++i) {
        results[i].push_back(result[i]);
      }
    }
    for (size_t sort_number = 0; sort_number < sorts.size(); ++sort_number) {
      PrintColumn<size_t>(sort_labels[sort_number], results[sort_number], 0);
    }
    ++counter;
    cout << endl;
  }
}

template <class ValueType>
void Print(const vector<ValueType>& data) {
  cout << "[ ";
  for (auto elem : data) {
    cout << elem << " ";
  }
  cout << "]" << endl;
}

int PrimeGenerator::operator()(int min_) const {
  while (TestPrime(min_) == false) {
    ++min_;
  }
  return min_;
}

bool PrimeGenerator::TestPrime(int prime) {
  if (prime <= 1) {
    return false;
  }
  for (int divisor = 2; divisor * divisor <= prime; ++divisor) {
    if (prime % divisor == 0) {
      return false;
    }
  }
  return true;
}
