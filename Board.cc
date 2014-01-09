//Required includes
#include "Board.h"
#include "Point.cc"
#include <algorithm>
#include <iterator>
#include <iostream>

//Board constructor
Board::Board(int level, std::string levelFile) {
	//Initialize key variables in board
	width = 10;
	height = 15;
	grid = new char*[width];

	//Loop to initialize char array with spaces
	for(int i = 0; i < width; i++) {
		grid[i] = new char[height];
		for(int j = 0; j < height; j++) {
			grid[i][j] = ' ';
		}
	}
	
	//Initialize the width and height trackers
	widths = new int[height];
	heights = new int[width];
}//End of constructor

Board::~Board() {
	delete[] widths;
	delete[] heights;
}

//Returns width of grid
int Board::getWidth() {
	return width;
}
	
//Returns height of grid
int Board::getHeight() {
	return height;
}

//Returns max height of columns
int Board::getMaxHeight() {
	int max = 0;
	for(int i = 0; i < width; i++) {
		if (heights[i] > max) 
			max = heights[i];
	}
	return max;
}

//Test function used to determine if reachable
void Board::sanityCheck() {
}

//Returns height to the nearest block
int Board::dropHeight(Piece *piece, int x) {
	//Declare and initialize local variables
	int high = height;
	int result = 0;
	int temp = 0;
	int *skirt = piece->getSkirt();

	//Loop to find height of column
	for(int i = x; i < x + piece->getWidth(); i++) {
		temp = skirt[i-x]-heights[i];
		if(temp < high) {
			high = temp;
			result = i;
		}
	}

	//Return heightest point - lowest point distance for column x
	return heights[result]-skirt[result-x];
}//End of drop height

//Returns the height of a given column x
int Board::getColumnHeight(int x) {
	return heights[x];
}

//Returns the width of a given row y
int Board::getRowWidth(int y) {
	return widths[y];
}

//Returns the char at a given x, y coordinate
char Board::getGrid(int x, int y) {
	if(x<0 || x>width) return ' ';
	if(y<0 || y>height) return ' ';
		return grid[x][y];
}

//Places a piece on the grid of chars	
int Board::place(Piece *piece, int x, int y) {
	//Gets the body of given piece
	Point body[4] = {piece->getBody()[0], piece->getBody()[1], piece->getBody()[2], piece->getBody()[3]};

	//Use local variables for position on grid
	int putx;
	int puty;
	bool row_filled = false;
          
	//Loop for determining if pieces will be placed properly on the grid
	for(int i = 0; i < 4; i++) {
           putx = x+body[i].getX();
           puty = y+body[i].getY();
		
           if(putx<0 || putx>=width) return PLACE_OUT_BOUNDS;
           if(puty<0 || puty>=height) return PLACE_OUT_BOUNDS;
           if(grid[putx][puty] != ' ') return PLACE_BAD;
       }
	
	//Loop to place each type block of piece to the grid and increment vars, check for filled row
       for(int i = 0; i < 4; i++) {
		putx = x+body[i].getX();
		puty = y+body[i].getY();
		grid[putx][puty] = piece->getType();
		widths[puty]++;
		if(puty+1 > heights[putx]) 
		heights[putx] = puty+1;
                
		if(widths[puty] >= width) row_filled = true;
        }

	//TEST
	#ifdef DEGUG
		sanityCheck();
	#endif
            
	//Determine if row is filled or just placed properly
	if(row_filled) return PLACE_ROW_FILLED;
	return PLACE_OK;
}//End of place

//Removes the old block indicators from the grid
int Board::unplace(Piece *piece, int x, int y) {
	//Gets the body of piece
	Point body[4] = {piece->getBody()[0], piece->getBody()[1], piece->getBody()[2], piece->getBody()[3]};

	int putx;
	int puty;
	bool row_filled = false;

	//Loops to put spaces in all the spots where piece is currently and decrement vars
       for(int i = 0; i < 4; i++) {
		putx = x+body[i].getX();
		puty = y+body[i].getY();
		grid[putx][puty] = ' ';
		widths[puty]--;
		if(puty+1 > heights[putx]) 
		heights[putx] = puty-1;
                
		if(widths[puty] >= width) row_filled = true;
        }

	//TEST
	#ifdef DEGUG
		sanityCheck();
	#endif
            
	return PLACE_OK;
}//End of unplace

//Clears filled rows and return number of rows cleared
int Board::clearRows() {
	int cleared = 0;
	
       
	//THIS NEEDS WORK     
	for(int i = height-1; i > 0; i--) {
		if(widths[i] >= width) {
			cleared++;

			for(int j = 0; j < width; j++) {
				for(int k = i; k > 0; k--) {
					grid[j][k] = grid[j][k-1];
				}
				grid[j][0] = ' ';
			}
			for(int k = i; k > 0; k--) {
				widths[k] = widths[k-1];
			}
			widths[0] = ' ';
 			i--;
		}
	}

	if(cleared > 0) recalcHeights();
	#ifdef DEGUG
		sanityCheck();
	#endif
	return cleared;
}//End of clear rows
            
//Recalc heights, will update the array of heights of columns
void Board::recalcHeights() {
	int j;
	bool sent = false;
	
	//Loop for width of grid
	for(int i = 0; i < width; i++) {
		sent = false;

		//Loop for height of grid
		for(j = height-1; j >=0 ; j--) {
			//Check if grid has something at this index and increment the height
			if(grid[i][j] && !sent) {
				heights[i] = j+1;
				sent = true;
			}
		}
		if(!sent) heights[i] = 0;      
	}        
}//End of recalc heights

//Overloaded output operator
std::ostream& operator<<(std::ostream &os, const Board &b) {
	os << std::endl;
	
	//Loop to add each index of grid to os
	for(int i = 0; i < b.height; i++) {
		for(int j = 0; j < b.width; j++) {
			os << b.grid[j][i];	
		}
		os << std::endl;
	}
	
	//Loop to add the bottom of the grid 10 dashes
	for(int i = 0; i < b.width; i++) os << "-";
	os << std::endl;
	
	//Return os
	return os;
}//End of operator <<