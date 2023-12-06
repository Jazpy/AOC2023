#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::vector;
using std::string;
using std::pair;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

void solution(vector<string> &lines, long &silver, long &gold)
{
    vector<string> str_times;
    vector<string> str_dists;
    vector<string> toks;
    boost::split(toks, lines[0], boost::is_any_of(":"), boost::token_compress_on);
    boost::split(str_times, toks[1], boost::is_any_of(" "), boost::token_compress_on);
    boost::split(toks, lines[1], boost::is_any_of(":"), boost::token_compress_on);
    boost::split(str_dists, toks[1], boost::is_any_of(" "), boost::token_compress_on);

    string str_gold_time = "";
    string str_gold_dist = "";
    vector<pair<uint64_t, uint64_t>> silver_races;
    for (size_t i = 1; i != str_times.size(); ++i) {
        silver_races.push_back(pair<uint64_t, uint64_t>(stol(str_times[i]), stol(str_dists[i])));

        str_gold_time += str_times[i];
        str_gold_dist += str_dists[i];
    }
    pair<uint64_t, uint64_t> gold_race(stol(str_gold_time), stol(str_gold_dist));

    silver = 1;
    for (auto &p : silver_races) {
        uint64_t wins = 0;

        for (uint64_t a_time = 1; a_time != p.first; ++a_time) {
            uint64_t a_dist = (p.first - a_time) * a_time;

            if (a_dist > p.second)
                ++wins;
        }

        silver *= wins;
    }

    gold = 0;
    for (uint64_t a_time = 1; a_time != gold_race.first; ++a_time) {
        uint64_t a_dist = (gold_race.first - a_time) * a_time;

        if (a_dist > gold_race.second)
            ++gold;
    }
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/06.txt");

    long silver = 0;
    long gold   = 0;

    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
