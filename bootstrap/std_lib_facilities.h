#pragma once

/*
 * Simple "Programming: Principles and Practice using C++ (second edition)"
 * course header to be used for the first few weeks. It provides the most common
 * standard headers (in the global namespace) and minimal exception/error
 * support.
 *
 * Students: please don't try to understand the details of headers just yet. All
 * will be explained. This header is primarily used so that you don't have to
 * understand every concept all at once.
 *
 * By Chapter 10, you don't need this file and after Chapter 21, you'll
 * understand it.
 */

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <forward_list>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <random>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

typedef long Unicode;

using namespace std;

template <class T>
std::string to_string(const T& t) {
    std::ostringstream os;
    os << t;
    return os.str();
}

// Enhanced vector range error reporting
struct Range_error : std::out_of_range {
    int index;
    Range_error(int i)
        : std::out_of_range("Range error: " + std::to_string(i)), index(i) {}
};

// trivially range-checked vector (no iterator checking):
template <class T>
struct Vector : public std::vector<T> {
    using size_type = typename std::vector<T>::size_type;

    using std::vector<T>::vector;  // inheriting constructor

    // rather than return at(i);
    T& operator[](unsigned int i) {
        if (i < 0 || this->size() <= i) throw Range_error(i);
        return std::vector<T>::operator[](i);
    }
    const T& operator[](unsigned int i) const {
        if (i < 0 || this->size() <= i) throw Range_error(i);
        return std::vector<T>::operator[](i);
    }
};

// disgusting macro hack to get a range checked vector:
#define vector Vector

// trivially range-checked string (no iterator checking):
struct String : std::string {
    using size_type = std::string::size_type;
    // using string::string;

    // rather than return at(i);
    char& operator[](unsigned int i) {
        if (i < 0 || size() <= i) throw Range_error(i);
        return std::string::operator[](i);
    }

    const char& operator[](unsigned int i) const {
        if (i < 0 || size() <= i) throw Range_error(i);
        return std::string::operator[](i);
    }
};

namespace std {

template <>
struct hash<String> {
    size_t operator()(const String& s) const { return hash<std::string>()(s); }
};

}  // of namespace std

struct Exit : std::runtime_error {
    Exit() : std::runtime_error("Exit") {}
};

// error() simply disguises throws:
inline void error(const string& s) { throw runtime_error(s); }

inline void error(const string& s, const string& s2) { error(s + s2); }

inline void error(const string& s, int i) {
    std::ostringstream os;
    os << s << ": " << i;
    error(os.str());
}

// needed for binary I/O
template <class T>
char* as_bytes(T& i) {
    void* addr = &i;                  // get the address of the first byte
                                      // of memory used to store the object
    return static_cast<char*>(addr);  // treat that memory as bytes
}

inline void keep_window_open() {
    std::cin.clear();
    std::cout << "Please enter a character to exit\n";
    char ch;
    std::cin >> ch;
}

inline void keep_window_open(string s) {
    if (s == "") return;
    std::cin.clear();
    std::cin.ignore(120, '\n');

    std::string ss;
    do {
        std::cout << "Please enter " << s << " to exit\n";
    } while (std::cin >> ss && ss != s);
}

// error function to be used (only) until error() is introduced in Chapter 5:
// write ``error: s and exit program
inline void simple_error(std::string s) {
    std::cerr << "error: " << s << '\n';
    keep_window_open();  // for some Windows environments
    exit(1);
}

// make std::min() and std::max() accessible on systems with antisocial macros:
#undef min
#undef max

// run-time checked narrowing cast (type conversion). See ???.
template <class R, class A>
R narrow_cast(const A& a) {
    R r = R(a);
    if (A(r) != a) error(string("info loss"));
    return r;
}

// random number generators. See 24.7.

inline int randint(int min, int max) {
    static std::default_random_engine ran;
    return std::uniform_int_distribution<>{min, max}(ran);
}

inline int randint(int max) { return randint(0, max); }

// inline double sqrt(int x) { return sqrt(double(x)); }  // to match C++0x

// container algorithms. See 21.9.

template <typename C>
using Value_type = typename C::value_type;

template <typename C>
using Iterator = typename C::iterator;

template <typename C>
// requires Container<C>()
void sort(C& c) {
    std::sort(c.begin(), c.end());
}

template <typename C, typename Pred>
// requires Container<C>() && Binary_Predicate<Value_type<C>>()
void sort(C& c, Pred p) {
    std::sort(c.begin(), c.end(), p);
}

template <typename C, typename Val>
// requires Container<C>() && Equality_comparable<C,Val>()
Iterator<C> find(C& c, Val v) {
    return std::find(c.begin(), c.end(), v);
}

template <typename C, typename Pred>
// requires Container<C>() && Predicate<Pred,Value_type<C>>()
Iterator<C> find_if(C& c, Pred p) {
    return std::find_if(c.begin(), c.end(), p);
}
