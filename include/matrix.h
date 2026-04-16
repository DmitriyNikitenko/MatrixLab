#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <string>
#include "fraction.h"

class Matrix {
private:
	int rows;
	int cols;
	std::vector<Fraction> arr;
public:
	//Конструкторы
	Matrix();
	Matrix(int num);
	Matrix(const Fraction& frac);
	Matrix(int rows, int cols);
	Matrix(int rows, int cols, const std::vector<Fraction>& arr);
	Matrix(const Matrix& other);
	Matrix(Matrix&& other) noexcept;
	~Matrix();

	//Основные функции
	Matrix& transpose();
	Fraction det() const;
	Matrix inv() const;
	Fraction sum() const;
	Fraction prod() const;
	Matrix min(const Matrix& other);
	Matrix max(const Matrix& other);
	Matrix mul(const Matrix& other);
	Matrix div(const Matrix& other);
	bool isNumber() const;
	int getRows() const;
	int getCols() const;
	void setRows(int r);
	void setCols(int c);
	std::vector<Fraction>& getArr();
	const std::vector<Fraction>& getArr() const;

	//Операторы
	Matrix& operator=(const Matrix& other);
	Matrix& operator=(Matrix&& other) noexcept;

	Matrix& operator*=(const Matrix& other);
	Matrix& operator*=(const Fraction& frac);
	Matrix& operator/=(const Matrix& other);
	Matrix operator-() const;
	Matrix& operator-=(const Matrix& other);
	Matrix& operator+=(const Matrix& other);
	Fraction& operator()(int row, int col);
	const Fraction& operator()(int row, int col) const;
};

//Дополнительно
Matrix vertcat(const std::vector<Matrix>& vec);
Matrix horzcat(const std::vector<Matrix>& vec);
Matrix zeros(int rows, int cols);
Matrix ones(int rows, int cols);
Matrix eye(int rows, int cols);
Matrix linspace(const Fraction& first, const Fraction& second, int n);

Matrix operator+(const Matrix& left, const Matrix& right);
Matrix operator-(const Matrix& left, const Matrix& right);
Matrix operator*(const Matrix& left, const Matrix& right);
Matrix operator*(const Matrix& left, const Fraction& right);
Matrix operator*(const Fraction& left, const Matrix& right);
Matrix operator/(const Matrix& left, const Matrix& right);

#endif
