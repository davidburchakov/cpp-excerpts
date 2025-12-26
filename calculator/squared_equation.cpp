#include <cmath>
#include <limits>
#include <iostream>
#include <vector>
#include <string>
enum class parse_code { SUCCESS, EMPTY_LINE, UNPARSABLE_INPUT, TRAILING_GARBAGE, OVERFLOW_LONG, OVERFLOW_INT, FAILURE };
namespace auxiliary {
    parse_code str_to_int(const std::string &line, int &res) {
        char *end = nullptr;
        errno = 0;
        long value = std::strtol(line.c_str(), &end, 10);
        if (line.empty()) {
            return parse_code::EMPTY_LINE;
        } else if (end == line) {
            return parse_code::UNPARSABLE_INPUT;
        } else if (*end == '\0') {
            // return parse_code::TRAILING_GARBAGE;
            // we actually expect input with trailing garbage!
        } else if (errno == ERANGE) {
            return parse_code::OVERFLOW_LONG;
        } else if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max()) {
            return parse_code::OVERFLOW_INT;
        }

        res = static_cast<int>(value);
        return parse_code::SUCCESS;
    }
} namespace aux = auxiliary;

class Equation {
    std::vector<std::string> tokenize(const std::string &line) {
        std::vector<std::string> tokens;
        std::string token;
        for (char c : line) {
            if (c == ' ') {
                if (!token.empty()) tokens.push_back(token);
                token.clear();
            } else {
                token += c;
            }
        }
        if (!token.empty()) tokens.push_back(token);
        return tokens;
    }
public:
    int fst;
    int snd;
    int thrd;
    Equation(double fst = 0, double snd = 0, double thrd = 0): fst(fst), snd(snd), thrd(thrd) { }
    Equation(const std::string &line) {
        int a = 0, b = 0, c = 0;
        parse_code parse_res = parse_equation(line, a, b, c);
        this->fst = a;
        this->snd = b;
        this->thrd = c;
    }

    parse_code parse_equation(const std::string &line, int &a, int &b, int &c) {
        std::vector<std::string> tokens = tokenize(line);

        std::string fst_str = tokens[0];
        int fst_int = 0;
        if (aux::str_to_int(fst_str, fst_int) != parse_code::SUCCESS) {
            std::cerr << "error occurred!: " << fst_str <<  std::endl;
            return parse_code::FAILURE;
        }

        std::string snd_str = tokens[2];
        int snd_int = 0;
        if (aux::str_to_int(snd_str, snd_int) != parse_code::SUCCESS) {
            std::cerr << "error occurred!: " << snd_str <<  std::endl;
            return parse_code::FAILURE;
        }

        std::string thrd_str = tokens[4];
        int thrd_int = 0;
        if (aux::str_to_int(thrd_str, thrd_int) != parse_code::SUCCESS) {
            std::cerr << "error occurred!: " << thrd_str <<  std::endl;
            return parse_code::FAILURE;
        }
        a = fst_int;
        b = (tokens[1] == "-") ? (-snd_int) : snd_int;
        c = (tokens[3] == "-") ? (-thrd_int) : thrd_int;
        std::cout << "Successfully parsed the equation! " << line << std::endl;
        return parse_code::SUCCESS;
    }

    /*
     * Discriminant = b^2 - 4ac
     * x1,x2 = (-b +- sqrt(D)) / 2a
     */
    static parse_code calculate_squared_equation(const int a, const int b, const int c, double &x1, double &x2) {
        const int D = b*b - 4*a*c;
        try {
            x1 = (-b + std::sqrt(D)) / 2*a;
            x2 = (-b - std::sqrt(D)) / 2*a;
        } catch (std::exception &ex) {
            std::cerr << ex.what() << std::endl;
            return parse_code::FAILURE;
        }
        return parse_code::SUCCESS;
    }

    parse_code calculate_squared_equation(double &x1, double &x2) const {
        const int a = this->fst;
        if (a == 0) {
            return parse_code::FAILURE;
        }
        const int b = this->snd;
        const int c = this->thrd;
        try {
            const int D = (b*b) - (4*a*c);
            if (D < 0) {
                return parse_code::FAILURE;
            }
            x1 = ((-b) + std::sqrt(D)) / (2*a);
            x2 = ((-b) - std::sqrt(D)) / (2*a);
        } catch (std::exception &ex) {
            std::cerr << ex.what() << std::endl;
            return parse_code::FAILURE;
        }
        return parse_code::SUCCESS;
    }

};


int main(int argc, char *argv[]) {
    std::string line = "2x^2 + 5x - 10";
    Equation eq1(line);
    std::cout << "a: " << eq1.fst << " b: " << eq1.snd << " c: " << eq1.thrd << std::endl;
    double x1 = 0.0, x2 = 0.0;
    eq1.calculate_squared_equation(x1, x2);
    std::cout << "for: " << line << "\nx1: " << x1 << " x2: " << x2 << std::endl;
}

