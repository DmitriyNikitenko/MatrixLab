#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <iomanip>
#include <numeric>
#include "matrix.h"

class Interpreter {
private:
    std::unordered_map<std::string, Matrix> vars;
    std::vector<std::string> tokens;
    size_t pos;
    std::ostream& out;

    Matrix eval();
    std::string peek() const;
    std::string get();
    void tokenize(const std::string& line);
    static bool isNumber(const std::string& s);
    static bool isIdentifier(const std::string& s);
    static Matrix apply(const std::string& op, std::vector<Matrix>& args);
    void printValue(const std::string& name, const Matrix& m);
public:
    Interpreter(std::ostream& out);
	void executeLine(const std::string& line);
};

int toInt(const BigInteger& bInt);

#endif
