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
	bool white_in_check = false;
	bool black_in_check = false;
	bool white_can_castle_l = true;
	bool white_can_castle_r = true;
	bool black_can_castle_l = true;
	bool black_can_castle_r = true;
	bool checkmate = false;

	//converts board indexing to computer indexing for ease of use during functions
	pair<int, int> board2comp(pair<int, int> boardPos) {
		return { 9 - boardPos.first, boardPos.second };
	}

	//this function uses computer indexing
	char check_white_piece(pair<int, int> boardPos) {
		//cout << game_board[pos.first][pos.second] << endl;
		pair<int, int> pos = board2comp(boardPos);
		switch (game_board[pos.first][pos.second]) {
		case 'P': return 'p';
		case 'R': return 'r';
		case 'K': return 'k';
		case 'B': return 'b';
		case 'Q': return 'q';
		case 'X': return 'x';
		default: return 'n';
		}
	}

	//this function uses computer indexing
	char check_black_piece(pair<int, int> boardPos) {
		pair<int, int> pos = board2comp(boardPos);
		switch (game_board[pos.first][pos.second]) {
		case 'p': return 'p';
		case 'r': return 'r';
		case 'k': return 'k';
		case 'b': return 'b';
		case 'q': return 'q';
		case 'x': return 'x';
		default: return 'n';
		}
	}

	//returns true if there is a piece at the given location
	bool check_piece(pair<int, int> pos) {
		return !(check_white_piece(pos) == 'n' && check_black_piece(pos) == 'n');
	}

	bool check_pawn_move(pair<int, int> start, pair<int, int> end) {
		//changing all whites turns
		if (check_white_piece(start) != 'n') {
			if (start.first == 2) {
				if (start.second == end.second) {
					if (!(end.first - start.first <= 2 && end.first - start.first > 0)) return false;
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
		pair<int, int> temp_start = start;
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
		}
		else if (start.second == end.second) {
			if (start.first < end.first) {
				++start.first;
				while (start.first < end.first) {
					if (check_piece(start)) return false;
					++start.first;
				}
			}
			else if (start.first > end.first) {
				--start.first;
				while (start.first > end.first) {
					if (check_piece(start)) return false;
					--start.first;
				}
			}
		}
		if (check_white_piece(temp_start) != 'n') {
			if (!(check_white_piece(end) == 'n')) return false;
		}
		else {
			if (!(check_black_piece(end) == 'n')) return false;
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
		else return false;
		if (check_white_piece(start) != 'n') {
			if (!(check_white_piece(end) == 'n')) return false;
		}
		else {
			if (!(check_black_piece(end) == 'n')) return false;
		}
		return true;
	}

	bool check_bishop_move(pair<int, int> start, pair<int, int> end) {
		pair<int, int> temp_start = start;
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
		if (check_white_piece(temp_start) != 'n') {
			if (!(check_white_piece(end) == 'n')) return false;
		}
		else {
			if (!(check_black_piece(end) == 'n')) return false;
		}
		return true;
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

	//use for checking during castling
	bool check_for_check_w_pos(pair<int, int> pos) {
		char piece;
		for (int k = 1; k < 9; k++) {
			for (int l = 1; l < 9; l++) {
				piece = check_black_piece({ k,l });
				switch (piece) {
				case 'p': if (check_pawn_move({ k,l }, pos)) return true;
				case 'r': if (check_rook_move({ k,l }, pos)) return true;
				case 'k': if (check_knight_move({ k,l }, pos)) return true;
				case 'b': if (check_bishop_move({ k,l }, pos)) return true;
				case 'q': if (check_queen_move({ k,l }, pos)) return true;
				case 'x': if (check_king_move({ k,l }, pos)) return true;
				default: break;
				}
			}
		}
		return false;
	}

	bool check_for_check_b_pos(pair<int, int> pos) {
		char piece;
		for (int k = 1; k < 9; k++) {
			for (int l = 1; l < 9; l++) {
				piece = check_white_piece({ k,l });
				switch (piece) {
				case 'p': if (check_pawn_move({ k,l }, pos)) return true;
				case 'r': if (check_rook_move({ k,l }, pos)) return true;
				case 'k': if (check_knight_move({ k,l }, pos)) return true;
				case 'b': if (check_bishop_move({ k,l }, pos)) return true;
				case 'q': if (check_queen_move({ k,l }, pos)) return true;
				case 'x': if (check_king_move({ k,l }, pos)) return true;
				default: break;
				}
			}
		}
		return false;
	}

	//checks if white is in check
	void check_for_check_white() {
		pair<int, int> king_pos;
		char piece;
		for (int i = 1; i < 9; i++) {
			for (int j = 1; j < 9; j++) {
				if (check_white_piece({ i,j }) == 'x') king_pos = { i,j };
			}
		}
		for (int k = 1; k < 9; k++) {
			for (int l = 1; l < 9; l++) {
				piece = check_black_piece({ k,l });
				switch (piece) {
				case 'p':
					if (check_pawn_move({ k,l }, king_pos)) {
						white_in_check = true;
						return;
					}
				case 'r':
					if (check_rook_move({ k,l }, king_pos)) {
						white_in_check = true;
						return;
					}
				case 'k':
					if (check_knight_move({ k,l }, king_pos)) {
						white_in_check = true;
						return;
					}
				case 'b':
					if (check_bishop_move({ k,l }, king_pos)) {
						white_in_check = true;
						return;
					}
				case 'q':
					if (check_queen_move({ k,l }, king_pos)) {
						white_in_check = true;
						return;
					}
				case 'x':
					if (check_king_move({ k,l }, king_pos)) {
						white_in_check = true;
						return;
					}
				default: break;
				}
			}
		}
		white_in_check = false;
	}

	//checks if black is in check
	void check_for_check_black() {
		pair<int, int> king_pos;
		char piece;
		for (int i = 1; i < 9; i++) {
			for (int j = 1; j < 9; j++) {
				if (check_black_piece({ i,j }) == 'x') king_pos = { i,j };
			}
		}
		for (int k = 1; k < 9; k++) {
			for (int l = 1; l < 9; l++) {
				piece = check_white_piece({ k,l });
				switch (piece) {
				case 'p':
					if (check_pawn_move({ k,l }, king_pos)) {
						black_in_check = true;
						return;
					}
				case 'r':
					if (check_rook_move({ k,l }, king_pos)) {
						black_in_check = true;
						return;
					}
				case 'k':
					if (check_knight_move({ k,l }, king_pos)) {
						black_in_check = true;
						return;
					}
				case 'b':
					if (check_bishop_move({ k,l }, king_pos)) {
						black_in_check = true;
						return;
					}
				case 'q':
					if (check_queen_move({ k,l }, king_pos)) {
						black_in_check = true;
						return;
					}
				case 'x':
					if (check_king_move({ k,l }, king_pos)) {
						black_in_check = true;
						return;
					}
				default: break;
				}
			}
		}
		black_in_check = false;
	}

	//note to self
	//need to fix the edge case where a player attempts to move their king 
	//for the first time but the move would result in check -- the can_castle
	//bools will be false because they attempted to move their king even though
	//they did not end up moving it, so they still should be able to castle
	bool check_valid_castle(char s) {
		if (whites_turn) {
			switch (s) {
			case 'a':
				if (!white_can_castle_l) {
					cout << "You have already moved either the rook or the king, and cannot castle. Please try another move\n";
					return false;
				}
				if (check_piece({ 1,2 }) || check_piece({ 1,3 }) || check_piece({ 1,4 })) {
					cout << "You cannot castle through pieces, please try another move\n";
					return false;
				}
				if (check_for_check_w_pos({ 1,2 }) || check_for_check_w_pos({ 1,3 }) || check_for_check_w_pos({ 1,4 })) {
					cout << "You cannot castle through check, please try another move\n";
					return false;
				}
				if (check_for_check_w_pos({ 1,1 })) {
					cout << "You cannot castle into check, please try another move\n";
					return false;
				}
				return true;
			case 'h':
				if (!white_can_castle_r) {
					cout << "You have already moved either the rook or the king, and cannot castle. Please try another move\n";
					return false;
				}
				if (check_piece({ 1,6 }) || check_piece({ 1,7 })) {
					cout << "You cannot castle through pieces, please try another move\n";
					return false;
				}
				if (check_for_check_w_pos({ 1,6 }) || check_for_check_w_pos({ 1,7 })) {
					cout << "You cannot castle through check, please try another move\n";
					return false;
				}
				if (check_for_check_w_pos({ 1,8 })) {
					cout << "You cannot castle into check, please try another move\n";
					return false;
				}
				return true;
			}
		}
		else {
			switch (s) {
			case 'a':
				if (!black_can_castle_l) {
					cout << "You have already moved either the rook or the king, and cannot castle. Please try another move\n";
					return false;
				}
				if (check_piece({ 8,2 }) || check_piece({ 8,3 }) || check_piece({ 8,4 })) {
					cout << "You cannot castle through pieces, please try another move\n";
					return false;
				}
				if (check_for_check_b_pos({ 8,2 }) || check_for_check_b_pos({ 8,3 }) || check_for_check_b_pos({ 8,4 })) {
					cout << "You cannot castle through check, please try another move\n";
					return false;
				}
				if (check_for_check_b_pos({ 8,1 })) {
					cout << "You cannot castle into check, please try another move\n";
					return false;
				}
				return true;
			case 'h':
				if (!black_can_castle_r) {
					cout << "You have already moved either the rook or the king, and cannot castle. Please try another move\n";
					return false;
				}
				if (check_piece({ 8,6 }) || check_piece({ 8,7 })) {
					cout << "You cannot castle through pieces, please try another move\n";
					return false;
				}
				if (check_for_check_b_pos({ 8,6 }) || check_for_check_b_pos({ 8,7 })) {
					cout << "You cannot castle through check, please try another move\n";
					return false;
				}
				if (check_for_check_b_pos({ 8,8 })) {
					cout << "You cannot castle into check, please try another move\n";
					return false;
				}
				return true;
			}
		}
		return false;
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
			switch (k) {
			case 1:
				game_board[1][k] = 'r';
				game_board[8][k] = 'R';
				break;
			case 2:
				game_board[1][k] = 'k';
				game_board[8][k] = 'K';
				break;
			case 3:
				game_board[1][k] = 'b';
				game_board[8][k] = 'B';
				break;
			case 4:
				game_board[1][k] = 'q';
				game_board[8][k] = 'Q';
				break;
			case 5:
				game_board[1][k] = 'x';
				game_board[8][k] = 'X';
				break;
			case 6:
				game_board[1][k] = 'b';
				game_board[8][k] = 'B';
				break;
			case 7:
				game_board[1][k] = 'k';
				game_board[8][k] = 'K';
				break;
			case 8:
				game_board[1][k] = 'r';
				game_board[8][k] = 'R';
				break;
			default: break;
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
		cout << "Version 1.2 supports standard two player chess games, and \nnow enforces check and allows the player to castle!!\n";
		cout << "However, it still does not support checkmate, pawn promotion, or the en passant.\n";
		cout << "Don't worry -- the developers are hard at work implementing these features!\n";
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

	//note to self
	//add switch case where possible
	void make_player_move() {
		char letter;
		char p_type;
		string move;
		int number;
		bool valid_start = false;
		bool valid_end = false;
		pair<int, int> computer_start;
		pair<int, int> board_start;
		pair<int, int> board_end;
		pair<int, int> computer_end;
		if ((whites_turn && white_in_check) || !whites_turn && black_in_check) cout << "You are in check! Make a move that will keep your king out of danger!\n";
		while (!valid_end) {
			while (!valid_end) {
				while (!valid_start) {
					cout << "Enter position of piece to be moved: ";
					bool in_range = true;
					bool correct_color = true;
					//cin >> letter >> number;
					cin >> move;
					cout << endl;
					if (move == "castle") {
						bool valid_side = false;
						char side;
						cout << "Enter the side you would like to castle to ('a' or 'h'): ";
						while (!valid_side) {
							cin >> side;
							cout << endl;
							if (side != 'a' && side != 'h') cout << "Please enter a valid side ('a' or 'h')\n";
							else valid_side = true;
						}
						if (check_valid_castle(side)) {
							if (whites_turn) {
								if (side == 'a') {
									//king goes to c1, rook goes to d1
									game_board[8][vals['c']] = 'X';
									game_board[8][vals['e']] = '.';
									game_board[8][vals['d']] = 'R';
									game_board[8][vals['a']] = '.';
								}
								else {
									//king goes to g1, rook goes to f1
									game_board[8][vals['g']] = 'X';
									game_board[8][vals['e']] = '.';
									game_board[8][vals['f']] = 'R';
									game_board[8][vals['h']] = '.';
								}
							}
							else {
								if (side == 'a') {
									//king goes to c8, rook goes to d8
									game_board[1][vals['c']] = 'x';
									game_board[1][vals['e']] = '.';
									game_board[1][vals['d']] = 'r';
									game_board[1][vals['a']] = '.';
								}
								else {
									//king goes to g8, rook goes to f8
									game_board[1][vals['g']] = 'x';
									game_board[1][vals['e']] = '.';
									game_board[1][vals['f']] = 'r';
									game_board[1][vals['h']] = '.';
								}
							}
							return;
						}
						else {
							cout << "Invalid castle, please enter another move\n\n";
						}
					}
					else if (move.size() != 2 || vals[move[0]] == 0 || !(int(move[1]) > 48 && int(move[1]) < 57)) {
						cout << "Please enter a valid move input, which can either be <letter><number>, or \"castle\"\n";
						valid_start = false;
					}
					else {
						letter = move[0];
						number = int(move[1]) - 48;
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
				}
				while (!valid_end) {
					cout << "Enter position for piece to be moved to: ";
					cin >> letter >> number;
					cout << endl;
					if (letter == 'n') {
						valid_start = false;
						break;
					}
					board_end = { number, vals[letter] };
					computer_end = { 9 - number, vals[letter] };
					switch (p_type) {
					case 'p':
						if (check_pawn_move(board_start, board_end)) valid_end = true;
						else cout << "Invalid pawn move. Please try again, or select a new piece by entering n0\n";
						break;
					case 'r':
						if (check_rook_move(board_start, board_end)) {
							valid_end = true;
							if (whites_turn) {
								if (letter == 'a') white_can_castle_l = false;
								else if (letter == 'h') white_can_castle_r = false;
							}
							else {
								if (letter == 'a') black_can_castle_l = false;
								else if (letter == 'h') black_can_castle_r = false;
							}
						}
						else cout << "Invalid rook move. Please try again, or select a new piece by entering n0\n";
						break;
					case 'k':
						if (check_knight_move(board_start, board_end)) valid_end = true;
						else cout << "Invalid knight move. Please try again, or select a new piece by entering n0\n";
						break;
					case 'b':
						if (check_bishop_move(board_start, board_end)) valid_end = true;
						else cout << "Invalid bishop move. Please try again, or select a new piece by entering n0\n";
						break;
					case 'q':
						if (check_queen_move(board_start, board_end)) valid_end = true;
						else cout << "Invalid queen move. Please try again, or select a new piece by entering n0\n";
						break;
					case 'x':
						if (check_king_move(board_start, board_end)) {
							valid_end = true;
							if (whites_turn) {
								white_can_castle_l = false;
								white_can_castle_r = false;
							}
							else {
								black_can_castle_l = false;
								black_can_castle_r = false;
							}
						}
						else cout << "Invalid king move. Please try again, or select a new piece by entering n0\n";
						break;
					default:
						break;
					}
				}
			}
			game_board[computer_start.first][computer_start.second] = '.';
			if (whites_turn) p_type = toupper(p_type);
			char temp = game_board[computer_end.first][computer_end.second];
			game_board[computer_end.first][computer_end.second] = p_type;
			if (whites_turn) {
				check_for_check_white();
				if (white_in_check) {
					cout << "You cannot make a move that results in your king being in check! Please try again, or select a new piece by entering n0\n";
					game_board[computer_start.first][computer_start.second] = p_type;
					game_board[computer_end.first][computer_end.second] = temp;
					valid_end = false;
				}
				else {
					check_for_check_black();
					if (black_in_check) cout << "Check!\n\n";
				}
			}
			else {
				check_for_check_black();
				if (black_in_check) {
					cout << "You cannot make a move that results in your king being in check! Please try again, or select a new piece by entering n0\n";
					game_board[computer_start.first][computer_start.second] = p_type;
					game_board[computer_end.first][computer_end.second] = temp;
					valid_end = false;
				}
				else {
					check_for_check_white();
					if (white_in_check) cout << "Check!\n\n";
				}
			}
		}
	}

	void initialize_players() {
		bool valid_players = false;
		while (!valid_players) {
			cout << "Enter number of human players: ";
			cin >> num_players;
			cout << endl;
			switch (num_players) {
			case 0:
				cout << "Bot versus bot play not yet available, please select another option\n";
				break;
			case 1:
				cout << "Player versus bot play not yet available, please select another option\n";
				break;
			case 2:
				valid_players = true;
				break;
			default:
				cout << "Invalid number of players. Please enter 0, 1, or 2\n";
			}
		}
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
		switch (num_players) {
		case 0:
			play_bvb();
			break;
		case 1:
			play_pvb();
			break;
		case 2:
			play_pvp();
			break;
		}
	}
};