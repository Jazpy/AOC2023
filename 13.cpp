#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::vector;
using std::string;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

void solution(vector<string> &lines, int64_t &silver, int64_t &gold)
{
    for (auto &l : lines) {
        vector<string> toks;
        boost::split(toks, l, boost::is_any_of(" "), boost::token_compress_on);
    }
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/13.txt");

    int64_t silver = 0;
    int64_t gold   = 0;

    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
