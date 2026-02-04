#pragma once
#include <vector>

class Core {
  public:
    struct BOARD_STATUS {
        bool isWin;
        char winner;
    };

    int          char_to_game_int(char value);
    BOARD_STATUS check_board(std::vector<std::vector<char>>& board, size_t size);

  private:
    BOARD_STATUS check_horizontally(std::vector<std::vector<char>>& board, size_t size);
    BOARD_STATUS check_vertically(std::vector<std::vector<char>>& board, size_t size);
    BOARD_STATUS check_cross(std::vector<std::vector<char>>& board, size_t size);
};
