#pragma once

#ifndef _IO_H_
#define _IO_H_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

// https://thecoatlessprofessor.com/programming/cpp/differences-in-primitive-data-types-between-os-x-and-windows/
// replace float by double for portability ?
#ifndef f64
typedef double f64;
#endif

// https://stackoverflow.com/questions/21046177/portability-for-binary-file-in-c
// u64: a crossplatform type,  instead of unsigned...
// https://stackoverflow.com/questions/9278254/c-cross-platform-way-to-define-64bit-unsigned-integer
#ifndef u64
#ifdef WIN32
typedef unsigned __int64 u64;
#else  // !WIN32
typedef unsigned long long u64;
#endif
#endif

#define debugPrint(a) debugPrintFunction(__FUNCTION__, a)

static void debugPrintFunction(std::string const& func, std::string const& text)
{
	std::cout << "[" << func << "] " << text << std::endl;
}

class IO {
public:
  static void IO::write_STRING(std::fstream& file, std::string name) {
    // To write a string, print out its length, and then the char *
    // However, X.size() is a temp, and you cannot write a temp,
    // so you need a new variable.
    u64 Xsize = name.size();
    // The next line is how you write most types:  cast a pointer to a
    // type to a char * and give sizeof the type
    file.write(reinterpret_cast<char*>(&Xsize), sizeof(u64));
    file.write(name.c_str(), Xsize * sizeof(char));
  }
  static std::string IO::read_STRING(std::fstream& file) {
    u64 strSize;
    file.read(reinterpret_cast<char*>(&strSize), sizeof(u64));
    // Cannot read to a string directly, must have buffer intermediate
    std::vector<char> tmpV(strSize);
    file.read(reinterpret_cast<char*>(&tmpV[0]), strSize * sizeof(char));
    std::string result(tmpV.begin(), tmpV.end());
    return result;
  }
  static void IO::write_TAG(std::fstream& file, std::string const& tag) {
    write_STRING(file, tag);
  }
  static std::string IO::read_TAG(std::fstream& file) {
    return read_STRING(file);
  }
  static void IO::write_SINT(std::fstream& file, u64 size) {
    file.write(reinterpret_cast<char*>(&size), sizeof(u64));
  }
  static u64 IO::read_SINT(std::fstream& file) {
    u64 size;
    file.read(reinterpret_cast<char*>(&size), sizeof(u64));
    return size;
  }
  static void IO::write_FLOAT(std::fstream& file, f64 val) {
    file.write(reinterpret_cast<char*>(&val), sizeof(f64));
  }
  static f64 IO::read_FLOAT(std::fstream& file) {
    f64 val;
    file.read(reinterpret_cast<char*>(&val), sizeof(f64));
    return val;
  }
  static std::string IO::check_TAG(std::fstream& file) {
    auto rp            = file.tellg();
    std::string result = read_TAG(file);
    file.seekg(rp, std::ios::beg);
    return result;
  }

};
#endif
