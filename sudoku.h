#pragma once

#include <vector>
#include <array>
#include <string>

using namespace std;

using board = array<array<int, 9>, 9>;

vector<board> read_boards(const string& filename);

void write_boards(const string& filename, const vector<board>& boards);

vector<board> solve_boards(const vector<board>& board);

vector<board> generate_final(int count);

vector<board> generate_game(int num, int mode, string range, bool unique);

