#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <regex>
#include <ctype.h>

using std::cout;
using std::vector;
using std::string;
using std::map;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

int match_numbers(string &s)
{
    static map<string, int> digit_map = {
        {"one",  1}, {"two", 2}, {"three", 3}, {"four",  4},
        {"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9},
        {"eno",  1}, {"owt", 2}, {"eerht", 3}, {"ruof",  4},
        {"evif", 5}, {"xis", 6}, {"neves", 7}, {"thgie", 8}, {"enin", 9}
    };

    std::regex r0("(one|two|three|four|five|six|seven|eight|nine|[1-9])");
    std::regex r1("(eno|owt|eerht|ruof|evif|xis|neves|thgie|enin|[1-9])");
    std::smatch m0;
    std::match_results<std::string::const_reverse_iterator> m1;
    std::regex_search(s.cbegin(),  s.cend(),  m0, r0);
    std::regex_search(s.crbegin(), s.crend(), m1, r1);

    auto m0v = m0.str(0);
    auto m1v = m1.str(0);

    int d0 = (isdigit(m0v[0]) ? std::stoi(m0v) : digit_map[m0v]) * 10;
    int d1 =  isdigit(m1v[0]) ? std::stoi(m1v) : digit_map[m1v];

    return d0 + d1;
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/01.txt");

    long silver = 0;
    long gold   = 0;
    for (auto &e : lines) {
        auto s0 = std::find_if(e.begin(),  e.end(),  isdigit);
        auto s1 = std::find_if(e.rbegin(), e.rend(), isdigit);
        silver += (*s0 - '0') * 10 + (*s1 - '0');
        gold   += match_numbers(e);
    }

    cout << silver << "\n";
    cout << gold   << "\n";

    return 0;
}
