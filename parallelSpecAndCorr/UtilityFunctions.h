#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H

#include <algorithm>

#include <cassert>

#include <cstdarg>

#include <cstring>
using std::memcmp;

#include <ctime>
using std::time;  using std::localtime;

#include <filesystem>
using std::filesystem::path;
using std::filesystem::canonical;
using std::filesystem::absolute;
using std::filesystem::exists;

#include <functional>
using std::plus;

#include <iomanip>
using std::put_time;

#include <list>
using std::list;

#include <locale>
using std::locale;

#include <map>
using std::map;

#include <numeric>
using std::accumulate;

#include <ostream>
using std::ostream;

#include <regex>
using std::regex;
using std::regex_match;
using std::smatch;
using std::regex_constants::match_flag_type;
using std::regex_constants::match_default;

#include <sstream>
using std::ostringstream;
using std::istringstream;

#include <cstdint>

#include <string>
using std::string;
using std::literals::string_literals::operator""s;

#include <unordered_map>
using std::unordered_map;

#include <utility>
using std::pair;

#include <vector>
using std::vector;

#include "ForEach.h"

typedef int32_t tWeight;
typedef uint64_t tIndex;
typedef unsigned long long tPerfCounter;

// Default locale.
extern locale Locale;

#ifdef DEBUG
#define Break(cond) if (cond) {__asm__("int $3"); }
#else
#define Break(cond)
#endif

#define MakeString(x) #x

// ******************************************************************************************************
// This function implements a string printf function.  It's name and functionality matches the
// function implemented in the C++20 include <format>.
// ******************************************************************************************************
inline string Format(const string format, ...)
{
    va_list ap;

    // First get number of characters needed (less terminating null).
    va_start(ap, format);
    auto nChars { vsnprintf(nullptr, 0u, format.c_str(), ap) };
    va_end(ap);

    // Allocate buffer, including room for null.
    vector<char> Buffer(nChars + 1);

    // Print for real this time.
    va_start(ap, format);
    nChars = vsnprintf(Buffer.data(), Buffer.size(), format.c_str(), ap);
    va_end(ap);

    // Return resulting string.
    return string(Buffer.data());
}

inline string replace_all(const string &inString, const string &what, const string &with)
{
    string outString { inString };
    for (string::size_type pos{ 0u }; outString.npos != (pos = outString.find(what.data(), pos, what.length())); pos += with.length())
    {
        outString.replace(pos, what.length(), with.data(), with.length());
    }

    return outString;
}

inline path Canonical(const path &filePath)
{
    if (exists(filePath))
    {
        // File exists.  We can return canonical form.
        return canonical(filePath);
    }
    else
    {
        // File does not exist.  the best we can do is absolute form.
        return absolute(filePath);
    }
}

// Handy == operator on vectors.
template<class T>
inline bool operator==(const vector<T> &lhs, const vector<T> &rhs)
{
    return (lhs.size()*sizeof(T) == rhs.size()*sizeof(T)) ? memcmp(lhs.data(), rhs.data(), sizeof(T) * lhs.size()) == 0 : false;
}

template<class T>
inline bool operator!=(const vector<T> &lhs, const vector<T> &rhs)
{
    return !(lhs == rhs);
}

template<class T1, class T2>
inline bool operator==(const vector<T1> &lhs, const vector<T2> &rhs)
{
    return (lhs.size()*sizeof(T1) == rhs.size()*sizeof(T2)) ? memcmp(lhs.data(), rhs.data(), sizeof(T1) * lhs.size()) == 0 : false;
}

template<class T1, class T2>
inline bool operator!=(const vector<T1> &lhs, const vector<T2> &rhs)
{
    return !(lhs == rhs);
}

template<class T>
inline bool operator ==(const vector<T> &lhs, const T *rhs)
{
    return memcmp(lhs.data(), rhs, sizeof(T) * lhs.size()) == 0;
}

template<class T>
inline bool operator!=(const vector<T> &lhs, const T *rhs)
{
    return !(lhs == rhs);
}

// Handy vector initializer
template<class T>
inline void initializeVector(vector<T> &vec, const T &&value)
{
	for (auto &entry : vec)
	{
		entry = value;
	}

	return;
}

// Handy vector reduction functions.
template<class T>
inline T vectorReduceAdd(const vector<T> &vec) { return accumulate(vec.cbegin(), vec.cend(), T(0), std::plus<T>()); }

template<class T>
inline T vectorReduceMin(const vector<T> &vec) { return accumulate(vec.cbegin(), vec.cend(), vec[0u], std::min); }

template<class T>
inline T vectorReduceMax(const vector<T> &vec) { return accumulate(vec.cbegin(), vec.cend(), vec[0u], std::max); }

// Get regular expression match into a vector<string>
inline bool matchRE(const string &str, vector<string> &matches, const regex &regEx, match_flag_type matchFlags = match_default)
{
    smatch regExcMatch;
    matches.clear();

    if (regex_match(str, regExcMatch, regEx, matchFlags))
    {
        matches.reserve(regExcMatch.size());
        for (auto i { 1u }; i < regExcMatch.size(); ++i)
        {
            matches.push_back(regExcMatch[i]);
        }

        return true;
    }
    else
    {
        return false;
    }
}

// Handy debugging macros.
#define dumpVar(var) #var ":" << var
#define sepDumpVar(var) ", " #var ": " << var

// Exposes the member variables of structs (under the assumption that a memberlist macro exists for said struct)
#define exposeStruct(varType, var) auto &[varType##MemberList] { var }
#define exposeStructConst(varType, var) const auto &[varType##MemberList] { var }

#define sepDumpListVar(var) << "," #var ":" << var
#define dumpList(first, ...) dumpVar(first) _FOR_EACH(sepDumpListVar, __VA_ARGS__)

#define HERE "File: " << __FILE__ << ", Line: " << __LINE__

// Constant value macros
#define MAX_INT() (0b0111'1111'1111'1111'1111'1111'1111'1111)
#define TNA() (~0u)

// The following operators implement operator<< for vectors, lists, maps and unordered_maps.
// They will for work for a container of any type as long as that container has an operator<< defined.


// Specializations for an array of characters and const char *.
// They must be converted to string to avoid ambiguus operators.v
template<int n>
inline ostream& operator<<(ostream &os, const char (&obj)[n])
{
    return os << string(obj);
}

inline ostream& operator<<(ostream &os, const char *obj)
{
    return os << string(obj);
}


#define tPairMemberList first, second
template<typename F, typename S>
inline ostream& operator<<(ostream &os, const pair<F, S> &obj)
{
    auto &[tPairMemberList] { obj };
    return os << "{" << dumpVar(first) << sepDumpVar(second) << "}";
}

inline ostream& operator<<(ostream &os, const string &s)
{
    return os << s.c_str();
}

template<template<typename ... > typename C, typename T, typename A, typename ... Tn>
inline ostream& operator<<(ostream &os, const C<T, A, Tn ...> &container)
{
    // Container open brace.
    os << "{\n"s;

    for (const auto &entry : container)
    {
        os << entry << ",\n"s;
    }

    // Container close brace;
    return os << "}"s;
}

//Convert a string to any numeric type with >> defined.
template<typename T>
inline T toNumber(const string &value)
{
    T temp;
    istringstream iss(value);
    iss >> temp;

    return temp;
}

// Convent any type with << defined to a string.
template<typename T>
inline string toString(const T &value)
{
    ostringstream oss;
    oss << value;

    return oss.str();
}

// Get a copy of lower case string.
inline string toLower(const string &source)
{
    string temp(source.size(), ' ');
    transform(source.begin(), source.end(), temp.begin(), ::tolower);

    return temp;
}

// Lower case string in place.
inline string& toLowerThis(string &source)
{
    transform(source.begin(), source.end(), source.begin(), ::tolower);

    return source;
}

// Get a copy of Upper case string.
inline string toUpper(const string &source)
{
    string temp(source.size(), ' ');
    transform(source.begin(), source.end(), temp.begin(), ::toupper);

    return temp;
}

// Upper case string in place.
inline string& toUpperThis(string &source)
{
    transform(source.begin(), source.end(), source.begin(), ::toupper);

    return source;
}

inline string getTimeStamp(const string &fmt = "%Y-%m-%d %H:%M:%S"s)
{
    auto t = time(nullptr);
    auto tm = localtime(&t);
    assert(tm);
    return toString(put_time(tm, fmt.c_str()));
}

#endif
