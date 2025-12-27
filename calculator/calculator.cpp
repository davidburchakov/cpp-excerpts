#include <cmath>
#include <iostream>

namespace auxiliary {
    enum class status_code { SUCCESS, FAILURE, EMPTY_LINE, UNPARSABLE_INPUT, TRAILING_GARBAGE, OVERFLOW };
    static status_code str_to_d(const std::string &line, double &res) noexcept {
        char *end = nullptr;
        errno = 0;
        const double value = strtod(line.c_str(), &end);
        if (line.empty()) {
            return status_code::EMPTY_LINE;
        } else if (end == line.c_str()) {
            return status_code::UNPARSABLE_INPUT;
        } else if (*end != '\0') {
            return status_code::TRAILING_GARBAGE;
        } else if (errno == ERANGE) {
            return status_code::OVERFLOW;
        }
        res = value;
        return status_code::SUCCESS;
    }

    static void get_status_log(const status_code &result_code) noexcept {
        switch (result_code) {
            case status_code::EMPTY_LINE:
                std::cerr << "Empty line!" << std::endl;
                break;
            case status_code::UNPARSABLE_INPUT:
                std::cerr << "Unparsable input!" << std::endl;
                break;
            case status_code::TRAILING_GARBAGE:
                std::cerr << "Trailing Garbage!" << std::endl;
                break;
            case status_code::OVERFLOW:
                std::cerr << "double has overflown!" << std::endl;
                break;
            case status_code::SUCCESS:
                std::cout << "Successfully parsed!" << std::endl;
                break;
            default:
                std::cout << "unknown error case" << std::endl;
        }
    }
} namespace aux = auxiliary;

class Calculator {
    double a;
    double b;
public:
    Calculator(const double &a = 0.0, const double &b = 0.0): a(a), b(b) { }
    Calculator(const std::string &line1 = "0", const std::string &line2 = "0") {
        aux::status_code result_code = aux::str_to_d(line1, a);
        aux::get_status_log(result_code);
        if (result_code != aux::status_code::SUCCESS) {
            a = 0.0;
            std::cerr << "Unable to parse line1: " << line1 << std::endl;
        }

        if (aux::str_to_d(line2, b) != aux::status_code::SUCCESS) {
            b = 0.0;
            std::cerr << "Unable to parse line2: " << line2 << std::endl;
        }
    }

    void setA(const double new_a) { a = new_a; }
    void setB(const double new_b) { b = new_b; }

    [[nodiscard]] double addition() const {
        const double res = a + b;
        if (!std::isfinite(res)) {
            throw std::overflow_error("Double has overflown!");
        }
        return res;
    }

    [[nodiscard]] double subtraction() const {
        const double res = a - b;
        if (!std::isfinite(res)) {
            throw std::overflow_error("Double has overflown!");
        }
        return res;
    }

    [[nodiscard]] double division() const {
        if (b == 0.0) {
            throw std::runtime_error("Zero Division!");
        }
        const double res = a / b;
        if (!std::isfinite(res)) {
            throw std::overflow_error("Double has overflown!");
        }
        return res;
    }

    [[nodiscard]] double multiplication() const {
        const double res = a * b;
        if (!std::isfinite(res)) {
            throw std::overflow_error("Double has overflown!");
        }
        return res;
    }
};

static void get_user_input(double &x, double &y) {
    std::string line1, line2;
    aux::status_code result_code = aux::status_code::EMPTY_LINE;
    do {
        std::cout << "Please provide x: \n";
        if (!std::getline(std::cin, line1)) {
            std::cerr << "EOF occurred! (ctrl + D)" << std::endl;
            throw std::runtime_error("EOF occurred!");
        }
        if (line1 == "exit"){ break; }
        result_code = aux::str_to_d(line1, x);
        aux::get_status_log(result_code);
    } while (result_code != aux::status_code::SUCCESS);

    do {
        std::cout << "Please provide y: \n";
        if (!std::getline(std::cin, line2)) {
            std::cerr << "EOF occurred! (Ctrl + D)" << std::endl;
            throw std::runtime_error("User interrupted!");
        }
        if (line2 == "exit") { break; }
        result_code = aux::str_to_d(line2, y);
        aux::get_status_log(result_code);
    } while (result_code != aux::status_code::SUCCESS);
}

int main(){
    double x = 0.0, y = 0.0;
    get_user_input(x, y);
    Calculator calculator(x, y);
    std::cout << calculator.addition() << std::endl;
    std::cout << calculator.subtraction() << std::endl;
    std::cout << calculator.division() << std::endl;
    std::cout << calculator.multiplication() << std::endl;
}
