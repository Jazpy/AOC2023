#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <stack>
#include <boost/algorithm/string.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using std::cout;
using std::vector;
using std::string;
using std::unordered_set;
using std::stack;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

int64_t min(int64_t a, int64_t b) { return a < b ? a : b; }
int64_t max(int64_t a, int64_t b) { return a > b ? a : b; }

struct cube {
    struct vec3 {
        int64_t x, y, z;
        vec3(int64_t x, int64_t y, int64_t z) : x(x), y(y), z(z) {}
    };

    vec3 e0, e1;
    unordered_set<size_t> supports;
    int64_t lo_z, hi_z;

    cube(int64_t x0, int64_t y0, int64_t z0, int64_t x1, int64_t y1, int64_t z1) :
        e0(x0, y0, z0), e1(x1, y1, z1), lo_z(std::min(z0, z1)), hi_z(max(z0, z1)) {}

    void drop(int64_t drop_z)
    {
        e0.z -= drop_z;
        e1.z -= drop_z;
        lo_z -= drop_z;
        hi_z -= drop_z;
    }

    bool rests_on(const cube &o)
    {
        return min(e0.x, e1.x) <= max(o.e0.x, o.e1.x) && max(e0.x, e1.x) >= min(o.e0.x, o.e1.x) &&
               min(e0.y, e1.y) <= max(o.e0.y, o.e1.y) && max(e0.y, e1.y) >= min(o.e0.y, o.e1.y) &&
               lo_z == o.hi_z + 1;
    }

    struct zort {
        bool operator()(const cube &a, const cube &b) { return min(a.e0.z, a.e1.z) > min(b.e0.z, b.e1.z); }
    };
};

void drop_cubes(vector<cube> &cubes)
{
    for (int i = cubes.size() - 1; i >= 0; --i) {
        auto &curr_cube = cubes[i];

        bool dropped = false;
        while (!dropped) {
            for (size_t j = i + 1; j != cubes.size(); ++j) {
                if (curr_cube.rests_on(cubes[j])) {
                    dropped = true;
                    curr_cube.supports.insert(j);
                }
            }

            if (!dropped) {
                curr_cube.drop(1);
                dropped = curr_cube.lo_z == 0;
            }
        }
    }
}

int64_t chaininator(vector<cube> cubes, size_t bearer) {
    int64_t ret = 0;

    stack<size_t> s;
    s.push(bearer);

    while (!s.empty()) {
        size_t gone = s.top();
        s.pop();

        for (size_t i = 0; i != cubes.size(); ++i) {
            if (cubes[i].supports.contains(gone)) {
                cubes[i].supports.erase(gone);
                if (cubes[i].supports.empty()) {
                    ++ret;
                    s.push(i);
                }
            }
        }
    }

    return ret;
}

void solution(vector<string> &lines, int64_t &silver, int64_t &gold)
{
    vector<cube> cubes;
    int64_t min_z = 1000000;
    for (auto &l : lines) {
        vector<string> toks;
        boost::split(toks, l, boost::is_any_of("~,"), boost::token_compress_on);
        vector<int64_t> ints;

        for (auto &t : toks)
            ints.push_back(stol(t));

        min_z = min(min_z, ints[2]);
        min_z = min(min_z, ints[5]);
        cubes.push_back(cube(ints[0], ints[1], ints[2], ints[3], ints[4], ints[5]));
    }

    std::sort(cubes.begin(), cubes.end(), cube::zort());
    for (auto &c : cubes)
        c.drop(min_z - 1);
    drop_cubes(cubes);

    unordered_set<size_t> non_disintegratable;
    for (auto &c : cubes) {
        if (c.supports.size() == 1)
            non_disintegratable.insert(*begin(c.supports));
    }
    silver = cubes.size() - non_disintegratable.size();

    gold = 0;
    for (auto i : non_disintegratable)
        gold += chaininator(cubes, i);
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/22.txt");

    int64_t silver = 0, gold = 0;
    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
