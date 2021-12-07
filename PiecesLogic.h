#include "common.hpp"
#include <vector>

using namespace::sf;

class Pieces {

private:



public:

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



}; 

