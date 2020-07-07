#pragma once
#include <utility>      // std::pair, std::get
#include <vector>      // std::pair, std::get
using namespace std;

enum Piece {
	P1, R1, N1, B1, Q1, K1,
	P2, R2, N2, B2, Q2, K2, E
};

typedef pair<int, int> pii;
typedef pair<pii, pii> Move;

class GameBoard {
private:
	Piece state[9][9];
	vector<pair< Piece, Move> > history;
	char pieceNames[13] = { 'P', 'R', 'N', 'B', 'Q', 'K','p', 'r', 'n', 'b', 'q', 'k', 'E' };

public:
	GameBoard();
	bool movePiece(int x, int y, int nx, int ny);
	bool addPiece(int x, int y, Piece);
	bool removePiece(int x, int y);
	bool undoMove();

	void init();
	void rotatePlayers();
	void exportData(string);
	void printHistory(ostream&);

	char getPiece(int x, int y, bool& isBlack);

};