
 /**
  * MIN-MAX AI by Krzysztof Szewczyk (a.k.a. Palaiologos)
  * Copyright (C) 2018 by Krzysztof Szewczyk.
  * 
  * @TODO for @Condzi: Integrate this AI to game.
  */

#pragma once

/**
 * fromiD - return character based on player numeric ID.
 * This function is part of private API.
 */

fromid(i) int i; {
    switch(i) {
        case -1:
            return 'X';
        case 0:
            return ' ';
        case 1:
            return 'O';
    }
}

/**
 * isover - check is game finished by victory of player or draw.
 * This function is part of private API.
 */

isover(board) const int board[9]; {
    unsigned wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    int i;
    
    for(i = 0; i < 8; ++i)
        if(board[wins[i][0]] != 0 && board[wins[i][0]] == board[wins[i][1]] && board[wins[i][0]] == board[wins[i][2]])
            return board[wins[i][2]];
    return 0;
}

/**
 * minmax - this function returns score of board for specified player.
 * This function is part of private API.
 */

minmax(int board[9], int player) {
    int winner, score = -2, i, move;
    winner = isover(board);
    if(winner != 0) return winner*player;
    move = -1;
    
    
    for(i = 0; i < 9; ++i) {
        if(board[i] == 0) {
            board[i] = player;
            int thisScore = -minmax(board, player*-1);
            if(thisScore > score) {
                score = thisScore;
                move = i;
            }
            board[i] = 0;
        }
    }
    
    return move == -1 ? 0 : score;
}

/**
 * aimove - modify passed array to contain move of AI.
 * Warning: It's propably impossible to win against this AI. It's moving as O player.
 * This function is part of public API.
 */

aimove(board) int board[9]; {
    int move = -1, score = -2, i, tempScore;
    
    for(i = 0; i < 9; ++i) {
        if(board[i] == 0) {
            board[i] = 1;
            tempScore = -minmax(board, -1);
            board[i] = 0;
            if(tempScore > score) {
                score = tempScore;
                move = i;
            }
        }
    }
    
    board[move] = 1;
    return 0;
}
