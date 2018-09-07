#include <iostream>
#include <string>

using namespace std;

//declaring function prototypes
bool playerWin(char[][3], char);
bool boardFull(char[][3]);
bool gameOver(char[][3]);
bool isEven(int);
void printBuffer(int);
void display(char[][3], string[][5]);
void initBoard(char[][3]);
void userInput(char[][3], int&, int&);
bool posAvailable(char[][3], int&, int&);
void hintPrompt();
void AIMove(char[][3], int&, int&); //added int q char w char grid
void placeOnBoard(char[][3], int&, int&, char);
void alignGridSymb(char[][3], char[][3]);
void alignGridNum(int [][3], int&, int, int); //change name from align to alignGridNum to prevent confusion with alignGridSymb()
void createAlignments();
void print2dArray(int [][3]);
void testAlignments(int [][3]);
void createGrid(char[][3], char[]); //char board char grid
bool winOrBlock(int[][3], char[][3], char[]); //int q, char w, char grid
void outputGrid(char[][3], char[], string[][5]);
void outputAlignSymb(char[][3], char [][3]); //board, w


int main() {


    string displayBoard[5][5];
    string displayGrid[5][5];
    char board[3][3];
    char grid[9]; //added grid to more intuitively place the marked position & to transcribe 2d arrays to 1d
    int q[8][3]; //thought this should be declared here instead of inside a function. this contains the alignment combos for grid location
    char w[8][3]; //this contains the alignment combos for grid symbol 
    bool userTurn = true;
    int row;
    int col;
    int checkPos;
    char replay = 'y';

    //TESTING -- TO BE DELETED LATERRR  
    testAlignments(q);
    cout << endl;
    outputGrid(board, grid, displayGrid);
    outputAlignSymb(board, w);

    while (replay == 'y' || replay == 'Y') {
        initBoard(board);
        //        createGrid(board, grid); //create grid

        while (!gameOver(board)) {
            if (userTurn) {
                display(board, displayBoard);
                userInput(board, row, col);
                board[row][col] = 'X';
            } else {
                AIMove(board, row, col);
                printBuffer(100);
                cout << "(the AI moved to spot x, y)\n" << endl;
            }

            //TESTING PURPOSE -- TO BE DELETED LATER
            outputAlignSymb(board, w); //This shows the char w[8][3] alignments
            cout << winOrBlock(q, w, grid) << endl;

            userTurn = !userTurn;
        }

        cout << "would you like to play again? 'y' for yes, 'n' for no >>> ";
        cin >> replay;
    }

    return 0;
}
//Implement HINT:

//prompt the user to input a location on the board. Keep prompting if the position is not available.
//userInput(row, col);
//check: Available()
/*while (!posAvailable(board, row, col)) {
 //userInput(row, col);
 }*/

//place down the mark

//declaring function implementations

bool isEven(int checkPos) {
    if (checkPos % 2 == 0) {
        return true;
    }
    return false;
}

void initBoard(char board [][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

void initGrid(char grid[9]){
    
}

void display(char board[][3], string displayBoard[][5]) {
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
                displayBoard[i][j] = buffer.insert(1, 1, board[i / 2][j / 2]);
            }
        }
    }
    string sideBuffer[5][6];
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            if (j < 1) {
                sideBuffer[i][j] = "\t\t";
            } else {
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
}

void userInput(char board[][3], int& row, int& col) {
    cout << "\nEnter the location where you want to place your mark.\n" << endl;
    cout << "\t\tRow: ";
    cin >> row;
    cout << "\t\tCol: ";
    cin >> col;
    if (!posAvailable(board, row, col)) {
        cout << "that is not a valid location, try again." << endl;
        userInput(board, row, col);
    }
}

bool posAvailable(char board[][3], int& row, int& col) {
    if (board[row][col] != ' ') {
        return false;
    } else if (row < 0 || row > 2 || col < 0 || col > 2) {
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

//added q, w, grid

void AIMove(char board[][3], int& row, int& col) {

    //tells if a position is found
    bool found = false;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (posAvailable(board, i, j) && !found) {
                row = i;
                col = j;
                found = true;
            }
        }
    }
    placeOnBoard(board, row, col, 'O');

}

void placeOnBoard(char board[][3], int& row, int& col, char symbol) {
    board[row][col] = symbol;
}

bool gameOver(char board[][3]) {
    if (playerWin(board, 'X')) {
        cout << "X wins!";
        return true;
    } else if (playerWin(board, 'O')) {
        cout << "O wins!";
        return true;
    } else if (boardFull(board)) {
        cout << "tie!";
        return true;
    } else {
        return false;
    }
}

bool boardFull(char board[][3]) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}

bool playerWin(char board[][3], char symbol) {
    return false;
}

void printBuffer(int lines) {
    for (int i = 0; i < lines; i++) {
        cout << endl;
    }
}

void computerMove(char board[][3]) {
    cout << "boop";
    // create a 2d array of pointers

    // complete row
    // block
    // take middle
    // if player has two opposite corners, take a middle edge space
    // take corner
}

//void initQ(int q[][3]) {
//    for (int i = 0; i < 8; i++) {
//        for (int j = 0; j < 3; j++) {
//            q[i][j] = 0;
//        }
//    }
//}

void alignGridNum(int q[][3], int &c, int pos, int skip) {
    cout << endl;
    for (int i = 0; i < 3; i++) {
        q[c][i] = pos;
        pos += skip;
    }
    c++;
}

//this function creates the array alignment of the grid symbol

void alignGridSymb(char board[][3], char w[][3]) {
    //rows
    w[0][0] = board[0][0]; //0
    w[0][1] = board[0][1]; //1
    w[0][2] = board[0][2]; //2

    w[1][0] = board[1][0]; //3
    w[1][1] = board[1][1]; //4
    w[1][2] = board[1][2]; //5

    w[2][0] = board[2][0]; //6
    w[2][1] = board[2][1]; //7
    w[2][2] = board[2][2]; //8

    w[3][0] = board[0][2]; //2
    w[3][1] = board[1][1]; //4
    w[3][2] = board[2][0]; //6

    w[4][0] = board[0][0]; //0
    w[4][1] = board[1][0]; //3
    w[4][2] = board[2][0]; //6

    w[5][0] = board[0][1]; //1
    w[5][1] = board[1][1]; //4
    w[5][2] = board[2][1]; //7

    w[6][0] = board[0][2]; //2
    w[6][1] = board[1][2]; //5
    w[6][2] = board[2][2]; //8

    w[7][0] = board[0][0]; //0
    w[7][1] = board[1][1]; //4
    w[7][2] = board[2][2]; //8
}

//this function outputs the symbols in the alignment w[8][3]
void outputAlignSymb(char board[][3], char w[][3]) {
    alignGridSymb(board, w);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3; j++) {
            cout << w[i][j];
        }
        cout << endl;
    }
}
//this function creates the array alignment of the grid location

void createAlignments(int alignments[][3]) {
    int c = 0;

    for (int i = 1; i < 5; i++) {
        if (i % 2 == 0) {
            if (i == 2) {
                alignGridNum(alignments, c, 2, i);
            } else {
                alignGridNum(alignments, c, 0, i);
            }
        } else {
            for (int j = 0; j < 3; j++) {
                if (i == 1) {
                    alignGridNum(alignments, c, j * 3, i);
                } else {
                    alignGridNum(alignments, c, j, i);
                }
            }
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

void testAlignments(int q[][3]) {
    //    int q[8][3];
    //    initQ(q);
    createAlignments(q);
    print2dArray(q);
}

void createGrid(char board[][3], char grid[]) { //i think we can delete board[][]

//    grid[0] = board[0][0];
//    grid[1] = board[0][1];
//    grid[2] = board[0][2];
//    grid[3] = board[1][0];
//    grid[4] = board[1][1];
//    grid[5] = board[1][2];
//    grid[6] = board[2][0];
//    grid[7] = board[2][1];
//    grid[8] = board[2][2];
    grid[0] = 0;
    grid[1] = 1;
    grid[2] = 2;
    grid[3] = 3;
    grid[4] = 4;
    grid[5] = 5;
    grid[6] = 6;
    grid[7] = 7;
    grid[8] = 8;
}

void outputGrid(char board[][3], char grid[], string displayGrid[][5]) { //i think we can delete board[][] here
    
    createGrid(board, grid);
    
//    for (int i = 0; i < 9; i++) {
//        cout << "grid[" << i << "]: " << i << endl;
//    }
    int count = 0;
    for (int i = 0; i < 5; i ++) {
        for (int j = 0; j < 5; j++) {
            if (isEven(i) && !isEven(j)) {
                cout << "|";
            } else if (!isEven(i) && !isEven(j)) {
                cout << "+";
            } else if (!isEven(i) && isEven(j)) {
                cout << "---";
            } else {
                cout << " " << count << " ";
                count++;
            }
        }
        cout << endl;
    }
//    for (int i = 0; i < 5; i++) {
//        for (int j = 0; j < 5; j++) {
//            if (isEven(i) && !isEven(j)) {
//                displayGrid[i][j] = "|";
//            } else if (!isEven(i) && !isEven(j)) {
//                displayGrid[i][j] = "+";
//            } else if (!isEven(i) && isEven(j)) {
//                displayGrid[i][j] = "---";
//            } else {
//                string buffer = "  ";
//                cout << i << " | " << j << endl;
//                displayGrid[i][j] = buffer.insert(1, 1, board[i / 2][j / 2]);
//            }
//        }
//    }
//    string sideBuffer[5][6];
//    for (int i = 0; i < 5; i++) {
//        for (int j = 0; j < 6; j++) {
//            if (j < 1) {
//                sideBuffer[i][j] = "\t\t";
//            } else {
//                sideBuffer[i][j] = displayGrid[i][j - 1];
//            }
//        }
//    }
//    for (int i = 0; i < 5; i++) {
//        for (int j = 0; j < 6; j++) {
//            cout << sideBuffer[i][j];
//        }
//        cout << endl;
//    }
    
}

bool winOrBlock(int q[][3], char w[][3], char grid[]) {
    //loop through w and find if there is an alignment containing a blank space and two symbols of the same kind 
    for (int i = 0; i < 8; i++) {
        //initialize count for finding a particular symbol
        int countO = 0;
        int countX = 0;
        //loop through each set of alignment (3 symbols/blank per alignment)
        for (int j = 0; j < 3; j++) {
            if (w[i][j] == 'O') {
                countO++;
            } else if (w[i][j] == 'X') {
                countX++;
            }
            //if an alignment has two of the same with a blank and should be block or can win
            if ((countX == 2 && countO == 0) || (countO == 2 && countX == 0)) {
                for (int k = 0; k < 3; k++) {
                    if (w[i][k] == ' ') {

                        //////Right but how do i call this?
                        int placeOnGrid = q[i][k];
                        grid[placeOnGrid] = 'O';

                        return true;
                    }
                }
            }
        }
    }
    return false;
}
