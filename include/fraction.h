#ifndef FRACTION_H
#define FRACTION_H
#include "biginteger.h"
#include <stdexcept>

class Fraction {
private:
	BigInteger numerator;
	BigInteger denominator;
public:
	Fraction();
	Fraction(int num);
	Fraction(int numerator, int denominator);
	Fraction(const BigInteger& num);
	Fraction(const BigInteger& numerator, const BigInteger& denominator);
	Fraction(const Fraction& other);
	Fraction(Fraction&& other) noexcept;
	~Fraction();

	Fraction& operator=(const Fraction& other);
	Fraction& operator=(Fraction&& other) noexcept;
	Fraction& operator+=(const Fraction& other);
	Fraction& operator-=(const Fraction& other);
	Fraction& operator*=(const Fraction& other);
	Fraction& operator/=(const Fraction& other);
	Fraction operator-();

	const BigInteger& getNumerator() const;
	const BigInteger& getDenominator() const;
	void equilibrate();
	bool isInteger() const;
	bool isLess(const Fraction& other) const;
	bool isEqual(const Fraction& other) const;
};

std::string toString(const Fraction& frac);
BigInteger gcd(const BigInteger& first, const BigInteger& second);
Fraction operator+(const Fraction& left, const Fraction& right);
Fraction operator-(const Fraction& left, const Fraction& right);
Fraction operator*(const Fraction& left, const Fraction& right);
Fraction operator/(const Fraction& left, const Fraction& right);

bool operator==(const Fraction& left, const Fraction& right);
bool operator!=(const Fraction& left, const Fraction& right);
bool operator<(const Fraction& left, const Fraction& right);
bool operator>(const Fraction& left, const Fraction& right);
bool operator<=(const Fraction& left, const Fraction& right);
bool operator>=(const Fraction& left, const Fraction& right);

#endif
