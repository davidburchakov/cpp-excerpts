#include <cstdlib>
#include <cerrno>
#include <cmath>
#include <limits>
#include <iostream>

enum class status_code { SUCCESS, FAILURE, EMPTY_LINE, UNPARSABLE_INPUT, TRAILING_GARBAGE, OVERFLOW_LONG, OVERFLOW_INT };
status_code str_to_int(const std::string &line, int &res) noexcept {
    char *end = nullptr;
    errno = 0;
    const long value = strtol(line.c_str(), &end, 10);
    if (line.empty()) {
        return status_code::EMPTY_LINE;
    } else if (end == line.c_str()) {
        return status_code::UNPARSABLE_INPUT;
    } else if (*end != '\0') {
        return status_code::TRAILING_GARBAGE;
    } else if (errno == ERANGE) {
        return status_code::OVERFLOW_LONG;
    } else if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max()) {
        return status_code::OVERFLOW_INT;
    }
    res = static_cast<int>(value);
    return status_code::SUCCESS;
}

void handle_square() {
    std::string line;
    int x;
    std::cout << "Input x of a square: " << std::endl;
    std::getline(std::cin, line);
    status_code result_code = str_to_int(line, x);
    if (result_code != status_code::SUCCESS) {
        throw std::runtime_error("some problem occurred!");
    }
    int max_safe = static_cast<int>(std::sqrt(std::numeric_limits<int>::max()));
    if (x < -max_safe || x > max_safe) {
        throw std::overflow_error("int may overflow!");
    }
    if (x == 0) {
        throw std::runtime_error("Side cannot be 0!");
    }
    int square_area = x * x;
    std::cout << "Area of a square is: " << square_area << std::endl;
}

void handle_rectangle() {
    std::string line1, line2;
    int a = 0, b = 0;
    std::cout << "Input a of a rectangle: ";
    if (!std::getline(std::cin, line1)) {
        throw std::runtime_error("EOF occurred!");
    }
    if (str_to_int(line1, a) != status_code::SUCCESS) {
        throw std::runtime_error("Error occurred during str to int conversion!");
    }
    
    std::cout << "Input b of a rectangle: ";
    if (!std::getline(std::cin, line2)) {
        throw std::runtime_error("EOF occurred!");
    }

    if (str_to_int(line2, b) != status_code::SUCCESS) {
        throw std::runtime_error("Error occurred during str to int conversion!");
    }

    if (a != 0) {
        if ((b > std::numeric_limits<int>::max() / a) ||
            (b < std::numeric_limits<int>::min() / a)) {
            throw std::overflow_error("rectangle area may overflow!");
        }
    }
    int rectangle_area = a * b;
    std::cout << "area of a rectangle: " << rectangle_area << std::endl;
}

void handle_triangle() {
    std::string line1, line2, line3;
    int a = 0, b = 0, angle_deg = 0;
    std::cout << "Enter a:";
    if (!std::getline(std::cin, line1)) {
        throw std::runtime_error("EOF occurred! / ctrl + D");
    }
    if (str_to_int(line1, a) != status_code::SUCCESS) {
        throw std::runtime_error("Error occurred during conversion!");
    }

    std::cout << "Enter b:";
    if (!std::getline(std::cin, line2)) {
        throw std::runtime_error("EOF occurred! / ctrl + D");
    }
    if (str_to_int(line2, b) != status_code::SUCCESS) {
        throw std::runtime_error("Error occurred during parsing str to int!");
    }

    std::cout << "Enter sinus alpha: ";
    if (!std::getline(std::cin, line3)) {
        throw std::runtime_error("EOF occurred! / ctrl + D");
    }

    if (str_to_int(line3, angle_deg) != status_code::SUCCESS) {
        throw std::runtime_error("Error occurred during parsing str to int!");
    }
    constexpr double pi = 3.14159265358979323846;
    double angle_rad = static_cast<double>(angle_deg) * pi / 180.0;

    // area = 1/2*a*b*sin(a)
    double area_triangle = static_cast<double>(a) * static_cast<double>(b) * std::sin(angle_rad) * 0.5;
    if (!std::isfinite(area_triangle)) {
        throw std::overflow_error("Double overflowed!");
    }
    std::cout << "Area of the triangle is: " << area_triangle << std::endl;
}

int main() {
    std::string line;
    std::cout << "square(1)\trectangle(2)\ttriangle(3)" << std::endl;
    if (!std::getline(std::cin, line)) {
        throw std::runtime_error("EOF occurred!");
    }
    int num_case = 0;
    status_code result_code = str_to_int(line, num_case);
    if (result_code != status_code::SUCCESS) {
        throw std::runtime_error("error occurred!");
    }

    switch (num_case) {
        case 1:
            handle_square();
            break;
        case 2:
            handle_rectangle();
            break;
        case 3:
            handle_triangle();
            break;
    }
}

