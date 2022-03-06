#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <cmath>
#include <cctype>

using namespace std;

class chess_game {
 private:
	 vector<vector<char>> game_board;
	 int num_players;
	 bool whites_turn = true;
	 map<char, int> vals;
	 bool check = false;
	 bool checkmate = false;

	 //converts board indexing to computer indexing for ease of use during functions
	 pair<int, int> board2comp(pair<int, int> boardPos) {
		 return { 9 - boardPos.first, boardPos.second };
	 }

	 //this function uses computer indexing
	 char check_white_piece(pair<int, int> boardPos) {
		 //cout << game_board[pos.first][pos.second] << endl;
		 pair<int, int> pos = board2comp(boardPos);
		 if (game_board[pos.first][pos.second] == 'P') return 'p';
		 else if (game_board[pos.first][pos.second] == 'R') return 'r';
		 else if (game_board[pos.first][pos.second] == 'K') return 'k';
		 else if (game_board[pos.first][pos.second] == 'B') return 'b';
		 else if (game_board[pos.first][pos.second] == 'Q') return 'q';
		 else if (game_board[pos.first][pos.second] == 'X') return 'x';
		 else return 'n';
	 }

	 //this function uses computer indexing
	 char check_black_piece(pair<int, int> boardPos) {
		 pair<int, int> pos = board2comp(boardPos);
		 if (game_board[pos.first][pos.second] == 'p') return 'p';
		 else if (game_board[pos.first][pos.second] == 'r') return 'r';
		 else if (game_board[pos.first][pos.second] == 'k') return 'k';
		 else if (game_board[pos.first][pos.second] == 'b') return 'b';
		 else if (game_board[pos.first][pos.second] == 'q') return 'q';
		 else if (game_board[pos.first][pos.second] == 'x') return 'x';
		 else return 'n';
	 }

	 //returns true if there is a piece at the given location
	 bool check_piece(pair<int, int> pos) {
		 return !(check_white_piece(pos) == 'n' && check_black_piece(pos) == 'n');
	 }

	 bool check_pawn_move(pair<int, int> start, pair<int, int> end) {
		 //cout << start.first << " " << start.second << endl;
		 //cout << end.first << " " << end.second << endl;
		 if (whites_turn) {
			 if (start.first == 2) {
				 if (start.second == end.second) {
					 if(!(end.first - start.first <= 2 && end.first - start.first > 0)) return false;
					 if (check_piece({ start.first + 1, start.second })) return false;
					 if (end.first - start.first == 2) {
						 if (check_piece(end)) return false;
					 }
				 }
				 else {
					 if (start.second == end.second - 1 || start.second == end.second + 1) {
						 if (end.first - start.first == 1) {
							 if (check_black_piece(end) == 'n') return false;
						 }
						 else return false;
					 }
					 else return false;
				 }
			 }
			 else {
				 if (start.second == end.second) {
					 if (!(end.first - start.first == 1)) return false;
					 if (check_piece({ start.first, start.second + 1 })) return false;
				 }
				 else {
					 if (start.second == end.second - 1 || start.second == end.second + 1) {
						 if (end.first - start.first == 1) {
							 if (!(check_black_piece(end) == 'n')) return false;
						 }
						 else return false;
					 }
					 else return false;
				 }
			 }
		 }
		 else {
			 if (start.first == 7) {
				 if (start.second == end.second) {
					 if (!(start.first - end.first <= 2 && start.first - end.first > 0)) return false;
					 if (check_piece({ start.first - 1, start.second })) return false;
					 if (start.first - end.first == 2) {
						 if (check_piece(end)) return false;
					 }
				 }
				 else {
					 if (start.second == end.second - 1 || start.second == end.second + 1) {
						 if (start.first - end.first == 1) {
							 if (check_white_piece(end) == 'n') return false;
						 }
						 else return false;
					 }
					 else return false;
				 }
			 }
			 else {
				 if (start.second == end.second) {
					 if (!(start.first - end.first == 1)) return false;
					 if (check_piece({ start.first - 1, start.second })) return false;
				 }
				 else {
					 if (start.second == end.second - 1 || start.second == end.second + 1) {
						 if (start.first - end.first == 1) {
							 if (check_white_piece(end) == 'n') return false;
						 }
						 else return false;
					 }
					 else return false;
				 }
			 }
		 }
		 return true;
	 }

	 bool check_rook_move(pair<int, int> start, pair<int, int> end) {
		 if (start.first != end.first && start.second != end.second) return false;
		 if (start.first == end.first) {
			 if (start.second < end.second) {
				 ++start.second;
				 while (start.second < end.second) {
					 if (check_piece(start)) return false;
					 ++start.second;
				 }
			 }
			 else if (start.second > end.second) {
				 --start.second;
				 while (start.second > end.second) {
					 if (check_piece(start)) return false;
					 --start.second;
				 }
			 }
			 if (whites_turn) {
				 if (!(check_white_piece(end) == 'n')) return false;
			 }
			 else if (!whites_turn) {
				 if (!(check_black_piece(end) == 'n')) return false;
			 }
		 }
		 return true;
	 }

	 bool check_knight_move(pair<int, int> start, pair<int, int> end) {
		 if (start.first == end.first + 1 || start.first == end.first - 1) {
			 if (!(start.second == end.second + 2 || start.second == end.second - 2)) return false;
		 }
		 else if (start.second == end.second + 1 || start.second == end.second - 1) {
			 if (!(start.first == end.first + 2 || start.first == end.first - 2)) return false;
		 }
		 if (whites_turn) {
			 if (!(check_white_piece(end) == 'n')) return false;
		 }
		 else if (!whites_turn) {
			 if (!(check_black_piece(end) == 'n')) return false;
		 }
	 }

	 bool check_bishop_move(pair<int, int> start, pair<int, int> end) {
		 if (!(abs(start.first - end.first) == abs(start.second - end.second))) return false;
		 if (start.first < end.first && start.second < end.second) {
			 ++start.first;
			 ++start.second;
			 while (start != end) {
				 if (check_piece(start)) return false;
				 ++start.first;
				 ++start.second;
			 }
		 }
		 else if (start.first < end.first && start.second > end.second) {
			 ++start.first;
			 --start.second;
			 while (start != end) {
				 if (check_piece(start)) return false;
				 ++start.first;
				 --start.second;
			 }
		 }
		 else if (start.first > end.first && start.second < end.second) {
			 --start.first;
			 ++start.second;
			 while (start != end) {
				 if (check_piece(start)) return false;
				 --start.first;
				 ++start.second;
			 }
		 }
		 else if (start.first > end.first && start.second > end.second) {
			 --start.first;
			 --start.second;
			 while (start != end) {
				 if (check_piece(start)) return false;
				 --start.first;
				 --start.second;
			 }
		 }
		 if (whites_turn) {
			 if (!(check_white_piece(end) == 'n')) return false;
		 }
		 else if (!whites_turn) {
			 if (!(check_black_piece(end) == 'n')) return false;
		 }
	 }

	 bool check_queen_move(pair<int, int> start, pair<int, int> end) {
		 return (check_bishop_move(start, end) || check_rook_move(start, end));
	 }

	 bool check_king_move(pair<int, int> start, pair<int, int> end) {
		 if (((abs(start.first - end.first) + abs(start.second - end.second)) == 1)) {
			 return check_rook_move(start, end);
		 }
		 else if (((abs(start.first - end.first) + abs(start.second - end.second)) == 2)) {
			 return check_bishop_move(start, end);
		 }
		 else return false;
	 }
 public:
	 chess_game() {
		 vals['a'] = 1;
		 vals['b'] = 2;
		 vals['c'] = 3;
		 vals['d'] = 4;
		 vals['e'] = 5;
		 vals['f'] = 6;
		 vals['g'] = 7;
		 vals['h'] = 8;
		 game_board.resize(9, vector<char>(9));
		 game_board[0][0] = 'W';
		 int char_index = 97;
		 int int_index = 56;
		 for (int i = 1; i < 9; i++) game_board[0][i] = char(char_index++);
		 for (int j = 1; j < 9; j++) game_board[j][0] = char(int_index--);
		 for (int k = 1; k < 9; ++k) {
			 if (k == 1 || k == 8) {
				 game_board[1][k] = 'r';
				 game_board[8][k] = 'R';
			 }
			 else if (k == 2 || k == 7) {
				 game_board[1][k] = 'k';
				 game_board[8][k] = 'K';
			 }
			 else if (k == 3 || k == 6) {
				 game_board[1][k] = 'b';
				 game_board[8][k] = 'B';
			 }
			 else if (k == 4) {
				 game_board[1][k] = 'q';
				 game_board[8][k] = 'Q';
			 }
			 else {
				 game_board[1][k] = 'x';
				 game_board[8][k] = 'X';
			 }
			 game_board[2][k] = 'p';
			 game_board[7][k] = 'P';
		 }
		 for (int l = 3; l < 7; l++) {
			 for (int m = 1; m < 9; m++) {
				 game_board[l][m] = '.';
			 }
		 }
	 }

	 void initialize_game() {
		 cout << "Welcome to the Gushee chess simulator!\n";
		 cout << "Version 1.0 supports standard two player chess games, but does not yet enforce check or checkmate, so please play by the rules!!\n";
		 cout << "To move, please write board coordinates as <letter><number>\n";
		 cout << "Enjoy the game!!!\n";
		 cout << "-----------------------------------------------------------\n\n\n";
	 }

	 void print_board() {
		 for (int i = 0; i < 9; i++) {
			 for (int j = 0; j < 9; j++) {
				 cout << game_board[i][j] << " ";
				 if (j == 0) {
					 cout << "| ";
				 }
			 }
			 cout << endl;
			 if (i == 0) {
				 cout << "- + - - - - - - - -\n";
			 }
		 }
		 cout << "\n";
	 }

	 void make_bot_move() {
		
	 }

	 void make_player_move() {
		 char letter;
		 int number;
		 bool valid_start = false;
		 char p_type;
		 pair<int, int> computer_start;
		 pair<int, int> board_start;
		 while (!valid_start) {
			 cout << "Enter position of piece to be moved: ";
			 bool in_range = true;
			 bool correct_color = true;
			 cin >> letter >> number;
			 cout << endl;
			 if (vals[letter] == 0 || number < 1 || number > 8) {
				 cout << "Entry out of board range, please try again\n";
				 in_range = false;
			 }
			 board_start = { number, vals[letter] };
			 computer_start = { 9 - number, vals[letter] };
			 if (whites_turn) {
				 p_type = check_white_piece(board_start);
			 }
			 else if (!whites_turn) {
				 p_type = check_black_piece(board_start);
			 }
			 if (p_type == 'n') {
				 cout << "Entry is not the location of a piece of your color, please try again\n";
				 correct_color = false;
			 }
			 valid_start = in_range && correct_color;
		 }
		 bool valid_end = false;
		 pair<int, int> board_end;
		 pair<int, int> computer_end;
		 while (!valid_end) {
			 cout << "Enter position for piece to be moved to: ";
			 cin >> letter >> number;
			 cout << endl;
			 board_end = { number, vals[letter] };
			 computer_end = { 9 - number, vals[letter] };
			 //for debugging
			 //cout << end_location.first << " " << end_location.second << endl;
			 if (p_type == 'p') {
				 if(check_pawn_move(board_start, board_end)) valid_end = true;
				 else {
					 cout << "Invalid pawn move. Please try again\n";
				 }
			 }
			 else if (p_type == 'r') {
				 if(check_rook_move(board_start, board_end)) valid_end = true;
				 else {
					 cout << "Invalid rook move. Please try again\n";
				 }
			 }
			 else if (p_type == 'k') {
				 if(check_knight_move(board_start, board_end)) valid_end = true;
				 else {
					 cout << "Invalid knight move. Please try again\n";
				 }
			 }
			 else if (p_type == 'b') {
				 if(check_bishop_move(board_start, board_end)) valid_end = true;
				 else {
					 cout << "Invalid bishop move. Please try again\n";
				 }
			 }
			 else if (p_type == 'q') {
				 if(check_queen_move(board_start, board_end)) valid_end = true;
				 else {
					 cout << "Invalid queen move. Please try again\n";
				 }
			 }
			 else {
				 if(check_king_move(board_start, board_end)) valid_end = true;
				 else {
					 cout << "Invalid king move. Please try again\n";
				 }
			 }
		 }
		 game_board[computer_start.first][computer_start.second] = '.';
		 if (whites_turn) p_type = toupper(p_type);
		 game_board[computer_end.first][computer_end.second] = p_type;
	 }

	 void initialize_players() {
		 cout << "Enter number of human players: ";
		 cin >> num_players;
		 cout << endl;
	 }

	 void play_bvb() {
	 
	 }

	 void play_pvb() {
	 
	 }

	 void play_pvp() {
		 while (!checkmate) {
			 if (whites_turn) cout << "White's move\n\n";
			 else cout << "Black's move\n\n";
			 print_board();
			 make_player_move();
			 whites_turn = !whites_turn;
		 }
	 }

	 void play_game() {
		 initialize_game();
		 initialize_players();
		 if (num_players == 0) play_bvb();
		 else if (num_players == 1) play_pvb();
		 else play_pvp();
	 }
};