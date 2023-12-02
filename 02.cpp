#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <boost/algorithm/string.hpp>

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

void solution(vector<string> &lines, map<string, uint32_t> &limits, uint32_t &silver, uint32_t &gold)
{
    for (auto &e : lines) {
        vector<string> toks;
        vector<string> idx_toks;
        boost::split(toks,     e,       boost::is_any_of(":"));
        boost::split(idx_toks, toks[0], boost::is_any_of(" "), boost::token_compress_on);

        auto index    = stoi(idx_toks[idx_toks.size() - 1]);
        auto possible = true;

        vector<string> draws;
        boost::split(draws, toks[1], boost::is_any_of(";"));

        map<string, int> gold_mins = {{"red", 0}, {"green", 0}, {"blue", 0}};

        for (auto &d : draws) {
            vector<string> colors;
            boost::split(colors, d, boost::is_any_of(","));

            for (auto &c : colors) {
                boost::trim(c);
                vector<string> color_toks;
                boost::split(color_toks, c, boost::is_any_of(" "), boost::token_compress_on);
                boost::trim(color_toks[1]);
                auto num_color = stoi(color_toks[0]);

                gold_mins[color_toks[1]] = num_color > gold_mins[color_toks[1]] ? num_color : gold_mins[color_toks[1]];

                if (num_color > limits[color_toks[1]])
                    possible = false;
            }
        }

        if (possible)
            silver += index;

        uint32_t pwr = 1;
        for (auto &m : gold_mins)
            pwr *= m.second;
        gold += pwr;
    }
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/02.txt");

    map<string, uint32_t> silver_limits = {
        {"red", 12}, {"green", 13}, {"blue", 14}
    };

    uint32_t silver = 0;
    uint32_t gold   = 0;

    solution(lines, silver_limits, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
