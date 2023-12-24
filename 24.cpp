#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <z3++.h>

using std::cout;
using std::vector;
using std::string;
using namespace z3;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

struct vec3 {
    double x, y, z;
    vec3(double x, double y, double z = 0) : x(x), y(y), z(x) {};

    vec3& operator+=(vec3 const& o)&
    {
        x += o.x;
        y += o.y;
        return *this;
    }

    friend vec3 operator+(vec3 a, vec3 const &b)
    {
        a += b;
        return a;
    }

    string to_string()
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};

struct hailstone {
    vec3 p, v;
    double m, b;
    hailstone(double x, double y, double z, double vx, double vy, double vz) :
        p(x, y, z), v(vx, vy, vz), m(v.y / v.x), b(p.y - m * p.x) {}

    vec3 intersection(hailstone &o)
    {
        if (m == o.m)
            return vec3(0, 0);

        double i_x = (o.b - b) / (m - o.m);
        double i_y = m * i_x + b;

        double t_0 = (i_x - p.x) / v.x;
        double t_1 = (i_x - o.p.x) / o.v.x;

        if (t_0 < 0 || t_1 < 0)
            return vec3(0, 0);

        return vec3(i_x, i_y);
    }
};

uint64_t window_collisions(vector<hailstone> &hailstones, double low, double high)
{
    uint64_t ret = 0;

    for (size_t i = 0; i != hailstones.size(); ++i) {
        for (size_t j = i + 1; j != hailstones.size(); ++j) {
            auto p = hailstones[i].intersection(hailstones[j]);
            if (p.x >= low && p.x <= high && p.y >= low && p.y <= high)
                ++ret;
        }
    }

    return ret;
}

uint64_t throw_rock(vector<hailstone> &hailstones)
{
    context ctx;
    expr x  = ctx.real_const("x");
    expr y  = ctx.real_const("y");
    expr z  = ctx.real_const("z");
    expr vx = ctx.real_const("vx");
    expr vy = ctx.real_const("vy");
    expr vz = ctx.real_const("vz");

    vector<expr> t_is;
    for (size_t i = 0; i != hailstones.size() - 1; ++i)
        t_is.push_back(ctx.real_const(("t_" + std::to_string(i)).c_str()));

    solver s(ctx);
    s.add(x > 0);
    s.add(y > 0);
    s.add(z > 0);

    for (size_t i = 0; i != 3; ++i) {
        auto &h  = hailstones[i];
        s.add(ctx.real_val(int(h.p.x)) + ctx.real_val(int(h.v.x)) * t_is[i] == x + vx * t_is[i]);
        s.add(ctx.real_val(int(h.p.y)) + ctx.real_val(int(h.v.y)) * t_is[i] == y + vy * t_is[i]);
        s.add(ctx.real_val(int(h.p.z)) + ctx.real_val(int(h.v.z)) * t_is[i] == z + vz * t_is[i]);
    }

    return m.eval(x + y + z);
}

void solution(vector<string> &lines, uint64_t &silver, uint64_t &gold)
{
    vector<hailstone> hailstones;
    for (auto &l : lines) {
        vector<string> toks;
        vector<double> d_toks;
        boost::split(toks, l, boost::is_any_of(",@"), boost::token_compress_on);
        for (auto &t : toks)
            if (t.size())
                d_toks.push_back(stod(t));

        hailstones.push_back(hailstone(d_toks[0], d_toks[1], d_toks[2], d_toks[3], d_toks[4], d_toks[5]));
    }

    silver = window_collisions(hailstones, 200000000000000, 400000000000000);
    gold   = throw_rock(hailstones);
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/24.txt");

    uint64_t silver = 0, gold = 0;
    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
