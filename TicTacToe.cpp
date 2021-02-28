#include <iostream>
#include <algorithm>

using namespace std;

/*
	determains if a move is valid

	parameters:
		index = index on board
		board = board to check

	return:
		if valid, returns true
		else, returns false
*/
bool validMove(const int index, const char board[]) {
	if (board[index] == ' ') return true;
	return false;
}

/*
	gets valid user input

	parameters:
		board = the board to validate inputs against
		numPad = determains if placement of numbers on the num 
				 pad will match up with the placement of a tic
				 tac toe board

	return:
		an int representing the desired index on the board
*/
int getInput(const char board[], bool numPad = true) {
	int input;
	do {
		if (numPad) {
			cout << "Where do you want to play? (1-9): "; cin >> input;
			if (input > 6) input -= 7;
			else if (input < 4) input += 5;
			else input--;
		} else {
			cout << "Where do you want to play? (0-8): "; cin >> input;
		}

		if (!validMove(input, board)) cout << "Invalid Move" << endl;
		else break;

	} while (true);

	return input;
}

/*
	output the board to the console

	parameters:
		board = the board to output
*/
void outputBoard(const char board[]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << board[i * 3 + j];
			if (j != 2) cout << "|";
		}
		cout << endl;
		if (i != 2) cout << "-+-+-" << endl;
	}
}

/*
	Update board and switch players

	parameters:
		currentPlayer = current player
		index = index on board to play at
		board = board to play on
*/
void takeTurn(char &currentPlayer, const int index, char board[]) {
	board[index] = currentPlayer;
	currentPlayer = currentPlayer == 'O' ? 'X' : 'O';
}

/*
	Get the current state of the board

	parameters:
		board = the board to check

	return:
		a char representing the state of the game
		' ' = continue
 		'-' = draw
 		'O' = O wins
 		'X' = X wins
*/
char findGameState(const char board[]) {
	// check horizontal
	if (board[0] != ' ' && board[0] == board[1] && board[1] == board[2]) return board[0];
	if (board[3] != ' ' && board[3] == board[4] && board[4] == board[5]) return board[3];
	if (board[6] != ' ' && board[6] == board[7] && board[7] == board[8]) return board[6];
	// check vertical
	if (board[0] != ' ' && board[0] == board[3] && board[3] == board[6]) return board[0];
	if (board[1] != ' ' && board[1] == board[4] && board[4] == board[7]) return board[1];
	if (board[2] != ' ' && board[2] == board[5] && board[5] == board[8]) return board[2];
	// check diagonal
	if (board[0] != ' ' && board[0] == board[4] && board[4] == board[8]) return board[0];
	if (board[2] != ' ' && board[2] == board[4] && board[4] == board[6]) return board[2];
	// check for draw
	for (int i = 0; i < 9; i++)
		if (board[i] == ' ') return ' '; // there is still a spot to play, continue game
	// it is a draw
	return '-';
}

/*
	determain the outcome of a play, assuming both players are perfect

	parameters:
		board = the board after the move in question has been played
		currentPlayer = whos turn is it

	return:
		an int representing the outcome of a play, assuming both players are perfect
		1 = O wins
		0 = Draw
		-1 = X wins
*/
int minimax(char board[], char currentPlayer) {
	char gameState = findGameState(board);
	if (gameState == 'O') return 1;
	else if (gameState == 'X') return -1;
	else if (gameState == '-') return 0;

	int bestEval = currentPlayer == 'O' ? -2 : 2;

		for (int i = 0; i < 9; i++) {
			if (validMove(i, board)) {
				char newBoard[9];
				for (int j = 0; j < 9; j++)
					newBoard[j] = board[j];

				takeTurn(currentPlayer, i, newBoard);

				if (currentPlayer == 'X') bestEval = max(bestEval, minimax(newBoard, currentPlayer));
				else bestEval = min(bestEval, minimax(newBoard, currentPlayer));
		
				currentPlayer = currentPlayer == 'O' ? 'X' : 'O';
			}
		}

	return bestEval;
}

/*
	determain the best play to make

	parameters:
		currentPlayer = X or O
		board = the current board

	return:
		an int representing the best index to play on the board
*/
int findBestMove(char currentPlayer, const char board[]) {
	int bestEval;
	bestEval = currentPlayer == 'X' ? 2 : -2;
	int bestIndex;

	for (int i = 0; i < 9; i++) {
		if (validMove(i, board)) {
			char newBoard[9];
			for (int j = 0; j < 9; j++)
				newBoard[j] = board[j];
			newBoard[i] = currentPlayer;
			char nextPlayer = currentPlayer == 'X' ? 'O' : 'X';
			int eval = minimax(newBoard, nextPlayer);

			if (currentPlayer == 'X') {
				if (eval < bestEval) {
					bestEval = eval;
					bestIndex = i;
				}
			} else {
				if (eval > bestEval) {
					bestEval = eval;
					bestIndex = i;
				}
			}

		}
	}

	return bestIndex;
}

/*
	clear the board for a new game

	parameters:
		board = the board to clear
*/
void initBoard(char board[]) {
	for (int i = 0; i < 9; i++)
		board[i] = ' ';
}

int main() {
	
	// game variables
	char board[9];
	char currentPlayer;
	bool ai;
	bool starts;
	bool numPad;
	char ans;

	// game loop
	do {

		// set game variables
		initBoard(board);
		currentPlayer = 'X';

		cout << "Do you want to use a num pad? (y/n): "; cin >> ans;
		if (ans == 'y') {
			numPad = true;
			cout << "Here is how board input works:" << endl;
			cout << "7|8|9" << endl;
			cout << "-+-+-" << endl;
			cout << "4|5|6" << endl;
			cout << "-+-+-" << endl;
			cout << "1|2|3" << endl;
		} else {
			numPad = false;
			cout << "Here is how board input works:" << endl;
			cout << "0|1|2" << endl;
			cout << "-+-+-" << endl;
			cout << "3|4|5" << endl;
			cout << "-+-+-" << endl;
			cout << "6|7|8" << endl;
		}

		cout << "Do you want to play with AI? (y/n): "; cin >> ans;
		if (ans == 'y') {
			ai = true;
			cout << "Your fate has been sealed..." << endl;
			cout << "X goes first," << endl;
			cout << "Do you want to be X? (y/n): "; cin >> ans;
			if (ans == 'y') starts = true;
			else starts = false;
		} else {
			ai = false;
		}

		// round loop
		do {

			if (starts) {
				outputBoard(board);
				takeTurn(currentPlayer, getInput(board), board);
			}

			if (ai) {
				if (findGameState(board) != ' ') break;
				takeTurn(currentPlayer, findBestMove(currentPlayer, board), board);
			}

			if (!starts) {
				if (findGameState(board) != ' ') break;
				outputBoard(board);
				takeTurn(currentPlayer, getInput(board), board);
			}


		} while (findGameState(board) == ' ');

		// round ends
		outputBoard(board);
		char gameState = findGameState(board);
		if (gameState == '-') cout << "Draw!" << endl;
		else cout << gameState << " Wins!" << endl;

		cout << "Do you want to play again? (y/n): "; cin >> ans;

	} while (ans == 'y');

	cout << "Thanks for playing!" << endl;

}

