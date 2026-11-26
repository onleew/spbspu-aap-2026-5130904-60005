#include <iostream>
#include <limits>
#include <cstdlib>
#include <cstddef>
#include <cctype>
#include <new>

namespace chernenko {
  const int error_ec = 1;
  const int good_ec = 0;

  const std::size_t min_read_buffer_size = 67;
  const char* const roman_alphabet = "abcdefghijklmnopqrstuvwxyz";
  const std::size_t roman_alp_size = 27;
  const std::size_t max_sizet_v = std::numeric_limits< std::size_t >::max();
  const std::size_t char_size_v = sizeof(char);
  const int true_out_v = 1;
  const int false_out_v = 0;

  class Reader {
  public:
    Reader();
    ~Reader();

    void readChar(char c);
    void endRead();
    const char* getData() const noexcept;
  private:
    char* data_;
    std::size_t size_;
    std::size_t alloc_size_;

    void extendBuffer();
  };

  char* allocBuffer(std::size_t size);
  int hasRepeat(const char* str) noexcept;
  void formMissingLetters(const char* str, char* buf) noexcept;
  int run(std::istream& input, std::ostream& out, std::ostream& err);
}

int main()
{
  return chernenko::run(std::cin, std::cout, std::cerr);
}

chernenko::Reader::Reader():
  data_(allocBuffer(min_read_buffer_size)),
  size_(0),
  alloc_size_(min_read_buffer_size)
{}

chernenko::Reader::~Reader()
{
  std::free(data_);
}

void chernenko::Reader::readChar(char c)
{
  data_[size_++] = c;
  if (size_ == alloc_size_) {
    extendBuffer();
  }
}

void chernenko::Reader::endRead()
{
  data_[size_++] = 0;
}

const char* chernenko::Reader::getData() const noexcept
{
  return data_;
}

void chernenko::Reader::extendBuffer()
{
  if (alloc_size_ > max_sizet_v - min_read_buffer_size) {
    throw std::bad_alloc();
  }
  alloc_size_ += min_read_buffer_size;
  char* const new_data = allocBuffer(alloc_size_);
  for (std::size_t i = 0; i < size_; i++) {
    new_data[i] = data_[i];
  }
  std::free(data_);
  data_ = new_data;
}

char* chernenko::allocBuffer(std::size_t size)
{
  if (size > max_sizet_v / char_size_v) {
    throw std::bad_alloc();
  }
  char* const res = static_cast< char* >(std::malloc(size * char_size_v));
  if (res == nullptr) {
    throw std::bad_alloc();
  }
  return res;
}

int chernenko::hasRepeat(const char* str) noexcept
{
  for (const char* i = str; *i != 0; i++) {
    for (const char* j = i + 1; *j != 0; j++) {
      if (*i == *j) {
        return true_out_v;
      }
    }
  }
  return false_out_v;
}

void chernenko::formMissingLetters(const char* str, char* buf) noexcept
{
  for (const char* i = roman_alphabet; *i != 0; i++) {
    const char* j = str;
    while (*j != 0) {
      if (std::tolower(*j) == *i) {
        break;
      }
      j++;
    }
    if (*j == 0) {
      *buf++ = *i;
    }
  }
  *buf = 0;
}

int chernenko::run(std::istream& input, std::ostream& out, std::ostream& err)
{
  try {
    Reader reader;
    char c = 0;
    while (input.get(c) && c != '\n') {
      reader.readChar(c);
    }
    if (!input) {
      err << "Read error" << std::endl;
      return error_ec;
    }
    reader.endRead();

    out << chernenko::hasRepeat(reader.getData()) << std::endl;

    char missing_letters[roman_alp_size];
    chernenko::formMissingLetters(reader.getData(), missing_letters);
    out << missing_letters << std::endl;
  } catch (const std::bad_alloc&) {
    err << "Alloc error" << std::endl;
    return error_ec;
  }
  return good_ec;
}
