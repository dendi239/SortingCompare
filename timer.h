#include <ctime>

class Timer {
 private:
  static double Seconds(clock_t delay);
  
  clock_t time_start;
  
 public:
  Timer() : time_start{clock()} {}
  void Start() { time_start = clock(); }
  double CurrentTime() const;
  double Stop();
};

double Timer::CurrentTime() const {
  clock_t time_delay = clock() - time_start;
  return Seconds(time_delay);
}

double Timer::Stop() {
  clock_t time_delay = clock() - time_start;
  double result = Seconds(time_delay);
  time_start = clock();
  return result;
}

double Timer::Seconds(clock_t delay) {
  return static_cast<double>(delay) / CLOCKS_PER_SEC;
}
