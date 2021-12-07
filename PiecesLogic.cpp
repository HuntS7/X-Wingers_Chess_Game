#include "PiecesLogic.h"
#include "main.cpp"


Vector2f Pieces::CalcMouseLocation(Vector2f pos) {

    Vector2f coords;
    //calculate which tile the player is on 
    //Looping through the board array and checking if the mouse position is within the boundaries - SH
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int x = pos.x;
            int y = pos.y;
            Vector2f boardLocation = BoardLocationArray[i][j];
            int boardLocX = boardLocation.x + 100;
            int boardLocY = boardLocation.y + 100;

            if (x >= boardLocation.x && x <= boardLocX && y >= boardLocation.y && y <= boardLocY) {
                coords.x = i;
                coords.y = j;
                return coords;
            }
        }
    }
}

void Pieces::CalculatePawnMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck) {

    int oldX = oldBoardSquare.x;
    int oldY = oldBoardSquare.y;
    int newX = newBoardSquare.x;
    int newY = newBoardSquare.y;
    int calcYDiff = newY - oldY;
    int calcXDiff = newX - oldX;
    int enemyPiece = ChessPieceArray[newX][newY];


    if (chessPiece < 0) { //white move
        if (whiteMoveCount + 1 == 1 && calcYDiff != 0 && calcYDiff <= -2 && oldX == newX) { //if first move pawn can move 2 spaces
            UpdateWhiteChessPiece(chessPiece, oldX, oldY, newX, newY);

        }
        else if (enemyPiece == 0 && calcYDiff != 0 && calcYDiff == -1 && oldX == newX) { //after first move pawn can only move 1 tile up
            UpdateWhiteChessPiece(chessPiece, oldX, oldY, newX, newY);
        }
        else if (enemyPiece > 0 && calcYDiff == -1 && abs(calcXDiff) == 1) {//if enemy piece is in new location pawn can move diagonally
            if (!isKingCheck) {
                //valid - update chessPieceArray & Move image to new location
                UpdateWhiteChessPiece(chessPiece, oldX, oldY, newX, newY);
                RemoveEnemyChessPiece(enemyPiece, newX, newY);
            }
            else {
                isKingInCheck = true;
            }
        }
        else {
            //not valid - send error
            //only send error message when we aren't checking for king in check
            if (!isKingCheck) {

            }

        }
    }
    else if (chessPiece > 0) { //black move
        if (blackMoveCount + 1 == 1 && calcYDiff != 0 && calcYDiff <= 2 && oldX == newX) {
            UpdateBlackChessPiece(chessPiece, oldX, oldY, newX, newY);

        }
        else if (enemyPiece < 0 && calcYDiff != 0 && calcYDiff == 1 && oldX == newX) { //after first move pawn can only move 1 tile up
            UpdateBlackChessPiece(chessPiece, oldX, oldY, newX, newY);

        }
        else if (enemyPiece != 0 && calcYDiff == 1 && abs(calcXDiff) == 1) {//if enemy piece is in new location pawn can move diagonally
            if (!isKingCheck) {
                UpdateBlackChessPiece(chessPiece, oldX, oldY, newX, newY);
                RemoveEnemyChessPiece(enemyPiece, newX, newY);
            }
            else {
                isKingInCheck = true;
            }

        }
        else {
            //not valid - send error
            //only send error message if we aren't checking for king in check
            if (!isKingCheck) {

            }

        }
    }

    //TRANSFORM PAWN WHEN IT REACHES OTHER SIDE - SH
    if (newY == 0 || newY == 7) {
        //add window with the pawn options
        // once user clicks on option, update the ChessArray with new piece type in the new location
    }
}

void Pieces::CalculateRookMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck) {
    int oldX = oldBoardSquare.x;
    int oldY = oldBoardSquare.y;
    int newX = newBoardSquare.x;
    int newY = newBoardSquare.y;
    int calcYDiff = newY - oldY;
    int calcXDiff = newX - oldX;
    int enemyPiece = ChessPieceArray[newX][newY];
    bool isPathClear = true;
    int currentTile;

    //figure out if the path is clear from old location to new location
    if (calcYDiff == 0 && calcXDiff < 0) { //left across
        for (int i = oldX; i >= newX; i--) {
            currentTile = ChessPieceArray[oldY][i - 1];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }
    }
    else if (calcYDiff == 0 && calcXDiff > 0) { //right across

        for (int i = oldX; i <= newX; i++) {
            currentTile = ChessPieceArray[oldY][i + 1];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }

    }
    else if (calcYDiff > 0 && calcXDiff == 0) {//vertical down

        for (int i = oldY; i <= newY; i++) {
            currentTile = ChessPieceArray[i + 1][oldX];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }
    }
    else if (calcYDiff < 0 && calcXDiff == 0) {//vertical up
        for (int i = oldY; i >= newY; i--) {
            currentTile = ChessPieceArray[i - 1][oldX];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }
    }


    if (isPathClear) {
        if (calcYDiff == 0 && abs(calcXDiff) > 0 ||
            (abs(calcYDiff) > 0 && calcXDiff == 0)) {
            if (!isKingCheck) {
                if (chessPiece < 0) {
                    UpdateWhiteChessPiece(chessPiece, oldX, oldY, newX, newY);

                }
                else {
                    UpdateBlackChessPiece(chessPiece, oldX, oldY, newX, newY);
                }
                RemoveEnemyChessPiece(enemyPiece, newX, newY);
            }
            else {
                isKingInCheck = true;
            }
        }
        else {
            //invalid move send error
            if (!isKingCheck) {

            }
        }
    }
    else {
        //invalid move send error
        if (!isKingCheck) {

        }
    }

}

void Pieces::CalculateKnightMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck) {
    int oldX = oldBoardSquare.x;
    int oldY = oldBoardSquare.y;
    int newX = newBoardSquare.x;
    int newY = newBoardSquare.y;
    int calcYDiff = newY - oldY;
    int calcXDiff = newX - oldX;
    int enemyPiece = ChessPieceArray[newX][newY];
    bool isPathClear = CheckForTeamPiece(chessPiece, newX, newY);

    //L-shape move validation - SH
    if (isPathClear) {
        if ((calcYDiff == 2 && abs(calcXDiff) == 1) || //2 down & 1 across
            (calcYDiff == -2 && abs(calcXDiff) == 1) || // 2 up & 1 across
            (abs(calcYDiff) == 1 && calcXDiff == 2) || //2 right & 1 up/down
            (abs(calcYDiff) == 1 && calcXDiff == -2)) { //2 left & 1 up/down
            if (!isKingCheck) {
                if (chessPiece > 0) {
                    UpdateBlackChessPiece(chessPiece, oldX, oldY, newX, newY);
                }
                else {
                    UpdateWhiteChessPiece(chessPiece, oldX, oldY, newX, newY);
                }
                RemoveEnemyChessPiece(enemyPiece, newX, newY);
            }
            else {
                isKingInCheck = true;
            }
        }
        else {
            //invalid move - send error
            //only send error message if we aren't checking for king in check
            if (!isKingCheck) {

            }
        }
    }
    else {
        //your team's piece is already in that location - send error SH
        //only send error message if we aren't checking for king in check
        if (!isKingCheck) {

        }
    }

}

void Pieces::CalculateBishopMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck) {
    int oldX = oldBoardSquare.x;
    int oldY = oldBoardSquare.y;
    int newX = newBoardSquare.x;
    int newY = newBoardSquare.y;
    int calcYDiff = newY - oldY;
    int calcXDiff = newX - oldX;
    int enemyPiece = ChessPieceArray[newX][newY];
    bool isPathClear = true;
    int currentTile;

    //figure out if the path is clear from old location to new location
    if (calcYDiff > 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) { //Down Right Diag
        for (int i = oldX + 1; i <= newX; i++) {
            currentTile = ChessPieceArray[oldY + 1][oldX];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }
    }
    else if (calcYDiff > 0 && calcXDiff < 0 && abs(calcYDiff) == abs(calcXDiff)) { //Down Left Diag
        for (int i = oldX - 1; i >= newX; i--) {
            currentTile = ChessPieceArray[oldY + 1][i];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }

    }
    else if (calcYDiff < 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) {//Up left diag
        for (int i = oldY - 1; i >= newY; i--) {
            currentTile = ChessPieceArray[oldY - 1][i];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }
    }
    else if (calcYDiff < 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) {//Up Right Diag
        for (int i = oldY; i >= newY; i--) {
            currentTile = ChessPieceArray[oldY - 1][i];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }
    }

    //L-shape move validation - SH
    if (isPathClear) {
        if ((calcYDiff == 2 && abs(calcXDiff) == 1) || //2 down & 1 across
            (calcYDiff == -2 && abs(calcXDiff) == 1) || // 2 up & 1 across
            (abs(calcYDiff) == 1 && calcXDiff == 2) || //2 right & 1 up/down
            (abs(calcYDiff) == 1 && calcXDiff == -2)) { //2 left & 1 up/down
            if (!isKingCheck) {
                if (chessPiece > 0) {
                    UpdateBlackChessPiece(chessPiece, oldX, oldY, newX, newY);
                }
                else {
                    UpdateWhiteChessPiece(chessPiece, oldX, oldY, newX, newY);
                }
                RemoveEnemyChessPiece(enemyPiece, newX, newY);
            }
            else {
                isKingInCheck = true;
            }
        }
        else {
            //invalid move - send error
            //send error if we aren't checking for king in check
            if (!isKingCheck) {}
        }
    }
    else {
        //your team's piece is already in that location - send error SH
         //send error if we aren't checking for king in check
        if (!isKingCheck) {}
    }

}

void Pieces::CalculateQueenMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck) {
    int oldX = oldBoardSquare.x;
    int oldY = oldBoardSquare.y;
    int newX = newBoardSquare.x;
    int newY = newBoardSquare.y;
    int calcYDiff = newY - oldY;
    int calcXDiff = newX - oldX;
    int enemyPiece = ChessPieceArray[newX][newY];
    bool isPathClear = true;
    int currentTile;

    // Figure out it path is clear  from old position to new position
    if (calcYDiff > 0 && calcXDiff == 0) {//vertical down

        for (int i = oldY; i <= newY; i++) {
            currentTile = ChessPieceArray[i + 1][oldX];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }
    }
    else if (calcYDiff < 0 && calcXDiff == 0) {//vertical up
        for (int i = oldY; i >= newY; i--) {
            currentTile = ChessPieceArray[i - 1][oldX];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }
    }
    else if (calcYDiff == 0 && calcXDiff < 0) { //left across
        for (int i = oldX; i >= newX; i--) {
            currentTile = ChessPieceArray[oldY][i - 1];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }
    }
    else if (calcYDiff == 0 && calcXDiff > 0) { //right across

        for (int i = oldX; i <= newX; i++) {
            currentTile = ChessPieceArray[oldY][i + 1];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }

    }
    else if (calcYDiff > 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) { //Down Right Diag
        for (int i = oldX + 1; i <= newX; i++) {
            currentTile = ChessPieceArray[oldY + 1][oldX];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }
    }
    else if (calcYDiff > 0 && calcXDiff < 0 && abs(calcYDiff) == abs(calcXDiff)) { //Down Left Diag
        for (int i = oldX - 1; i >= newX; i--) {
            currentTile = ChessPieceArray[oldY + 1][i];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }

    }
    else if (calcYDiff < 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) {//Up left diag
        for (int i = oldY - 1; i >= newY; i--) {
            currentTile = ChessPieceArray[oldY - 1][i];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }
    }
    else if (calcYDiff < 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) {//Up Right Diag
        for (int i = oldY; i >= newY; i--) {
            currentTile = ChessPieceArray[oldY - 1][i];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }
    }


    if (isPathClear) {

        if ((abs(calcXDiff) > 0 && calcYDiff == 0) || //horizontal movement
            (abs(calcYDiff) > 0 && calcXDiff == 0) || // vertical movement
            (abs(calcXDiff) > 0 && abs(calcYDiff) > 0 && abs(calcXDiff) == abs(calcYDiff))) { // diagonal movement
            if (!isKingCheck) {


                if (chessPiece > 0) {
                    UpdateBlackChessPiece(chessPiece, oldX, oldY, newX, newY);
                }
                else {
                    UpdateWhiteChessPiece(chessPiece, oldX, oldY, newX, newY);
                }
                RemoveEnemyChessPiece(enemyPiece, newX, newY);
            }
            else {
                isKingInCheck = true;
            }
        }
        else {
            //send error
            if (!isKingCheck) {}
        }
    }
    else {
        //send error
        if (!isKingCheck) {}
    }
}

void Pieces::CalculateKingMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck) {
    int oldX = oldBoardSquare.x;
    int oldY = oldBoardSquare.y;
    int newX = newBoardSquare.x;
    int newY = newBoardSquare.y;
    int calcYDiff = newY - oldY;
    int calcXDiff = newX - oldX;
    int enemyPiece = ChessPieceArray[newX][newY];
    bool isPathClear = true;
    int currentTile;

    if ((abs(calcXDiff) == 1 && calcYDiff == 0) ||
        (abs(calcYDiff) == 1 && calcXDiff == 0) ||
        (abs(calcXDiff) == 1 && abs(calcYDiff) == 1)) {
        if (!isKingCheck) {
            if (chessPiece > 0) {
                UpdateBlackChessPiece(chessPiece, oldX, oldY, newX, newY);
            }
            else {
                UpdateWhiteChessPiece(chessPiece, oldX, oldY, newX, newY);
            }
            RemoveEnemyChessPiece(enemyPiece, newX, newY);
        }
        else {
            isKingInCheck = true;
        }
    }
    else {
        //send error
        if (!isKingCheck) {}
        else {
            //THEY LOSE - SEND OUT MESSAGE TO END GAME - SH

        }
    }


}

void Pieces::IsKingInCheck() {
    int currentTile;
    Vector2f currentBoardTile, whiteKingBoardTile, blackKingBoardTile;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (ChessPieceArray[i][j] == 4) {
                blackKingBoardTile.x = i;
                blackKingBoardTile.y = j;
            }
            else if (ChessPieceArray[i][j] == -4) {
                whiteKingBoardTile.x = i;
                whiteKingBoardTile.y = j;
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            currentTile = ChessPieceArray[i][j];

            //can the piece land on the king?
            //is it a valid move? (ie is path clear etc)
            switch (currentTile) {
            case 1:
                CalculateRookMove(currentBoardSquare, whiteKingBoardTile, currentTile, true);
                CalculateRookMove(currentBoardSquare, blackKingBoardTile, currentTile, true);
                break;
            case 2:
                CalculateKnightMove(currentBoardSquare, whiteKingBoardTile, currentTile, true);
                CalculateKnightMove(currentBoardSquare, blackKingBoardTile, currentTile, true);
                break;
            case 3:
                CalculateBishopMove(currentBoardSquare, whiteKingBoardTile, currentTile, true);
                CalculateBishopMove(currentBoardSquare, blackKingBoardTile, currentTile, true);
                break;
            case 4:
                CalculateKingMove(currentBoardSquare, whiteKingBoardTile, currentTile, true);
                CalculateKingMove(currentBoardSquare, blackKingBoardTile, currentTile, true);
                break;
            case 5:
                CalculateQueenMove(currentBoardSquare, whiteKingBoardTile, currentTile, true);
                CalculateQueenMove(currentBoardSquare, blackKingBoardTile, currentTile, true);
                break;
            case 6:
                CalculatePawnMove(currentBoardSquare, whiteKingBoardTile, currentTile, true);
                CalculatePawnMove(currentBoardSquare, blackKingBoardTile, currentTile, true);
                break;
            }

            if (isKingInCheck) {
                break;

            }
        }
    }

    if (isKingInCheck) {
        //send error - only let user move king
    }
}
void Pieces::UpdateWhiteChessPiece(int chessPiece, int oldX, int oldY, int newX, int newY) {
    //valid - update chessPieceArray & Move image to new location
    ChessPieceArray[oldY][oldX] = 0;
    ChessPieceArray[newX][newY] = chessPiece;


    whitePieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * newY, tileWidth * newX), Color::White, chessPiece));
    whitePieces[newX + newY].getShape().setTexture(&whitePieceTextures[chessPiece]);

    whiteMoveCount++;
}

void Pieces::UpdateBlackChessPiece(int chessPiece, int oldX, int oldY, int newX, int newY) {
    //valid - update chessPieceArray & Move image to new location
    ChessPieceArray[oldY][oldX] = 0;
    ChessPieceArray[newX][newY] = chessPiece;


    blackPieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * newY, tileWidth * newX), Color::White, chessPiece));
    blackPieces[newX + newY].getShape().setTexture(&blackPieceTextures[abs(chessPiece)]);

    blackMoveCount++;
}

void Pieces::RemoveEnemyChessPiece(int enemyPiece, int enemyLocX, int enemyLocY) {
    if (enemyPiece != 0) {
        //remove enemyPiece from Sprites
    }
}

bool Pieces::CheckForTeamPiece(int chessPiece, int newLocX, int newLocY) {
    bool isValid = true;
    if ((chessPiece < 0 && ChessPieceArray[newLocX][newLocY] < 0) ||
        (chessPiece > 0 && ChessPieceArray[newLocX][newLocY] > 0)) { //white team
        isValid = false;
    }

    return isValid;
}

void Pieces::CalculateMove(Vector2i pos[]) {

    //Calculate which tile user clicks on
    Vector2f oldBoardSquare = CalcMouseLocation(Vector2f(pos[0]));
    Vector2f newBoardSquare = CalcMouseLocation(Vector2f(pos[1]));

    int j = oldBoardSquare.x;
    int i = oldBoardSquare.y;

    //Check if there's a chess piece on the old location
    if (ChessPieceArray[i][j] != 0) {
        //There's a piece in the old square! Figure out if the new location is valid
        if (ChessPieceArray[i][j] == -1 || ChessPieceArray[i][j] == 1) //rook
        {
            CalculateRookMove(oldBoardSquare, newBoardSquare, ChessPieceArray[i][j], false);
        }
        else if (ChessPieceArray[i][j] == -2 || ChessPieceArray[i][j] == 2) //knight
        {
            CalculateKnightMove(oldBoardSquare, newBoardSquare, ChessPieceArray[i][j], false);
        }
        else if (ChessPieceArray[i][j] == -3 || ChessPieceArray[i][j] == 3) //bishop
        {
            CalculateBishopMove(oldBoardSquare, newBoardSquare, ChessPieceArray[i][j], false);
        }
        else if (ChessPieceArray[i][j] == -4 || ChessPieceArray[i][j] == 4) //king
        {
            CalculateKingMove(oldBoardSquare, newBoardSquare, ChessPieceArray[i][j], false);
        }
        else if (ChessPieceArray[i][j] == -5 || ChessPieceArray[i][j] == 5) //queen
        {
            CalculateQueenMove(oldBoardSquare, newBoardSquare, ChessPieceArray[i][j], false);
        }
        else if (ChessPieceArray[i][j] == -6 || ChessPieceArray[i][j] == 6) //pawn
        {
            CalculatePawnMove(oldBoardSquare, newBoardSquare, ChessPieceArray[i][j], false);
        }

        IsKingInCheck();
    }
    else {
        //send user error message
    }

    mouseCount = 0;

}