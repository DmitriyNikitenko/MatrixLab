#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H
#include <cstdint>
#include <string>
#include <stdexcept>

enum class biginteger_base { hex, dec };

class BigInteger {

    //Поля класса
    uint32_t* data;
    size_t size;
    bool is_negative = false;


    //Функция увеличения памяти
    void resize(size_t new_size);

    void create_from_dec_string(const std::string& str);

    void create_from_hex_string(const std::string& str);

    static bool isAbsLess(const BigInteger& left, const BigInteger& right);

    static void absolute_difference(uint32_t*& data, size_t& size,
        const uint32_t* other_data, size_t other_size);
public:
    //-------------------------------------К О Н С Т Р У К Т О Р Ы-----------------------------------------------------------
    BigInteger();
    BigInteger(int num);
    BigInteger(unsigned int num);
    BigInteger(long num);
    BigInteger(long long num);
    BigInteger(unsigned long num);
    BigInteger(unsigned long long num);
    BigInteger(const BigInteger& other);
    BigInteger(BigInteger&& other) noexcept;
    BigInteger(std::string str, biginteger_base base = biginteger_base::dec);
    ~BigInteger();

    //------------------------------------------------О П Е Р А Т О Р Ы-------------------------------------------------------------
        //Оператор присваивания
    BigInteger& operator=(const BigInteger& other);

    //Оператор перемещения
    BigInteger& operator=(BigInteger&& other) noexcept;

    //Унарные операторы
    BigInteger operator-() const;
    BigInteger operator+() const;

    //Операторы сравнения
    bool operator!=(const BigInteger& other) const;
    bool operator<(const BigInteger& other) const;
    bool operator<=(const BigInteger& other) const;
    bool operator>(const BigInteger& other) const;
    bool operator>=(const BigInteger& other) const;

    //Инкремент и декремент
    BigInteger& operator++();
    BigInteger operator++(int);
    BigInteger& operator--();
    BigInteger operator--(int);

    //Арифмитические операторы
    BigInteger& operator+=(const BigInteger& other);
    BigInteger& operator-=(const BigInteger& other);
    BigInteger& operator*=(const BigInteger& other);
    BigInteger& operator/=(const BigInteger& divisor);
    BigInteger& operator%=(const BigInteger& other);

    //------------------------------------------------Г Е Т Т Е Р Ы   И   С Е Т Т Е Р Ы---------------------------------------------------------
        //Геттеры
    bool get_is_negative() const;
    size_t get_size() const;
    const uint32_t* get_data() const;

    //Сеттер
    void set_is_negative(bool set_value);

    //---------------------------------------------Д О П О Л Н И Т Е Л Ь Н О-------------------------------------------------
    BigInteger abs() const;
    bool isZero() const;
    int divide_by_10();
    int divide_by_16();
    static bool isLess(const BigInteger& left, const BigInteger& right);
};
//-----------------------------------------------------------В Н Е Ш Н И Е----------------------------------------------------------------

//Бинарный оператор сравнения
bool operator==(const BigInteger& left, const BigInteger& right);

std::string to_string(const BigInteger& b_int);
std::string to_hex(const BigInteger& b_int);

//Бинарные арифметические операторы
BigInteger operator+(const BigInteger& left, const BigInteger& right);
BigInteger operator-(const BigInteger& left, const BigInteger& right);
BigInteger operator*(const BigInteger& left, const BigInteger& right);
BigInteger operator/(const BigInteger& left, const BigInteger& right);
BigInteger operator%(const BigInteger& left, const BigInteger& right);

#endif