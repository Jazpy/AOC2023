#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/multiprecision/cpp_int.hpp>

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

int64_t shoelace(const vector<vector<int64_t>> &points, int64_t perimeter)
{
    int64_t area = 0;
    for (size_t i = 0; i != points.size() - 1; ++i)
        area += (points[i][0] * points[i + 1][1]) - (points[i][1] * points[i + 1][0]);
    return abs(area / 2) - (perimeter / 2) + 1 + perimeter;
}

void solution(const vector<string> &lines, int64_t &silver, int64_t &gold)
{
    vector<vector<int64_t>> silver_points = {{0, 0}};
    vector<vector<int64_t>> gold_points   = {{0, 0}};
    map<char, vector<int>>  moves  = {{'U', {0, 1}}, {'R', {1, 0}}, {'D', {0, -1}}, {'L', {-1, 0}},
                                      {'3', {0, 1}}, {'0', {1, 0}}, {'1', {0, -1}}, {'2', {-1, 0}}};

    int64_t silver_perimeter = 0;
    int64_t gold_perimeter   = 0;
    for (auto &l : lines) {
        vector<string> toks;
        boost::split(toks, l, boost::is_any_of(" "), boost::token_compress_on);

        auto &silver_dir  = moves[toks[0][0]];
        auto  silver_mag  = stol(toks[1]);
        silver_perimeter += silver_mag;
        silver_points.push_back({silver_points.back()[0] + (silver_dir[0] * silver_mag),
                                 silver_points.back()[1] + (silver_dir[1] * silver_mag)});

        auto  hex       = toks[2].substr(2, 6);
        auto &gold_dir  = moves[hex[hex.size() - 1]];
        auto  gold_mag  = stol(hex.substr(0, 5), nullptr, 16);
        gold_perimeter += gold_mag;

        gold_points.push_back({gold_points.back()[0] + (gold_dir[0] * gold_mag),
                               gold_points.back()[1] + (gold_dir[1] * gold_mag)});
    }

    silver = shoelace(silver_points, silver_perimeter);
    gold   = shoelace(gold_points,   gold_perimeter);
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/18.txt");

    int64_t silver = 0;
    int64_t gold   = 0;
    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
