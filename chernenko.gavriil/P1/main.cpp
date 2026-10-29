#include <cstddef>
#include <iostream>
#include <limits>

const std::size_t max_sizet_v = std::numeric_limits< std::size_t >::max();

const int good_ec = 0;
const int incorrect_input_ec = 1;
const int runtime_err_ec = 2;

const int stop_number = 0;

class Counter {
public:
  virtual void next_n(int n) noexcept = 0;
  virtual bool is_err() const noexcept = 0;
  virtual void print(std::ostream& out_res, std::ostream& out_err) const noexcept = 0;
};

class CounterMonDec: public Counter {
private:
  int prev_number_;
  std::size_t cur_length_;
  std::size_t max_length_;
  bool is_overflow_;

public:
  CounterMonDec():
    prev_number_(stop_number),
    cur_length_(0),
    max_length_(0),
    is_overflow_(false)
  {}

  void next_n(int n) noexcept override
  {
    if (is_err()) {
      return;
    }
    if (prev_number_ == 0 || n > prev_number_) {
      cur_length_ = 1;
      prev_number_ = n;
      return;
    }
    if (cur_length_ == max_sizet_v) {
      is_overflow_ = true;
      return;
    }
    cur_length_++;
    if (cur_length_ > max_length_) {
      max_length_ = cur_length_;
    }
    prev_number_ = n;
  }

  bool is_err() const noexcept override
  {
    return is_overflow_;
  }

  void print(std::ostream& out_res, std::ostream& out_err) const noexcept override
  {
    if (is_err()) {
      out_err << "CounterMonDec overflow error" << std::endl;
      return;
    }
    out_res << max_length_ << std::endl;
  }
};

class CounterDivRem: public Counter {
private:
  int prev_number_;
  std::size_t div_count_;
  bool is_overflow_;
  bool is_invalid_seq_;

public:
  CounterDivRem():
    prev_number_(stop_number),
    div_count_(0),
    is_overflow_(false),
    is_invalid_seq_(true)
  {}

  void next_n(int n) noexcept override
  {
    if (prev_number_ == 0) {
      prev_number_ = n;
      return;
    }
    if (is_invalid_seq_) {
      is_invalid_seq_ = false;
    }
    if (!is_overflow_ && n % prev_number_ == 0) {
      if (div_count_ == max_sizet_v) {
        is_overflow_ = true;
        return;
      }
      div_count_++;
    }
    prev_number_ = n;
  }

  bool is_err() const noexcept override
  {
    return is_overflow_ || is_invalid_seq_;
  }

  void print(std::ostream& out_res, std::ostream& out_err) const noexcept override
  {
    if (is_err()) {
      out_err << "CounterDivRem error" << std::endl;
      return;
    }
    out_res << div_count_ << std::endl;
  }
};

class Runner {
private:
  Counter& cnt_a_;
  Counter& cnt_b_;
public:
  Runner(Counter& a, Counter& b):
    cnt_a_(a),
    cnt_b_(b)
  {}

  int run(std::istream& input, std::ostream& out, std::ostream& err)
  {
    int n = 0;
    while (input >> n && n != stop_number) {
      cnt_a_.next_n(n);
      cnt_b_.next_n(n);
      if (cnt_a_.is_err() && cnt_b_.is_err()) {
        break;
      }
    }
    if (!input) {
      err << "Incorect input error" << std::endl;
      return incorrect_input_ec;
    }
    cnt_a_.print(out, err);
    cnt_b_.print(out, err);
    if (cnt_a_.is_err() || cnt_b_.is_err()) {
      return runtime_err_ec;
    }
    return good_ec;
  }
};

int main()
{
  CounterDivRem counter_dr{};
  CounterMonDec counter_md{};
  return Runner(counter_dr, counter_md).run(std::cin, std::cout, std::cerr);
}
