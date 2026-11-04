#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstddef>
#include <limits>
#include <new>

const int invalid_program_agrument_ec = 1;
const int invalid_file_ec = 2;
const int good_ec = 0;

const std::size_t max_elements = 10000;
const int max_int = std::numeric_limits< int >::max();
const int min_int = std::numeric_limits< int >::min();
const std::size_t max_sizet = std::numeric_limits< std::size_t >::max();

namespace chernenko {
  void runStack(std::istream& input, std::ostream& out);
  void runDynamic(std::istream& input, std::ostream& out);
  void readMatrix(std::istream& input, int* matrix, std::size_t size);
  std::size_t countColumnsNotsame(std::size_t n, std::size_t m, const int* matrix);
  int minSumParallMaind(std::size_t n, std::size_t m, const int* matrix);
  int min(int a, int b);
  std::size_t min(std::size_t a, std::size_t b);
  void add(int&, int);
}

int main(int argc, char** argv)
{
  if (argc > 4) {
    std::cerr << "Too many arguments" << std::endl;
    return invalid_program_agrument_ec;
  }
  if (argc < 4) {
    std::cerr << "Not enough arguments" << std::endl;
    return invalid_program_agrument_ec;
  }
  if (argv[1][0] < '0' || argv[1][0] > '9' || (argv[1][0] != 0 && argv[1][1] != 0)) {
    std::cerr << "First parameter is not a number" << std::endl;
    return invalid_program_agrument_ec;
  }
  if (argv[1][0] < '1' || argv[1][0] > '2') {
    std::cerr << "First parameter is out of range" << std::endl;
    return invalid_program_agrument_ec;
  }
  bool is_dynamic = false;
  if (argv[1][0] == '2') {
    is_dynamic = true;
  }
  std::ifstream in_file(argv[2]);
  if (!in_file) {
    std::cerr << "Incorrect input file" << std::endl;
    return invalid_program_agrument_ec;
  }
  std::ofstream out_file(argv[3]);
  if (!out_file) {
    std::cerr << "Incorrect output file" << std::endl;
    return invalid_program_agrument_ec;
  }
  try {
    if (is_dynamic) {
      chernenko::runDynamic(in_file, out_file);
    } else {
      chernenko::runStack(in_file, out_file);
    }
  } catch (const std::overflow_error& e) {
    std::cerr << "Overflow error: " << e.what() << std::endl;
    return invalid_file_ec;
  } catch (const std::underflow_error& e) {
    std::cerr << "Underflow error: " << e.what() << std::endl;
    return invalid_file_ec;
  } catch (const std::runtime_error& e) {
    std::cerr << "Runtime error: " << e.what() << std::endl;
    return invalid_file_ec;
  } catch (const std::bad_alloc& e) {
    std::cerr << "Allocation error: " << e.what() << std::endl;
    return invalid_file_ec;
  }
  return good_ec;
}

void chernenko::runStack(std::istream& input, std::ostream& out)
{
  std::size_t i = 0;
  std::size_t rows = 0, cols = 0;
  while (true) {
    if (!(input >> rows)) {
      if (input.eof() && i > 0) {
        break;
      }
      throw std::runtime_error("Incorrect file content");
    }
    if (!(input >> cols)) {
      throw std::runtime_error("Incorrect file content");
    }
    std::size_t spec1 = 0;
    int spec2 = 0;
    if (cols != 0 && rows > max_elements / cols) {
      throw std::runtime_error("Incorrect file content");
    }
    if (rows * cols == 0 && rows != cols) {
      throw std::runtime_error("Incorrect file content");
    }
    int matrix[max_elements]{};
    chernenko::readMatrix(input, matrix, rows * cols);
    spec1 = chernenko::countColumnsNotsame(rows, cols, matrix);
    spec2 = chernenko::minSumParallMaind(rows, cols, matrix);
    out << spec1 << " " << spec2 << std::endl;
    i++;
  }
}

void chernenko::runDynamic(std::istream& input, std::ostream& out)
{
  std::size_t i = 0;
  std::size_t rows = 0, cols = 0;
  while (true) {
    if (!(input >> rows)) {
      if (input.eof() && i > 0) {
        break;
      }
      throw std::runtime_error("Incorrect file content");
    }
    if (!(input >> cols)) {
      throw std::runtime_error("Incorrect file content");
    }
    std::size_t spec1 = 0;
    int spec2 = 0;
    if (cols != 0 && rows > max_sizet / cols) {
      throw std::runtime_error("Incorrect file content");
    }
    if (rows * cols == 0 && rows != cols) {
      throw std::runtime_error("Incorrect file content");
    }
    int* const matrix = new int[rows * cols]{};
    try {
      chernenko::readMatrix(input, matrix, rows * cols);
      spec1 = chernenko::countColumnsNotsame(rows, cols, matrix);
      spec2 = chernenko::minSumParallMaind(rows, cols, matrix);
    } catch (...) {
      delete[] matrix;
      throw;
    }
    delete[] matrix;
    out << spec1 << " " << spec2 << std::endl;
    i++;
  }
}

void chernenko::readMatrix(std::istream& input, int* matrix, std::size_t size)
{
  for (std::size_t i = 0; i < size; i++) {
    if (!(input >> matrix[i])) {
      throw std::runtime_error("Incorrect file content");
    }
  }
}

std::size_t chernenko::countColumnsNotsame(std::size_t n, std::size_t m, const int* matrix)
{
  std::size_t count = 0;
  for (std::size_t i = 0; i < m; i++) {
    bool flag = true;
    for (std::size_t j = 1; j < n; j++) {
      if (matrix[(j - 1) * m + i] == matrix[j * m + i]) {
        flag = false;
        break;
      }
    }
    if (flag) {
      count++;
    }
  }
  return count;
}

int chernenko::minSumParallMaind(std::size_t n, std::size_t m, const int* matrix)
{
  const std::size_t len = chernenko::min(n, m);
  if (len < 2) {
    return 0;
  }
  int res = std::numeric_limits< int >::max();
  for (std::size_t d = 1; d < len; d++) {
    int cur = 0;
    for (std::size_t j = d, i = 0; j < len; j++, i++) {
      chernenko::add(cur, matrix[j * m + i]);
    }
    res = chernenko::min(res, cur);
    cur = 0;
    for (std::size_t j = 0, i = d; i < len; j++, i++) {
      chernenko::add(cur, matrix[j * m + i]);
    }
    res = chernenko::min(res, cur);
  }
  return res;
}

int chernenko::min(int a, int b)
{
  if (a < b) {
    return a;
  }
  return b;
}

std::size_t chernenko::min(std::size_t a, std::size_t b)
{
  if (a < b) {
    return a;
  }
  return b;
}

void chernenko::add(int& a, int b)
{
  if (a > 0 && b > 0 && a > max_int - b) {
    throw std::overflow_error("Too big int value");
  }
  if (a < 0 && b < 0 && a < min_int - b) {
    throw std::underflow_error("Too small int value");
  }
  a += b;
}
