
#include <iostream>
#include <string>

using namespace std;

// declaring function prototypes

// game loop functions
void initBoard(char[9]);
bool gameOver(char[9]);
bool boardFull(char[9]);
bool playerWin(char[9], char);
bool posAvailable(char[9], int);
void placeOnBoard(char[9], int&, char);

// prompts
int userInput(char[9]);
int gameModePrompt();

// display
void display(char[9]);
char[][3] from1Dto2D(char[9]);
void printBuffer(int);
bool isEven(int);

// AI
int computerMove(char[9], char, char);
int completeRow(char[9], int[][3], char);
bool checkCorner(char[9]);
bool checkOppositeCorners(char[9], char);
void createAlignments(int[][3]);
void align(int[][3], int&, int, int);

//debugging functions
void dp(char[9]);
void test();
void initQ(int[][3]);
void print2dArray(int[][3]);
void testAlignments();

char intToChar(int);

// board symbols
const char X = 'X';
const char O = 'O';

int main() {
	char board[9];
	bool userTurn = true;
	char replay = 'y';

	// replay the game as often as the player likes until they quit
	while (replay == 'y' || replay == 'Y') {
		initBoard(board);
		bool playerVsAi = false;
		bool aiVsAi = false;
		bool playerVsPlayer = false;
		

		if (gameModePrompt() == '1')
		{
			playerVsAi = true;
		}
		else if (gameModePrompt() == '2')
		{
			aiVsAi = true;
		}
		else
		{
			playerVsPlayer = true;
		}

		// main game loop
		while (!gameOver(board)) {
			int move = 0;
			display(board);
			// players OR first AI turn
			if (userTurn) {

				if (playerVsAi || playerVsPlayer) {
					move = userInput(board);
				}
				else {
					move = computerMove(board, O, X);
				}
				placeOnBoard(board, move, O);
			}
			// The opponent's turn
			else {
				if (playerVsPlayer)
				{
					move = userInput(board);
				}
				else
				{
					move = computerMove(board, X, O);
				}
				placeOnBoard(board, move, X);
			}
			userTurn = !userTurn;
			printBuffer(100);
		}

		cout << "\nwould you like to play again? 'y' for yes, 'n' for no >>> ";
		cin >> replay;
		printBuffer(100);
	}
	return 0;
}

//
// declaring function implementations
//

// fills out an empty char array with empty space chars
void initBoard(char board[9]) {
	for (int i = 0; i < 9; i++) {
		board[i] = ' ';
	}
}

// checks to see if either the board is full or if someone has won the game
bool gameOver(char board[9]) {
	if (playerWin(board, 'X')) {
		display(board);
		cout << "\n\t\tX wins!\n";
		return true;
	}
	else if (playerWin(board, 'O')) {
		display(board);
		cout << "\n\t\tO wins!\n";
		return true;
	}
	else if (boardFull(board)) {
		display(board);
		cout << "\n\t\ttie!\n";
		return true;
	}
	else {
		return false;
	}
}

// checks if there are no more empty spaces left on the board
bool boardFull(char board[9]) {
	for (int i = 0; i < 9; i++) {
		if (!(board[i] == O || board[i] == X)) {
			return false;
		}
	}
	return true;
}

// checks to see if anyone has won the game
bool playerWin(char board[9], char symbol) {
	int q[8][3];
	createAlignments(q);
	for (int i = 0; i < 8; i++) {
		int symbolCount = 0;
		for (int j = 0; j < 3; j++) {
			if (board[q[i][j]] == symbol) {
				symbolCount++;
			}
		}
		if (symbolCount == 3) {
			return true;
		}
	}
	return false;
}

// checks to see if the selected space is not taken on the board
bool posAvailable(char board[9], int space) {
	if (board[space] != ' ') {
		return false;
	}
	else if (space < 0 || space > 9) {
		return false;
	}
	return true;
}

// puts the desired symbol in the specified spot on the board
void placeOnBoard(char board[9], int& space, char symbol) {
	board[space] = symbol;
	//cout << "\nboard[" << space << "] > " << board[space] << " <\n\n";
}

// 
//
//  AI FUNCTIONS BELOW
// 
//

// the steps the AI uses to determine where to move on the board 
int computerMove(char board[9], char symbol, char opponentSymbol) {
	//create a 2d array containing all rows (vertical, horizontal, and diagonal) from the board
	int q[8][3];
	createAlignments(q);

	int move = -1;
	int condition1 = completeRow(board, q, symbol);
	int condition2 = completeRow(board, q, opponentSymbol);

	// first priority is to try to complete a row
	if (condition1 != -1) {
		if (posAvailable(board, condition1)) {
			move = condition1;
		}
	}
	// second priority is to try to block the opponent from completing a row
	else if (condition2 != -1) {
		if (posAvailable(board, condition2)) {
			move = condition2;
		}
	}
	// third priority, if the opponent has taken two opposite corners, the AI choses to  take
	// any side/edge spot, to prevent the opponent from making a fork
	else if (checkOppositeCorners(board, opponentSymbol)) {
		for (int i = 1; i < 8; i += 2) {
			if (posAvailable(board, i)) {
				move = i;
			}
		}
	}
	// fourth priorioty is to try to take the middle, if available
	else if (posAvailable(board, 4)) {
		move = 4;
	}
	// if nothing else can be done, take a corner
	else if (checkCorner(board)) {
		for (int i = 0; i < 9; i += 2) {
			if (posAvailable(board, i)) {
				move = i;
			}
		}
	}
	// lastlty, take an open space if all else fails
	else {
		for (int i = 1; i < 9; i += 2) {
			if (posAvailable(board, i)) {
				move = i;
			}
		}
	}
	return move;
}

// checks to see if there is a row that has two of the same symbol and one empty space, 
// so as to either win by completing that row, or block the opponent from winning
int completeRow(char board[9], int q[][3], char symbol) {
	for (int i = 0; i < 8; i++) {
		int symbolCount = 0;
		for (int j = 0; j < 3; j++) {
			if (board[q[i][j]] == symbol) {
				symbolCount++;
			}
		}

		// if it finds two of the same symbol in a row, then it finds if the other spot
		// in that row is an empty space and returns that index if it is
		if (symbolCount == 2) {
			for (int j = 0; j < 3; j++) {
				if (board[q[i][j]] == ' ') {
					return q[i][j];
				}
			}
		}
	}
	return -1;
}

// find if there is an empty corner space on the board
bool checkCorner(char board[9]) {
	for (int i = 0; i < 9; i += 2) {
		if (posAvailable(board, i)) {
			return true;
		}
	}
	return false;
}

// check to see if the two corners diagonally opposite eachother on the board have the same symbol
bool checkOppositeCorners(char board[9], char symbol) {
	if (board[0] == symbol && board[8] == symbol) {
		return true;
	}
	if (board[2] == symbol && board[6] == symbol) {
		return true;
	}
	return false;
}

// create a 2d array that contains all possible rows/alignments for which can win by having '3' in a row
// the 2d array is an integer 8x3 array and contains the index of the corresponding spot on the game board,
// the pattern for the alignments is created algorithmically here.
void createAlignments(int alignments[][3]) {
	int c = 0;

	for (int i = 1; i < 5; i++) {
		if (i % 2 == 0) {
			if (i == 2) {
				align(alignments, c, 2, i);
			}
			else {
				align(alignments, c, 0, i);
			}
		}
		else {
			for (int j = 0; j < 3; j++) {
				if (i == 1) {
					align(alignments, c, j * 3, i);
				}
				else {
					align(alignments, c, j, i);
				}
			}
		}
	}
}

// companion function to createAlignments(), it fills out each row of the 8x3 array by starting at a
// given integer and incrementing by the given skip argument
void align(int q[][3], int &c, int pos, int skip) {
	for (int i = 0; i < 3; i++) {
		q[c][i] = pos;
		pos += skip;
	}
	c++;
}

//
//
// Prompts & Dialog Functions
//
//

// container for the dialog and prompts that are needed for the user to play the game.
int userInput(char board[9]) {
	char space;
	int boardIndex;

	cout << "\nEnter the location where you want to place your mark.\n(type '?' for a hint)\n\n";
	cout << "\t\tspace: ";
	cin >> space;

	boardIndex = space - 49;

	if (space == '?') {
		cout << "\n\tHINT: claim position " << computerMove(board, O, X) + 1 << "\n";
		boardIndex = userInput(board);
	}

	if (!posAvailable(board, boardIndex)) {
		cout << "that is not a valid location, try again." << endl;
		boardIndex = userInput(board);
	}
	return boardIndex;
}

int gameModePrompt() {
	char answer;
	cout << "Please select a game mode:\n" << endl;
	cout << "\t\t1. Player vs Computer AI" << endl;
	cout << "\t\t2. AI vs AI" << endl;
	cout << "\t\t3. Player vs Player";
	cout << "\n\n\tchoose a number >> ";
	cin >> answer;
	printBuffer(100);

	if ((answer == '1') || (answer == '2') || (answer == '3')) {
		return answer;
	}
	else {
		cout << "Not a valid response. Please try again.\n";
		return gameModePrompt();
	}
}

//
//
// Printing/Display Fucntions
//
//

// displays a pretty tic tac toe board with formatting
void display(char board[9]) {
	// convert the board array into a 2D array  and number empty spaces
	char board2D[3][3];
	int count = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[count] == ' ') {
				board2D[i][j] = intToChar(count + 1);
			}
			else {
				board2D[i][j] = board[count];
			}
			count++;
		}
	}

	// transpose the board onto the octothorpe diagram of the tic-tac-toe board
	string displayBoard[5][5];
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (isEven(i) && !isEven(j)) {
				displayBoard[i][j] = "|";
			}
			else if (!isEven(i) && !isEven(j)) {
				displayBoard[i][j] = "+";
			}
			else if (!isEven(i) && isEven(j)) {
				displayBoard[i][j] = "---";
			}
			else {
				string buffer = "  ";
				displayBoard[i][j] = buffer.insert(1, 1, board2D[i / 2][j / 2]);
			}
		}
	}

	// adds an 8 space buffer between the left side of the console and the tic tac toe board. 
	string sideBuffer[5][6];
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 6; j++) {
			if (j < 1) {
				sideBuffer[i][j] = "\t\t";
			}
			else {
				sideBuffer[i][j] = displayBoard[i][j - 1];
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 6; j++) {
			cout << sideBuffer[i][j];
		}
		cout << endl;
	}
	cout << endl;
}

char[][3] from1Dto2D(char board[9]) // converts a 1D array into a 2D array
{
	char board2D[3][3];
	int count = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[count] == ' ') {
				board2D[i][j] = intToChar(count + 1);
			}
			else {
				board2D[i][j] = board[count];
			}
			count++;
		}
	}
	return board2D;
}

// prints n lines, used for refreshing/ updating the board every turn
void printBuffer(int lines) {
	for (int i = 0; i < lines; i++) {
		cout << endl;
	}
}

// check if integer is even, only used by the display() Function.
bool isEven(int i) {
	if (i % 2 == 0) {
		return true;
	}
	return false;
}

//
//
// THE FUNCTIONS BELOW ARE FOR DUBUGGING PURPOSES ONLY
//
//

void initQ(int q[][3]) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j<3; j++) {
			q[i][j] = 0;
		}
	}
}

void print2dArray(int arr[][3]) {
	for (int x = 0; x < 8; x++) {
		cout << endl;
		for (int y = 0; y < 3; y++) {
			cout << arr[x][y] << " ";
		}
	}
}

void testAlignments() {
	int q[8][3];
	initQ(q);
	createAlignments(q);
	print2dArray(q);
}

void test() {
	char testBoard[9] = { 'X', ' ', ' ', 'X', ' ', ' ', ' ', ' ', ' ' };
	int q[8][3];
	createAlignments(q);
	int result = completeRow(testBoard, q, 'X');
	cout << result;
}

// dumb cast int to char
char intToChar(int i) {
	char j[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	return j[i];
}





bool moveTwo(char board[9], char symbol)
{
	int total = 0;
	for (int i = 1; i < 9; i++) {
		if (board[i] == symbol) {
			total++;
		}
	}
	if (total == 2)
		return true;
	return false;
}

int defeatFork(char board[9], char symbol)
{
	if (moveTwo(board, symbol))
	{
		if(!posAvailable(board, 1)&&!posAvailable(board, 6))
		{ 
			return 0;
		}
		if ((posAvailable(board, 1) && !posAvailable(board, 8))
		{
			return 2;
		}
		if(!posAvailable(board, 3) && (!posAvailable(board, 2))
		{ 
			return 0;
		}
		if (!posAvailable(board, 3) && !posAvailable(board, 8))
		{
			return 6;
		}
		if(!posAvailable(board, 5) && !posAvailable(board, 0))
		{ 
			return 2;
		}
		if (!posAvailable(board, 5) && !posAvailable(board, 6))
		{
			return 8;
		}
		if (!posAvailable(board, 7) && !posAvailable(board, 0))
		{
			return 6;
		}
		if (!posAvailable(board, 7) && !posAvailable(board, 2))
		{
			return 8;
		}
	}

	return -1;
}
