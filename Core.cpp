#include "Core.h"

int Core::char_to_game_int(char value) {
    if (value == 'X')
        return 1;
    if (value == 'O')
        return 2;
    return 0;
}

Core::BOARD_STATUS Core::check_board(std::vector<std::vector<char>>& board, size_t size) {
    BOARD_STATUS res = check_horizontally(board, size);
    if (res.isWin)
        return res;
    res = check_vertically(board, size);
    if (res.isWin)
        return res;
    res = check_cross(board, size);
    if (res.isWin)
        return res;
    return {false, ' '};
}

Core::BOARD_STATUS Core::check_horizontally(std::vector<std::vector<char>>& board, size_t size) {
    for (size_t i = 0; i < size; i++) {
        char first = board[i][0];
        if (first == ' ')
            continue;
        bool win = true;
        for (size_t j = 1; j < size; j++) {
            if (board[i][j] != first) {
                win = false;
                break;
            }
        }
        if (win)
            return {true, first};
    }
    return {false, ' '};
}

Core::BOARD_STATUS Core::check_vertically(std::vector<std::vector<char>>& board, size_t size) {
    for (size_t j = 0; j < size; j++) {
        char first = board[0][j];
        if (first == ' ')
            continue;
        bool win = true;
        for (size_t i = 1; i < size; i++) {
            if (board[i][j] != first) {
                win = false;
                break;
            }
        }
        if (win)
            return {true, first};
    }
    return {false, ' '};
}

Core::BOARD_STATUS Core::check_cross(std::vector<std::vector<char>>& board, size_t size) {
    // Top-left to bottom-right
    char first = board[0][0];
    if (first != ' ') {
        bool win = true;
        for (size_t i = 1; i < size; i++)
            if (board[i][i] != first)
                win = false;
        if (win)
            return {true, first};
    }
    // Top-right to bottom-left
    first = board[0][size - 1];
    if (first != ' ') {
        bool win = true;
        for (size_t i = 1; i < size; i++)
            if (board[i][size - 1 - i] != first)
                win = false;
        if (win)
            return {true, first};
    }
    return {false, ' '};
}
