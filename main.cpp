//include classes and headers
#include "common.hpp"

using namespace::std;
using namespace::sf;


class BoardPiece {
private:
    Texture pieceTexture;

    RectangleShape piece;

    int pieceType; // 0 = rook, 1 = knight, 2 = bishop, 3 = king, 4 = queen, 5 = pawn, 6 = light tile, 7 = dark tile.
    Vector2f position;


public:
    BoardPiece(Vector2f, Vector2f, Color, int);
    

    RectangleShape& getShape();
    int getPieceType();

    void setPieceType(int);
    void setBoardLocationArray();
   
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

int BoardPiece::getPieceType() {
    return pieceType;
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
    vector<std::string> blackPieceTextureNames;
    vector<Texture> blackPieceTextures;
    vector<BoardPiece> blackPieces;

    //initialise pawn array for first move
    vector<BoardPiece> pawnArray;

    // Init board piece variables.
    vector<string> boardTileTextureNames;
    vector<Texture> boardTileTextures;
    vector<BoardPiece> boardTiles;

    // Init window.
    RenderWindow window;

public:

    Game();

    void initWhitePieces();
    void initBlackPieces();
    void initBoardPieces();
    void CalculateMove(Vector2i mouseClick1, Vector2i mouseClick2);
    void CalculatePawnMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck);
    void CalculateRookMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck);
    void CalculateKnightMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck);
    void CalculateBishopMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck);
    void CalculateQueenMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck);
    void CalculateKingMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck);
    void UpdateWhiteChessPiece(BoardPiece oldBoardSquare, BoardPiece newBoardSquare);
    void UpdateBlackChessPiece(BoardPiece oldBoardSquare, BoardPiece newBoardSquare);
    void RemoveEnemyChessPiece(BoardPiece newBoardSquare);
    bool CheckForTeamPiece(BoardPiece newBoardSquare);
    void IsKingInCheck();

    BoardPiece CalcMouseLocation(Vector2f pos);
    Vector2i mouseClick1;
    Vector2i mouseClick2;
    //BoardPiece currentBoardSquare;
    //BoardPiece whiteKingTile;
    //BoardPiece blackKingTile;
    int mouseCount = 0;
    int whiteMoveCount = 0;
    int blackMoveCount = 0;
    bool isKingInCheck = false;
    bool playerWhite = true;

    void loop();
};

Game::Game() {
    // Init general variables.
    tileWidth = 100;

    // Init white piece variables.
    chessPieceOrder = {0, 1, 2, 3, 4, 2, 1, 0, 5};
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

    // Create white Pieces.
    for(int i = 0; i < chessPieceOrder.size(); i++) {
        if(chessPieceOrder[i] == 5) {
            // Create white pawns.
            for(int j = 0; j < 8; j++) {
                whitePieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * j, tileWidth * 6), Color::White, chessPieceOrder[i]));
                whitePieces[i + j].getShape().setTexture(&whitePieceTextures[i]);
                pawnArray.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * j, tileWidth * 6), Color::White, chessPieceOrder[i]));
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
        if(chessPieceOrder[i] == 5) {
            // Create Black pawns.
            for(int j = 0; j < 8; j++) {
                blackPieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * j, tileWidth), Color::Black, chessPieceOrder[i]));
                blackPieces[i + j].getShape().setTexture(&blackPieceTextures[i]);
                pawnArray.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * j, tileWidth), Color::Black, chessPieceOrder[i]));

            }
        } else {
            // Create all other Black pieces.
            blackPieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * i, 0), Color::Black, chessPieceOrder[i]));
            blackPieces[i].getShape().setTexture(&blackPieceTextures[i]);
        }
    }
}

void Game::initBoardPieces() {
    int startingColor = 0; // 0 = light tile, 1 = dark tile
    int currentColor = startingColor;
    Vector2f position;

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
            position.x = tileWidth * j;
            position.y = tileWidth * i;
            if (currentColor == 0) {
                boardTiles.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), position, Color::White, 6));
                boardTiles[(i * 8) + j].getShape().setTexture(&boardTileTextures[0]);
                currentColor++;
            }
            else {
                boardTiles.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), position, Color::White, 7));
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

BoardPiece Game::CalcMouseLocation(Vector2f pos) {

    int mouseClickX = pos.x;
    int mouseClickY = pos.y;

    for (int i = 0; i < boardTiles.size(); i++) {
        Vector2f tilePosition = boardTiles[i].getShape().getPosition();

        int boardLocX = tilePosition.x + 100;
        int boardLocY = tilePosition.y + 100;
        if (mouseClickX >= tilePosition.x && mouseClickX <= boardLocX) {

            if (mouseClickY >= tilePosition.y && mouseClickY <= boardLocY) {
                BoardPiece boardLocation = boardTiles[i];
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (boardLocation.getShape().getPosition() == whitePieces[i].getShape().getPosition()) {
                        return whitePieces[i];
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (boardLocation.getShape().getPosition() == blackPieces[i].getShape().getPosition()) {
                        return blackPieces[i];
                    }
                }

                return boardTiles[i];
            }
        }
    }
}

void Game::CalculatePawnMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck) {

    Vector2f oldBoardTile = oldBoardSquare.getShape().getPosition();
    Vector2f newBoardTile = newBoardSquare.getShape().getPosition();
    int calcYDiff = newBoardTile.y - oldBoardTile.y;
    int calcXDiff = newBoardTile.x - oldBoardTile.x;
    bool teamPieceInNewTile = CheckForTeamPiece(newBoardSquare);
    bool isPawnFirstMove = false;


    if (!teamPieceInNewTile && !isKingInCheck) {

        // Loop through to check if individual pawns first move
        for (int i = 0; i < pawnArray.size(); i++) {
            if (oldBoardSquare.getShape().getPosition() == pawnArray[i].getShape().getPosition() && 
                oldBoardSquare.getShape().getFillColor() == pawnArray[i].getShape().getFillColor()) {
                isPawnFirstMove = true;
                break;
            }
        }

        if (oldBoardSquare.getShape().getFillColor() == Color::White) { //white move
            if  (isPawnFirstMove && 
                calcYDiff != 0 && 
                calcYDiff <= -200 && 
                oldBoardTile.x == newBoardTile.x) { //if first move pawn can move 2 spaces
                UpdateWhiteChessPiece(oldBoardSquare, newBoardSquare);
                playerWhite = !playerWhite;

            }
            else if (newBoardSquare.getPieceType() > 5 && 
                calcYDiff != 0 && calcYDiff == -100 && 
                oldBoardTile.x == newBoardTile.x) { //after first move pawn can only move 1 tile up
                UpdateWhiteChessPiece(oldBoardSquare, newBoardSquare);
                playerWhite = !playerWhite;
            }
            else if (newBoardSquare.getPieceType() < 6 && calcYDiff == -100 && abs(calcXDiff) == 100) {//if enemy piece is in new location pawn can move diagonally
                if (!isKingCheck) {
                    //valid - update chessPieceArray & Move image to new location
                    RemoveEnemyChessPiece(newBoardSquare);
                    UpdateWhiteChessPiece(oldBoardSquare, newBoardSquare);
                    
                    playerWhite = !playerWhite;
                }
                else {
                    isKingInCheck = true;
                }
            }
            else {
                //not valid - send error
                //only send error message when we aren't checking for king in check
                if (!isKingCheck) {
                    cout << "Invalid move" << endl;
                }

            }
        }
        else if (oldBoardSquare.getShape().getFillColor() == Color::Black) { //black move
            if (isPawnFirstMove && calcYDiff != 0 && calcYDiff <= 200 && oldBoardTile.x == newBoardTile.x) {
                UpdateBlackChessPiece(oldBoardSquare, newBoardSquare);
                playerWhite = !playerWhite;

            }
            else if (newBoardSquare.getPieceType() > 5 && calcYDiff != 0 && calcYDiff == 100 && oldBoardTile.x == newBoardTile.x) { //after first move pawn can only move 1 tile up
                UpdateBlackChessPiece(oldBoardSquare, newBoardSquare);
                playerWhite = !playerWhite;

            }
            else if (newBoardSquare.getPieceType() < 6 && calcYDiff == 100 && abs(calcXDiff) == 100) {//if enemy piece is in new location pawn can move diagonally
                if (!isKingCheck) {
                    UpdateBlackChessPiece(oldBoardSquare, newBoardSquare);
                    RemoveEnemyChessPiece(newBoardSquare);
                    playerWhite = !playerWhite;
                }
                else {
                    isKingInCheck = true;
                }

            }
            else {
                //not valid - send error
                //only send error message if we aren't checking for king in check
                if (!isKingCheck) {
                    // Error message goes here
                    cout << "Invalid move" << endl;
                }

            }
        }

    }
}

void Game::CalculateRookMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck) {
    Vector2f oldBoardTile = oldBoardSquare.getShape().getPosition();
    Vector2f newBoardTile = newBoardSquare.getShape().getPosition();
    int calcYDiff = newBoardTile.y - oldBoardTile.y;
    int calcXDiff = newBoardTile.x - oldBoardTile.x;
    bool teamPieceInNewTile = CheckForTeamPiece(newBoardSquare);
    bool isPathClear = true;
    BoardPiece currentTile = oldBoardSquare;
    int locationInc = 100;


    //figure out if the path is clear from old location to new location
    if (!teamPieceInNewTile && !isKingInCheck) {

        if (calcYDiff == 0 && calcXDiff < 0) { //left across
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x - locationInc;
                newLocation.y = currentTile.getShape().getPosition().y;
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);
            }
            
        }
        else if (calcYDiff == 0 && calcXDiff > 0) { //right across
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x + locationInc;
                newLocation.y = currentTile.getShape().getPosition().y;
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);
            }

        }
        else if (calcYDiff > 0 && calcXDiff == 0) {//vertical down
            
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x;
                newLocation.y = currentTile.getShape().getPosition().y + locationInc;
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);
            }
        }
        else if (calcYDiff < 0 && calcXDiff == 0) {//vertical up
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition() && isPathClear) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x;
                newLocation.y = currentTile.getShape().getPosition().y - locationInc;
                
                
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear = false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear = false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);

            }
        }

        if (isPathClear) {
            if ((calcYDiff == 0 && abs(calcXDiff) > 0) ||  // Rook going horizontally
                (abs(calcYDiff) > 0 && calcXDiff == 0)) {  // Rook going Vetically
                if (!isKingCheck) {
                    if (oldBoardSquare.getShape().getFillColor() == Color::White) {
                        UpdateWhiteChessPiece(oldBoardSquare, newBoardSquare);

                    }
                    else {
                        UpdateBlackChessPiece(oldBoardSquare, newBoardSquare);
                    }
                    RemoveEnemyChessPiece(newBoardSquare);
                    playerWhite = !playerWhite;
                }
                else {
                    isKingInCheck = true;
                }
            }
            else {
                //invalid move send error
                if (!isKingCheck) {
                    cout << "Invalid move" << endl;
                }
            }
        }
        else {
            //invalid move send error
            if (!isKingCheck) {
                cout << "Invalid move" << endl;
            }
        }

    }
}

void Game::CalculateKnightMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck) {
    Vector2f oldBoardTile = oldBoardSquare.getShape().getPosition();
    Vector2f newBoardTile = newBoardSquare.getShape().getPosition();
    int calcYDiff = newBoardTile.y - oldBoardTile.y;
    int calcXDiff = newBoardTile.x - oldBoardTile.x;
    bool teamPieceInNewTile = CheckForTeamPiece(newBoardSquare);

    //L-shape move validation - SH
    if (!teamPieceInNewTile && !isKingInCheck) {
        if ((calcYDiff == 200 && abs(calcXDiff) == 100) || //2 down & 1 across
            (calcYDiff == -200 && abs(calcXDiff) == 100) || // 2 up & 1 across
            (abs(calcYDiff) == 100 && calcXDiff == 200) || //2 right & 1 up/down
            (abs(calcYDiff) == 100 && calcXDiff == -200)){ //2 left & 1 up/down
            if (!isKingCheck) {
                if (oldBoardSquare.getShape().getFillColor() == Color::White) {
                    UpdateWhiteChessPiece(oldBoardSquare, newBoardSquare);
                }
                else {
                    UpdateBlackChessPiece(oldBoardSquare, newBoardSquare);
                }
                RemoveEnemyChessPiece(newBoardSquare);
                playerWhite = !playerWhite;
            }
            else {
                isKingInCheck = true;
            }
        }
        else {
            //invalid move - send error
            //only send error message if we aren't checking for king in check
            if (!isKingCheck) {
                cout << "Invalid move" << endl;
            }
        }
    }
    else {
        //your team's piece is already in that location - send error SH
        //only send error message if we aren't checking for king in check
        if (!isKingCheck) {
            cout << "Invalid move" << endl;
        }
    }
   
}

void Game::CalculateBishopMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck) {
    Vector2f oldBoardTile = oldBoardSquare.getShape().getPosition();
    Vector2f newBoardTile = newBoardSquare.getShape().getPosition();
    int calcYDiff = newBoardTile.y - oldBoardTile.y;
    int calcXDiff = newBoardTile.x - oldBoardTile.x;
    bool teamPieceInNewTile = CheckForTeamPiece(newBoardSquare);
    bool isPathClear = true;
    BoardPiece currentTile = oldBoardSquare;
    int locationInc = 100;

    //figure out if the path is clear from old location to new location
    if (!teamPieceInNewTile && !isKingInCheck) {
        if (calcYDiff > 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) { //Down Right Diag
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x + locationInc;
                newLocation.y = currentTile.getShape().getPosition().y + locationInc;
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);
                
            }
        }
        else if (calcYDiff > 0 && calcXDiff < 0 && abs(calcYDiff) == abs(calcXDiff)) { //Down Left Diag
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x - locationInc;
                newLocation.y = currentTile.getShape().getPosition().y + locationInc;
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);
            }
        }
        else if (calcYDiff < 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) {//Up left diag
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x - locationInc;
                newLocation.y = currentTile.getShape().getPosition().y - locationInc;
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);
            } 
        }
        else if (calcYDiff < 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) {//Up Right Diag
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x + locationInc;
                newLocation.y = currentTile.getShape().getPosition().y - locationInc;
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);
            }
        }

        //L-shape move validation - SH
        if (isPathClear) {
            if ((calcYDiff == 200 && abs(calcXDiff) == 100) || //2 down & 1 across
                (calcYDiff == -200 && abs(calcXDiff) == 100) || // 2 up & 1 across
                (abs(calcYDiff) == 100 && calcXDiff == 200) || //2 right & 1 up/down
                (abs(calcYDiff) == 100 && calcXDiff == -200)) { //2 left & 1 up/down
                if (!isKingCheck) {
                    if (oldBoardSquare.getShape().getFillColor() == Color::White) {
                        UpdateWhiteChessPiece(oldBoardSquare, newBoardSquare);
                    }
                    else {
                        UpdateBlackChessPiece(oldBoardSquare, newBoardSquare);
                    }
                    RemoveEnemyChessPiece(newBoardSquare);
                    playerWhite = !playerWhite;
                }
                else {
                    isKingInCheck = true;
                }
            }
            else {
                //invalid move - send error
                //send error if we aren't checking for king in check
                if (!isKingCheck) {}
                    cout << "Invalid move" << endl;
            }
        }
        else {
            //your team's piece is already in that location - send error SH
             //send error if we aren't checking for king in check
            if (!isKingCheck) {}
                cout << "Invalid move" << endl;
        }

    }
}

void Game::CalculateQueenMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck) {
    Vector2f oldBoardTile = oldBoardSquare.getShape().getPosition();
    Vector2f newBoardTile = newBoardSquare.getShape().getPosition();
    int calcYDiff = newBoardTile.y - oldBoardTile.y;
    int calcXDiff = newBoardTile.x - oldBoardTile.x;
    bool teamPieceInNewTile = CheckForTeamPiece(newBoardSquare);
    bool isPathClear = true;
    BoardPiece currentTile = oldBoardSquare;
    int locationInc = 100;


    // Figure out it path is clear  from old position to new position
    if (!teamPieceInNewTile && !isKingInCheck) {

        if (calcYDiff > 0 && calcXDiff == 0) {//vertical down
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x;
                newLocation.y = currentTile.getShape().getPosition().y + locationInc;
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);
            }
            
        }
        else if (calcYDiff < 0 && calcXDiff == 0) {//vertical up
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x;
                newLocation.y = currentTile.getShape().getPosition().y - locationInc;
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);
            }
        }
        else if (calcYDiff == 0 && calcXDiff < 0) { //left across
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x;
                newLocation.y = currentTile.getShape().getPosition().y - locationInc;
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);
            }
        }
        else if (calcYDiff == 0 && calcXDiff > 0) { //right across
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x;
                newLocation.y = currentTile.getShape().getPosition().y + locationInc;
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);
            }

        }
        else if (calcYDiff > 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) { //Down Right Diag
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x + locationInc;
                newLocation.y = currentTile.getShape().getPosition().y + locationInc;
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);
            }
        }
        else if (calcYDiff > 0 && calcXDiff < 0 && abs(calcYDiff) == abs(calcXDiff)) { //Down Left Diag
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x - locationInc;
                newLocation.y = currentTile.getShape().getPosition().y + locationInc;
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);
            }
        }
        else if (calcYDiff < 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) {//Up left diag
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x - locationInc;
                newLocation.y = currentTile.getShape().getPosition().y - locationInc;
                for (int i = 0; i < whitePieces.size(); i++) {
                    if (whitePieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                for (int i = 0; i < blackPieces.size(); i++) {
                    if (blackPieces[i].getShape().getPosition() == newLocation) {
                        isPathClear == false;
                        break;
                    }
                }
                currentTile.getShape().setPosition(newLocation);
            }
        }
        else if (calcYDiff < 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) {//Up Right Diag
        while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
            Vector2f newLocation;
            newLocation.x = currentTile.getShape().getPosition().x + locationInc;
            newLocation.y = currentTile.getShape().getPosition().y - locationInc;
            for (int i = 0; i < whitePieces.size(); i++) {
                if (whitePieces[i].getShape().getPosition() == newLocation) {
                    isPathClear == false;
                    break;
                }
            }
            for (int i = 0; i < blackPieces.size(); i++) {
                if (blackPieces[i].getShape().getPosition() == newLocation) {
                    isPathClear == false;
                    break;
                }
            }
            currentTile.getShape().setPosition(newLocation);
        }
        }


        if (isPathClear) {

            if ((abs(calcXDiff) > 0 && calcYDiff == 0) || //horizontal movement
                (abs(calcYDiff) > 0 && calcXDiff == 0) || // vertical movement
                (abs(calcXDiff) > 0 && abs(calcYDiff) > 0 && abs(calcXDiff) == abs(calcYDiff))) { // diagonal movement
                if (!isKingCheck) {
                    if (oldBoardSquare.getShape().getFillColor() == Color::White) {
                        UpdateWhiteChessPiece(oldBoardSquare, newBoardSquare);
                    }
                    else {
                        UpdateBlackChessPiece(oldBoardSquare, newBoardSquare);
                    }
                    RemoveEnemyChessPiece(newBoardSquare);
                    playerWhite = !playerWhite;
                }
                else {
                    isKingInCheck = true;
                }
            }
            else {
                //send error
                if (!isKingCheck) {
                    cout << "Invalid move" << endl;
                }
            }
        }
        else {
            //send error
            if (!isKingCheck) {
                cout << "Invalid move" << endl;
            }
        }
    }
    else {
    //king is in check - message
        cout << "Your king is in check!" << endl;
    }
}

void Game::CalculateKingMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck) {
    Vector2f oldBoardTile = oldBoardSquare.getShape().getPosition();
    Vector2f newBoardTile = newBoardSquare.getShape().getPosition();
    int calcYDiff = newBoardTile.y - oldBoardTile.y;
    int calcXDiff = newBoardTile.x - oldBoardTile.x;
    bool teamPieceInNewTile = CheckForTeamPiece(newBoardSquare);
    bool isPathClear = true;

    if (!teamPieceInNewTile) {

        if ((abs(calcXDiff) == 100 && calcYDiff == 0) ||  // King moving Horizontally
            (abs(calcYDiff) == 100 && calcXDiff == 0) ||  // King moving Vertically
            (abs(calcXDiff) == 100 && abs(calcYDiff) == 100)) {  // King moving diagonally
            if (!isKingCheck) {
                if (oldBoardSquare.getShape().getFillColor() == Color::White) {
                    UpdateWhiteChessPiece(oldBoardSquare, newBoardSquare);
                }
                else {
                    UpdateBlackChessPiece(oldBoardSquare, newBoardSquare);
                }
                RemoveEnemyChessPiece(newBoardSquare);
                playerWhite = !playerWhite;
                isKingInCheck = false;
            }
            else {
                isKingInCheck = true;
            }
        }
        else {
            //send error
            if (!isKingCheck) {
            }
            else {
                //THEY LOSE - SEND OUT MESSAGE TO END GAME - SH
                cout << "GAME OVER!" << endl;
            }
        }
    }
}

void Game::IsKingInCheck() {
    int currentTile;
    int whiteKingLoc = 0;
    int blackKingLoc = 0;

    for (int i = 0; i < boardTiles.size(); i++) {
        if (boardTiles[i].getPieceType() == 3 && boardTiles[i].getShape().getFillColor() == Color::White) {
            whiteKingLoc = i;
        }
        else if (boardTiles[i].getPieceType() == 3 && boardTiles[i].getShape().getFillColor() == Color::Black) {
            blackKingLoc = i;
        }
    }

    for (int i = 0; i < boardTiles.size(); i++) {
        BoardPiece currentBoardTile = boardTiles[i];
        currentTile = currentBoardTile.getPieceType();

        //can the piece land on the king?
        //is it a valid move? (ie is path clear etc)
        switch (currentTile) {
        case 0:
            CalculateRookMove(currentBoardTile, boardTiles[whiteKingLoc], true);
            CalculateRookMove(currentBoardTile, boardTiles[blackKingLoc], true);
            break;
        case 1:
            CalculateKnightMove(currentBoardTile, boardTiles[whiteKingLoc], true);
            CalculateKnightMove(currentBoardTile, boardTiles[blackKingLoc], true);
            break;
        case 2:
            CalculateBishopMove(currentBoardTile, boardTiles[whiteKingLoc], true);
            CalculateBishopMove(currentBoardTile, boardTiles[blackKingLoc], true);
            break;
        case 3:
            CalculateKingMove(currentBoardTile, boardTiles[whiteKingLoc], true);
            CalculateKingMove(currentBoardTile, boardTiles[blackKingLoc], true);
            break;
        case 4:
            CalculateQueenMove(currentBoardTile, boardTiles[whiteKingLoc], true);
            CalculateQueenMove(currentBoardTile, boardTiles[blackKingLoc], true);
            break;
        case 5:
            CalculatePawnMove(currentBoardTile, boardTiles[whiteKingLoc], true);
            CalculatePawnMove(currentBoardTile, boardTiles[blackKingLoc], true);
            break;
        }

        if (isKingInCheck) {
            break;
               
        }
    }
    

    if (isKingInCheck) {
        //send error - only let user move king
        cout << "Your king is in check!" << endl << "Get your king out of check!" << endl;
    }
}

void Game::UpdateWhiteChessPiece(BoardPiece oldBoardSquare, BoardPiece newBoardSquare) {
    int whitePiece;
    for (int i = 0; i < whitePieces.size(); i++) {
        if (whitePieces[i].getShape().getPosition() == oldBoardSquare.getShape().getPosition()) {
            whitePiece = i;
            break;
        }
    }
    Vector2f newPosition = newBoardSquare.getShape().getPosition();
    whitePieces[whitePiece].getShape().setPosition(newPosition);
    whiteMoveCount++;
}

void Game::UpdateBlackChessPiece(BoardPiece oldBoardSquare, BoardPiece newBoardSquare) {
    int blackPiece;
    for (int i = 0; i < blackPieces.size(); i++) {
        if (blackPieces[i].getShape().getPosition() == oldBoardSquare.getShape().getPosition()) {
            blackPiece = i;
            break;
        }
    }
    Vector2f newPosition = newBoardSquare.getShape().getPosition();
    blackPieces[blackPiece].getShape().setPosition(newPosition);
    blackMoveCount++;
}

void Game::RemoveEnemyChessPiece(BoardPiece newBoardPiece) {
    int num;
    if (newBoardPiece.getPieceType() < 6) {
        //remove enemyPiece from Sprites
        if (newBoardPiece.getShape().getFillColor() == Color::White) {
            for (int i = 0; i < whitePieces.size(); i++) {
                if (newBoardPiece.getShape().getPosition() == whitePieces[i].getShape().getPosition()) {
                    num = i;
                    break;
                }
            }

            int arraySize = whitePieces.size();
            if (num != arraySize - 1) {
                swap(whitePieces[num], whitePieces[arraySize - 1]);
            }
            whitePieces.pop_back();
        }
        else {
            for (int i = 0; i < blackPieces.size(); i++) {
                if (newBoardPiece.getShape().getPosition() == blackPieces[i].getShape().getPosition()) {
                    num = i;
                    break;
                }
            }

            int arraySize = blackPieces.size();
            if (num != arraySize - 1) {
                swap(blackPieces[num], blackPieces[arraySize - 1]);
            }
            blackPieces.pop_back();
        }
    }
}

bool Game::CheckForTeamPiece(BoardPiece newBoardSquare) {
    bool teamPieceInNewTile = false;
    if (newBoardSquare.getShape().getFillColor() == Color::White && newBoardSquare.getPieceType() < 6 && playerWhite ||
            newBoardSquare.getShape().getFillColor() == Color::Black && newBoardSquare.getPieceType() < 6 && !playerWhite) {
            teamPieceInNewTile = true;
    }

    return teamPieceInNewTile;
}

void Game::CalculateMove(Vector2i mouseClick1, Vector2i mouseClick2) {

    //Calculate which tile user clicks on
    BoardPiece oldBoardSquare = CalcMouseLocation(Vector2f(mouseClick1));
    BoardPiece newBoardSquare = CalcMouseLocation(Vector2f(mouseClick2));

    int j = oldBoardSquare.getShape().getPosition().x;
    int i = oldBoardSquare.getShape().getPosition().y;

    //Check if there's a chess piece on the old location
    if (oldBoardSquare.getPieceType() < 6) {
        //There's a piece in the old square! Figure out if the new location is valid
        if (oldBoardSquare.getPieceType() == 0) //rook
        {
            CalculateRookMove(oldBoardSquare, newBoardSquare, false);
        }
        else if (oldBoardSquare.getPieceType() == 1) //knight
        {
            CalculateKnightMove(oldBoardSquare, newBoardSquare, false);
        }
        else if (oldBoardSquare.getPieceType() == 2) //bishop
        {
            CalculateBishopMove(oldBoardSquare, newBoardSquare, false);
        }
        else if (oldBoardSquare.getPieceType() == 3) //king
        {
            CalculateKingMove(oldBoardSquare, newBoardSquare, false);
        }
        else if (oldBoardSquare.getPieceType() == 4) //queen
        {
            CalculateQueenMove(oldBoardSquare, newBoardSquare, false);
        }
        else if (oldBoardSquare.getPieceType() == 5) //pawn
        {
            CalculatePawnMove(oldBoardSquare, newBoardSquare, false);
        }

        IsKingInCheck();

     
    }
    else {
        //send user error message
        cout << "Invalid move" << endl;
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
            else  if (event.type == Event::MouseButtonPressed) {

                cout << "mouseCount=" << mouseCount;
                if (mouseCount == 0) {
                    mouseClick1 = Mouse::getPosition(window);//Register mouse click - SH
                    std::cout << "pos_x=" << mouseClick1.x << " pos_y=" << mouseClick1.y << "\n"; //output location in console
                    BoardPiece mouseLocation = CalcMouseLocation(Vector2f(mouseClick1));


                    if (playerWhite && mouseLocation.getPieceType() < 6 && mouseLocation.getShape().getFillColor() == Color::White) { //white player's turn
                        mouseCount++; //increase count of array
                    }
                    else if (!playerWhite && mouseLocation.getShape().getFillColor() == Color::Black) { //black player's turn
                        mouseCount++; //increase count of array
                    }
                    else {
                        //Error - clicked opponent's piece
                        cout << "Click on your own pieces!" << endl;
                    }
                }
                else { //when we have 2 mouse clicks (ie player has made a move
                    mouseClick2 = Mouse::getPosition(window);//Register mouse click - SH
                    std::cout << "pos_x=" << mouseClick2.x << " pos_y=" << mouseClick2.y << "\n"; //output location in console
                    CalculateMove(mouseClick1, mouseClick2); //Calculte the move
                }

            }

        }
        window.clear(Color::Black);

      
        for(auto x : boardTiles) {window.draw(x.getShape());}
        for(auto x : whitePieces) {window.draw(x.getShape());}
        for(auto x : blackPieces) {window.draw(x.getShape());}
        window.display();
    }
}

int main() {
    Game gm;

    gm.initWhitePieces();
    gm.initBlackPieces();
    gm.initBoardPieces();
    gm.loop();

    // Exit program.
    return 0;
}