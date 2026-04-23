#include "../include/interpreter.h"

Interpreter::Interpreter(std::ostream& out)
    : pos(0), out(out) {}
void Interpreter::executeLine(const std::string& line) {
    tokenize(line);
    pos = 0;
    eval();
    if (pos != tokens.size()) {
        throw std::runtime_error("Extra tokens");
    }
}
void Interpreter::tokenize(const std::string& line) {
    tokens.clear();
    std::string cur;
    for (char c : line) {
        if (std::isspace(c) != 0) {
            if (!cur.empty()) {
                tokens.push_back(cur);
                cur.clear();
            }
        }
        else if (c == '(' || c == ')') {
            if (!cur.empty()) {
                tokens.push_back(cur);
                cur.clear();
            }
            tokens.emplace_back(1, c);
        }
        else {
            cur += c;
        }
    }
    if (!cur.empty()) {
        tokens.push_back(cur);
    }
}
std::string Interpreter::peek() const {
    if (pos >= tokens.size()) {
        throw std::runtime_error("Unexpected end");
    }
    return tokens[pos];
}
std::string Interpreter::get() {
    if (pos >= tokens.size()) {
        throw std::runtime_error("Unexpected end");
    }
    return tokens[pos++];
}
bool Interpreter::isNumber(const std::string& s) {
    if (s.empty()) { return false; }
    return std::isdigit(s[0]) != 0 || s[0] == '-';
}
bool Interpreter::isIdentifier(const std::string& s) {
    if (std::isalpha(s[0]) == 0) { return false; }
    for (size_t i = 0; i < s.size(); ++i) {
        if (std::isalnum(s[i]) == 0) { return false; }
    }
    return true;
}
Matrix Interpreter::eval() {
    std::string t = get();
    if (t == "(") {
        std::string op = get();
        if (op == "=") {
            std::string name = get();
            if (!isIdentifier(name)) {
                throw std::runtime_error("Invalid identifier");
            }
            Matrix value = eval();
            if (get() != ")") {
                throw std::runtime_error("Expected )");
            }
            vars[name] = value;
            return value;
        }
        if (op == "disp") {
            std::string name = "ans";
            if (isIdentifier(peek())) {
                name = peek();
            }
            Matrix value = eval();
            if (get() != ")") {
                throw std::runtime_error("Expected )");
            }
            printValue(name, value);
            return value;
        }
        std::vector<Matrix> args;
        while (peek() != ")") {
            args.push_back(eval());
        }
        get();
        return apply(op, args);
    }
    if (isNumber(t)) {
        Fraction f(t);
        return Matrix(1, 1, std::vector<Fraction>{f});
    }
    if (isIdentifier(t)) {
        if (vars.find(t) == vars.end()) {
            throw std::runtime_error("Unknown variable");
        }
        return vars[t];
    }

    throw std::runtime_error("Invalid token");
}
Matrix Interpreter::apply(const std::string& op, std::vector<Matrix>& args) {
    if (op == "+") {
        if (args.size() != 2) {
            throw std::runtime_error("+ expects exactly 2 arguments");
        }
        return args[0] + args[1];
    }
    if (op == "-") {
        if (args.size() == 1) {
            return -args[0];
        }
        if (args.size() == 2) {
            return args[0] - args[1];
        }
        throw std::runtime_error("- expects 1 or 2 arguments");
    }
    if (op == "*") {
        if (args.size() != 2) {
            throw std::runtime_error("* expects exactly 2 arguments");
        }
        return args[0] * args[1];
    }
    if (op == "/") {
        if (args.size() != 2) {
            throw std::runtime_error("/ expects exactly 2 arguments");
        }
        return args[0] / args[1];
    }
    if (op == ".*") {
        if (args.size() != 2) {
            throw std::runtime_error(".* expects exactly 2 arguments");
        }
        return args[0].mul(args[1]);
    }
    if (op == "./") {
        if (args.size() != 2) {
            throw std::runtime_error("./ expects exactly 2 arguments");
        }
        return args[0].div(args[1]);
    }
    if (op == "transpose") {
        if (args.size() != 1) {
            throw std::runtime_error("transpose expects exactly 1 argument");
        }
        Matrix tmp(args[0]);
        tmp.transpose();
        return tmp;
    }
    if (op == "det") {
        if (args.size() != 1) {
            throw std::runtime_error("det expects exactly 1 argument");
        }
        return Matrix(1, 1, std::vector<Fraction>{ args[0].det() });
    }
    if (op == "inv") {
        if (args.size() != 1) {
            throw std::runtime_error("inv expects exactly 1 argument");
        }
        return args[0].inv();
    }
    if (op == "sum") {
        if (args.size() != 1) {
            throw std::runtime_error("sum expects exactly 1 argument");
        }
        return Matrix(1, 1, std::vector<Fraction>{ args[0].sum() });
    }
    if (op == "prod") {
        if (args.size() != 1) {
            throw std::runtime_error("prod expects exactly 1 argument");
        }
        return Matrix(1, 1, std::vector<Fraction>{ args[0].prod() });
    }
    if (op == "zeros" || op == "ones") {
        if (args.size() != 2) {
            throw std::runtime_error(op + " expects exactly 2 arguments");
        }
        if (!args[0].isNumber() || !args[1].isNumber()) {
            throw std::runtime_error(op + ": arguments must be scalars");
        }
        Fraction fr = args[0](0, 0);
        Fraction fc = args[1](0, 0);
        if (!fr.isInteger() || !fc.isInteger()) {
            throw std::runtime_error(op + ": size must be integer");
        }
        int r = toInt(fr.getNumerator());
        int c = toInt(fc.getNumerator());
        if (r < 0 || c < 0) {
            throw std::runtime_error(op + ": size must be non-negative");
        }
        return (op == "zeros") ? zeros(r, c) : ones(r, c);
    }
    if (op == "eye") {
        if (args.size() != 1) {
            throw std::runtime_error("eye expects exactly 1 argument");
        }
        if (!args[0].isNumber()) {
            throw std::runtime_error("eye: argument must be scalar");
        }
        Fraction fn = args[0](0, 0);
        if (!fn.isInteger()) {
            throw std::runtime_error("eye: size must be integer");
        }
        int n = toInt(fn.getNumerator());
        if (n < 0) {
            throw std::runtime_error("eye: size must be non-negative");
        }
        return eye(n, n);
    }
    if (op == "linspace") {
        if (args.size() != 3) {
            throw std::runtime_error("linspace expects exactly 3 arguments");
        }
        if (!args[2].isNumber()) {
            throw std::runtime_error("linspace: n must be scalar");
        }
        Fraction fn = args[2](0, 0);
        if (!fn.isInteger()) {
            throw std::runtime_error("linspace: n must be integer");
        }
        int n = toInt(fn.getNumerator());
        if (n <= 0) {
            throw std::runtime_error("linspace: n must be positive");
        }
        Fraction x0 = args[0](0, 0);
        Fraction x1 = args[1](0, 0);
        return linspace(x0, x1, n);
    }
    if (op == "vertcat") {
        if (args.empty()) {
            throw std::runtime_error("vertcat expects at least 1 argument");
        }
        return vertcat(args);
    }
    if (op == "horzcat") {
        if (args.empty()) {
            throw std::runtime_error("horzcat expects at least 1 argument");
        }
        return horzcat(args);
    }
    if (op == "min") {
        if (args.size() != 2) {
            throw std::runtime_error("min expects exactly 2 arguments");
        }
        return args[0].min(args[1]);
    }
    if (op == "max") {
        if (args.size() != 2) {
            throw std::runtime_error("max expects exactly 2 arguments");
        }
        return args[0].max(args[1]);
    }

    throw std::runtime_error("Unknown operator: " + op);
}
void Interpreter::printValue(const std::string& name, const Matrix& m) {
    out << name << " = ";
    if (m.isNumber()) {
        out << toString(m(0, 0)) << "\n";
        return;
    }
    int rows = m.getRows();
    int cols = m.getCols();

    std::vector<size_t> widths(cols, 0);
    for (int c = 0; c < cols; ++c) {
        for (int r = 0; r < rows; ++r) {
            std::string s = toString(m(r, c));
            widths[c] = std::max(widths[c], s.size());
        }
    }

    out << "[\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            std::string s = toString(m(r, c));
            out << std::setw(static_cast<int>(widths[c]))
                << std::right
                << s;
            if (c + 1 < cols) {
                out << " ";
            }
        }
        out << "\n";
    }
    out << "]\n";
}
int toInt(const BigInteger& bInt) {
    if (bInt.get_is_negative()) {
        throw std::runtime_error("Expected non-negative integer");
    }

    std::string s = to_string(bInt);
    long long v = std::stoll(s);
    if (v > std::numeric_limits<int>::max()) {
        throw std::runtime_error("Integer value is too large");
    }

    return static_cast<int>(v);
}

