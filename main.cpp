
#include <iostream>
#include <string>

using namespace std;

//declaring function prototypes
bool playerWin(char[9], char);
bool boardFull(char[9]);
bool gameOver(char[9]);

bool isEven(int);
void printBuffer(int);
void display(char[9], string[][5]);
void initBoard(char[9]);
void userInput(char[9], int&);
bool posAvailable(char[9], int);
void hintPrompt();
int computerMove(char[9]);
void placeOnBoard(char[9], int&, char);
void align(int [][3], int&, int, int);
void createAlignments(int [][3]);
int completeRow(char[9], int[][3], char);

//debugging functions
void test();
void initQ(int [][3]);
void print2dArray(int [][3]);
void testAlignments();
 

int main() {

    test();
    //testAlignments();

    string displayBoard[5][5];
    char board[9];

    bool userTurn = true;
    int space;
    int checkPos;
    char replay = 'y';

    while(replay == 'y' || replay == 'Y'){
        initBoard(board);

        while(!gameOver(board)){  
            if (userTurn){
                display(board, displayBoard);
                userInput(board, space);
                board[space] = 'X';
            }
            else{
                int move = computerMove(board);
                printBuffer(100);
                cout << "(the AI moved to spot x, y)\n" << endl;
            }
            userTurn = !userTurn;
        }

        cout << "would you like to play again? 'y' for yes, 'n' for no >>> ";
        cin >> replay;
    }

    return 0;
}

//
//declaring function implementations
//

bool isEven(int checkPos) {
    if (checkPos % 2 == 0) {
        return true;
    }
    return false;
}

// fills out an empty char array with empty space chars
void initBoard(char board[9]) {
    for (int i = 0; i < 9; i++) {
        board[i] = ' ';
    }
}

// displays a pretty tic tac toe board
// with formatting
void display(char board[9], string displayBoard[][5]) {

    // number the empty spaces of the board
    for(int i = 0; i < 9; i++){
        if(board[i] == ' '){
            char number = i;
            board[i] = i;
        }
    }

    // convert the board array into a 2D array for display purposes
    char board2D[3][3];
    int count = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; i < 3; j++){
            board2D[i][j] = board[count];
            count++;
        }
    }

    // transpose the board onto the octothorpe diagram of the tic-tac-toe board
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (isEven(i) && !isEven(j)) {
                displayBoard[i][j] = "|";
            } else if (!isEven(i) && !isEven(j)) {
                displayBoard[i][j] = "+";
            } else if (!isEven(i) && isEven(j)) {
                displayBoard[i][j] = "---";
            } else {
                string buffer = "  ";
                displayBoard[i][j] = buffer.insert(1, 1, board2D[i / 2][j / 2]);
            }
        }
    }

    // adds an 8 space buffer between the left side
    // of the console and the tic tac toe board. 
    string sideBuffer[5][6];
    for(int i = 0; i < 5; i++){
        for (int j = 0; j < 6; j++){
            if(j < 1){
                sideBuffer[i][j] = "\t\t";
            }
            else{
                sideBuffer[i][j] = displayBoard[i][j-1];
            }
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            cout << sideBuffer[i][j];
        }
        cout << endl;
    }
}

// container for the dialog and prompts that are needed
// for the user to play the game.
void userInput(char board[9], int& space) {
    cout << "\nEnter the location where you want to place your mark.\n" << endl;
    cout << "\t\tspace: ";
    cin >> space;
    space -= 1;
    if(!posAvailable(board, space)){
        cout << "that is not a valid location, try again." << endl;
        userInput(board, space);
    }
}

// checks to see if the selected space is not taken
// on the board
bool posAvailable(char board[9], int space) {
    if (board[space] != ' ') {
        return false;
    } 
    else if (space < 0 || space > 9) {
        return false;
    }
    return true;
}

// asks the user if it would like to see where the AI
// suggests the best spot to move is
void hintPrompt() {
    char yesHint = ' ';
    cout << "Do you want a hint? Enter Y to get a hint. Otherwise, enter anything to move on.";
    cin >> yesHint;

    if (yesHint == 'Y' || yesHint == 'y') {
        //Imple Algo;
    }
}

// puts the desired symbol in the specified spot on the board
void placeOnBoard(char board[9], int& space, char symbol) {
    board[space] = symbol;
}

// checks to see if either the board is full or if
// someone has won the game
bool gameOver(char board[9]){
    if(playerWin(board, 'X')){
        cout << "X wins!";
        return true;
    }
    else if(playerWin(board, 'O')){
        cout << "O wins!";
        return true;
    }
    else if(boardFull(board)){
        cout << "tie!";
        return true;
    }
    else{
        return false;
    }
}

// checks if there are not any more empty spaces left
// on the board
bool boardFull(char board[9]){
    for(int i = 0; i < 9; i++){
        if(!(board[i] == 'X' || board[i] == 'O')){
            return false;
        }
    }
    return true;
}

// checks to see if anyone has won the game
bool playerWin(char board[9], char symbol){
    int q[8][3];
    createAlignments(q);
    for(int i = 0; i < 8; i++){
        int symbolCount = 0;
        for(int j = 0; j < 3; j++){
            if(board[q[i][j]] == symbol){
                symbolCount++;
            }
        }
        if(symbolCount == 3){
            return true;
        }
    }
    return false;
}

// prints n lines, used for refreshing/ updating the board every turn
void printBuffer(int lines){
    for(int i = 0; i < lines; i++){
        cout << endl;
    }
}

//
//
// AI FUNCTIONS BELOW
//
//

// the steps the AI uses to determine where to move on the board
// 
int computerMove(char board[9], char symbol, char otherSymbol){

    //create a 2d array containing all rows (vertical, horizontal, and diagonal) from the board
    int q[8][3];
    createAlignments(q);

    int move = -1;
    int condition1 = completeRow(board, q, symbol);
    int condition2 = completeRow(board, q, otherSymbol);

    // first priority os to try to complete a row
    if(condition1 != -1){
        if(posAvailable(board, condition1)){
            move = condition1;
        }
    }
    // second priority is to try to block the opponent from completing a row
    else if(condition2 != -1){
        if(posAvailable(board, condition2)){
            move = condition2;
        }
    }
    // third priorioty is to try to take the middle, if available
    else if(posAvailable(board, 4)){
        move = 4;
    }
    // lastly, if nothing else can be done, take a corner
    else{
        for(int i = 0; i < 9; i+=2){
            if(posAvailable(board, i)){
                move = i;
            }
        }
    }
    return move;
}

// checks to see if there is a row that has two of the same symbol and one empty space, 
// so as to either win by completing that row, or block the opponent from winning
int completeRow(char board[9], int q[][3], char symbol){
    for(int i = 0; i < 8; i++){
        int symbolCount = 0;
        for(int j = 0; j < 3; j++){
            if(board[q[i][j]] == symbol){
                symbolCount++;
            }
        }
        if(symbolCount == 2){
            for(int k = 0; k < 3; k++){
                if(board[q[i][k]] == ' '){
                    return q[i][k];
                }
            }
        }
    }
    return -1;
}

// companion function to createAlignments(), it fills out each row of the 8x3 array by starting at a
// given integer and incrementing by the given skip argument
void align(int q[][3], int &c, int pos, int skip){
    cout << endl;
    for(int i = 0; i < 3; i++){
        q[c][i] = pos;
        pos += skip;
    }
    c++;
}

// creates a 2d array that contains all possible rows/alignments for which can win by having '3' in a row
// the 2d array is an integer 8x3 array and contains the index of the corresponding spot on the game board,
// the pattern for the alignments is created algorithmically here.
void createAlignments(int alignments[][3]){
    int c = 0;

    for(int i = 1; i < 5; i++){
        if(i % 2 == 0){
            if(i == 2){
                align(alignments, c, 2, i);
            }
            else{
                align(alignments, c, 0, i);
            }
        }
        else{
            for(int j = 0; j < 3; j++){
                if(i == 1){
                    align(alignments, c, j*3, i);
                }
                else{
                    align(alignments, c, j, i);
                }
            }
        }
    }
}

//
//
// THE FUNCTIONS BELOW ARE FOR DUBUGGING PURPOSES ONLY
//
//

void initQ(int q[][3]){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j<3;j++){
            q[i][j]=0;
        }
    }
}

void print2dArray(int arr[][3]){
    for(int x = 0; x < 8; x++){
        cout << endl;
        for(int y = 0; y < 3; y++){
            cout << arr[x][y] << " ";
        }
    }
}

void testAlignments(){
    int q[8][3];
    initQ(q);
    createAlignments(q);
    print2dArray(q);
}

void test(){
    char testBoard[9] = {'X', ' ', ' ', 'X', ' ', ' ', ' ', ' ', ' '};
    int q[8][3];
    createAlignments(q);
    int result = completeRow(testBoard, q, 'X');
    cout << result;
}