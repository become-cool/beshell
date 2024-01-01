/*
 * Copyright (C) 2021 - 2022 absop (zlang0@163.com)
 */

#include <initializer_list>
#include <iostream>
#include <map>
#include <string>
#include <vector>


struct Option {
    bool is_given;
    unsigned int num_args;
#ifdef TEST_OPTION_PARSER
    std::string pattern;
#endif
    std::vector<std::string> values;
    void reset() {
        is_given = false ;
        values.clear() ;
    }
};


class Options {
  public:
    Options(const std::string &lines)
    : Options(split(lines, "\n"))
    {}

    Options(const std::initializer_list<std::string> &lines)
    : Options(std::vector<std::string>(lines))
    {}

    Options(const std::vector<std::string> &lines) {
        std::vector<size_t> usage_line_indexes;
        std::vector<std::string> option_explains;
        std::string::size_type max_pattern_length = 0;
        for (auto &line : lines) {
            if (line.substr(0, 3) != "  -") {
                _usage_lines.push_back(line);
                continue;
            }
            if (!(line.length() > 3)) {
                continue;
            }
            size_t option_index = _options.size();
            std::string option_pattern;
            std::string::size_type i = 3;
            for (auto & name : parse_names(line, i)) {
                _option_index_by_names[name] = option_index;
                if (!option_pattern.empty()) {
                    option_pattern.append(", ");
                } else {
                    option_pattern.append("  ");
                }
                option_pattern.push_back('-');
                option_pattern.append(name);
            }
            Option option = {
#ifdef TEST_OPTION_PARSER
                .pattern = option_pattern.substr(2)
#endif
            };
            for (auto & ph : parse_placeholders(line, i)) {
                option_pattern.push_back(' ');
                option_pattern.append(ph);
                option.num_args += 1;
            }
            if (option.num_args &&
                option_pattern.length() > 3 &&
                option_pattern.substr(option_pattern.length() - 3)  == "...") {
                option.num_args = -1;
            }
            usage_line_indexes.push_back(_usage_lines.size());
            _usage_lines.push_back(option_pattern);
            _options.push_back(option);
            option_explains.push_back(line.substr(i));
            if (option_pattern.length() > max_pattern_length) {
                max_pattern_length = option_pattern.length();
            }
        }
        max_pattern_length += 4;
        for (size_t i = 0; i < usage_line_indexes.size(); ++i) {
            std::string &line = _usage_lines[usage_line_indexes[i]];
            line.append(max_pattern_length - line.length(), ' ');
            line.append(option_explains[i]);
        }
    }

    void parse(const std::vector<std::string> & argv) {
        
        reset() ;

        std::string key, val;
        std::string::size_type pos;
        Option *option;
        for (int i = 1; i < (int)argv.size(); ++i) {
            val = argv[i];
            if (val[0] == '-') {
parse_option:
                if ((pos = val.find('=')) != std::string::npos) {
                    key = val.substr(1, pos - 1);
                    val = val.substr(pos + 1);
                } else {
                    key = val.substr(1);
                    val.clear();
                }
                option = find_option_by_name(key);
                if (option) {
                    option->is_given = true;
                } else {
                    // invalid_option(argv[0], key);
                    continue ;
                }
                unsigned int n = 0;
                if (!val.empty()) {
                    goto push_option_val;
                }
                for (; n < option->num_args && ++i < (int)argv.size();) {
                    val = argv[i];
                    if (val[0] == '-') {
                        goto parse_option;
                    }
push_option_val:
                    option->values.push_back(val);
                    n += 1;
                }
            } else {
                _args.push_back(val);
            }
        }
#ifdef TEST_OPTION_PARSER
        std::cout << "Given options:" << std::endl;
        for (auto &option : _options) {
            if (option.is_given) {
                std::cout << "  " << option.pattern
                          << ": num_args: " << option.num_args
                          << ", values:";
                for (auto &v : option.values) {
                    std::cout << " " << v;
                }
                std::cout << std::endl;
            }
        }
        std::cout << "Other options:" << std::endl;
        for (auto &option : _options) {
            if (!option.is_given) {
                std::cout << "  " << option.pattern << std::endl;
            }
        }
        std::cout << "args:";
        for (auto &x : _args) {
            std::cout << " " << x;
        }
        std::cout << std::endl << std::endl;
#endif
        // if (has("?")) {
        //     show_usage();
        //     exit(0);
        // }
    }


    const std::vector<std::string> & usage() {
        return _usage_lines ;
    }
    void show_usage() {
        for (auto &line : _usage_lines) {
            std::cout << line << std::endl;
        }
    }

    const std::vector<std::string> &args() const { return _args; }

    const std::string & operator[](int i) {
        if(i<0) {
            i = _args.size() + i ;
        }
        if(i<0 || i>=(int)_args.size()) {
            static std::string option_empty_value ;
            option_empty_value = "" ;
            return option_empty_value ;
        }
        return _args[i] ;
    }
    size_t length() {
        return _args.size() ;
    }

    const std::string &get(const std::string &name,
                           const std::string &dval) {
        return get(name, 0, dval);
    }

    int get(const std::string &name, int dval) {
        return get(name, 0, dval);
    }

    const std::string &get(const std::string &name, size_t i,
                           const std::string &dval) {
        const Option *opt = find_option_by_name(name);
        if (opt && opt->is_given && opt->values.size() > i) {
            return opt->values[i];
        }
        return dval;
    }

    int get(const std::string &name, size_t i, int dval) {
        const Option *opt = find_option_by_name(name);
        if (opt && opt->is_given && opt->values.size() > i) {
            return std::stoi(opt->values[i]);
        }
        return dval;
    }

    bool has(const std::string &name) {
        const Option *opt = find_option_by_name(name);
        return opt && opt->is_given;
    }

    void reset() {
        _args.clear() ;
        for(auto & opt: _options) {
            opt.reset() ;
        }
    }

  private:
    std::map<std::string, size_t> _option_index_by_names;
    std::vector<std::string> _args;
    std::vector<std::string> _usage_lines;
    std::vector<Option> _options;

    Option *find_option_by_name(const std::string &name) {
        auto iter = _option_index_by_names.find(name);
        if (iter != _option_index_by_names.end()) {
            size_t option_index = iter->second;
            return &_options[option_index];
        }
        return nullptr;
    }

    std::vector<std::string> parse_names(const std::string &line,
                                         std::string::size_type &i) {
        std::vector<std::string> names;
        std::string name;
        char c = 0 ;
        if (line[i] == '(') {
            for (++i; i < line.length(); ++i) {
                c = line[i];
                if (c == '|' || c == ')') {
                    if (!name.empty()) {
                        names.push_back(name);
                        name.clear();
                    }
                    if (c == ')') {
                        break;
                    }
                } else if (c != ' ') {
                    name.push_back(c);
                }
            }
            if (c != ')' || (++i < line.length() && line[i] != ' ')) {
                invalid_option(nullptr, line);
            }
        } else {
            for (; i < line.length(); ++i) {
                c = line[i];
                if (c == ' ') {
                    break;
                }
                name.push_back(c);
            }
            names.push_back(name);
        }
        return names;
    }

    std::vector<std::string> parse_placeholders(
        const std::string &line, std::string::size_type &i) {
        std::vector<std::string> phs;
        std::string ph;
        char c;
        int nspace = 0;
        for (; i < line.length(); ++i) {
            c = line[i];
            if (c == ' ') {
                if (!ph.empty()) {
                    phs.push_back(ph);
                    ph.clear();
                }
                nspace += 1;
            } else {
                if (nspace > 1) {
                    break;
                }
                nspace = 0;
                ph.push_back(c);
            }
        }
        return phs;
    }

    std::string invalid_option(const char *prog, const std::string &name) {
        if (prog) {
            std::cout << prog << ": error: unknown command line option '"
                      << name << "'\n";
        } else {
            std::cout << "invalid option entry '" << name << "'\n";
        }
        exit(1);
    }

    std::vector<std::string> split(const std::string& str,
                                   const std::string& sep) {
        std::vector<std::string> v;

        std::string::size_type lensep = sep.length();
        std::string::size_type start = 0;
        std::string::size_type end = 0;
        while ((end = str.find(sep, start)) != std::string::npos) {
            v.push_back(str.substr(start, end - start));
            start = end + lensep;
        }

        v.push_back(str.substr(start));

        return v;
    }
};
