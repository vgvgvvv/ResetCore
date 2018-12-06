#include "string_util.h"

namespace TableLib
{
    string StringUtil::ToLower(const string& str) {
        string t = str;
        std::transform(t.begin(), t.end(), t.begin(), tolower);
        return t;
    }

    string StringUtil::ToUpper(const string& str) {
        string t = str;
        std::transform(t.begin(), t.end(), t.begin(), toupper);
        return t;
    }

    string StringUtil::TrimLeft(const string& str) {
        string t = str;
        t.erase(0, t.find_first_not_of(" /t/n/r"));
        return t;
    }

    string StringUtil::TrimRight(const string& str) {
        string t = str;
        t.erase(t.find_last_not_of(" /t/n/r") + 1);
        return t;
    }

    string StringUtil::Trim(const string& str) {
        string t = str;
        t.erase(0, t.find_first_not_of(" /t/n/r"));
        t.erase(t.find_last_not_of(" /t/n/r") + 1);
        return t;
    }

    bool StringUtil::StartWith(const string& str, const string& substr) {
        return str.find(substr) == 0;
    }

    bool StringUtil::EndWith(const string& str, const string& substr) {
        return str.rfind(substr) == (str.length() - substr.length());
    }

    bool StringUtil::EqualIgnoreCase(const string &str1, const string &str2) {
        return ToLower(str1) == ToLower(str2);
    }

    void StringUtil::Split(const std::string &s, char sep, std::vector<std::string> *ret) {
        ret->clear();
        std::size_t split_idx = 0;
        std::size_t tmp_idx = 0;
        do
        {
            tmp_idx = s.find(sep, split_idx);
            if (tmp_idx != std::string::npos)
            {
                ret->push_back(s.substr(split_idx, tmp_idx - split_idx));
                split_idx = tmp_idx + 1;
            }
            else
            {
                ret->push_back(s.substr(split_idx));
            }
        } while (tmp_idx != std::string::npos);
    }
}
