//include classes and headers
#include "common.hpp"
#include "Pieces.h"

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
    void CalculatePawnMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece);

    Vector2f CalcMouseLocation(Vector2f pos);
    Vector2i pos[2];
    Vector2f currentBoardSquare;
    int mouseCount = 0;
    int moveCount = 0;

    void loop();

    //void movePiece(int, int, int); // X, Y, piece to move.

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
                whitePieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * j, tileWidth), Color::White, chessPieceOrder[i]));
                whitePieces[i + j].getShape().setTexture(&whitePieceTextures[i]);
            }
        } else {
            // Create all other white pieces.
            whitePieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * i, 0), Color::White, chessPieceOrder[i]));
            whitePieces[i].getShape().setTexture(&whitePieceTextures[i]);
        }
    }

    //for (int i = 0; i < 8; i++) {
    //    for (int j = 0; j < 8; j++) {
    //        int chessPiece = ChessPieceArray[i][j];

    //        if (chessPiece == 1 || chessPiece == 2 || chessPiece == 3 || chessPiece == 4 || chessPiece == 5) {
    //            /*whitePieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), BoardLocationArray[j][i], Color::White, chessPiece));
    //            whitePieces[i + j].getShape().setTexture(&whitePieceTextures[chessPiece -1]);*/
    //            whitePieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * i, 0), Color::White, chessPiece));
    //             whitePieces[i].getShape().setTexture(&whitePieceTextures[chessPiece -1]);
    //        }
    //        else if (chessPiece == 6) {
    //            whitePieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * j, tileWidth), Color::White, chessPiece));
    //            whitePieces[i + j].getShape().setTexture(&whitePieceTextures[chessPiece -1]);
    //        }
    //        else if (chessPiece == -1 || chessPiece == -2 || chessPiece == -3 || chessPiece == -4 || chessPiece == -5) {
    //            /*blackPieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), BoardLocationArray[i][j], Color::White, abs(chessPiece)));
    //            blackPieces[i + j].getShape().setTexture(&blackPieceTextures[abs(chessPiece) - 1]);*/
    //        }


    //    }
    //}

    

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
                blackPieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * j, tileWidth * 6), Color::White, chessPieceOrder[i]));
                blackPieces[i + j].getShape().setTexture(&blackPieceTextures[i]);
            }
        } else {
            // Create all other white pieces.
            blackPieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * i, tileWidth * 7), Color::White, chessPieceOrder[i]));
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

void Game::CalculatePawnMove(Vector2f oldBoardSquare, Vector2f newBoardSquare, int chessPiece) {

    int oldX = oldBoardSquare.x;
    int oldY = oldBoardSquare.y;
    int newX = newBoardSquare.x;
    int newY = newBoardSquare.y;
    int calcYDiff = newY - oldY;

    if (moveCount + 1 == 1) {
        if (calcYDiff != 0 && abs(calcYDiff) <= 2 && oldX == newX) {
            //valid - update chessPieceArray & Move image to new location
            ChessPieceArray[oldY][oldX] = 0;
            ChessPieceArray[newX][newY] = chessPiece;


            blackPieces.push_back(BoardPiece(Vector2f(tileWidth, tileWidth), Vector2f(tileWidth * newX, tileWidth * 6), Color::White, abs(chessPiece)));
            blackPieces[newX + newY].getShape().setTexture(&blackPieceTextures[abs(chessPiece)]);
        
        }
        else {
            //not valid - send error

        }
    }
    else {
        //if (calcYDiff != 0 && abs(calcXDiff) <= 1 && oldY == newY) {
        //    //valid - update chessPieceArray & Move image to new location
        //    ChessPieceArray[oldX][oldY] = 0;
        //    ChessPieceArray[newX][newY] = chessPiece;

        //}
        //else {
        //    //not valid - send error

        //}
    }

    mouseCount = 0;
    moveCount++;

}

void Game::CalculateMove(Vector2i pos[]) {

    //Calculate which tile user clicks on
    Vector2f oldBoardSquare = CalcMouseLocation(Vector2f(pos[0]));
    Vector2f newBoardSquare = CalcMouseLocation(Vector2f(pos[1]));

    int j = oldBoardSquare.x;
    int i = oldBoardSquare.y;

    //Check if there's a chess piece on the old location
    if (ChessPieceArray[i][j] < 0) {
        //There's a piece in the old square! Figure out if the new location is valid
        if (ChessPieceArray[i][j] == -1) //rook
        {

        }
        else if (ChessPieceArray[i][j] == -2) //knight
        {

        }
        else if (ChessPieceArray[i][j] == -3) //bishop
        {

        }
        else if (ChessPieceArray[i][j] == -4) //queen
        {

        }
        else if (ChessPieceArray[i][j] == -5) //king
        {

        }
        else if (ChessPieceArray[i][j] == -6) //pawn
        {
            CalculatePawnMove(oldBoardSquare, newBoardSquare, ChessPieceArray[i][j]);
        }
    }
    else {
        //send user error message
    }

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

        window.clear(Color::Black);

        if (event.type == Event::MouseButtonPressed) {
            pos[mouseCount] = Mouse::getPosition(window);
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
