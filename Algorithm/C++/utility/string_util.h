
#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__

#include <string>
#include <vector>
#include <algorithm>

using std::string;

namespace TableLib
{
    class StringUtil
    {
    public:
        static string ToLower(const string& str);
        static string ToUpper(const string& str);

        static string TrimLeft(const string& str);
        static string TrimRight(const string& str);
        static string Trim(const string& str);

        static bool StartWith(const string& str, const string& substr);
        static bool EndWith(const string& str, const string& substr);

        static bool EqualIgnoreCase(const string &str1, const string &str2);

        static void Split(const std::string &s, char sep, std::vector<std::string> *ret);
    };
}

#endif //__STRING_UTIL_H__