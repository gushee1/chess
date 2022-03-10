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
	bool checkmate = false;

	//converts board indexing to computer indexing for ease of use during functions
	//requires a coordinate in "board indexing" (where the vertical index counts down)
	//in order to work as expected in other functions
	pair<int, int> board2comp(pair<int, int> boardPos) {
		return { 9 - boardPos.first, boardPos.second };
	}

	//returns the type of white piece that is located at the position on the board, 
	//or returns n if the position is empty or is that of a black piece
	char check_white_piece(pair<int, int> boardPos) {
		pair<int, int> pos = board2comp(boardPos);
		if (game_board[pos.first][pos.second] == 'P') return 'p';
		else if (game_board[pos.first][pos.second] == 'R') return 'r';
		else if (game_board[pos.first][pos.second] == 'K') return 'k';
		else if (game_board[pos.first][pos.second] == 'B') return 'b';
		else if (game_board[pos.first][pos.second] == 'Q') return 'q';
		else if (game_board[pos.first][pos.second] == 'X') return 'x';
		else return 'n';
	}

	//returns the type of black piece that is located at the position on the board, 
	//or returns n if the position is empty or is that of a white piece
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

	//returns true if there is a piece (of any color) at the given location
	//this function takes coords in computer indexing but converts to board indexing 
	//within the individual checking functions
	bool check_piece(pair<int, int> pos) {
		return !(check_white_piece(pos) == 'n' && check_black_piece(pos) == 'n');
	}

	//checks to see if the input start and end coordinates are a valid move for a pawn
	bool check_pawn_move(pair<int, int> start, pair<int, int> end) {
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

	//checks to see if the input start and end coordinates are a valid move for a rook
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

	//checks to see if the input start and end coordinates are a valid move for a knight
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

	//checks to see if the input start and end coordinates are a valid move for a bishop
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

	//checks to see if the input start and end coordinates are a valid move for a queen
	//this function relies on the fact that a queen can make the same moves as a rook or a bishop
	bool check_queen_move(pair<int, int> start, pair<int, int> end) {
		return (check_bishop_move(start, end) || check_rook_move(start, end));
	}

	//checks to see if the input start and end coordinates are a valid move for a king
	bool check_king_move(pair<int, int> start, pair<int, int> end) {
		if (((abs(start.first - end.first) + abs(start.second - end.second)) == 1)) {
			return check_rook_move(start, end);
		}
		else if (((abs(start.first - end.first) + abs(start.second - end.second)) == 2)) {
			return check_bishop_move(start, end);
		}
		else return false;
	}

	//modifies the class variable "white_in_check" to be true iff white is in check
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
				if (piece != 'n') {
					if (piece == 'p') {
						if (check_pawn_move({ k,l }, king_pos)) {
							white_in_check = true;
							return;
						}
					}
					else if (piece == 'r') {
						if (check_rook_move({ k,l }, king_pos)) {
							white_in_check = true;
							return;
						}
					}
					else if (piece == 'k') {
						if (check_knight_move({ k,l }, king_pos)) {
							white_in_check = true;
							return;
						}
					}
					else if (piece == 'b') {
						if (check_bishop_move({ k,l }, king_pos)) {
							white_in_check = true;
							return;
						}
					}
					else if (piece == 'q') {
						if (check_queen_move({ k,l }, king_pos)) {
							white_in_check = true;
							return;
						}
					}
					else if (piece == 'k') {
						if (check_king_move({ k,l }, king_pos)) {
							white_in_check = true;
							return;
						}
					}
				}
			}
		}
		white_in_check = false;
	}

	//modifies the class variable "black_in_check" to be true iff black is in check
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
				if (piece != 'n') {
					if (piece == 'p') {
						if (check_pawn_move({ k,l }, king_pos)) {
							black_in_check = true;
							return;
						}
					}
					else if (piece == 'r') {
						if (check_rook_move({ k,l }, king_pos)) {
							black_in_check = true;
							return;
						}
					}
					else if (piece == 'k') {
						if (check_knight_move({ k,l }, king_pos)) {
							black_in_check = true;
							return;
						}
					}
					else if (piece == 'b') {
						if (check_bishop_move({ k,l }, king_pos)) {
							black_in_check = true;
							return;
						}
					}
					else if (piece == 'q') {
						if (check_queen_move({ k,l }, king_pos)) {
							black_in_check = true;
							return;
						}
					}
					else if (piece == 'k') {
						if (check_king_move({ k,l }, king_pos)) {
							black_in_check = true;
							return;
						}
					}
				}
			}
		}
		black_in_check = false;
	}
public:
	//default game constructor
	//sets up the board for a traditional game of chess
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

	//prints out a description of the version and game before initiating play
	void initialize_game() {
		cout << "Welcome to the Gushee chess simulator!\n";
		cout << "Version 1.1 supports standard two player chess games, and now enforces check!!\n";
		cout << "However, it still does not support checkmate, castling, pawn promotion, or the en passant.\n";
		cout << "Don't worry -- the developers are hard at work implementing these features!\n";
		cout << "To move, please write board coordinates as <letter><number>\n";
		cout << "Enjoy the game!!!\n";
		cout << "-----------------------------------------------------------\n\n\n";
	}

	//prints the chess board as it currently stands with moves accounted for
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

	//executes a bot move -- yet to be implemented
	void make_bot_move() {

	}

	//executes a player move while enforcing that the player is making a valid move
	void make_player_move() {
		char letter;
		char p_type;
		int number;
		bool valid_start = false;
		bool valid_end = false;
		pair<int, int> computer_start;
		pair<int, int> board_start;
		pair<int, int> board_end;
		pair<int, int> computer_end;
		if ((whites_turn && white_in_check) || !whites_turn && black_in_check) cout << "You are in check! Make a move that will keep your king out of danger!\n";
		while (!valid_end) {
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
				if (p_type == 'p') {
					if (check_pawn_move(board_start, board_end)) valid_end = true;
					else {
						cout << "Invalid pawn move. Please try again, or select a new piece by entering n0\n";
					}
				}
				else if (p_type == 'r') {
					if (check_rook_move(board_start, board_end)) valid_end = true;
					else {
						cout << "Invalid rook move. Please try again, or select a new piece by entering n0\n";
					}
				}
				else if (p_type == 'k') {
					if (check_knight_move(board_start, board_end)) valid_end = true;
					else {
						cout << "Invalid knight move. Please try again, or select a new piece by entering n0\n";
					}
				}
				else if (p_type == 'b') {
					if (check_bishop_move(board_start, board_end)) valid_end = true;
					else {
						cout << "Invalid bishop move. Please try again, or select a new piece by entering n0\n";
					}
				}
				else if (p_type == 'q') {
					if (check_queen_move(board_start, board_end)) valid_end = true;
					else {
						cout << "Invalid queen move. Please try again, or select a new piece by entering n0\n";
					}
				}
				else {
					if (check_king_move(board_start, board_end)) valid_end = true;
					else {
						cout << "Invalid king move. Please try again, or select a new piece by entering n0\n";
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


	//used to initialize the number of players--this function is currently
	//trivial but will be more useful when the bot move function is implemented
	void initialize_players() {
		bool valid_players = false;
		while (!valid_players) {
			cout << "Enter number of human players: ";
			cin >> num_players;
			cout << endl;
			if (num_players == 0) {
				cout << "Bot versus bot play not yet available, please select another option\n";
			}
			else if (num_players == 1) {
				cout << "Player versus bot play not yet available, please select another option\n";
			}
			else if (num_players == 2) valid_players = true;
			else {
				cout << "Invalid number of players. Please enter 0, 1, or 2\n";
			}
		}
	}

	//executes a game between two bots--not yet implemented
	void play_bvb() {

	}

	//executes a player versus bot game--not yet implemented
	void play_pvb() {

	}

	//executes a two player game of chess
	void play_pvp() {
		while (!checkmate) {
			if (whites_turn) cout << "White's move\n\n";
			else cout << "Black's move\n\n";
			print_board();
			make_player_move();
			whites_turn = !whites_turn;
		}
	}

	//executes the playing of the program, with any valid number of players 
	void play_game() {
		initialize_game();
		initialize_players();
		if (num_players == 0) play_bvb();
		else if (num_players == 1) play_pvb();
		else play_pvp();
	}
};