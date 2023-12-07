#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <map>
#include <algorithm>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::vector;
using std::string;
using std::array;
using std::map;

void get_lines(vector<string> &lines, string filename)
{
    string         line;
    std::ifstream  f(filename);

    while (std::getline(f, line))
        if (line.size() > 0)
            lines.push_back(line);
}

int get_type(array<int, 5> &sets, int jokers)
{
    int type = 0;
    if (jokers == 5 || sets[4 - jokers] > 0)                    // Five of a kind
        type = 6;
    else if (jokers == 4 || sets[3 - jokers] > 0)               // Four of a kind
        type = 5;
    else if ((jokers == 0 && (sets[2] == 1 && sets[1] == 1)) || // Full house
             (jokers == 1 && (sets[1] == 2)))
        type = 4;
    else if ((jokers == 0 && (sets[2] == 1)) ||                 // Three of a kind
             (jokers == 1 && (sets[1] == 1)) ||
             (jokers == 2 && (sets[0] >= 1)))
        type = 3;
    else if (jokers == 0 && (sets[1] == 2))                     // Two pairs
        type = 2;
    else if ((jokers == 0 && (sets[1] == 1)) || jokers == 1)    // One pair
        type = 1;

    return type;
}

bool compare_cards(const array<int, 3> &lhs, const array<int, 3> &rhs)
{
    if (lhs[1] != rhs[1])
        return lhs[1] < rhs[1];
    return lhs[0] < rhs[0];
}

void solution(vector<string> &lines, long &silver, long &gold)
{
    map<char, int> card_map = {
        {'2', 0x2}, {'3', 0x3}, {'4', 0x4}, {'5', 0x5}, {'6', 0x6}, {'7', 0x7}, {'8', 0x8}, {'9', 0x9},
        {'T', 0xA}, {'J', 0xB}, {'Q', 0xC}, {'K', 0xD}, {'A', 0xE}};
    vector<array<int, 3>> silver_hands;
    vector<array<int, 3>> gold_hands;

    for (auto &l : lines) {
        vector<string> toks;
        boost::split(toks, l, boost::is_any_of(" "), boost::token_compress_on);
        int bid = stoi(toks[1]);

        map<char, int> counts;
        int silver_card_number = 0x0;
        int gold_card_number   = 0x0;
        for (auto c : toks[0]) {
            silver_card_number = silver_card_number * 0x10 + card_map[c];
            if (c != 'J')
                gold_card_number = gold_card_number * 0x10 + card_map[c];
            else
                gold_card_number = gold_card_number * 0x10 + 0x1;

            counts[c] = counts.contains(c) ? counts[c] + 1 : 1;
        }

        // Get type of silver hand
        array<int, 5> silver_sets = {0, 0, 0, 0, 0}; // singles, pairs, threes, fours, fives
        for (auto &p : counts)
            ++silver_sets[p.second - 1];
        silver_hands.push_back({silver_card_number, get_type(silver_sets, 0), bid});

        // Get type of gold hand
        array<int, 5> gold_sets = {0, 0, 0, 0, 0}; // singles, pairs, threes, fours, fives
        int jokers = counts.contains('J') ? counts['J'] : 0;
        for (auto &p : counts) {
            if (p.first != 'J')
                ++gold_sets[p.second - 1];
        }
        gold_hands.push_back({gold_card_number, get_type(gold_sets, jokers), bid});
    }

    silver = 0;
    std::sort(silver_hands.begin(), silver_hands.end(), compare_cards);
    for (size_t i = 0; i != silver_hands.size(); ++i)
        silver += silver_hands[i][2] * (i + 1);

    gold = 0;
    std::sort(gold_hands.begin(), gold_hands.end(), compare_cards);
    for (size_t i = 0; i != gold_hands.size(); ++i)
        gold += gold_hands[i][2] * (i + 1);
}

int main()
{
    vector<string> lines;
    get_lines(lines, "inputs/07.txt");

    long silver = 0;
    long gold   = 0;

    solution(lines, silver, gold);
    cout << silver << "\n" << gold << "\n";

    return 0;
}
