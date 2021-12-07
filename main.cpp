//include classes and headers
#include "common.hpp"
#include "PiecesLogic.h"

using namespace::std;

class BoardPiece {
private:
    Texture pieceTexture;

    RectangleShape piece;

    int pieceType; // 1 = rook, 2 = knight, 3 = bishop, 4 = king, 5 = queen, 6 = pawn, 7 = light tile, 8 = dark tile.
 


public:
    BoardPiece(Vector2f, Vector2f, Color, int);
    

    RectangleShape& getShape();
    int getPieceType();

    void setPieceType(int);
    void setBoardLocationArray();
    void CalcMouseLocation(Vector2i pos);
   
};

BoardPiece::BoardPiece(Vector2f size, Vector2f position, Color color, int pType) {
    piece.setSize(size);
    piece.setPosition(position);
    piece.setFillColor(color);

    pieceType = pType;
}



RectangleShape& BoardPiece::getShape() {
    return piece;
}

class Game {
private:
    // Init general variables.
    int tileWidth;

    // Init white piece variables.
    vector<int> chessPieceOrder;
    vector<std::string> whitePieceTextureNames;
    vector<Texture> whitePieceTextures;
    vector<BoardPiece> whitePieces;

    // Init black piece variables.
    std::vector<std::string> blackPieceTextureNames;
    std::vector<Texture> blackPieceTextures;
    std::vector<BoardPiece> blackPieces;

    // Init board piece variables.
    std::vector<std::string> boardTileTextureNames;
    std::vector<Texture> boardTileTextures;
    std::vector<BoardPiece> boardTiles;

    // Init window.
    RenderWindow window;

public:

    Game();

    void initWhitePieces();
    void initBlackPieces();
    void initBoardPieces();
    void SetBoardLocationArray();
    void CalculateMove(Vector2i pos[]);
    void CalculatePawnMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck);
    void CalculateRookMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck);
    void CalculateKnightMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck);
    void CalculateBishopMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck);
    void CalculateQueenMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck);
    void CalculateKingMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck);
    void UpdateWhiteChessPiece(int chessPiece, int oldX, int oldY, int newX, int newY);
    void UpdateBlackChessPiece(int chessPiece, int oldX, int oldY, int newX, int newY);
    void RemoveEnemyChessPiece(int enemyPiece, int enemyLocX, int enemyLocY);
    bool CheckForTeamPiece(int chessPiece, int newLocX, int newLocY);
    void IsKingInCheck();

    Vector2f CalcMouseLocation(Vector2f pos);
    Vector2i pos[2];
    Vector2f currentBoardSquare;
    int mouseCount = 0;
    int whiteMoveCount = 0;
    int blackMoveCount = 0;
    bool isKingInCheck = false;

    void loop();

    //void movePiece(int, int, int); // X, Y, piece to move.
    Vector2f BoardLocationArray[8][8];
    Vector2f coords;
    int ChessPieceArray[8][8] =
    { 1, 2, 3, 4, 5, 3, 2, 1,
      6, 6, 6, 6, 6, 6, 6, 6,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
     -6,-6,-6,-6,-6,-6,-6,-6,
     -1,-2,-3,-4,-5,-3,-2,-1, };
};

Game::Game() {
    // Init general variables.
    tileWidth = 100;

    // Init white piece variables.
    chessPieceOrder = {1, 2, 3, 4, 5, 3, 2, 1, 6};
    whitePieceTextureNames = {
        "w_rook_png_128px.png", "w_knight_png_128px.png",
        "w_bishop_png_128px.png", "w_king_png_128px.png",
        "w_queen_png_128px.png", "w_bishop_png_128px.png",
        "w_knight_png_128px.png", "w_rook_png_128px.png",
        "w_pawn_png_128px.png"
    };
    whitePieceTextures.resize(9);

    // Init black piece variables.
    blackPieceTextureNames = {
        "b_rook_png_128px.png", "b_knight_png_128px.png",
        "b_bishop_png_128px.png", "b_king_png_128px.png",
        "b_queen_png_128px.png", "b_bishop_png_128px.png",
        "b_knight_png_128px.png", "b_rook_png_128px.png",
        "b_pawn_png_128px.png"
    };
    blackPieceTextures.resize(9);

    // Init board piece variables.
    boardTileTextureNames = {
        "square_brown_light_png_128px.png",
        "square_brown_dark_png_128px.png"
    };
    boardTileTextures.resize(2);

    // Init window.
    window.create(VideoMode(800 , 800), "SFML works!");
}

void Game::initWhitePieces() {
    // Create white piece textures.
    for(int i = 0; i < whitePieceTextureNames.size(); i++) {
        if(!whitePieceTextures[i].loadFromFile("bin/images/" + whitePieceTextureNames[i])) {
            std::cout << "Error getting " << whitePieceTextureNames[i] << ". Shutting down." << std::endl;
            //return 1;
        }
    }

    for (int i = 0; i < blackPieceTextureNames.size(); i++) {
        if (!blackPieceTextures[i].loadFromFile("bin/images/" + blackPieceTextureNames[i])) {
            std::cout << "Error getting " << blackPieceTextureNames[i] << ". Shutting down." << std::endl;
            //return 1;
        }
    }

    // Create white Pieces.
    for(int i = 0; i < chessPieceOrder.size(); i++) {
        if(chessPieceOrder[i] == 6) {
            // Create white pawns.
            for(int j = 0; j < 8; j++) {
                whitePieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * j, tileWidth *6), Color::White, chessPieceOrder[i]));
                whitePieces[i + j].getShape().setTexture(&whitePieceTextures[i]);
            }
        } else {
            // Create all other white pieces.
            whitePieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * i, tileWidth * 7), Color::White, chessPieceOrder[i]));
            whitePieces[i].getShape().setTexture(&whitePieceTextures[i]);
        }
    }
}

void Game::initBlackPieces() {
    // Create black piece textures.
    for(int i = 0; i < blackPieceTextureNames.size(); i++) {
        if(!blackPieceTextures[i].loadFromFile("bin/images/" + blackPieceTextureNames[i])) {
            std::cout << "Error getting " << blackPieceTextureNames[i] << ". Shutting down." << std::endl;
            //return 1;
        }
    }

    // Create black Pieces.
    for(int i = 0; i < chessPieceOrder.size(); i++) {
        if(chessPieceOrder[i] == 6) {
            // Create white pawns.
            for(int j = 0; j < 8; j++) {
                blackPieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * j, tileWidth), Color::White, chessPieceOrder[i]));
                blackPieces[i + j].getShape().setTexture(&blackPieceTextures[i]);
            }
        } else {
            // Create all other white pieces.
            blackPieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * i, 0), Color::White, chessPieceOrder[i]));
            blackPieces[i].getShape().setTexture(&blackPieceTextures[i]);
        }
    }
}

void Game::initBoardPieces() {
    int startingColor = 0; // 0 = light tile, 1 = dark tile
    int currentColor = startingColor;

    // Create board piece textures.
    for(int i = 0; i < boardTileTextureNames.size(); i++) {
        if(!boardTileTextures[i].loadFromFile("bin/images/" + boardTileTextureNames[i])) {
            std::cout << "Error getting " << boardTileTextureNames[i] << ". Shutting down." << std::endl;
            //return 1;
        }
    }

    // Create board pieces.
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(currentColor == 0) {
                boardTiles.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), BoardLocationArray[i][j], Color::White, 6));
                boardTiles[(i * 8) + j].getShape().setTexture(&boardTileTextures[0]);
                currentColor++;
            } else {
                boardTiles.push_back(BoardPiece(Vector2f(tileWidth, tileWidth),  BoardLocationArray[i][j], Color::White, 7));
                boardTiles[(i * 8) + j].getShape().setTexture(&boardTileTextures[1]);
                currentColor--;
            }
        }

        if(startingColor == 0) {
            startingColor++;
            currentColor = startingColor;
        } else {
            startingColor--;
            currentColor = startingColor;
        }
    }
}

void Game::SetBoardLocationArray() {

    //calculating the positions of each square in the board by multiplying by the width of each tile- SH
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            coords.y = j * tileWidth;
            coords.x = i * tileWidth;
            BoardLocationArray[i][j] = coords;
        }
    }

}

Vector2f Game::CalcMouseLocation(Vector2f pos) {

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

void Game::CalculatePawnMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck) {

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
    else if (chessPiece > 0){ //black move
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

void Game::CalculateRookMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck) {
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
            currentTile = ChessPieceArray[oldY][i -1];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }
    }
    else if (calcYDiff == 0 && calcXDiff > 0) { //right across
        
        for (int i = oldX; i <= newX; i++) {
            currentTile = ChessPieceArray[oldY][i+1];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }

    }
    else if (calcYDiff > 0 && calcXDiff == 0) {//vertical down
        
        for (int i = oldY; i <= newY; i++) {
            currentTile = ChessPieceArray[i +1][oldX];
            if (currentTile != 0) {
                isPathClear = false;
                break;
            }
        }
    }
    else if(calcYDiff < 0 && calcXDiff == 0) {//vertical up
        for (int i = oldY; i >= newY; i--) {
            currentTile = ChessPieceArray[i -1][oldX];
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

void Game::CalculateKnightMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck) {
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
            (abs(calcYDiff) == 1 && calcXDiff == -2)){ //2 left & 1 up/down
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

void Game::CalculateBishopMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck) {
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
        for (int i = oldY -1; i >= newY; i--) {
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

void Game::CalculateQueenMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck) {
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

void Game::CalculateKingMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece, bool isKingCheck) {
    int oldX = oldBoardSquare.x;
    int oldY = oldBoardSquare.y;
    int newX = newBoardSquare.x;
    int newY = newBoardSquare.y;
    int calcYDiff = newY - oldY;
    int calcXDiff = newX - oldX;
    int enemyPiece = ChessPieceArray[newX][newY];
    bool isPathClear = true;
    int currentTile;

    if((abs(calcXDiff) == 1 && calcYDiff ==0) ||
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

void Game::IsKingInCheck() {
    int currentTile;
    Vector2f currentBoardTile, whiteKingBoardTile, blackKingBoardTile;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (ChessPieceArray[i][j] == 4) {
                blackKingBoardTile.x = i;
                blackKingBoardTile.y = j;
            }
            else if(ChessPieceArray[i][j] == -4) {
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

void Game::UpdateWhiteChessPiece(int chessPiece, int oldX, int oldY, int newX, int newY) {
    //valid - update chessPieceArray & Move image to new location
    ChessPieceArray[oldY][oldX] = 0;
    ChessPieceArray[newX][newY] = chessPiece;

    
    //whitePieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * newY, tileWidth * newX), Color::White, chessPiece));
    //whitePieces[newX + newY].getShape().setTexture(&whitePieceTextures[chessPiece]);

    whiteMoveCount++;
}

void Game::UpdateBlackChessPiece(int chessPiece, int oldX, int oldY, int newX, int newY) {
    //valid - update chessPieceArray & Move image to new location
    ChessPieceArray[oldY][oldX] = 0;
    ChessPieceArray[newX][newY] = chessPiece;


    //blackPieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * newY, tileWidth * newX), Color::White, chessPiece));
    //blackPieces[newX + newY].getShape().setTexture(&blackPieceTextures[abs(chessPiece)]);

    blackMoveCount++;
}

void Game::RemoveEnemyChessPiece(int enemyPiece, int enemyLocX, int enemyLocY) {
    if (enemyPiece != 0) {
        //remove enemyPiece from Sprites
    }
}

bool Game::CheckForTeamPiece(int chessPiece, int newLocX, int newLocY) {
    bool isValid = true;
    if ((chessPiece < 0 && ChessPieceArray[newLocX][newLocY] < 0) ||
        (chessPiece > 0 && ChessPieceArray[newLocX][newLocY] > 0)) { //white team
        isValid = false;
    }
    
    return isValid;
}

void Game::CalculateMove(Vector2i pos[]) {

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

void Game::loop() {
    // SFML game Loop. 
    while(window.isOpen()) {


        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
                window.close();
            }
        }
        Vector2i mouse = Mouse::getPosition(window);
        std::cout << "pos_x=" << mouse.x << " pos_y=" << mouse.y << "\n";
        window.clear(Color::Black);

       if (event.type == Event::MouseButtonPressed) {
            pos[mouseCount] = Mouse::getPosition(window);
            std::cout << "pos_x=" << pos[mouseCount].x << " pos_y=" << pos[mouseCount].y << "\n";
            mouseCount++;
            if (mouseCount == 2) {
                CalculateMove(pos);
            }
            
        } 
        for(auto x : boardTiles) {window.draw(x.getShape());}
        for(auto x : whitePieces) {window.draw(x.getShape());}
        for(auto x : blackPieces) {window.draw(x.getShape());}
        window.display();
    }
}

/*void movePiece(int x, int y, int piece) {
    /*bool xComplete = 0, yComplete = 0;
    while(xComplete == 0 && yComplete == 0) {

    }*

    if() {

    }
}*/

int main() {
    Game gm;

    gm.SetBoardLocationArray();
    gm.initWhitePieces();
    gm.initBlackPieces();
    gm.initBoardPieces();
    gm.loop();

    // Exit program.
    return 0;
}
