#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::vector;
using std::string;
using std::set;
using std::map;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

void solution(vector<string> &lines, uint32_t &silver, uint32_t &gold)
{
    map<size_t, uint32_t> copies;
    for (size_t i = 1; i <= lines.size(); ++i)
        copies[i] = 1;

    for (size_t i = 1; i <= lines.size(); ++i) {
        auto l = lines[i - 1];
        vector<string> card;
        boost::split(card, l, boost::is_any_of(":"), boost::token_compress_on);
        vector<string> all_nums;
        boost::split(all_nums, card[1], boost::is_any_of("|"),
            boost::token_compress_on);
        vector<string> win_toks;
        vector<string> num_toks;
        boost::trim(all_nums[0]);
        boost::trim(all_nums[1]);
        boost::split(win_toks, all_nums[0], boost::is_any_of(" "),
            boost::token_compress_on);
        boost::split(num_toks, all_nums[1], boost::is_any_of(" "),
            boost::token_compress_on);

        set<int>    winners;
        vector<int> numbers; // Maybe there's repeats?
        for (auto &t : win_toks)
          winners.insert(stoi(t));
        for (auto &t : num_toks)
          numbers.push_back(stoi(t));

        uint32_t matches = 0;
        for (auto &n : numbers)
            if (winners.contains(n))
                ++matches;

        if (!matches)
            continue;

        silver += 1 << (matches - 1);

        for (size_t next = i + 1; next <= i + matches; ++next)
            copies[next] += copies[i];
    }

    for (auto &p : copies)
      gold += p.second;
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/04.txt");

    uint32_t silver = 0;
    uint32_t gold   = 0;

    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
