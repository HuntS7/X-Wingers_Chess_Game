//include classes and headers
#include <iostream>
#include <string>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class BoardPiece {
private:
    sf::Texture pieceTexture;

    sf::RectangleShape piece;

    int pieceType; // 0 = rook, 1 = knight, 2 = bishop, 3 = king, 4 = queen, 5 = pawn, 6 = light tile, 7 = dark tile.
public:
    BoardPiece(sf::Vector2f, sf::Vector2f, sf::Color, int);

    sf::RectangleShape& getShape();
    int getPieceType();

    void setPieceType(int);
};

BoardPiece::BoardPiece(sf::Vector2f size, sf::Vector2f position, sf::Color color, int pType) {
    piece.setSize(size);
    piece.setPosition(position);
    piece.setFillColor(color);

    pieceType = pType;
}

sf::RectangleShape& BoardPiece::getShape() {
    return piece;
}

class Game {
private:
    // Init general variables.
    int pieceSize;

    // Init white piece variables.
    std::vector<int> whitePieceOrder;
    std::vector<std::string> whitePieceTextureNames;
    std::vector<sf::Texture> whitePieceTextures;
    std::vector<BoardPiece> whitePieces;

    // Init black piece variables.
    std::vector<int> blackPieceOrder;
    std::vector<std::string> blackPieceTextureNames;
    std::vector<sf::Texture> blackPieceTextures;
    std::vector<BoardPiece> blackPieces;

    // Init board piece variables.
    std::vector<std::string> boardTileTextureNames;
    std::vector<sf::Texture> boardTileTextures;
    std::vector<BoardPiece> boardTiles;

    // Init window.
    sf::RenderWindow window;

public:

    Game();

    void initWhitePieces();
    void initBlackPieces();
    void initBoardPieces();

    void loop();

    //void movePiece(int, int, int); // X, Y, piece to move.

};

Game::Game() {
    // Init general variables.
    pieceSize = 64;

    // Init white piece variables.
    whitePieceOrder = {0, 1, 2, 3, 4, 2, 1, 0, 5};
    whitePieceTextureNames = {
        "w_rook_png_128px.png", "w_knight_png_128px.png",
        "w_bishop_png_128px.png", "w_king_png_128px.png",
        "w_queen_png_128px.png", "w_bishop_png_128px.png",
        "w_knight_png_128px.png", "w_rook_png_128px.png",
        "w_pawn_png_128px.png"
    };
    whitePieceTextures.resize(9);

    // Init black piece variables.
    blackPieceOrder = {0, 1, 2, 3, 4, 2, 1, 0, 5};
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
        "square brown light_png_128px.png",
        "square brown dark_png_128px.png"
    };
    boardTileTextures.resize(2);

    // Init window.
    window.create(sf::VideoMode(1280 , 720), "SFML works!"/*, sf::Style::Fullscreen*/);
}

void Game::initWhitePieces() {
    // Create white piece textures.
    for(int i = 0; i < whitePieceTextureNames.size(); i++) {
        if(!whitePieceTextures[i].loadFromFile("images/" + whitePieceTextureNames[i])) {
            std::cout << "Error getting " << whitePieceTextureNames[i] << ". Shutting down." << std::endl;
            //return 1;
        }
    }

    // Create white Pieces.
    for(int i = 0; i < whitePieceOrder.size(); i++) {
        if(whitePieceOrder[i] == 5) {
            // Create white pawns.
            for(int j = 0; j < 8; j++) {
                whitePieces.push_back(BoardPiece(sf::Vector2f(pieceSize, pieceSize), sf::Vector2f(pieceSize * j, pieceSize), sf::Color::White, whitePieceOrder[i]));
                whitePieces[i + j].getShape().setTexture(&whitePieceTextures[i]);
            }
        } else {
            // Create all other white pieces.
            whitePieces.push_back(BoardPiece(sf::Vector2f(pieceSize, pieceSize), sf::Vector2f(pieceSize * i, 0), sf::Color::White, whitePieceOrder[i]));
            whitePieces[i].getShape().setTexture(&whitePieceTextures[i]);
        }
    }
}

void Game::initBlackPieces() {
    // Create black piece textures.
    for(int i = 0; i < blackPieceTextureNames.size(); i++) {
        if(!blackPieceTextures[i].loadFromFile("images/" + blackPieceTextureNames[i])) {
            std::cout << "Error getting " << blackPieceTextureNames[i] << ". Shutting down." << std::endl;
            //return 1;
        }
    }

    // Create black Pieces.
    for(int i = 0; i < blackPieceOrder.size(); i++) {
        if(blackPieceOrder[i] == 5) {
            // Create white pawns.
            for(int j = 0; j < 8; j++) {
                blackPieces.push_back(BoardPiece(sf::Vector2f(pieceSize, pieceSize), sf::Vector2f(pieceSize * j, pieceSize * 6), sf::Color::White, blackPieceOrder[i]));
                blackPieces[i + j].getShape().setTexture(&blackPieceTextures[i]);
            }
        } else {
            // Create all other white pieces.
            blackPieces.push_back(BoardPiece(sf::Vector2f(pieceSize, pieceSize), sf::Vector2f(pieceSize * i, pieceSize * 7), sf::Color::White, blackPieceOrder[i]));
            blackPieces[i].getShape().setTexture(&blackPieceTextures[i]);
        }
    }
}

void Game::initBoardPieces() {
    int startingColor = 0; // 0 = light tile, 1 = dark tile
    int currentColor = startingColor;

    // Create board piece textures.
    for(int i = 0; i < boardTileTextureNames.size(); i++) {
        if(!boardTileTextures[i].loadFromFile("images/" + boardTileTextureNames[i])) {
            std::cout << "Error getting " << boardTileTextureNames[i] << ". Shutting down." << std::endl;
            //return 1;
        }
    }

    // Create board pieces.
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(currentColor == 0) {
                boardTiles.push_back(BoardPiece(sf::Vector2f(pieceSize, pieceSize), sf::Vector2f(pieceSize * j, pieceSize * i), sf::Color::White, 6));
                boardTiles[(i * 8) + j].getShape().setTexture(&boardTileTextures[0]);
                currentColor++;
            } else {
                boardTiles.push_back(BoardPiece(sf::Vector2f(pieceSize, pieceSize), sf::Vector2f(pieceSize * j, pieceSize * i), sf::Color::White, 7));
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

void Game::loop() {
    // SFML game Loop.
    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);
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

    gm.initWhitePieces();
    gm.initBlackPieces();
    gm.initBoardPieces();
    gm.loop();

    // Exit program.
    return 0;
}
