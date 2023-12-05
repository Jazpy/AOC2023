#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <map>
#include <limits>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::vector;
using std::string;
using std::tuple;
using std::map;

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
    vector<long> seeds;
    vector<vector<tuple<long, long, long>>> all_maps;
    size_t curr_map_idx;
    for (size_t i = 0; i != lines.size(); ++i) {
        auto l = lines[i];

        vector<string> toks;
        // Seeds
        if (i == 0) {
            boost::split(toks, l.substr(7), boost::is_any_of(" "), boost::token_compress_on);
            for (auto &s : toks)
                seeds.push_back(stol(s));
        // New map
        } else if (!isdigit(l[0])) {
            boost::split(toks, l, boost::is_any_of(" "), boost::token_compress_on);
            all_maps.push_back(vector<tuple<long, long, long>>());
            curr_map_idx = all_maps.size() - 1;
        // Map line
        } else {
            boost::split(toks, l, boost::is_any_of(" "), boost::token_compress_on);
            long source = stol(toks[1]);
            long dest   = stol(toks[0]);
            all_maps[curr_map_idx].push_back({source, source + (stol(toks[2]) - 1), source - dest});
        }
    }

    silver = std::numeric_limits<long>::max();
    for (auto s : seeds) {
        for (auto &m : all_maps) {
            for (auto [lo, hi, shift] : m) {
                if (s >= lo && s <= hi) {
                    s -= shift;
                    break;
                }
            }
        }

        silver = s < silver ? s : silver;
    }

    gold = std::numeric_limits<long>::max();
    for (size_t i = 0; i < seeds.size(); i += 2) {
        long seed_start = seeds[i];
        long seed_num   = seeds[i + 1];
        long skip       = seed_num;
        for (long s = seed_start; s < seed_start + seed_num; s += skip) {
            auto s_p = s;
            skip = seed_num;
            for (auto &m : all_maps) {
                bool mapped = false;
                for (auto [lo, hi, shift] : m) {

                    if (s_p >= lo && s_p <= hi) {
                        skip = (hi - s_p + 1) < skip ? (hi - s_p + 1) : skip;
                        s_p -= shift;
                        mapped = true;
                        break;
                    }
                }

                if (!mapped)
                    skip = 1;
            }

            gold = s_p < gold ? s_p : gold;
        }
    }
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/05.txt");

    long silver = 0;
    long gold   = 0;

    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
