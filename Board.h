//Include guard
#ifndef __BOARD_H__
#define __BOARD_H__

//Required includes
#include <string>
#include <stdio.h>
#include "Piece.h"

//Create class board
class Board  {	
	//Public methods	
	public:
		Board(int level, std::string levelFile);
		~Board();
		int getWidth();
		int getHeight();
		int getMaxHeight();
		int dropHeight(Piece *piece, int x);
		int getColumnHeight(int x);
		int getRowWidth(int y);
		char getGrid(int x, int y);
		int place(Piece *piece, int x, int y);
		int unplace(Piece *piece, int x, int y);
		int clearRows();
		friend std::ostream &operator<<(std::ostream &os, const Board &b);

	//Private fields and methods
	private:
		void sanityCheck();
        	void recalcHeights();
		void arrayCopy(int *src, int srcPos, int *dest, int destPos, int length);
		
		int width;
		int height;
        	int *widths;
		int *heights;
		char **grid;
		
		static const int PLACE_OK = 0;
		static const int PLACE_ROW_FILLED = 1;
		static const int PLACE_OUT_BOUNDS = 2;
		static const int PLACE_BAD = 3;
};

#endif