#include "../include/fraction.h"

Fraction::Fraction() : denominator(BigInteger(1)) {}
Fraction::Fraction(int num) : numerator(BigInteger(num)), denominator(BigInteger(1)) {}
Fraction::Fraction(int numerator, int denominator) : numerator(BigInteger(numerator)),
				denominator((denominator != 0) ? BigInteger(denominator) : throw std::runtime_error("Divide by zero")) {}
Fraction::Fraction(const BigInteger& num) : numerator(BigInteger(num)), denominator(BigInteger(1)) {}
Fraction::Fraction(const BigInteger& numerator, const BigInteger& denominator) : numerator(BigInteger(numerator)), 
				denominator((denominator != 0) ? BigInteger(denominator) : throw std::runtime_error("Divide by zero")) {}
Fraction::Fraction(const Fraction& other) : numerator(other.numerator), denominator(other.denominator) {}
Fraction::Fraction(Fraction&& other) noexcept {
	numerator = std::move(other.numerator);
	denominator = std::move(other.denominator);
}
Fraction::~Fraction() = default;

Fraction& Fraction::operator=(const Fraction& other) {
	if (*this != other) {
		numerator = other.numerator;
		denominator = other.denominator;
	}
	return *this;
}
Fraction& Fraction::operator=(Fraction&& other) noexcept {
	if (*this != other) {
		numerator = std::move(other.numerator);
		denominator = std::move(other.denominator);
	}
	return *this;
}

Fraction& Fraction::operator+=(const Fraction& other) {
	if (denominator != other.denominator) {
		Fraction frac(other);
		numerator *= frac.denominator;
		frac.numerator *= denominator;
		denominator *= frac.denominator;
		numerator += frac.numerator;
	}
	else {
		numerator += other.numerator;
	}
	equilibrate();
	return *this;
}
Fraction& Fraction::operator-=(const Fraction& other){
	if (denominator != other.denominator) {
		Fraction frac(other);
		numerator *= frac.denominator;
		frac.numerator *= denominator;
		denominator *= frac.denominator;
		numerator -= frac.numerator;
	}
	else {
		numerator -= other.numerator;
	}
	equilibrate();
	return *this;
}
Fraction& Fraction::operator*=(const Fraction& other){
	numerator *= other.numerator;
	denominator *= other.denominator;
	equilibrate();
	return *this;
}
Fraction& Fraction::operator/=(const Fraction& other){
	numerator *= other.denominator;
	denominator *= other.numerator;
	equilibrate();
	return *this;
}
Fraction Fraction::operator-() {
	Fraction tmp(*this);
	tmp.numerator.set_is_negative(!tmp.numerator.get_is_negative());
	return tmp;
}

const BigInteger& Fraction::getNumerator() const {
	return numerator;
}
const BigInteger& Fraction::getDenominator() const {
	return denominator;
}
void Fraction::equilibrate(){
	if (numerator.isZero()) {
		denominator = BigInteger(1);
		return;
	}
	if(denominator.get_is_negative()){
		denominator.set_is_negative(false);
		(numerator.get_is_negative() ? numerator.set_is_negative(false) : numerator.set_is_negative(true));
	}
	BigInteger g = gcd(numerator.abs(), denominator);
	numerator /= g;
	denominator /= g;
}
bool Fraction::isInteger() const {
	return denominator == 1;
}
bool Fraction::isLess(const Fraction& other) const {
	if (denominator != other.denominator) {
		BigInteger num(numerator);
		num *= other.denominator;
		BigInteger other_num(other.numerator);
		other_num *= denominator;
		return num < other_num;
	}
	return numerator < other.numerator;
}
bool Fraction::isEqual(const Fraction& other) const{
	if (denominator != other.denominator) {
		BigInteger num(numerator);
		num *= other.denominator;
		BigInteger other_num(other.numerator);
		other_num *= denominator;
		return num == other_num;
	}
	return numerator == other.numerator;
}

std::string toString(const Fraction& frac) {
	std::string result;
	result += to_string(frac.getNumerator());
	if(frac.getDenominator() != BigInteger(1)){
		result += "/";
		result += to_string(frac.getDenominator());
	}
	return result;
}
BigInteger gcd(const BigInteger& first, const BigInteger& second) {
	BigInteger a(first);
	BigInteger b(second);
	while (b > 0) {
		a %= b;
		std::swap(a, b);
	}
	return a;
}
Fraction operator+(const Fraction& left, const Fraction& right) {
	Fraction frac(left);
	frac += right;
	return frac;
}
Fraction operator-(const Fraction& left, const Fraction& right) {
	Fraction frac(left);
	frac -= right;
	return frac;
}
Fraction operator*(const Fraction& left, const Fraction& right) {
	Fraction frac(left);
	frac *= right;
	return frac;
}
Fraction operator/(const Fraction& left, const Fraction& right) {
	Fraction frac(left);
	frac /= right;
	return frac;
}

bool operator==(const Fraction& left, const Fraction& right){
	return left.isEqual(right);
}
bool operator!=(const Fraction& left, const Fraction& right){
	return !left.isEqual(right);
}
bool operator<(const Fraction& left, const Fraction& right){
	return left.isLess(right);
}
bool operator>(const Fraction& left, const Fraction& right){
	return right.isLess(left);
}
bool operator<=(const Fraction& left, const Fraction& right){
	return left.isLess(right) || left.isEqual(right);
}
bool operator>=(const Fraction& left, const Fraction& right){
	return right.isLess(left) || right.isEqual(left);
}



