/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 */

#include <iostream>
#include <string>
using namespace std;

//declaring function prototypes
bool noWin();
bool boardNotFull();
bool isEven(int);
void display(string[][3], string[][5]);

int main() {

    string board[3][3];
    string displayBoard[5][5];
    //    char user = 'X';
    //    char AI = 'O';
    bool usersTurn = true; //place 'O'; if false, place 'X'
    //    bool noWin = true;
    int row;
    int col;
    int checkPos;

    //        for (int i = 0; i < 5; i++) {
    //            for(int j = 0; j < 5; j++) {
    //                displayBoard[i][j]='3';
    //                cout << displayBoard[i][j];
    //            }
    //            cout << endl;
    //        }

    //    for (int i = 0; i < 5; i++) {
    //        for (int j = 0; j < 5; j++) {
    //            displayBoard[i][j] = '4';
    //            cout << displayBoard[i][j];
    //        }
    //        cout << endl;
    //    }
    //    for (int i = 0; i < 3; i++) {
    //        for (int j = 0; j < 3; j++) {
    //            board[i][j] = " ";
    //        }
    //    }


    //    for (int i = 0; i < 5; i++) {
    //        for (int j = 0; j < 5; j++) {
    //            if (isEven(i) && !isEven(j)) {
    //                displayBoard[i][j] = "|";
    //            } else if (!isEven(i) && !isEven(j)) {
    //                displayBoard[i][j] = "+";
    //            } else if (!isEven(i) && isEven(j)) {
    //                displayBoard[i][j] = "---";
    //            } else {
    //                displayBoard[i][j] = " " + board[i / 2][j / 2] + " ";
    //            }
    //        }
    //    }

    //    for (int i = 0; i < 5; i++) {
    //        for (int j = 0; j < 5; j++) {
    //            cout << displayBoard[i][j];
    //        }
    //        cout << endl;
    //    }

    display(board, displayBoard);
    while (noWin() && boardNotFull()) {
        cout << "Enter the row and col position of where you want to place your mark:";
        //

        cin >> row >> col;
        if (usersTurn) {
            board[row][col] = 'X';
            cout << board[row][col];
        } else {

        }
        usersTurn = !usersTurn;
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

bool noWin() {
    //if(){return true}
    cout << "Stubb";
    return false;
}
//declaring function implementations

bool boardNotFull() {
    //if(){return true}
    cout << "Stubbber";
    return false;
}

void display(string board[][3], string displayBoard[][5]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = "O";
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (isEven(i) && !isEven(j)) {
                displayBoard[i][j] = "|";
            } else if (!isEven(i) && !isEven(j)) {
                displayBoard[i][j] = "+";
            } else if (!isEven(i) && isEven(j)) {
                displayBoard[i][j] = "---";
            } else {
                displayBoard[i][j] = " " + board[i / 2][j / 2] + " ";
            }
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            cout << displayBoard[i][j];
        }
        cout << endl;
    }
}