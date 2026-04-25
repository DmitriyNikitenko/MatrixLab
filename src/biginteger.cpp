#include "../include/biginteger.hpp"
#include <cstdint>
#include <string>
#include <stdexcept>

//---------------------------------------П Р И В А Т Н Ы Е   Ф У Н К Ц И И-------------------------------------------------------------

//Функция увеличения памяти
void BigInteger::resize(size_t new_size) {
    uint32_t* new_data = new uint32_t[new_size]();
    const size_t elements_to_copy = std::min(size, new_size);
    for (size_t i = 0; i < elements_to_copy; ++i) {
        new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
    size = new_size;
}

void BigInteger::create_from_dec_string(const std::string& str) {
    if (str.empty()) {
        data = new uint32_t[1]{ 0 };
        size = 1;
        return;
    }
    try {
        size_t position = 0;
        while (str[position] == '0' && position < str.size()) {
            ++position;
        }
        if (position >= str.size()) {
            throw std::invalid_argument("There is not enough information in the line");
        }
        data = new uint32_t[1]{ 0 };
        size = 1;
        for (char c : str) {
            if (isdigit(c) == 0) {
                throw std::invalid_argument("Invalid decimal digit");
            }
            uint32_t digit = c - '0';
            uint64_t carry = digit;
            // Умножение на 10 и добавление цифры
            for (size_t i = 0; i < size; ++i) {
                uint64_t temp = (uint64_t)data[i] * 10 + carry;
                data[i] = static_cast<uint32_t>(temp);
                carry = temp >> 32;
            }
            // Обработка переноса
            while (carry > 0) {
                resize(size + 1);
                data[size - 1] = static_cast<uint32_t>(carry);
                carry >>= 32;
            }
        }
    }
    catch (std::invalid_argument error_message) {
        delete[] data;
        throw std::invalid_argument(error_message.what());
    }
}

void BigInteger::create_from_hex_string(const std::string& str) {
    if (str.empty()) {
        data = new uint32_t[1]{ 0 };
        size = 1;
        return;
    }
    try {
        size_t position = 0;
        while (str[position] == '0' && position < str.size()) {
            ++position;
        }
        if (position >= str.size()) {
            throw std::invalid_argument("There is not enough information in the line");
        }
        data = new uint32_t[1]{ 0 };
        size = 1;
        for (char c : str) {
            uint32_t digit;
            if (isdigit(c) != 0) {
                digit = c - '0';
            }
            else if (c >= 'A' && c <= 'F') {
                digit = 10 + (c - 'A');
            }
            else if (c >= 'a' && c <= 'f') {
                digit = 10 + (c - 'a');
            }
            else {
                throw std::invalid_argument("Invalid hex digit");
            }
            // Умножение текущего значения на 16
            uint64_t carry = digit;
            for (size_t i = 0; i < size; ++i) {
                uint64_t temp = (uint64_t)data[i] * 16 + carry;
                data[i] = static_cast<uint32_t>(temp);
                carry = temp >> 32;
            }
            // Обработка переноса
            while (carry > 0) {
                resize(size + 1);
                data[size - 1] = static_cast<uint32_t>(carry);
                carry >>= 32;
            }
        }
    }
    catch (std::invalid_argument error_message) {
        delete[] data;
        throw std::invalid_argument(error_message.what());
    }
}
bool BigInteger::isAbsLess(const BigInteger& left, const BigInteger& right) {
    if (left.get_size() != right.get_size()) {
        return left.get_size() < right.get_size();
    }
    for (size_t i = left.get_size() - 1; i >= 0; --i) {
        if (left.get_data()[i] != right.get_data()[i]) {
            return left.get_data()[i] < right.get_data()[i];
        }
    }
    return false;
}

void BigInteger::absolute_difference(uint32_t*& data, size_t& size,
    const uint32_t* other_data, size_t other_size) {
    // data всегда больше other_data
    //Используем знаковые типы данных (может быть ситуация loc_difference = -1 < 0)
    int64_t borrow = 0;
    int64_t loc_difference = 0;
    size_t max_size = std::max(size, other_size);
    for (size_t i = 0; i < max_size; ++i) {
        int64_t a = (i < size) ? data[i] : 0;
        int64_t b = (i < other_size) ? other_data[i] : 0;
        //Учитываем занятый бит
        loc_difference = a - borrow;
        if (loc_difference >= b) {
            loc_difference -= b;
            borrow = 0;
        }
        else {
            //Добавляем 1 ... (32 нуля)
            loc_difference += (1ULL << 32);
            loc_difference -= b;
            borrow = 1;
        }
        data[i] = static_cast<uint32_t>(loc_difference);
    }
    //Удаляем ведущие нули
    while (size > 1 && data[size - 1] == 0) {
        --size;
    }
    if (borrow != 0) {
        throw std::logic_error("data < other_data");
    }
}

//-------------------------------------К О Н С Т Р У К Т О Р Ы------------------------------------------------------------------------

BigInteger::BigInteger() : data(new uint32_t[1]{ 0 }), size(1), is_negative(false) {}

BigInteger::BigInteger(int num)
    : data(new uint32_t[1]{ static_cast<uint32_t>((num > 0) ? num : -num) }),
    size(1),
    is_negative(num < 0) {}

BigInteger::BigInteger(unsigned int num)
    : data(new uint32_t[1]{ static_cast<uint32_t>(num) }),
    size(1),
    is_negative(false) {}

BigInteger::BigInteger(long num) {
    is_negative = (num < 0);
    uint64_t abs_num = static_cast<uint64_t>(is_negative ? -num : num);
    // Всегда создаем массив из 2 элементов
    data = new uint32_t[2]{
        static_cast<uint32_t>(abs_num),          // Младшие 32 бита
        static_cast<uint32_t>(abs_num >> 32)     // Старшие 32 бита
    };
    size = 2;
    // Автоматическое удаление ведущих нулей
    if (data[1] == 0) {
        size = 1;
        uint32_t* new_data = new uint32_t[1]{ data[0] };
        delete[] data;
        data = new_data;
    }
}

BigInteger::BigInteger(long long num) {
    is_negative = (num < 0);
    uint64_t abs_num = static_cast<uint64_t>(is_negative ? -num : num);
    // Всегда создаем массив из 2 элементов
    data = new uint32_t[2]{
        static_cast<uint32_t>(abs_num),
        static_cast<uint32_t>(abs_num >> 32)
    };
    size = 2;
    // Автоматическое удаление ведущих нулей
    if (data[1] == 0) {
        size = 1;
        uint32_t* new_data = new uint32_t[1]{ data[0] };
        delete[] data;
        data = new_data;
    }
}

BigInteger::BigInteger(unsigned long num) : is_negative(false) {
    uint64_t abs_num = static_cast<uint64_t>(num);
    data = new uint32_t[2]{
        static_cast<uint32_t>(abs_num),
        static_cast<uint32_t>(abs_num >> 32)
    };
    size = (data[1] == 0) ? 1 : 2; // Удаление ведущих нулей
}

BigInteger::BigInteger(unsigned long long num) : is_negative(false) {
    uint64_t abs_num = static_cast<uint64_t>(num);
    data = new uint32_t[2]{
        static_cast<uint32_t>(abs_num),
        static_cast<uint32_t>(abs_num >> 32)
    };
    size = (data[1] == 0) ? 1 : 2; // Удаление ведущих нулей
}

BigInteger::BigInteger(const BigInteger& other) : is_negative(other.is_negative) {
    data = new uint32_t[other.size];
    for (size_t i = 0; i < other.size; ++i) {
        data[i] = other.data[i];
    }
    size = other.size;
}

BigInteger::BigInteger(BigInteger&& other) noexcept : data(other.data), size(other.size) {
    other.data = nullptr;
    other.size = 0;
}

BigInteger::BigInteger(std::string str, biginteger_base base) {
    if (str.empty()) {
        throw std::invalid_argument("String is empty");
    }
    if (str[0] == '-') {
        is_negative = true;
        str.erase(0, 1);
    }
    else if (str[0] == '+') {
        is_negative = false;
        str.erase(0, 1);
    }
    else {
        is_negative = false;
    }
    bool isZero = false;
    size_t i = 0;
    while (str[i] == '0') {
        ++i;
        isZero = true;
    }
    str.erase(0, i);
    if (str.empty()) {
        if (isZero) {
            data = new uint32_t[1]{ 0 };
            size = 1;
            is_negative = false;
            return;
        }
        throw std::invalid_argument("There is not enough information in the line");
    }
    switch (base)
    {
    case biginteger_base::dec: {
        create_from_dec_string(str);
        break;
    }
    case biginteger_base::hex: {
        create_from_hex_string(str);
        break;
    }
    default: {
        throw std::invalid_argument("Unsupported base");
    }
    }
}

BigInteger::~BigInteger() {
    delete[] data;
}

//------------------------------------------------О П Е Р А Т О Р Ы----------------------------------------------------------------------
//Оператор копирования
BigInteger& BigInteger::operator=(const BigInteger& other) {
    if (this != &other) {
        delete[] data;
        data = new uint32_t[other.size];
        for (int i = 0; i < other.size; ++i) {
            data[i] = other.data[i];
        }
        size = other.size;
        is_negative = other.is_negative;
    }
    return *this;
}

//Оператор перемещения
BigInteger& BigInteger::operator=(BigInteger&& other) noexcept {
    if (*this != other) {
        delete[] data;
        data = other.data;
        size = other.size;
        is_negative = other.is_negative;
        other.size = 0;
        other.data = nullptr;
    }
    return *this;
}

//Унарные операторы
BigInteger BigInteger::operator-() const {
    BigInteger result(*this);
    if (*this != BigInteger(0)) {
        result.set_is_negative(!is_negative);
    }
    return result;
}

BigInteger BigInteger::operator+() const {
    return *this;
}


//Операторы сравнения
bool BigInteger::operator!=(const BigInteger& other) const {
    return !(*this == other);
}
bool BigInteger::operator<(const BigInteger& other) const {
    return isLess(*this, other);
}
bool BigInteger::operator<=(const BigInteger& other) const {
    return isLess(*this, other) || (*this == other);
}
bool BigInteger::operator>(const BigInteger& other) const {
    return isLess(other, *this);
}
bool BigInteger::operator>=(const BigInteger& other) const {
    return !isLess(*this, other);
}


//Инкремент и декремент
BigInteger& BigInteger::operator++() { //Префиксный 
    *this += BigInteger(1);
    return *this;
}
BigInteger BigInteger::operator++(int) { //Постфиксный
    BigInteger temp = *this;
    ++*this;
    return temp;
}
BigInteger& BigInteger::operator--() { //Префиксный 
    *this -= BigInteger(1);
    return *this;
}
BigInteger BigInteger::operator--(int) { //Постфиксный
    BigInteger temp = *this;
    --*this;
    return temp;
}


//Арифмитические операторы
BigInteger& BigInteger::operator+=(const BigInteger& other) {
    if (is_negative == other.is_negative) {  //Новые данные для суммы
        size_t max_size = std::max(size, other.get_size());
        uint32_t* new_data = new uint32_t[max_size + 1]();
        uint64_t carry = 0;
        uint64_t sum = 0;
        size_t i;
        for (i = 0; i < max_size; ++i) {
            //Выбираем 0, ечли число закончилось
            uint64_t a = (i < size) ? data[i] : 0;
            uint64_t b = (i < other.get_size()) ? other.get_data()[i] : 0;
            //Находим сумму с учетом переноса
            sum = a + b + carry;
            //Записываем в итоговый массив суженное значение суммы
            new_data[i] = static_cast<uint32_t>(sum);
            //Перенос в старшие биты
            carry = sum >> 32;
        }
        if (carry != 0) {
            new_data[max_size] = static_cast<uint32_t>(carry);
            size = max_size + 1;
        }
        else {
            size = max_size;
        }
        // Удаление ведущих нулей
        while (size > 1 && new_data[size - 1] == 0) {
            --size;
        }
        delete[] data;
        data = new_data;
    }
    else {
        BigInteger first = *this;
        first.set_is_negative(false);
        BigInteger second = other;
        second.set_is_negative(false);
        if (first > second) {
            // |this| > |other|
            absolute_difference(data, size, other.get_data(), other.get_size());
        }
        else {
            // |this| <= |other|
            size_t temp_size = other.get_size();
            uint32_t* temp = new uint32_t[temp_size];
            for (size_t i = 0; i < temp_size; ++i) {
                temp[i] = other.get_data()[i];
            }
            //Ищем абсолютную разницу
            absolute_difference(temp, temp_size, data, size);
            //Удаляем старые данные 
            delete[] data;
            data = temp;
            size = temp_size;
            is_negative = other.get_is_negative();
        }
        //Уберем ведущие нули
        while (size > 1 && data[size - 1] == 0) {
            --size;
        }
        //Дополнительная проверка на 0
        if (size == 1 && data[0] == 0) {
            is_negative = false;
        }
    }
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    BigInteger temp = other;
    temp.set_is_negative(!other.get_is_negative());
    return *this += temp;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    // Обработка знака результата
    bool result_negative = (is_negative != other.is_negative);
    // Работа с абсолютными значениями
    const uint32_t* a_data = data;
    const uint32_t* b_data = other.get_data();
    size_t a_size = size;
    size_t b_size = other.get_size();
    /// 999 * 999 < 998 001 < 1 000 000 (любое n значное число * на любое m значное < n + m значное число)
    size_t result_size = a_size + b_size;
    uint32_t* result_data = new uint32_t[result_size](); // Инициализация нулями
    // Умножение в столбик
    for (size_t i = 0; i < a_size; ++i) {
        uint64_t carry = 0; // Перенос для текущего разряда
        for (size_t j = 0; j < b_size; ++j) {
            // Произведение текущих разрядов + перенос + текущее значение в result
            uint64_t product = (uint64_t)a_data[i] * b_data[j];
            uint64_t sum = product + result_data[i + j] + carry;
            // Обновление текущего разряда
            result_data[i + j] = static_cast<uint32_t>(sum);
            // Вычисление нового переноса
            carry = sum >> 32;
        }
        // Добавление оставшегося переноса в старшие разряды
        if (carry > 0) {
            result_data[i + b_size] += static_cast<uint32_t>(carry);
        }
    }
    // Обновление данных текущего объекта
    delete[] data;
    data = result_data;
    size = result_size;
    is_negative = result_negative;
    // Удаление ведущих нулей
    while (size > 1 && data[size - 1] == 0) {
        --size;
    }
    // Обработка нуля
    if (size == 1 && data[0] == 0) {
        is_negative = false;
    }
    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& divisor) {
    // Обработка знака результата
    bool result_negative = (is_negative != divisor.is_negative);
    // Проверка деления на ноль
    if (divisor.isZero()) {
        throw std::invalid_argument("Division by zero");
    }
    // Работа с модулями
    BigInteger dividend = this->abs();
    BigInteger divisor_abs = divisor.abs();
    if (dividend < divisor_abs) {
        *this = BigInteger(0);
        is_negative = false;
        return *this;
    }
    // Частное и остаток
    BigInteger quotient = 0;
    BigInteger remainder = 0;
    for (long long i = static_cast<long long>(dividend.size) - 1; i >= 0; --i) {
        // Сдвиг остатка и добавление текущего блока
        remainder *= BigInteger(0x100000000);
        remainder += BigInteger(dividend.data[i]);
        // Бинарный поиск q: divisor_abs * q <= remainder
        uint32_t q_low = 0;
        uint32_t q_high = 0xFFFFFFFF;
        uint32_t q = 0;
        BigInteger product;
        while (q_low <= q_high) {
            uint32_t mid = q_low + (q_high - q_low) / 2;
            product = divisor_abs;
            product *= BigInteger(mid); // Используем *=
            if (product <= remainder) {
                q = mid;
                q_low = mid + 1;
            }
            else {
                q_high = mid - 1;
            }
        }
        // Обновление остатка и частного
        product = divisor_abs;
        product *= BigInteger(q);
        remainder -= product;
        quotient *= BigInteger(0x100000000);
        quotient += BigInteger(q);
    }
    // Удаление ведущих нулей
    while (quotient.size > 1 && quotient.data[quotient.size - 1] == 0) {
        quotient.size--;
    }
    // Обновление результата
    quotient.is_negative = result_negative;
    *this = quotient;
    return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
    if (other.isZero()) {
        throw std::invalid_argument("Modulo by zero");
    }
    BigInteger temp = *this;
    temp /= other;
    temp *= other;
    *this -= temp;
    if (*this == 0) {
        is_negative = false;
    }
    return *this;
}

//------------------------------------------------Г Е Т Т Е Р Ы   И   С Е Т Т Е Р Ы---------------------------------------------------------
//Геттеры
bool BigInteger::get_is_negative() const {
    return is_negative;
}
size_t BigInteger::get_size() const {
    return size;
}
const uint32_t* BigInteger::get_data() const {
    return data;
}

//Сеттер
void BigInteger::set_is_negative(bool set_value) {
    is_negative = set_value;
}
//---------------------------------------------Д О П О Л Н И Т Е Л Ь Н О--------------------------------------------------------------
BigInteger BigInteger::abs() const {
    BigInteger temp = *this;
    temp.is_negative = false;
    return temp;
}

bool BigInteger::isZero() const {
    return (*this == 0);
}

int BigInteger::divide_by_10() {
    if (size == 1 && data[0] == 0) {
        return 0;
    }
    uint64_t remainder = 0; //Остаток 64 бита, чтобы учитывать перенос от старшего блока к младшему на 32 бита
    for (size_t i = size; i-- > 0;) {
        //data[i] последовательно делится на 10, но с учётом остатка от предыдущего деления
        uint64_t current = (remainder << 32) + data[i];
        uint64_t quotient = current / 10; //Значение , которое будет записано в data[i]
        remainder = current % 10;
        data[i] = static_cast<uint32_t>(quotient);
    }
    size_t new_size = size;
    while (new_size > 0 && data[new_size - 1] == 0) {
        --new_size;
    }
    if (new_size == 0) {
        delete[] data;
        data = new uint32_t[1]{ 0 };
        size = 1;
        is_negative = false;
        return static_cast<int>(remainder);
    }
    if (size != new_size) {
        uint32_t* new_data = new uint32_t[new_size];
        for (size_t i = 0; i < new_size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        size = new_size;
    }
    return static_cast<int>(remainder);
}

int BigInteger::divide_by_16() {
    if (size == 1 && data[0] == 0) {
        return 0;
    }
    uint64_t remainder = 0; //Остаток 64 бита, чтобы учитывать перенос от старшего блока к младшему на 32 бита
    for (size_t i = size; i-- > 0;) {
        //data[i] последовательно делится на 10, но с учётом остатка от предыдущего деления
        uint64_t current = (remainder << 32) + data[i];
        uint64_t quotient = current / 16; //Значение , которое будет записано в data[i]
        remainder = current % 16;
        data[i] = static_cast<uint32_t>(quotient);
    }
    size_t new_size = size;
    while (new_size > 0 && data[new_size - 1] == 0) {
        --new_size;
    }
    if (new_size == 0) {
        delete[] data;
        data = new uint32_t[1]{ 0 };
        size = 1;
        is_negative = false;
        return static_cast<int>(remainder);
    }
    if (size != new_size) {
        uint32_t* new_data = new uint32_t[new_size];
        for (size_t i = 0; i < new_size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        size = new_size;
    }
    return static_cast<int>(remainder);
}

bool BigInteger::isLess(const BigInteger& left, const BigInteger& right) {
    if (left == right) {
        return false;
    }
    if (left.get_is_negative() != right.get_is_negative()) {
        return left.get_is_negative();
    }
    if (left.get_is_negative()) {
        return !isAbsLess(left, right);
    }
    return isAbsLess(left, right);
}

//-----------------------------------------------------------В Н Е Ш Н И Е----------------------------------------------------------------

//Бинарный оператор сравнения
bool operator==(const BigInteger& left, const BigInteger& right) {
    if (left.get_is_negative() != right.get_is_negative() || left.get_size() != right.get_size()) {
        return false;
    }
    for (size_t i = 0; i < left.get_size(); ++i) {
        if (left.get_data()[i] != right.get_data()[i]) {
            return false;
        }
    }
    return true;
}

//Перевод BigInteger в строку
std::string to_string(const BigInteger& b_int) {
    if (b_int.isZero()) {
        return "0";
    }
    BigInteger temp = b_int.abs();
    std::string result;
    result.reserve(b_int.get_size() * 10);
    while (!temp.isZero()) {
        int digit = (temp.divide_by_10());
        result += std::to_string(digit);
    }
    if (b_int.get_is_negative()) {
        result += '-';
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::string to_hex(const BigInteger& b_int) {
    if (b_int.isZero()) {
        return "0";
    }
    BigInteger temp = b_int.abs();
    std::string result;
    const char alphabet[] = "0123456789ABCDEF";
    result.reserve(b_int.get_size() * 10);
    while (!temp.isZero()) {
        int digit = (temp.divide_by_16());
        result += alphabet[digit];
    }
    if (b_int.get_is_negative()) {
        result += '-';
    }
    std::reverse(result.begin(), result.end());
    return result;
}
//Бинарные операторы сравнения


//Бинарные арифметические операторы
BigInteger operator+(const BigInteger& left, const BigInteger& right) {
    BigInteger result = left;
    return result += right;
}
BigInteger operator-(const BigInteger& left, const BigInteger& right) {
    BigInteger result = left;
    return result -= right;
}
BigInteger operator*(const BigInteger& left, const BigInteger& right) {
    BigInteger result = left;
    return result *= right;
}
BigInteger operator/(const BigInteger& left, const BigInteger& right) {
    BigInteger result = left;
    return result /= right;
}
BigInteger operator%(const BigInteger& left, const BigInteger& right) {
    if (right.isZero()) {
        throw std::invalid_argument("Modulo by zero");
    }
    BigInteger temp = left / right;
    BigInteger remainder = left - temp * right;
    if (remainder == 0) {
        remainder.set_is_negative(false);
    }
    else {
        remainder.set_is_negative(left.get_is_negative());
    }
    return remainder;
}
