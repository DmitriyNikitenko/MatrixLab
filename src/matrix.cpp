#include "../include/matrix.h"

Matrix::Matrix() : rows(0), cols(0) {}
Matrix::Matrix(int num) : rows(1), cols(1), arr(1,Fraction(num)) {}
Matrix::Matrix(const Fraction& frac) : rows(1), cols(1), arr(1, Fraction(frac)) {}
Matrix::Matrix(int rows, int cols) {
	if (rows < 0 || cols < 0) { throw std::runtime_error("constructor: all arguments must be positive"); }
	this->rows = rows;
	this->cols = cols;
	arr = std::vector<Fraction>(static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols));
}
Matrix::Matrix(int rows, int cols,const std::vector<Fraction>& arr) {
	if (rows < 0 || cols < 0) { throw std::runtime_error("constructor: all arguments must be positive"); }
	if (arr.size() != static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols)) { throw std::runtime_error("constructor: invalid size of vector"); }
	this->rows = rows;
	this->cols = cols;
	this->arr = arr;
}
Matrix::Matrix(const Matrix& other) : rows(other.rows), cols(other.cols), arr(other.arr) {}
Matrix::Matrix(Matrix&& other) noexcept : rows(other.rows), cols(other.cols) {
	arr = std::move(other.arr);
}
Matrix::~Matrix() = default;


Matrix& Matrix::transpose() {
	Matrix result(*this);
	std::swap(result.rows, result.cols);
	result.arr.resize(static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols));
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c) {
			result(c, r) = (*this)(r, c);
		}
	}
	*this = result;
	return *this;
}
Fraction Matrix::det() const {
	if (rows != cols) { throw std::runtime_error("det: matrix is not square"); }
	Matrix tmp(*this);
	Fraction det = Fraction(1);
	for (int i = 0; i < rows; ++i) {
		int pivot = i;
		while (pivot < rows && tmp(pivot, i) == Fraction(0)) {
			++pivot;
		}
		if (pivot == rows) {
			return Fraction(0);
		}
		if (pivot != i) {
			for (int j = 0; j < cols; ++j) {
				std::swap(tmp(i, j), tmp(pivot, j));
			}
			det = -det;
		}
		Fraction diag = tmp(i, i);
		det *= diag;
		for (int r = i + 1; r < rows; ++r) {
			Fraction factor = tmp(r, i) / diag;
			for (int c = i; c < cols; ++c) {
				tmp(r, c) -= factor * tmp(i, c);
			}
		}
	}
	return det;
}
Matrix Matrix::inv() const {
	if (rows != cols) { throw std::runtime_error("inv: matrix is not square"); }
	int n = rows;
	Matrix aug(n, 2*n);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			aug(i, j) = (*this)(i, j);
		}
		for (int j = n; j < 2 * n; ++j) {
			aug(i, j) = (i == j - n) ? Fraction(1) : Fraction(0);
		}
	}
	for (int i = 0; i < n; ++i) {
		int pivot = i;
		while (pivot < n && aug(pivot, i) == Fraction(0)) {
			++pivot;
		}
		if (pivot == n) {
			throw std::runtime_error("inv: matrix is singular");
		}
		if (pivot != i) {
			for (int j = 0; j < 2 * n; ++j) {
				std::swap(aug(i, j), aug(pivot, j));
			}
		}
		Fraction div = aug(i, i);
		for (int j = 0; j < 2 * n; ++j) {
			aug(i, j) /= div;
		}
		for (int r = 0; r < n; ++r) {
			if (r == i) { continue; }
			Fraction factor = aug(r, i);
			for (int c = 0; c < 2 * n; ++c) {
				aug(r, c) -= factor * aug(i, c);
			}
		}
	}
	Matrix inv(n, n);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			inv(i, j) = aug(i, j + n);
		}
	}
	return inv;
}
Fraction Matrix::sum() const {
	if (arr.empty()) { return Fraction(0); }
	Fraction sum(arr[0]);
	for (size_t i = 1; i < arr.size(); ++i) {
		sum += arr[i];
	}
	return sum;
}
Fraction Matrix::prod() const {
	if (arr.empty()) { return Fraction(1); }
	Fraction mult(arr[0]);
	for (size_t i = 1; i < arr.size(); ++i) {
		mult *= arr[i];
	}
	return mult;
}
Matrix Matrix::min(const Matrix& other) {
	if (rows != other.rows || cols != other.cols || arr.size() != other.arr.size()) { throw std::runtime_error("Wrong size of matrix"); }
	std::vector<Fraction> vec;
	vec.resize(arr.size());
	for (size_t i = 0; i < arr.size(); ++i) {
		vec[i] = std::min(arr[i], other.arr[i]);
	}
	return Matrix(rows, cols, vec);
}
Matrix Matrix::max(const Matrix& other) {
	if (rows != other.rows || cols != other.cols || arr.size() != other.arr.size()) { throw std::runtime_error("Wrong size of matrix"); }
	std::vector<Fraction> vec;
	vec.resize(arr.size());
	for (size_t i = 0; i < arr.size(); ++i) {
		vec[i] = std::max(arr[i], other.arr[i]);
	}
	return Matrix(rows, cols, vec);
}
Matrix Matrix::mul(const Matrix& other) {
	if (rows != other.rows || cols != other.cols || arr.size() != other.arr.size()) { throw std::runtime_error("Wrong size of matrix"); }
	std::vector<Fraction> vec;
	vec.resize(arr.size());
	for (size_t i = 0; i < arr.size(); ++i) {
		vec[i] = arr[i] * other.arr[i];
	}
	return Matrix(rows, cols, vec);
}
Matrix Matrix::div(const Matrix& other) {
	if (rows != other.rows || cols != other.cols || arr.size() != other.arr.size()) { throw std::runtime_error("Wrong size of matrix"); }
	std::vector<Fraction> vec;
	vec.resize(arr.size());
	for (size_t i = 0; i < arr.size(); ++i) {
		vec[i] = arr[i] / other.arr[i];
	}
	return Matrix(rows, cols, vec);
}
bool Matrix::isNumber() const {
	return arr.size() == 1;
}
int Matrix::getRows() const {
	return rows;
}
int Matrix::getCols() const {
	return cols;
}
void Matrix::setRows(int r) {
	rows = r;
}
void Matrix::setCols(int c) {
	cols = c;
}
std::vector<Fraction>& Matrix::getArr() {
	return arr;
}
const std::vector<Fraction>& Matrix::getArr() const {
	return arr;
}

Matrix& Matrix::operator=(const Matrix& other) {
	if (this != &other) {
		arr = other.arr;
		rows = other.rows;
		cols = other.cols;
	}
	return *this;
}
Matrix& Matrix::operator=(Matrix&& other) noexcept {
	if (this != &other) {
		arr = std::move(other.arr);
		rows = other.rows;
		cols = other.cols;
	}
	return *this;
}

Matrix vertcat(const std::vector<Matrix>& vec) {
	if (vec.empty()) { return Matrix(); }
	Matrix result(vec[0]);
	for (size_t i = 1; i < vec.size(); ++i) {
		if (result.getCols() != vec[i].getCols()) { throw std::runtime_error("Invalid size of matrix"); }
		result.getArr().insert(result.getArr().end(), vec[i].getArr().begin(), vec[i].getArr().end());
		result.setRows(result.getRows() + vec[i].getRows());
	}
	return result;
}
Matrix horzcat(const std::vector<Matrix>& vec) {
	if (vec.empty()) { return Matrix(); }
	Matrix result(vec[0]);
	result.transpose();
	for (size_t i = 1; i < vec.size(); ++i) {
		Matrix dummy(vec[i]);
		dummy.transpose();
		if (result.getCols() != dummy.getCols()) { throw std::runtime_error("Invalid size of matrix"); }
		result.getArr().insert(result.getArr().end(), dummy.getArr().begin(), dummy.getArr().end());
		result.setRows(result.getRows() + vec[i].getRows());
	}
	result.transpose();
	return result;
}
Matrix zeros(int rows, int cols) {
	std::vector<Fraction> arr(static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols), static_cast<Fraction>(0));
	return Matrix(rows, cols, arr);
}
Matrix ones(int rows, int cols) {
	std::vector<Fraction> arr(static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols), static_cast<Fraction>(1));
	return Matrix(rows, cols, arr);
}
Matrix eye(int rows, int cols) {
	if (rows != cols) { throw std::runtime_error("Invalid size of matrix"); }
	std::vector<Fraction> arr(static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols), static_cast<Fraction>(0));
	for (size_t i = 0; i < rows; ++i) {
		arr[i * cols + i] = Fraction(1);
	}
	return Matrix(rows,cols,arr);
}
Matrix linspace(const Fraction& first, const Fraction& second, int n) {
	if (n <= 0) { throw std::runtime_error("linspace: n must be positive"); }
	if (n == 1) { return Matrix(1, 1, std::vector<Fraction>(1, first)); }
	std::vector<Fraction> arr;
	Fraction dummy(first);
	Fraction step = (second - first) / Fraction(n - 1);
	for (size_t i = 0; i < n; ++i) {
		arr.push_back(dummy);
		dummy += step;
	}
	return Matrix(1, n, arr);
}

Matrix& Matrix::operator*=(const Matrix& other) {
	if (other.cols == other.rows && other.rows == 1) {
		if (other.arr.size() != 1) { throw std::runtime_error("Invalid size of matrix"); }
		return *this *= other.arr[0];
	}
	if (cols == rows && rows == 1) {
		if (arr.size() != 1) { throw std::runtime_error("Invalid size of matrix"); }
		Matrix dummy(other);
		dummy *= arr[0];
		*this = dummy;
		return *this;
	}
	if (cols != other.rows) { throw std::runtime_error("Invalid size of matrix"); } 
	Matrix result;
	result.rows = rows;
	result.cols = other.cols;
	result.arr.resize(static_cast<size_t>(rows) * static_cast<size_t>(other.cols));
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < other.cols; ++j) {
			Fraction loc = Fraction(0);
			for (int k = 0; k < cols; ++k) {
				loc += (*this)(i,k) * other(k,j);
			}
			result(i, j) = loc;
		}
	}
	*this = result;
	return *this;
} 
Matrix& Matrix::operator*=(const Fraction& frac) {
	for (size_t i = 0; i < arr.size(); ++i) {
		arr[i] *= frac;
	}
	return *this;
}
Matrix& Matrix::operator/=(const Matrix& other) {
	return (*this) *= (other.inv());
}
Matrix Matrix::operator-() const {
	Matrix result(*this);
	if (!result.arr.empty()) {
		for (size_t i = 0; i < arr.size(); ++i) {
			if (result.arr[i] != static_cast<Fraction>(0)) {
				result.arr[i] *= Fraction(-1);
			}
		}
	}
	return result;
}
Matrix& Matrix::operator-=(const Matrix& other) {
	if (rows != other.rows || cols != other.cols || arr.size() != other.arr.size()) { throw std::runtime_error("Wrong size of matrix"); }
	for (size_t i = 0; i < arr.size(); ++i) {
		arr[i] -= other.arr[i];
	}
	return *this;
}
Matrix& Matrix::operator+=(const Matrix& other) {
	if (rows != other.rows || cols != other.cols || arr.size() != other.arr.size()) { throw std::runtime_error("Wrong size of matrix"); }
	for (size_t i = 0; i < arr.size(); ++i) {
		arr[i] += other.arr[i];
	}
	return *this;
}
Fraction& Matrix::operator()(int row,int col) {
	if (row < 0 || col < 0 || col >= cols || row >= rows) { throw std::runtime_error("Wrong index in matrix"); }
	return arr[row * cols + col];
}
const Fraction& Matrix::operator()(int row, int col) const {
	if (row < 0 || col < 0 || col >= cols || row >= rows) { throw std::runtime_error("Wrong index in matrix"); }
	return arr[row * cols + col];
}

Matrix operator+(const Matrix& left, const Matrix& right) {
	Matrix matrix(left);
	matrix += right;
	return matrix;
}
Matrix operator-(const Matrix& left, const Matrix& right) {
	Matrix matrix(left);
	matrix -= right;
	return matrix;
}
Matrix operator*(const Matrix& left, const Matrix& right) {
	Matrix matrix(left);
	matrix *= right;
	return matrix;
}
Matrix operator*(const Matrix& left, const Fraction& right) {
	Matrix matrix(left);
	matrix *= right;
	return matrix;
}
Matrix operator*(const Fraction& left, const Matrix& right) {
	return right * left;
}
Matrix operator/(const Matrix& left, const Matrix& right) {
	Matrix matrix(left);
	matrix /= right;
	return matrix;
}
