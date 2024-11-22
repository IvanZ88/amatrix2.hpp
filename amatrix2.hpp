// amatrix2.hpp
// Матрицы на основе std::vector.
// Упрощённая реализация в виде набора внешних функций для плотных матриц
// на основе структуры данных "массив массивов" ("способ 1").
// Операции текстового ввода-вывода прилагаются.
#ifndef AMATRIX2_HPP_INCLUDED_XAM100
#define AMATRIX2_HPP_INCLUDED_XAM100
#include <vector>
#include <iterator>
#include <type_traits>
#include <istream>
#include <ostream>


///////////////////////////////////////////////////////////////////////////////
// Структура "массив массивов" 
// на основе std::vector и шаблонов синонимов типа.

/// Шаблон синонима типа, представляющий строку матрицы как вектор элементов.
template <class T>
using Matrix_row = std::vector<T>;

/// Шаблон синонима типа, представляющий матрицу как вектор строк.
template <class T>
using Matrix = std::vector<Matrix_row<T>>;


///////////////////////////////////////////////////////////////////////////////
// Ввод-вывод вектора как последовательности значений,
// разделённых пробелами и завершаемых переводом строки.
// Функции readln ведут себя аналогично базовому варианту std::getline.

/// Вывести строку из элементов вектора в поток вывода.
template <class T>
std::ostream& writeln(std::ostream &os, const Matrix_row<T> &row)
{
  for (auto &item : row)
    os << ' ' << item;
  return os << '\n';
}

/// Пропустить "горизонтальные" пробелы (' ' и '\t').
inline std::istream& skiphws(std::istream &is)
{
  for (char ch; is.get(ch) && (ch == ' ' || ch == '\t');) {}
  return is.unget();
}

/// Прочитать строку из потока ввода как последовательность элементов вектора.
template <class T>
std::istream& readln(std::istream &is, Matrix_row<T> &row)
{
  row.clear();
  for (T item; skiphws(is);)
  {
    if (is.peek() == '\n')
      return is.ignore();
    if (!(is >> item))
      return is;
    row.push_back(item);
  }
  return is;
}


///////////////////////////////////////////////////////////////////////////////
// Ввод-вывод матрицы как последовательности строк-векторов.

/// Вывести матрицу в поток вывода.
template <class T>
std::ostream& writeln(std::ostream &os, const Matrix<T> &matrix)
{
  for (auto &row : matrix)
    writeln(os, row);
  return os << '\n'; // закрывающая пустая строка
}

/// Сделать массив массивов прямоугольным по максимальной строке ==
/// дополнить более короткие строки значениями по умолчанию.
template <class T>
void force_rectangularity(Matrix<T> &matrix)
{
  // Найти максимальный размер строки.
  size_t max_len = 0;
  for (auto &row : matrix)
    if (max_len < row.size())
      max_len = row.size();

  // Если max_len = 0, очистить матрицу.
  if (max_len == 0)
  {
    matrix.clear();
  }
  else
  {
    // Дополнить все строки до максимального размера.
    for (auto &row : matrix)
      row.resize(max_len);
  }
}

/// Прочитать из потока ввода матрицу как набор строк-векторов.
/// Заканчивает чтение на пустой строке (забирает эту строку из потока ввода).
template <class T>
std::istream& readln(std::istream &is, Matrix<T> &matrix)
{
  matrix.clear();
  for (Matrix_row<T> row; readln(is, row) && !row.empty();)
    matrix.push_back(row);
  force_rectangularity(matrix);
  return is;
}


///////////////////////////////////////////////////////////////////////////////
// Вспомогательные операции.

/// Узнать количество строк в матрице.
template <class T>
inline size_t rows(const Matrix<T> &matrix)
{
  return matrix.size();
}

/// Узнать количество столбцов в матрице.
template <class T>
inline size_t cols(const Matrix<T> &matrix)
{
  return matrix.empty() ? 0 : matrix.front().size();
}

/// Задать новые размеры матрицы.
template <class T>
Matrix<T>& reshape(Matrix<T> &matrix, size_t rows, size_t cols)
{
  matrix.resize(rows);
  for (auto &row : matrix)
    row.resize(cols);
  return matrix;
}

/// Записать в каждый элемент вектора заданное значение.
template <class T>
Matrix_row<T>& fill(Matrix_row<T> &row, T value)
{
  row.assign(row.size(), value);
  return row;
}

/// Записать в каждый элемент матрицы заданное значение.
template <class T>
Matrix<T>& fill(Matrix<T> &matrix, T value)
{
  for (auto &row : matrix)
    fill(row, value);
  return matrix;
}

/// Проверить, что все элементы вектора равны заданному значению.
template <class T>
bool consists_of(const std::vector<T> &vec, const T &value)
{
  for (auto &item : vec)
    if (item != value)
      return false;
  return true;
}

/// Проверить, что все элементы матрицы равны заданному значению.
template <class T>
bool consists_of(const Matrix<T> &matrix, T value)
{
  for (auto &row : matrix)
    if (!consists_of(row, value))
      return false;
  return true;
}


///////////////////////////////////////////////////////////////////////////////
// Создание матриц.

/// Создать объект матрицы (копию) из двумерного статического массива.
template <class T, size_t N, size_t M>
Matrix<T> matrix(const T (&arr)[N][M])
{
  Matrix<T> result(N);
  for (size_t i = 0; i < N; ++i)
    result[i].assign(&arr[i][0], &arr[i][M]);
  return result;
}


/// Создать матрицу заданных размеров с заданным значением элементов.
template <class T>
Matrix<T> matrix(size_t rows, size_t cols, T value = {})
{
  Matrix<T> result(rows);
  for (auto &row : result)
    row.assign(cols, value);
  return result;
}


/// Создать диагональную матрицу заданного размера.
template <class T>
Matrix<T> diagonal(size_t size, T value)
{
  Matrix<T> result(size);
  for (size_t i = 0; i < size; ++i)
  {
    auto &row = result[i];
    row.resize(size);
    row[i] = value;
  }

  return result;
}


/// Создать диагональную матрицу из диапазона.
/// Задействует некоторые "продвинутые" элементы современного C++.
/// It -- тип итератора, комбинация enable_if+is_convertible
/// используется, чтобы запретить компилятору использовать данный вариант
/// функции diagonal в том случае, если It не является однонаправленным итератором.
template <class It>
std::enable_if_t<std::is_convertible<
  typename std::iterator_traits<It>::iterator_category,
  std::forward_iterator_tag>::value,
Matrix<typename std::iterator_traits<It>::value_type>>
diagonal(It from, It to)
{
  using std::distance;
  const size_t size = distance(from, to);
  Matrix<typename std::iterator_traits<It>::value_type> result(size);
  for (size_t i = 0; i < size; ++i, ++from)
  {
    auto &row = result[i];
    row.resize(size);
    row[i] = *from;
  }

  return result;
}


/// Создать диагональную матрицу из произвольной коллекции.
/// С++11 вариант для MSVC2013, очень близок, но не идентичен
/// закомментированному варианту ниже.
template <class Col>
inline auto diagonal(const Col &collection)
  -> decltype(diagonal(std::begin(collection), std::end(collection)))
{
  return diagonal(std::begin(collection), std::end(collection));
}

/* C++14, но не MSVC2013:
template <class Col>
auto diagonal(const Col &collection)
{
  using std::begin;
  using std::end;
  return diagonal(begin(collection), end(collection));
}
//*/

/// Вариант, позволяющий выполнять diagonal({ список значений на диагонали }).
template <class T>
inline Matrix<T> diagonal(std::initializer_list<T> values)
{
  return diagonal(std::begin(values), std::end(values));
}

#endif//AMATRIX2_HPP_INCLUDED_XAM100
