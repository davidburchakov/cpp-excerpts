# Parsing

There's a myriad of ways to implement input parsing. All methods have their strengths and weaknesses, depending on C++ versions, coding styles and industries.

## I.

I'm fond of an implementation that uses C++98's ***std::strtol***(*const char* str*, *const char **str_end*, *int base*)

~~~
bool str_to_int98(const std::string &line, int &res) noexcept
~~~
function allows us to log what exactly was wrong in the input and handle cases of
* empty input
* nonnumeric input
* numeric input that is followed by trailing nonnumeric garbage
* overflow for long
* overflow for int

~~~
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstdio>

bool str_to_int98(const std::string &line, int &res) noexcept {
    char *end = nullptr;
    errno = 0;
    const long value = std::strtol(line.c_str(), &end, 10);
    if (line.empty()) {
        std::fputs("line is empty!\n", stderr);
    } else if (end == line.c_str()) {
        std::fprintf(stderr, "did not recognize even a single valid digit at the start of the string.\n");
    } else if (*end != '\0') {
        std::fputs("numeric result followed by trailing nonnumeric character(s)\n", stderr);
    } else if (errno == ERANGE) {
        std::fputs("Overflow or underflow for long\n", stderr);
    } else if (value < INT_MIN || value > INT_MAX) {
        std::fprintf(stderr, "Out of range for int\n");
    } else {
        // everything's alright
        res = static_cast<int>(value);
        return true;
    }
    return false;
}
~~~

It uses global ***errno*** variable to detect errors in C library calls.

And allows correct conversion without throwing exceptions.

using ***std::fputs*** and ***std::fprintf*** over ***std::cerr*** allows us to guarantee exception-safety and mark the function as ***noexcept***.

Because ***std::cerr/cout*** can potentially raise an exception, ***std::fputs/fprintf*** - can't

a [similar](https://en.cppreference.com/w/c/string/byte/strtof.html) function for double that uses std::strtod(const char *str. const char **str_end);

~~~
    bool str_to_double98(const std::string &line, double &res) noexcept {
        char *end = nullptr;
        errno = 0;
        const double value = std::strtod(line.c_str(), &end);
        if (line.empty()) {
            std::fputs("line is empty!\n", stderr);
        } else if (end == line) {
            std::fputs("No characters consumed. Did not recognize even "
                       "a single valid digit at the start of the string\n", stderr);
        } else if (*end != '\0') {
            std::fprintf(stderr, "numeric result followed by trailing nonnumeric character(s)\n");
        } else if (errno == ERANGE) {
            std::fprintf(stderr, "Overflow or underflow for double\n");
        } else {
            // everything's alright
            res = value;
            return true;
        }
        return false;
    }
~~~


## II. 
scanf
// TODO

## III. 
std::atoi
std::stoi
// TODO

## IV.
Streamstring
// TODO 

## V.
C++17
std::from_chars

~~~
    bool str_to_int17(const std::string &line, int &res) {
        if (line.empty()) {
            return false;
        }

        const char* begin = line.data();
        const char* end   = begin + line.size();

        auto [ptr, ec] = std::from_chars(begin, end, res, 10);

        // No characters consumed or trailing garbage
        if (ptr != end) {
            return false;
        }

        // Invalid input or overflow
        if (ec != std::errc()) {
            return false;
        }

        return true;
    }
~~~

// TODO

## Important links
[benchmark](https://quick-bench.com/q/GBzK53Gc-YSWpEA9XskSZLU963Y)

[what not to do](https://stackoverflow.com/questions/194465/how-to-parse-a-string-to-an-int-in-c/6154614#6154614)

[how to convert str to in stackoverflow thread](https://stackoverflow.com/questions/7663709/how-can-i-convert-a-stdstring-to-int)


