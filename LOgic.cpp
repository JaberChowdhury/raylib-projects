#include <iostream>
#include <vector>
using namespace std;

struct BOARD_STATUS {
    bool isWin;
    char winner;
};

int char_to_game_int(char value) {
    // here we take 'X' value as 1 and 'O' value as 2

    if (value == 'X') {
        return (value - '0') - 39;
    } else if (value == 'O') {
        return (value - '0') - 29;
    }

    return 0;
}

BOARD_STATUS check_horizontally(vector<vector<char>>& game_board, size_t size) {
    BOARD_STATUS result;
    result.winner = 'p';
    result.isWin  = false;
    // ======

    for (int i = 0; i < size; i++) {
        char winner = '0';

        int sum = 0;
        for (int j = 1; j < size; j++) {
            winner = game_board[i][j];
            if ((game_board[i][j] != game_board[i][j - 1])) {
                break;
            } else {
                sum += char_to_game_int(game_board[i][j - 1]);
            }
        }

        sum += char_to_game_int(game_board[i][size - 1]);
        if ((sum == size) || (sum == size * 2)) {
            result.winner = winner;
            result.isWin  = true;
            return result;
        }
    }

    return result;
}
BOARD_STATUS check_vertically(vector<vector<char>>& game_board, size_t size) {
    BOARD_STATUS result;
    result.winner = 'p';
    result.isWin  = false;
    // ||
    // ||
    // ||
    // ||

    for (int j = 0; j < size; j++) {
        int  sum    = 0;
        char winner = '0';
        for (int i = 1; i < size; i++) {
            winner = game_board[i][j];
            if (game_board[i][j] != game_board[i - 1][j]) {
                break;
            } else {
                sum += char_to_game_int(game_board[i][j]);
            }
        }
        sum += char_to_game_int(game_board[size - 1][j]);
        if ((sum == size) || (sum == size * 2)) {
            result.winner = winner;
            result.isWin  = true;
            return result;
        }
    }

    return result;
}
BOARD_STATUS check_cross(vector<vector<char>>& game_board, size_t size) {
    BOARD_STATUS result;
    result.winner = 'p';
    result.isWin  = false;

    // travarse j+n
    for (int i = 0; i < 2; i++) {
        int  sum    = 0;
        char winner = '0';
        if (i == 0) {
            for (int j = 1; j < size; j++) {
                winner = game_board[j][j];
                if (game_board[j][j] != game_board[j - 1][j - 1]) {
                    break;
                } else {
                    sum += char_to_game_int(game_board[j][j]);
                }
            }
            sum += char_to_game_int(game_board[size - 1][size - 1]);
        } else {
            for (int j = 1; j < size; j++) {
                winner = game_board[j][(size - 1) - j];
                if (game_board[j][(size - 1) - j] != game_board[j - 1][(size - 1) - (j - 1)]) {
                    break;
                } else {
                    sum += char_to_game_int(game_board[j - 1][(size - 1) - (j - 1)]);
                }
            }

            sum += char_to_game_int(game_board[size - 1][0]);
        }
        if ((sum == size) || (sum == size * 2)) {
            result.winner = winner;
            result.isWin  = true;
            return result;
        }
    }
    return result;
}

BOARD_STATUS check_board(vector<vector<char>>& game_board, size_t size) {
    BOARD_STATUS result = check_horizontally(game_board, size);
    if (result.isWin)
        return result;

    result = check_vertically(game_board, size);
    if (result.isWin)
        return result;

    result = check_cross(game_board, size);
    if (result.isWin)
        return result;

    return {false, '0'};
}
int main() {

    size_t               size = 5;
    vector<vector<char>> game_board(size, vector<char>(size));

    game_board = {
        // {'X', 'X', 'X', 'X', 'X'}, //
        {'0', '0', '0', '0', 'X'}, //
        {'0', 'X', '0', 'O', 'X'}, //
        {'0', '0', 'O', '0', 'X'}, //
        {'0', 'O', '0', 'X', 'X'}, //
        {'O', '0', '0', '0', 'X'}, //
    };

    BOARD_STATUS result = check_board(game_board, size);
    if (result.isWin) {
        cout << "board checked = " << result.winner << endl;
    } else {
        cout << "nope" << endl;
    }

    return 0;
}
