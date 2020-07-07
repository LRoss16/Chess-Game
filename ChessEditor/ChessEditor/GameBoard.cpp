#include "GameBoard.h"
#include <fstream>
#include <iostream>

bool GameBoard::movePiece(int x, int y, int nx, int ny) {
	if (state[nx][ny] == E && state[x][y] != E) {
		Move m = make_pair(make_pair(x, y), make_pair(nx, ny));
		history.push_back(make_pair(state[x][y], m));

		state[nx][ny] = state[x][y];
		state[x][y] = E;
		return true;
	}
	return false;
}

bool GameBoard::addPiece(int x, int y, Piece p) {
	if (state[x][y] == E) {
		state[x][y] = p;

		Move m = make_pair(make_pair(-1, 0), make_pair(x, y));
		history.push_back(make_pair(p, m));

		return true;
	}
	return false;
}

bool GameBoard::removePiece(int x, int y) {
	if (state[x][y] == E) return false;
	else {
		Move m = make_pair(make_pair(x, y), make_pair(-1, 0));
		history.push_back(make_pair(state[x][y], m));

		state[x][y] = E;
		return true;
	}
}

GameBoard::GameBoard() {
	for (int i = 1; i <= 8; i++)
		for (int j = 1; j <= 8; j++)
			state[i][j] = E;
}

void GameBoard::init() {
	for (int i = 1; i <= 8; i++) {
		state[i][2] = P2;
		state[i][7] = P1;
	}
	state[1][1] = state[8][1] = R2;
	state[1][8] = state[8][8] = R1;

	state[2][1] = state[7][1] = N2;
	state[2][8] = state[7][8] = N1;

	state[3][1] = state[6][1] = B2;
	state[3][8] = state[6][8] = B1;

	state[4][1] = Q2; state[5][1] = K2;
	state[4][8] = Q1; state[5][8] = K1;
}

void GameBoard::rotatePlayers() {
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 8; j++)
			swap(state[i][j], state[8 - i + 1][8 - j + 1]);

	for (auto& data : history) {
		Move& m = data.second;
		m.first.first = 8 - m.first.first + 1;
		m.first.second = 8 - m.first.second + 1;
		m.second.first = 8 - m.second.first + 1;
		m.second.second = 8 - m.second.second + 1;
	}
}

char GameBoard::getPiece(int x, int y, bool& isBlack) {
	if (state[x][y] < 6) isBlack = false;
	else isBlack = true;
	return pieceNames[(int)state[x][y]];
}

bool GameBoard::undoMove() {
	if (!history.size()) return false;

	auto data = history.back();
	history.pop_back();
	Piece p = data.first;
	Move m = data.second;

	int x = m.second.first;
	int y = m.second.second;

	int nx = m.first.first;
	int ny = m.first.second;

	if (x == -1) {
		state[nx][ny] = p;
		return true;
	}
	else if (nx == -1) {
		state[x][y] = E;
		return true;
	}
	else if (state[nx][ny] == E) {
		state[nx][ny] = state[x][y];
		state[x][y] = E;
		return true;
	}
	return false;
}

void GameBoard::exportData(string file) {
	ofstream out;
	if (file == "") file = "graph.txt";
	out.open(file, ofstream::out | ofstream::trunc);
	if (out.is_open()) {
		out << "Uppercase = White\tLowercase = Black\n";
		out << "Final State of the Board\n";

		for (int i = 1; i <= 8; i++) {
			for (int j = 1; j <= 8; j++) {
				bool isBlack = false;
				char p = getPiece(i, j, isBlack);
				out << p << ' ';
			}
			out << "\n";
		}

		printHistory(out);

		out.close();
		cout << "File exported successfully\n";
	}
	else {
		//error couldnt open the file
		cout << "Cant open the file\n";
	}


}

void GameBoard::printHistory(ostream& out) {
	out << "History of Moves\n";
	for (const auto data : history) {
		Piece p = data.first;
		Move m = data.second;

		out << pieceNames[p] << "(" << m.first.first << "," << m.first.second << ") -> (" << m.second.first << "," << m.second.second << ")\n";
	}
	out << "Uppercase = White\tLowercase = Black\n\n";
}