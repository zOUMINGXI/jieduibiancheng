#include "sudoku.h"
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

int current_num = 0;

int gen_rand(int s, int e) {
    mt19937 gen(std::random_device{}());
    uniform_int_distribution<> dis(s, e);
    return dis(gen);
}

vector<board> read_boards(const string& filename) {
    vector<board> ret;
    ifstream in(filename);
    if (!in) {
        cout << "无效的文件路径" << endl;
        return ret;
    }
    string line;
    while (getline(in, line) && !line.empty()) {
        stringstream line_stream(line);
        board b = {};
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                char v;
                line_stream >> v;
                if ((v < '1' || v > '9') && (v != '$')) {
                    cout << "无效的字符" << endl;
                    return ret;
                }
                b[i][j] = v == '$' ? 0 : v - '0';
            }
        }
        ret.push_back(b);
    }
    return ret;
}

void write_boards(const string& filename, const vector<board>& boards) {
    ofstream out(filename);
    for (const board & b : boards) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                char v = b[i][j] == 0 ? '$' : b[i][j] + '0';
                out << v;
            }
        }
        out << endl;
    }
}

void search(int i, int j, board& b, int solve_num) {
    if (j == 9) {
        i++;
        j = 0;
    }
    if (i == 9) {
        current_num++;
        return;
    }
    if (b[i][j] == 0) {
        bool exist[10] = { false };
        for (int k = 0; k < 9; k++) {
            if (k == j || b[i][k] == 0)
                continue;
            exist[b[i][k]] = 1;
        }
        for (int k = 0; k < 9; k++) {
            if (k == i || b[k][j] == 0)
                continue;
            exist[b[k][j]] = 1;
        }
        int n, m;
        if (i / 3 == 0)
            n = 0;
        else if (i / 6 == 0)
            n = 3;
        else
            n = 6;
        if (j / 3 == 0)
            m = 0;
        else if (j / 6 == 0)
            m = 3;
        else
            m = 6;
        for (int p = n; p < n + 3; p++) {
            for (int q = m; q < m + 3; q++) {
                if ((p == i && q == j) || b[p][q] == 0)
                    continue;
                exist[b[p][q]] = 1;
            }
        }

        for (int k = 1; k < 10; k++) {
            if (exist[k])
                continue;
            b[i][j] = k;
            search(i, j + 1, b, solve_num);
            if (current_num == solve_num)
                return;
            b[i][j] = 0;
        }
    }
    else {
        search(i, j + 1, b, solve_num);
    }
}

vector<board> solve_boards(const vector<board>& boards) {
    vector<board> ret;
    for (const board& b : boards) {
        current_num = 0;
        board res;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                res[i][j] = b[i][j];
            }
        }
        search(0, 0, res, 1);
        ret.push_back(res);
    }
    return ret;
}

vector<board> generate_final(int count) {
    vector<board> final_boards;
    int n = count;
    int first_line[9] = { 5, 1, 2, 3, 4, 6, 7, 8, 9 };
    int shift[9] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
    int pos1[6][3] = { {3, 4, 5},
                      {3, 5, 4},
                      {4, 5, 3},
                      {4, 3, 5},
                      {5, 4, 3},
                      {5, 3, 4} };
    int pos2[6][3] = { {6, 7, 8},
                      {6, 8, 7},
                      {7, 6, 8},
                      {7, 8, 6},
                      {8, 6, 7},
                      {8, 7, 6} };
    board b;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            b[i][j] = first_line[(j + shift[i]) % 9];
        }
    }
    do {
        board copy;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 9; j++) {
                copy[i][j] = b[i][j];
            }
        }
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                for (int k = 0; k < 3; k++) {
                    for (int p = 0; p < 9; p++) {
                        copy[k + 3][p] = b[pos1[i][k]][p];
                        copy[k + 6][p] = b[pos2[j][k]][p];
                    }
                }
                final_boards.push_back(copy);
                n--;
                if (n == 0)
                    return final_boards;
            }
        }
    } while (next_permutation(first_line + 1, first_line + 9));
    return final_boards;
}

vector<board> generate_game(int num, int mode, string range, bool unique) {
    vector<board> boards;
    int mid = range.find('~');
    int start = atoi(range.substr(0, mid).c_str());
    int end = atoi(range.substr(mid + 1).c_str());
    vector<board> final_boards = generate_final(num);
    for (const board& b : final_boards) {
        while (true) {
            board copy;
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    copy[i][j] = b[i][j];
                }
            }
            int empty_num = gen_rand(start, end);
            if ((end - start + 1) % 3 == 0) {
                int gap = (end - start + 1) / 3;
                empty_num = start + (mode - 1) * gap;
                empty_num += gen_rand(0, gap - 1);
            }
            else if ((end - start + 1) % 3 == 1) {
                int gap[3] = { (end - start + 1) / 3,
                              (end - start + 1) / 3,
                              (end - start + 1) / 3 + 1 };
                empty_num = start + (mode - 1) * gap[0];
                empty_num += gen_rand(0, gap[mode - 1] - 1);
            }
            else {
                int gap[3] = { (end - start + 1) / 3 + 1,
                              (end - start + 1) / 3 + 1,
                              (end - start + 1) / 3 };
                empty_num = start + (mode - 1) * gap[0];
                empty_num += gen_rand(0, gap[mode - 1] - 1);
            }
            int block_i = 0, block_j = 0;
            while (block_i != 9) {
                int pos[2];
                pos[0] = gen_rand(0, 8);
                pos[1] = gen_rand(0, 8);
                while (pos[0] == pos[1]) {
                    pos[1] = gen_rand(0, 8);
                }
                for (int k = 0; k < 2; k++) {
                    int i = pos[k] / 3;
                    int j = pos[k] % 3;
                    copy[block_i + i][block_j + j] = 0;
                }
                block_j += 3;
                if (block_j == 9) {
                    block_j = 0;
                    block_i += 3;
                }
            }
            empty_num -= 18;
            while (empty_num > 0) {
                int i = gen_rand(0, 8);
                int j = gen_rand(0, 8);
                if (copy[i][j] != 0) {
                    copy[i][j] = 0;
                    empty_num--;
                }
            }
            if (!unique) {
                boards.push_back(copy);
                break;
            }
            else {
                board to_solve;
                for (int i = 0; i < 9; i++) {
                    for (int j = 0; j < 9; j++) {
                        to_solve[i][j] = copy[i][j];
                    }
                }
                current_num = 0;
                search(0, 0, to_solve, 2);
                if (current_num == 1) {
                    boards.push_back(copy);
                    break;
                }
            }
        }
    }
    return boards;
}
