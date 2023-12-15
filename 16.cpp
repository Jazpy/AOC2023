#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <memory>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::vector;
using std::string;
using std::unique_ptr;
using std::make_unique;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

void solution(vector<string> &lines, uint64_t &silver, uint64_t &gold)
{
    for (auto &l : lines) {
        vector<string> toks;
        boost::split(toks, l, boost::is_any_of(" "), boost::token_compress_on);
    }
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/16.txt");

    uint64_t silver = 0;
    uint64_t gold   = 0;
    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
