
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

//declaring function implementations

bool isEven(int checkPos) {
    if (checkPos % 2 == 0) {
        return true;
    }
    return false;
}

void initBoard(char board[9]) {
    for (int i = 0; i < 9; i++) {
        char c = i;
        board[i] = c;
    }
}

void display(char board[9], string displayBoard[][5]) {
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
                //displayBoard[i][j] = buffer.insert(1, 1, board[i / 2][j / 2]);
            }
        }
    }
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

bool posAvailable(char board[9], int space) {
    if (board[space] != ' ') {
        return false;
    } 
    else if (space < 0 || space > 9) {
        return false;
    }
    return true;
}

void hintPrompt() {
    char yesHint = ' ';
    cout << "Do you want a hint? Enter Y to get a hint. Otherwise, enter anything to move on.";
    cin >> yesHint;

    if (yesHint == 'Y' || yesHint == 'y') {
        //Imple Algo;
    }
}

void placeOnBoard(char board[9], int& space, char symbol) {
    board[space] = symbol;
}

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

bool boardFull(char board[9]){
    for(int i = 0; i < 9; i++){
        if(!(board[i] == 'X' || board[i] == 'O')){
            return false;
        }
    }
    return true;
}

bool playerWin(char board[9], char symbol){
    return false;
}

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

int computerMove(char board[9], char symbol, char otherSymbol){

    //create a 2d array containing all rows (vertical, horizontal, and diagonal) from the board
    int q[8][3];
    createAlignments(q);

    int move = -1;
    int condition1 = completeRow(board, q, symbol);
    int condition2 = completeRow(board, q, otherSymbol);

    // try to complete a row
    if(condition1 != -1){
        if(posAvailable(board, condition1)){
            move = condition1;
        }
    }
    // try to block the opponent from completing a row
    else if(condition2 != -1){
        if(posAvailable(board, condition2)){
            move = condition2;
        }
    }
    // try to take the middle, if available
    else if(posAvailable(board, 4)){
        move = 4;
    }
    // take a corner
    else{
        for(int i = 0; i < 9; i+=2){
            if(posAvailable(board, i)){
                move = i;
            }
        }
    }
    return move;
}

int findEmpty(char board[9]){
    return 0;
}

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

void align(int q[][3], int &c, int pos, int skip){
    cout << endl;
    for(int i = 0; i < 3; i++){
        q[c][i] = pos;
        pos += skip;
    }
    c++;
}

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