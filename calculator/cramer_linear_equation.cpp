#include <vector>
#include <string>
#include <iostream>

enum class status_code { SUCCESS, FAILURE, EMPTY_LINE, UNPARSABLE_INPUT, TRAILING_GARBAGE, OVERFLOW, INVALID_FORMAT };
namespace auxiliary {
    static status_code str_to_double(const std::string &line, double &res) noexcept {
        double value = 0.0;
        char *end = nullptr;
        errno = 0;
        value = std::strtod(line.c_str(), &end);
        if (line.empty()) {
            return status_code::EMPTY_LINE;
        } else if (end == line.c_str()) {
            return status_code::UNPARSABLE_INPUT;
        } else if(*end != '\0') {
            // return status_code::TRAILING_GARBAGE;
            // we actually expect input followed by trailing nonnumerical character(s)
            // e.g., 2x^2
        } else if (errno == ERANGE) {
            return status_code::OVERFLOW; // overflow for long? or double? or anything depending on value?
        }
        res = value;
        return status_code::SUCCESS;
    }

    static std::vector<std::string> tokenize(const std::string &line) noexcept {
        std::vector<std::string> tokens;
        int i = 0, j = 0;
        std::string token;
        while (i < line.size()) {
            if (line[i] == ' ') {
                token = line.substr(j, i-j);
                tokens.push_back(token);
                j = i;
            }
            i++;
        }

        if (j < line.size()) {
            token = line.substr(j, i-j);
            tokens.push_back(token);
        }

        return tokens;
    }
} namespace aux = auxiliary;

class Cramer {
    // a1b2 - a2b1
    [[nodiscard]] static double determinant(const double a1, const double a2, const double b1, const double b2) noexcept {
        return (a1*b2 - a2*b1);
    }
public:
    double a1, a2;
    double b1, b2;
    double c1, c2;
    Cramer(double a1 = 0.0, double a2 = 0.0, double b1 = 0.0, double b2 = 0.0, double c1 = 0.0, double c2 = 0.0)
        : a1(a1), a2(a2), b1(b1), b2(b2), c1(c1), c2(c2) { }

    Cramer(const std::string &line1, const std::string &line2) {
        if (parse(line1, a1, b1, c1) != status_code::SUCCESS) {
            throw std::runtime_error("Unable to parse!");
        }
        if (parse(line2, a2, b2, c2) != status_code::SUCCESS) {
            throw std::runtime_error("Unable to parse!");
        }
    }

    static status_code parse(const std::string &line, double &a, double &b, double &c) noexcept {
        std::vector<std::string> tokens = aux::tokenize(line);
        if (tokens.size() < 5) {
            return status_code::INVALID_FORMAT;
        }

        std::string fst_str = tokens[0];
        double fst_double = 0.0;
        status_code result_code = aux::str_to_double(fst_str, fst_double);
        if (result_code != status_code::SUCCESS) {
            return status_code::FAILURE;
        }
        a = fst_double;

        std::string snd_str = tokens[2];
        double snd_double = 0.0;
        result_code = aux::str_to_double(snd_str, snd_double);
        if (result_code != status_code::SUCCESS) {
            return status_code::FAILURE;
        }
        b = (tokens[1] == "-") ? (-snd_double) : snd_double;

        std::string thrd_str = tokens[4];
        double thrd_double = 0.0;
        result_code = aux::str_to_double(thrd_str, thrd_double);
        if (result_code != status_code::SUCCESS) {
            return status_code::FAILURE;
        }
        c = thrd_double;
        return status_code::SUCCESS;
    }

    status_code solve(double &x, double &y) const noexcept {
        const double d_ab = determinant(a1, a2, b1, b2);
        const double d_cb = determinant(c1, c2, b1, b2);
        const double d_ac = determinant(a1, a2, c1, c2);
        if (d_cb == 0.0 || d_ac == 0.0) {
            return status_code::FAILURE;
            // throw std::runtime_error("Zero division!");
        }
        x = d_cb/d_ab;
        y = d_ac/d_ab;
        return status_code::SUCCESS;
    }
};

int main(int argc, char *argv[]) {
    std::string line1 = "5x + 6y = 10";
    std::string line2 = "10x + 10y = -8";
    Cramer cramer_parser(line1, line2);

    std::cout << cramer_parser.a1 << " " << cramer_parser.b1 << " " << cramer_parser.c1 << "\n";
    std::cout << cramer_parser.a2 << " " << cramer_parser.b2 << " " << cramer_parser.c2 << "\n";

    double x = 0.0, y = 0.0;
    cramer_parser.solve(x, y);
    std::cout << "solutions to problem:\n" << x << "\n" << y << std::endl;
}

