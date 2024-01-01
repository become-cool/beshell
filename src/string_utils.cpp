
#include "string_utils.hpp"
#include <algorithm>

namespace be {

    void split(std::string& s, std::string delim,std::vector<std::string> & ret) {
        size_t last = 0;
        size_t index=s.find_first_of(delim,last);
        while (index!=std::string::npos) {
            ret.push_back(s.substr(last,index-last));
            last=index+1;
            index=s.find_first_of(delim,last);
        }
        if (index-last>0) {
            ret.push_back(s.substr(last,index-last));
        }
    }

    std::string& trim(std::string &str) {
        // 删除前面的空白字符
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
            return !std::isspace(ch);
        }));

        // 删除后面的空白字符
        str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), str.end());
        return str ;
    }
    std::string & ltrim(std::string& str) {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
            return !std::isspace(ch);
        }));
        return str;
    }
    std::string & rtrim(std::string& str) {
        str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), str.end());
        return str;
    }
}