//Include guard
#ifndef __PIECE_H__
#define __PIECE_H__

//Required includes
#include <string>
#include <stdio.h>
#include "Point.cc"

//Piece class
class Piece {
	//Delcare private variables
	Point* body;
	Point* coords;
	int* skirt;
	int width;
	int height;
	Piece *next;
	char type;
	int level;
	
	//Dclare private methods
	Piece *pieceRow(Piece *root);
	void setPieceDims();
	void setPieceSkirt();
	

	public:
		//Declare methods
		Piece(char type, int level);
		~Piece();
		Piece();
		int getWidth();
		char getType();
		int getLevel();
		Point* getCoords();
		Piece *nextRotation();
		int getHeight();
		Point* getBody();
		int* getSkirt();
		Piece* generatePiece(double *probability, int level, std::string zero[], int currentIndex);
		bool equals(Piece *obj);
		Piece* getPieces();
		void rotate();
		void reverseRotate();
		friend std::ostream &operator<<(std::ostream &os, const Piece &p);
};

#endif