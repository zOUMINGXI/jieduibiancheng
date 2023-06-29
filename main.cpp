#include "cmdline.h"
#include "sudoku.h"

using namespace std;

int main(int argc, char* argv[]) {
	cmdline::parser parser;
	parser.add<int>("count", 'c', "�վ���Ϸ����", false, 20, cmdline::range(1, 1000000));
	parser.add<string>("solve", 's', "��ȡ��Ϸ�ļ�·��", false, "games.txt");
	parser.add<int>("number", 'n', "�����Ϸ����", false, 1000, cmdline::range(1, 10000));
	parser.add<int>("mode", 'm', "������Ϸ�Ѷ�", false, 1, cmdline::range(1, 3));
	parser.add<string>("range", 'r', "������Ϸ�ո�Χ", false, "20~55");
	parser.add("unique", 'u', "������Ϸ��Ψһ��");

	parser.parse_check(argc, argv);
	if (parser.exist("mode") && !parser.exist("number")) {
		cout << "m��nͬʱʹ��" << endl;
		return 0;
	}
	if (parser.exist("range") && !parser.exist("number")) {
		cout << "r��nͬʱʹ��" << endl;
		return 0;
	}
	if (parser.exist("unique") && !parser.exist("number")) {
		cout << "u��nͬʱʹ��" << endl;
		return 0;
	}

	if (parser.exist("count")) {
		int count = parser.get<int>("count");
		vector<board> final_boards = generate_final(count);
		write_boards("finals.txt", final_boards);
		return 0;
	}
	if (parser.exist("number")) {
		int number = parser.get<int>("number");
		int mode = parser.get<int>("mode");
		string range = parser.get<string>("range");
		bool unique = parser.exist("unique");
		vector<board> game_boards = generate_game(number, mode, range, unique);
		write_boards("games.txt", game_boards);
		return 0;
	}
	if (parser.exist("solve")) {
		string solve = parser.get<string>("solve");
		vector<board> game_boards = read_boards(solve);
		vector<board> answer_boards = solve_boards(game_boards);
		write_boards("answers.txt", answer_boards);
		return 0;
	}
	return 0;
}