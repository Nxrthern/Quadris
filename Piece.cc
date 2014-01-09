//Required includes
#include "Piece.h"
#include <cstdlib>
#include <iostream>

//Default constructor does nothing
Piece::Piece() {

}

//Default destructor
Piece::~Piece(){
	delete [] body;
	delete [] coords;
	delete [] next;
	delete [] skirt;
}

//Construct piece object given type and level
Piece::Piece(char type, int level) {
	this->type = type;
	this->level = level;
	body = new Point[4]();
	coords = new Point[4]();

	//Set location of block on a 4x4 grid used to easily manipulate its orientation and grid placement
	if(type == 'T'){
		body[0].setLocation(0, 0);
		body[1].setLocation(1, 0);
		body[2].setLocation(1, 1);
		body[3].setLocation(2, 0);
		coords[0].setLocation(0, 0);
		coords[1].setLocation(0, 1);
		coords[2].setLocation(1, 1);
		coords[3].setLocation(0, 2);
	}
	else if(type == 'I'){
		body[0].setLocation(0, 0);
		body[1].setLocation(1, 0);
		body[2].setLocation(2, 0);
		body[3].setLocation(3, 0);
		coords[0].setLocation(0, 0);
		coords[1].setLocation(0, 1);
		coords[2].setLocation(0, 2);
		coords[3].setLocation(0, 3);
	}
	else if(type == 'O'){
		body[0].setLocation(0, 0);
		body[1].setLocation(0, 1);
		body[2].setLocation(1, 0);
		body[3].setLocation(1, 1);
		coords[0].setLocation(0, 0);
		coords[1].setLocation(1, 0);
		coords[2].setLocation(0, 1);
		coords[3].setLocation(1, 1);
	}
	else if(type == 'J'){
		body[0].setLocation(0, 0);
		body[1].setLocation(0, 1);
		body[2].setLocation(1, 1);
		body[3].setLocation(2, 1);
		coords[0].setLocation(0, 0);
		coords[1].setLocation(1, 0);
		coords[2].setLocation(1, 1);
		coords[3].setLocation(1, 2);
	}
	else if(type == 'L'){
		body[0].setLocation(2, 0);
		body[1].setLocation(0, 1);
		body[2].setLocation(1, 1);
		body[3].setLocation(2, 1);
		coords[0].setLocation(0, 2);
		coords[1].setLocation(1, 0);
		coords[2].setLocation(1, 1);
		coords[3].setLocation(1, 2);
	}
	else if(type == 'Z'){
		body[0].setLocation(0, 0);
		body[1].setLocation(1, 0);
		body[2].setLocation(1, 1);
		body[3].setLocation(2, 1);
		coords[0].setLocation(0, 0);
		coords[1].setLocation(0, 1);
		coords[2].setLocation(1, 1);
		coords[3].setLocation(1, 2);
	}
	else if(type == 'S'){
		body[0].setLocation(0, 1);
		body[1].setLocation(1, 1);
		body[2].setLocation(1, 0);
		body[3].setLocation(2, 0);
		coords[0].setLocation(1, 0);
		coords[1].setLocation(1, 1);
		coords[2].setLocation(0, 1);
		coords[3].setLocation(0, 2);
	}

	//Set dimension and skirt of the piece
	this->setPieceDims();
	this->setPieceSkirt();
//t  0 0 1 0 1 1 2 0	
//i  0 0 0 1 0 2 0 3
//o  0 0 0 1 1 0 1 1
//j  0 0 1 0 1 1 1 2
//l  0 0 0 1 0 2 1 0
//s  0 0 1 0 1 1 2 1
//z  0 1 1 1 1 0 2 0
}

//Generates a piece given probabilities and level
Piece* Piece::generatePiece(double *probability, int level, std::string zero[], int currentIndex){
	if(level == 0) 		
		return new Piece(zero[currentIndex].c_str()[0], 0);

	//Set random number
	int rn = (rand()%100)+1;
	//>7	
	//Loop through piece types
	for(int x = 0; x < 7; x++){
		//Subtract probabilities to find the block type upon reaching 0
		rn -= probability[x];

		if(rn <= 0){
			if(x == 0){
				return new Piece('T', level);
			}
			else if(x == 1){
				return new Piece('I', level);
			}
			else if(x == 2){
				return new Piece('O', level);
			}
			else if(x ==3){
				return new Piece('J', level);
			}
			else if(x == 4){
				return new Piece('L', level);
			}
			else if(x == 5){
				return new Piece('S', level);
			}
			else if(x == 6){
				return new Piece('Z', level);
			}//End of index test
		}//End of 0 test
	}//End of type loop

	return new Piece('O', level);
}//End of generatePiece

//Return piece width
int Piece::getWidth()
{
	return(width);
}

//Return piece height
int Piece::getHeight()
{
	return(height);
}

//Return piece points
Point* Piece::getBody()
{
	return(body);
}

//Return piece coords
Point* Piece::getCoords()
{
	return(coords);
}

//Return piece type
char Piece::getType(){
	return type;
}

//Return piece level
int Piece::getLevel(){
	return level;
}

//Return the lowest point of the piece
int* Piece::getSkirt()
{
	return(skirt);
}

//Check if equal
bool Piece::equals(Piece *p)
{
	//Declare and initialize local variables
	int match = 0;
	int* ignore = new int[4];

	//Set values of ignore to 0
	for(unsigned int x = 0; x < sizeof(ignore)/sizeof(ignore[0]); x++)
		ignore[x] = 0;

	//is the object sharing the piece address
	if (p == this)
	{
		return(true);
	}

	//Is the object of different length
       if (sizeof(p->body)/sizeof(p->body[0]) != sizeof(this->body)/sizeof(this->body[0]))
       {
       	return false;
       }

	//Loop to compare points
       for(unsigned int i = 0; i < sizeof(this->body)/sizeof(this->body[0]); i++)
       {
		//Compare point i to each point in j and ignore a certain index if found next run
		for(unsigned int j = 0; j < sizeof(this->body)/sizeof(this->body[0]); j++){
			if(ignore[j] == 0){
				if(p->body[i].getX() == this->body[j].getX() && p->body[i].getY() == this->body[j].getY()){
					match++;
					ignore[j] = 1;
				}//End of match test
			}//End of ignore test
		}//End of j loop
       }//End of i loop

       //Return equality value
      	if(match == 4)
		return true;
	else
		return false;
}//End of equals

Piece *Piece::pieceRow(Piece* root)
{
      Piece* temp = root;
      Piece* prev = root;

      //Self infinite loop until break
      for(;;)
      {
		//This will save all future rotations in memory 
      		prev = temp;
              prev->setPieceDims();
              prev->setPieceSkirt();
              temp = new Piece(*prev);
              temp->rotate();
           
              if(!temp->equals(root))
                {
                    prev->next = temp;
                }
                else
                {
                    prev->next = root;
                    break;
                }
      }//End of loop

	//Return original piece
       return root;
}//End of piece rows

//Rotates a piece 
void Piece::rotate()
{
		//Declare new point array
            Point* temp = new Point[4];
	
            // switch x,y to y,x
            for(unsigned int i = 0; i < 4; i++)
            {
		temp[i].setLocation(body[i].getY(), body[i].getX());
            }

		//Delete old coordinates and set new to current piece	
	    delete [] body;
	    body = temp;
            setPieceDims();
	    temp = new Point[4];

		//attempts to center a piece
            for(unsigned int i = 0; i < 4; i++)
            {
		temp[i].setLocation((width-1) - body[i].getX(), body[i].getY());
            }
            delete [] body;
	    body = temp;
}//End of rotate

//Reverse rotate simply rotates 3 times in order to appear reversing once the other direction
void Piece::reverseRotate(){
	rotate();
	rotate();
	rotate();
}

//Sets width and height of piece
void Piece::setPieceDims()
{
            int wmax = -1;
            int hmax = -1;
            for(unsigned int i = 0; i < sizeof(body)/sizeof(body[0]); i++){
                if(body[i].getX() > wmax) wmax = body[i].getX();
                if(body[i].getY() > hmax) hmax = body[i].getY();
            }
            width = wmax+1;
            height = hmax+1;
}

//Sets the skirt of a piece
void Piece::setPieceSkirt()
{
            int wmax = width;
            int hmax;

            skirt = new int[wmax];

            for(int i = 0; i < wmax; i++)
            {
                Point *temp = NULL;
                hmax = 10000;
                for(unsigned int j = 0; j < 4; j++)
                {
                    if(body[j].getX() == i)
                    {
                        if(body[j].getY() < hmax)
                        {
                            hmax = body[j].getY();
                            temp = new Point(body[j]);
                        }
                    }
                }
                skirt[i] = temp->getY();
            }
}

//Overloaded operator << to print a piece
std::ostream& operator<<(std::ostream &os, const Piece &p) {
	os << std::endl;
	
	//Determine the piece to be printed and return the output
	if(p.type == 'T'){
		os << "TTT" << std::endl << " T ";
	}
	else if(p.type == 'I'){
		os << "IIII";
	}
	else if(p.type == 'O'){
		os << "OO " << std::endl << "OO ";
	}
	else if(p.type == 'J'){
		os << "J  " << std::endl << "JJJ";
	}
	else if(p.type == 'L'){
		os << "  L" << std::endl << "LLL";
	}
	else if(p.type == 'Z'){
		os << "ZZ " << std::endl << " ZZ";
	}
	else if(p.type == 'S'){
		os << " SS" << std::endl << "SS ";
	}

	return os;
}//End of operator<<