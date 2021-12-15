//include classes and headers
#include "common.hpp"

using namespace::std;
using namespace::sf;


class BoardPiece {
private:
    Texture pieceTexture;

    RectangleShape piece;

    int pieceType; // 0 = rook, 1 = knight, 2 = bishop, 3 = queen, 4 = king, 5 = pawn, 6 = light tile, 7 = dark tile.
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
        "w_bishop_png_128px.png", "w_queen_png_128px.png",
        "w_king_png_128px.png", "w_bishop_png_128px.png",
        "w_knight_png_128px.png", "w_rook_png_128px.png",
        "w_pawn_png_128px.png"
    };
    whitePieceTextures.resize(9);

    // Init black piece variables.
    blackPieceTextureNames = {
        "b_rook_png_128px.png", "b_knight_png_128px.png",
        "b_bishop_png_128px.png", "b_queen_png_128px.png",
        "b_king_png_128px.png", "b_bishop_png_128px.png",
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

    // Get x & y coords - SH
    int mouseClickX = pos.x;
    int mouseClickY = pos.y;

    // loop through board pieces - SH
    for (int i = 0; i < boardTiles.size(); i++) {
        Vector2f tilePosition = boardTiles[i].getShape().getPosition();

        int boardLocX = tilePosition.x + 100;
        int boardLocY = tilePosition.y + 100;

        // If the mouse coords are within the tile's coords - SH
        if (mouseClickX >= tilePosition.x && mouseClickX <= boardLocX &&
            mouseClickY >= tilePosition.y && mouseClickY <= boardLocY) {
            BoardPiece boardLocation = boardTiles[i];

            // Loop through white pieces to see if player has clicked on a white piece - SH
            for (int i = 0; i < whitePieces.size(); i++) {
                if (boardLocation.getShape().getPosition() == whitePieces[i].getShape().getPosition()) {
                    return whitePieces[i];
                }
            }
            // Loop through white pieces to see if player has clicked on a black piece - SH
            for (int i = 0; i < blackPieces.size(); i++) {
                if (boardLocation.getShape().getPosition() == blackPieces[i].getShape().getPosition()) {
                    return blackPieces[i];
                }
            }

            return boardTiles[i];

        }
    }
}

void Game::CalculatePawnMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck) {

    Vector2f oldBoardTile = oldBoardSquare.getShape().getPosition();
    Vector2f newBoardTile = newBoardSquare.getShape().getPosition();
    int calcYDiff = newBoardTile.y - oldBoardTile.y;
    int calcXDiff = newBoardTile.x - oldBoardTile.x;
    bool teamPieceInNewTile = (!isKingCheck) ? CheckForTeamPiece(newBoardSquare) : false;
    bool isPawnFirstMove = false;
    int locationInc = 100;
    bool isPathClear = true;


    if (!teamPieceInNewTile && !isKingInCheck) {

        // Loop through to check if individual pawns first move - SH
        for (int i = 0; i < pawnArray.size(); i++) {
            if (oldBoardSquare.getShape().getPosition() == pawnArray[i].getShape().getPosition() && 
                oldBoardSquare.getShape().getFillColor() == pawnArray[i].getShape().getFillColor()) {
                isPawnFirstMove = true;
                break;
            }
        }

        if (oldBoardSquare.getShape().getFillColor() == Color::White) { //white move - SH
            if  (isPawnFirstMove && 
                calcYDiff != 0 && 
                calcYDiff <= -200 && 
                oldBoardTile.x == newBoardTile.x) { // If first move pawn can move 2 spaces - SH
                BoardPiece currentTile = oldBoardSquare;

                // Loop until tile we are looking at is the same position as the newBoardSquare position OR isPathClear is false - SH
                while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition() && isPathClear) {
                    Vector2f newLocation;
                    newLocation.x = currentTile.getShape().getPosition().x;
                    newLocation.y = currentTile.getShape().getPosition().y - locationInc; //calculate position of the next the next tile up - SH

                    // Loop through the whitePieces to see if they are in the next tile up - if we find a piece in the next tile, set isPathClear to false - SH
                    for (int i = 0; i < whitePieces.size(); i++) {
                        if (whitePieces[i].getShape().getPosition() == newLocation) {
                            isPathClear = false;
                            break;
                        }
                    }
                    // Loop through the blackPieces to see if they are in the next tile up - if we find a piece in the next tile, set isPathClear to false - SH

                    for (int i = 0; i < blackPieces.size(); i++) {
                        if (blackPieces[i].getShape().getPosition() == newLocation) {
                            isPathClear = false;
                            break;
                        }
                    }
                    // Update currentTile with new tile - SH
                    currentTile.getShape().setPosition(newLocation);

                }
                if (isPathClear) {// If the path is clear - update white chess piece - SH
                    UpdateWhiteChessPiece(oldBoardSquare, newBoardSquare);
                    playerWhite = !playerWhite;
                }
                else if(!isKingCheck){
                    // Error
                    cout << "Invalid move - Pawn 3" << endl;
                }

            }
            else if (newBoardSquare.getPieceType() > 5 && 
                calcYDiff != 0 && calcYDiff == -100 && 
                oldBoardTile.x == newBoardTile.x) { // After first move pawn can only move 1 tile up - SH
                UpdateWhiteChessPiece(oldBoardSquare, newBoardSquare);
                playerWhite = !playerWhite;
            }
            else if (newBoardSquare.getPieceType() < 6 && calcYDiff == -100 && abs(calcXDiff) == 100) {// If enemy piece is in new location pawn can move diagonally - SH
                if (!isKingCheck) {
                    //Valid - update chessPieceArray & Move image to new location - SH
                    RemoveEnemyChessPiece(newBoardSquare);
                    UpdateWhiteChessPiece(oldBoardSquare, newBoardSquare);
                    
                    playerWhite = !playerWhite;
                }
                else {
                    isKingInCheck = true;
                }
            }
            else {
                // Not valid - send error - SH
                // Only send error message when we aren't checking for king in check - SH
                if (!isKingCheck) {
                    cout << "Invalid move - Pawn2" << endl;
                }

            }
        }
        else if (oldBoardSquare.getShape().getFillColor() == Color::Black) { // Black move - SH
            if (isPawnFirstMove && calcYDiff != 0 && calcYDiff <= 200 && oldBoardTile.x == newBoardTile.x) {
                BoardPiece currentTile = oldBoardSquare;
                while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition() && isPathClear) {
                    Vector2f newLocation;
                    newLocation.x = currentTile.getShape().getPosition().x;
                    newLocation.y = currentTile.getShape().getPosition().y + locationInc;


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
                if (isPathClear) {
                    UpdateBlackChessPiece(oldBoardSquare, newBoardSquare);
                    playerWhite = !playerWhite;
                }
                else {
                    // Error - SH
                    cout << "Invalid move - Pawn 1" << endl;
                }

            }
            else if (newBoardSquare.getPieceType() > 5 && calcYDiff != 0 && calcYDiff == 100 && oldBoardTile.x == newBoardTile.x) { // After first move pawn can only move 1 tile up - SH
                UpdateBlackChessPiece(oldBoardSquare, newBoardSquare);
                playerWhite = !playerWhite;

            }
            else if (newBoardSquare.getPieceType() < 6 && calcYDiff == 100 && abs(calcXDiff) == 100) {// If enemy piece is in new location pawn can move diagonally - SH
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
                // Not valid - send error - SH
                // Only send error message if we aren't checking for king in check - SH
                if (!isKingCheck) {
                    // Error message goes here - SH
                    cout << "Invalid move - Pawn" << endl;
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
    bool teamPieceInNewTile = (!isKingCheck) ? CheckForTeamPiece(newBoardSquare) : false;
    bool isPathClear = true;
    BoardPiece currentTile = oldBoardSquare;
    int locationInc = 100;


    // Figure out if the path is clear from old location to new location - SH
    if (!teamPieceInNewTile && !isKingInCheck) {

        if (calcYDiff == 0 && calcXDiff < 0) { // Left across - SH
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition() && isPathClear) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x - locationInc;
                newLocation.y = currentTile.getShape().getPosition().y;
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
        else if (calcYDiff == 0 && calcXDiff > 0) { // Right across - SH
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition() && isPathClear) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x + locationInc;
                newLocation.y = currentTile.getShape().getPosition().y;
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
        else if (calcYDiff > 0 && calcXDiff == 0) {// Vertical down - SH
            
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition() && isPathClear) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x;
                newLocation.y = currentTile.getShape().getPosition().y + locationInc;
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
        else if (calcYDiff < 0 && calcXDiff == 0) {// Vertical up - SH
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
            if ((calcYDiff == 0 && abs(calcXDiff) > 0) ||  // Rook going horizontally - SH
                (abs(calcYDiff) > 0 && calcXDiff == 0)) {  // Rook going Vetically - SH
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
                // Invalid move send error - SH
                if (!isKingCheck) {
                    cout << "Invalid move - Rook" << endl;
                }
            }
        }
        else {
            // Invalid move send error - SH
            if (!isKingCheck) {
                cout << "Invalid move - Rook1" << endl;
            }
        }

    }
}

void Game::CalculateKnightMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck) {
    Vector2f oldBoardTile = oldBoardSquare.getShape().getPosition();
    Vector2f newBoardTile = newBoardSquare.getShape().getPosition();
    int calcYDiff = newBoardTile.y - oldBoardTile.y;
    int calcXDiff = newBoardTile.x - oldBoardTile.x;
    bool teamPieceInNewTile = (!isKingCheck) ? CheckForTeamPiece(newBoardSquare) : false;

    // L-shape move validation - SH
    if (!teamPieceInNewTile && !isKingInCheck) {
        if ((calcYDiff == 200 && abs(calcXDiff) == 100) || // 2 down & 1 across - SH
            (calcYDiff == -200 && abs(calcXDiff) == 100) || // 2 up & 1 across - SH
            (abs(calcYDiff) == 100 && calcXDiff == 200) || // 2 right & 1 up/down - SH
            (abs(calcYDiff) == 100 && calcXDiff == -200)){ // 2 left & 1 up/down - SH
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
            // Invalid move - send error - SH
            // Only send error message if we aren't checking for king in check - SH
            if (!isKingCheck) {
                cout << "Invalid move - Knight" << endl;
            }
        }
    }
    else {
        // Your team's piece is already in that location - send error - SH
        // Only send error message if we aren't checking for king in check - SH
        if (!isKingCheck) {
            cout << "Invalid move - Knight1" << endl;
        }
    }
   
}

void Game::CalculateBishopMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck) {
    Vector2f oldBoardTile = oldBoardSquare.getShape().getPosition();
    Vector2f newBoardTile = newBoardSquare.getShape().getPosition();
    int calcYDiff = newBoardTile.y - oldBoardTile.y;
    int calcXDiff = newBoardTile.x - oldBoardTile.x;
    bool teamPieceInNewTile = (!isKingCheck) ? CheckForTeamPiece(newBoardSquare) : false;
    bool isPathClear = true;
    BoardPiece currentTile = oldBoardSquare;
    int locationInc = 100;

    // Figure out if the path is clear from old location to new location - SH
    if (!teamPieceInNewTile && !isKingInCheck) {
        if (calcYDiff > 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) { // Down Right Diag - SH
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x + locationInc;
                newLocation.y = currentTile.getShape().getPosition().y + locationInc;
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
        else if (calcYDiff > 0 && calcXDiff < 0 && abs(calcYDiff) == abs(calcXDiff)) { // Down Left Diag - SH
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x - locationInc;
                newLocation.y = currentTile.getShape().getPosition().y + locationInc;
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
        else if (calcYDiff < 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) {// Up left diag - SH
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x + locationInc;
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
        else if (calcYDiff < 0 && calcXDiff < 0 && abs(calcYDiff) == abs(calcXDiff)) {// Up Right Diag - SH
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x - locationInc;
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
            if (abs(calcXDiff) > 0 && 
                abs(calcYDiff) > 0 && 
                abs(calcXDiff) == abs(calcYDiff)) {
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
                // Invalid move - send error - SH
                // Send error if we aren't checking for king in check - SH
                if (!isKingCheck) {
                    cout << "Invalid move - Bishop1" << endl;
                }
            }
        }
        else {
            // Your team's piece is already in that location - send error SH
             // Send error if we aren't checking for king in check - SH
            if (!isKingCheck) {}
                cout << "Invalid move - Bishop" << endl;
        }

    }
}

void Game::CalculateQueenMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck) {
    Vector2f oldBoardTile = oldBoardSquare.getShape().getPosition();
    Vector2f newBoardTile = newBoardSquare.getShape().getPosition();
    int calcYDiff = newBoardTile.y - oldBoardTile.y;
    int calcXDiff = newBoardTile.x - oldBoardTile.x;
    bool teamPieceInNewTile = (!isKingCheck) ? CheckForTeamPiece(newBoardSquare) : false;
    bool isPathClear = true;
    BoardPiece currentTile = oldBoardSquare;
    int locationInc = 100;


    // Figure out it path is clear  from old position to new position - SH
    if (!teamPieceInNewTile && !isKingInCheck) {

        if (calcYDiff > 0 && calcXDiff == 0) {// Vertical down - SH
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x;
                newLocation.y = currentTile.getShape().getPosition().y + locationInc;
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
        else if (calcYDiff < 0 && calcXDiff == 0) {// Vertical up - SH
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
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
        else if (calcYDiff == 0 && calcXDiff < 0) { // Left across - SH
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
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
        else if (calcYDiff == 0 && calcXDiff > 0) { // Right across - SH
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x;
                newLocation.y = currentTile.getShape().getPosition().y + locationInc;
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
        else if (calcYDiff > 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) { // Down Right Diag - SH
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x + locationInc;
                newLocation.y = currentTile.getShape().getPosition().y + locationInc;
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
        else if (calcYDiff > 0 && calcXDiff < 0 && abs(calcYDiff) == abs(calcXDiff)) { // Down Left Diag - SH
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x - locationInc;
                newLocation.y = currentTile.getShape().getPosition().y + locationInc;
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
        else if (calcYDiff < 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) {// Up left diag - SH
            while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
                Vector2f newLocation;
                newLocation.x = currentTile.getShape().getPosition().x + locationInc;
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
        else if (calcYDiff < 0 && calcXDiff > 0 && abs(calcYDiff) == abs(calcXDiff)) {// Up Right Diag - SH
        while (currentTile.getShape().getPosition() != newBoardSquare.getShape().getPosition()) {
            Vector2f newLocation;
            newLocation.x = currentTile.getShape().getPosition().x - locationInc;
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

            if ((abs(calcXDiff) > 0 && calcYDiff == 0) || // Horizontal movement - SH
                (abs(calcYDiff) > 0 && calcXDiff == 0) || // Vertical movement - SH
                (abs(calcXDiff) > 0 && abs(calcYDiff) > 0 && abs(calcXDiff) == abs(calcYDiff))) { // Diagonal movement - SH
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
                // Send error - SH
                if (!isKingCheck) {
                    cout << "Invalid move - Queen" << endl;
                }
            }
        }
        else {
            // Send error - SH
            if (!isKingCheck) {
                cout << "Invalid move - Queen" << endl;
            }
        }
    }
    else {
    // king is in check - message - SH
        cout << "Your king is in check!" << endl;
    }
}

void Game::CalculateKingMove(BoardPiece oldBoardSquare, BoardPiece newBoardSquare, bool isKingCheck) {
    Vector2f oldBoardTile = oldBoardSquare.getShape().getPosition();
    Vector2f newBoardTile = newBoardSquare.getShape().getPosition();
    int calcYDiff = newBoardTile.y - oldBoardTile.y;
    int calcXDiff = newBoardTile.x - oldBoardTile.x;
    bool teamPieceInNewTile = (!isKingCheck) ? CheckForTeamPiece(newBoardSquare) : false;
    bool isPathClear = true;

    if (!teamPieceInNewTile) {

        if ((abs(calcXDiff) == 100 && calcYDiff == 0) ||  // King moving Horizontally - SH
            (abs(calcYDiff) == 100 && calcXDiff == 0) ||  // King moving Vertically - SH
            (abs(calcXDiff) == 100 && abs(calcYDiff) == 100)) {  // King moving diagonally - SH
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
            else if((abs(calcXDiff) > 100 && calcYDiff == 0) ||  // King moving Horizontally - SH
                (abs(calcYDiff) > 100 && calcXDiff == 0) ||  // King moving Vertically - SH
                (abs(calcXDiff) >= 100 && abs(calcYDiff) >= 100)) {  // King moving diagonally - SH
                cout << "Invalid move" << endl;
            }
            else {
                isKingInCheck = true;
            }
        }
        else {
            // Send error - SH
            if (!isKingCheck) {
                cout << "Invalid move - King" << endl;
            }
            //else if(isKingInCheck){
            //    //THEY LOSE - SEND OUT MESSAGE TO END GAME - SH
            //    cout << "GAME OVER!" << endl;
            //}
        }
    }
}

void Game::IsKingInCheck() {
    int currentTile;
    Vector2f whiteKingSize, blackKingSize, whiteKingPos, blackKingPos;
   

    for (int i = 0; i < whitePieces.size(); i++) {
        if (whitePieces[i].getPieceType() == 4 && whitePieces[i].getShape().getFillColor() == Color::White) {
            whiteKingSize = whitePieces[i].getShape().getSize();
            whiteKingPos = whitePieces[i].getShape().getPosition();
            break;
        }
    }
    for (int i = 0; i < blackPieces.size(); i++) {
        if (blackPieces[i].getPieceType() == 4 && blackPieces[i].getShape().getFillColor() == Color::Black) {
            blackKingSize = blackPieces[i].getShape().getSize();
            blackKingPos = blackPieces[i].getShape().getPosition();
            break;
        }
    }

    BoardPiece whiteKing = BoardPiece(whiteKingSize, whiteKingPos, Color::White, 4);
    BoardPiece blackKing = BoardPiece(blackKingSize, blackKingPos, Color::White, 4);

    // Loop through white piece array and check moves against black king position - SH
    for (int i = 0; i < whitePieces.size(); i++) {
        BoardPiece currentBoardTile = whitePieces[i];
        currentTile = currentBoardTile.getPieceType();
       

        // Can the piece land on the king? - SH
        // Is it a valid move? (ie is path clear etc) - SH
        switch (currentTile) {
        case 0:
            CalculateRookMove(currentBoardTile, blackKing, true);
            break;
        case 1:
            CalculateKnightMove(currentBoardTile, blackKing, true);
            break;
        case 2:
            CalculateBishopMove(currentBoardTile, blackKing, true);
            break;
        case 3:
            CalculateKingMove(currentBoardTile, blackKing, true);
            break;
        case 4:
            CalculateQueenMove(currentBoardTile, blackKing, true);
            break;
        case 5:
            CalculatePawnMove(currentBoardTile, blackKing, true);
            break;
        }

        if (isKingInCheck) {
            break;    
        }
    }

    // Loop through black piece array and check moves against white king - SH
    for (int i = 0; i < blackPieces.size(); i++) {
        BoardPiece currentBoardTile = blackPieces[i];
        currentTile = currentBoardTile.getPieceType();
        BoardPiece whiteKing = BoardPiece(whiteKingSize, whiteKingPos, Color::White, 4);
        BoardPiece blackKing = BoardPiece(blackKingSize, blackKingPos, Color::White, 4);
        
        // Can the piece land on the king? - SH
        // Is it a valid move? (ie is path clear etc) - SH
        switch (currentTile) {
        case 0:
            CalculateRookMove(currentBoardTile, whiteKing, true);
            break;
        case 1:
            CalculateKnightMove(currentBoardTile, whiteKing, true);
            break;
        case 2:
            CalculateBishopMove(currentBoardTile, whiteKing, true);
            break;
        case 3:
            CalculateKingMove(currentBoardTile, whiteKing, true);
            break;
        case 4:
            CalculateQueenMove(currentBoardTile, whiteKing, true);
            break;
        case 5:
            CalculatePawnMove(currentBoardTile, whiteKing, true);
            break;
        }

        if (isKingInCheck) {
            break;
        }
    }
    

    if (isKingInCheck) {
        // Send error - only let user move king - SH
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
        // Remove white piece from piece array - SH
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
            // Remove black piece from piece array - SH
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

    // Check position of movement against team pieces - SH
    if (newBoardSquare.getShape().getFillColor() == Color::White && newBoardSquare.getPieceType() < 6 && playerWhite ||
            newBoardSquare.getShape().getFillColor() == Color::Black && newBoardSquare.getPieceType() < 6 && !playerWhite) {
            teamPieceInNewTile = true;
    }

    return teamPieceInNewTile;
}

void Game::CalculateMove(Vector2i mouseClick1, Vector2i mouseClick2) {

    // Calculate which tiles user clicks on - SH
    BoardPiece oldBoardSquare = CalcMouseLocation(Vector2f(mouseClick1));
    BoardPiece newBoardSquare = CalcMouseLocation(Vector2f(mouseClick2));

    // Check if there's a chess piece on the old location -SH
    if (oldBoardSquare.getPieceType() < 6) {
        // There's a piece in the old square! Figure out if the new location is valid - SH
        if (oldBoardSquare.getPieceType() == 0) //Rook
        {
            CalculateRookMove(oldBoardSquare, newBoardSquare, false);
        }
        else if (oldBoardSquare.getPieceType() == 1) //knight
        {
            CalculateKnightMove(oldBoardSquare, newBoardSquare, false);
        }
        else if (oldBoardSquare.getPieceType() == 2) //Bishop
        {
            CalculateBishopMove(oldBoardSquare, newBoardSquare, false);
        }
        else if (oldBoardSquare.getPieceType() == 3) //Queen
        {
            CalculateQueenMove(oldBoardSquare, newBoardSquare, false);
        }
        else if (oldBoardSquare.getPieceType() == 4) //King
        {
            CalculateKingMove(oldBoardSquare, newBoardSquare, false);
        }
        else if (oldBoardSquare.getPieceType() == 5) //Pawn
        {
            CalculatePawnMove(oldBoardSquare, newBoardSquare, false);
        }

        IsKingInCheck();

     
    }
    else {
        // Send user error message - SH
        cout << "Invalid move" << endl;
    }

    mouseCount = 0;

}

void Game::loop() {
    // SFML game Loop. 
    while(window.isOpen()) {


        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) { // listen for window close - SH
                window.close();
            }
            else  if (event.type == Event::MouseButtonPressed) { // listen for mouse click - SH

                cout << "mouseCount=" << mouseCount; // Print out mouseCount -sH
                if (mouseCount == 0) { // If first mouse click
                    mouseClick1 = Mouse::getPosition(window); // Register mouse click - SH
                    std::cout << "pos_x=" << mouseClick1.x << " pos_y=" << mouseClick1.y << "\n"; // Output location in console -SH
                    BoardPiece mouseLocation = CalcMouseLocation(Vector2f(mouseClick1)); // Call function that calculates what piece player clicked on - SH


                    if (playerWhite && mouseLocation.getPieceType() < 6 && mouseLocation.getShape().getFillColor() == Color::White) { // If white player's turn & they clicked on a white piece- SH
                        mouseCount++; // Increase count of mouse clicks - SH
                    }
                    else if (!playerWhite && mouseLocation.getShape().getFillColor() == Color::Black) { // If black player's turn & they clicked on a black piece - SH
                        mouseCount++; // Increase count of mouse clicks - SH
                    }
                    else {
                        // Error - clicked opponent's piece - SH
                        cout << "Click on your own pieces!" << endl;
                    }
                }
                else { // When we have 2 mouse clicks (ie player has made a move)- SH
                    mouseClick2 = Mouse::getPosition(window);// Register mouse click - SH
                    std::cout << "pos_x=" << mouseClick2.x << " pos_y=" << mouseClick2.y << "\n"; // Output location in console - SH
                    CalculateMove(mouseClick1, mouseClick2); // Calculte the move - SH
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