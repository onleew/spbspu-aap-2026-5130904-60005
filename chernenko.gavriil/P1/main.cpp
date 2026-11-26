#include <cstddef>
#include <iostream>
#include <limits>

namespace chernenko {
  const int good_ec = 0;
  const int incorrect_input_ec = 1;
  const int runtime_err_ec = 2;

  const int stop_number = 0;
  const std::size_t max_sizet_v = std::numeric_limits< std::size_t >::max();

  class Counter {
  public:
    virtual void processN(int n) noexcept = 0;
    virtual bool isErr() const noexcept = 0;
    virtual void print(std::ostream& out_res, std::ostream& out_err) const noexcept = 0;
  };

  class CounterMonDec: public Counter {
  public:
    CounterMonDec();

    void processN(int n) noexcept override;
    bool isErr() const noexcept override;
    void print(std::ostream& out_res, std::ostream& out_err) const noexcept override;
  private:
    int prev_number_;
    std::size_t cur_length_;
    std::size_t max_length_;
    bool is_overflow_;
  };

  class CounterDivRem: public Counter {
  public:
    CounterDivRem():
      prev_number_(stop_number),
      div_count_(0),
      is_overflow_(false),
      is_invalid_seq_(true)
    {}

    void processN(int n) noexcept override;
    bool isErr() const noexcept override;
    void print(std::ostream& out_res, std::ostream& out_err) const noexcept override;
  private:
    int prev_number_;
    std::size_t div_count_;
    bool is_overflow_;
    bool is_invalid_seq_;
  };

  int run(std::istream& input, std::ostream& out, std::ostream& err);
}

int main()
{
  return chernenko::run(std::cin, std::cout, std::cerr);
}

chernenko::CounterMonDec::CounterMonDec():
  prev_number_(stop_number),
  cur_length_(0),
  max_length_(0),
  is_overflow_(false)
{}

void chernenko::CounterMonDec::processN(int n) noexcept
{
  if (isErr()) {
    return;
  }
  if (cur_length_ == max_sizet_v) {
    is_overflow_ = true;
    return;
  }
  if (prev_number_ == 0 || n > prev_number_) {
    cur_length_ = 0;
  }
  cur_length_++;
  if (cur_length_ > max_length_) {
    max_length_ = cur_length_;
  }
  prev_number_ = n;
}

bool chernenko::CounterMonDec::isErr() const noexcept
{
  return is_overflow_;
}

void chernenko::CounterMonDec::print(std::ostream& out_res, std::ostream& out_err) const noexcept
{
  if (isErr()) {
    out_err << "CounterMonDec overflow error" << std::endl;
    return;
  }
  out_res << max_length_ << std::endl;
}

void chernenko::CounterDivRem::processN(int n) noexcept
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

bool chernenko::CounterDivRem::isErr() const noexcept
{
  return is_overflow_ || is_invalid_seq_;
}

void chernenko::CounterDivRem::print(std::ostream& out_res, std::ostream& out_err) const noexcept
{
  if (isErr()) {
    out_err << "CounterDivRem error" << std::endl;
    return;
  }
  out_res << div_count_ << std::endl;
}

int chernenko::run(std::istream& input, std::ostream& out, std::ostream& err)
{
  CounterDivRem counter_dr;
  CounterMonDec counter_md;
  int n = 0;
  while (input >> n && n != stop_number) {
    counter_dr.processN(n);
    counter_md.processN(n);
    if (counter_dr.isErr() && counter_md.isErr()) {
      break;
    }
  }
  if (!input) {
    err << "Incorrect input error" << std::endl;
    return incorrect_input_ec;
  }
  counter_dr.print(out, err);
  counter_md.print(out, err);
  if (counter_dr.isErr() || counter_md.isErr()) {
    return runtime_err_ec;
  }
  return good_ec;
}
