//Required includes
#include "Board.h"
#include "Piece.h"
//#include "window.h"

#include <string>
#include <sstream>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <fstream>

//Set namespace
using namespace std;

//Declare and initialize main vars
string seed;
bool useGUI = true;
string scriptFile = "sequence.txt";
int startLevel = 0;
string zero[100];
int arraySize = 0;

//MAIN
int main(int argc, char *argv[]) {
	//Create string stream to use for seed for rand
	std::stringstream sstr;
	sstr << time(NULL);
	seed = sstr.str();

	//Interpret command line args
	for(int nArg = 1; nArg < argc; nArg++) {
		if(strcmp(argv[nArg], "-text"))
			useGUI = false;

		if(strcmp(argv[nArg], "-seed")) { 
			nArg++;
			seed = argv[nArg];
		}	

		if(strcmp(argv[nArg], "-scriptfile")) {
			nArg++;
			scriptFile = argv[nArg];
		}
	
		if(strcmp(argv[nArg], "-startlevel")) {
			nArg++;
			startLevel = atoi(argv[nArg]);
		}
	}

	ifstream file(scriptFile.c_str());
    	if(file.is_open())
    	{
		while(!file.eof()) {
        	  	file >> zero[arraySize];
			arraySize++;
        	}
    	}

	
	
	//Set seed generator
	srand(atoi(seed.c_str()));
	
	//Create board and probabilities, set pieces for current and next and place current
	Board *board = new Board(startLevel, scriptFile);
	
	//Setup level probabilities
	//NOTE: array is: T I O J I S Z
	double p[5][7] = { 	{1.0/7*100, 1.0/7*100, 1.0/7*100, 1.0/7*100, 1.0/7*100, 1.0/7*100, 1.0/7*100},
			     	{1.0/6*100, 1.0/6*100, 1.0/6*100, 1.0/6*100, 1.0/6*100, 1.0/12*100, 1.0/12*100},	
				{1.0/7*100, 1.0/7*100, 1.0/7*100, 1.0/7*100, 1.0/7*100, 1.0/7*100, 1.0/7*100},
				{1.0/9*100, 1.0/9*100, 1.0/9*100, 1.0/9*100, 1.0/9*100, 2.0/9*100, 2.0/9*100},
				{0, 100, 0, 0, 0, 0, 0},
			     };

	Piece *currentPiece = NULL;
	Piece *nextPiece = NULL;
	currentPiece = currentPiece->generatePiece(p[startLevel], startLevel, zero, 0%arraySize);
	nextPiece = nextPiece->generatePiece(p[startLevel], startLevel, zero, 1%arraySize);

	int xPos = 0, yPos = 0;
	int usedPieces = 0;
	int score = 0;
	int highScore = 0;
	int success = board->place(currentPiece, xPos, yPos);
	Piece *oldPieces[100];
	int currentLevel = startLevel;
	string command;
	bool clear = false;

	//Declare command strings
	string left = "left";
	string right = "right";
	string down = "down";
	string clockwise = "clockwise";
	string counterclockwise = "counterclockwise";
	string drop = "drop";
	string levelup = "levelup";
	string leveldown = "leveldown";
	string restart = "restart";
	string rename = "rename";

	//Loop for main function of the program
	while(true) {	
		//Print score, board, and next piece
		cout << "Level:    " << currentLevel << endl;
		cout << "Score:    " << score << endl;
		cout << "Hi Score: " << highScore << endl;
		cout << "----------" << endl << endl;
		cout << *board;
		cout << "Next:" << *nextPiece << endl;
		
		//Read in a command and interpret, will determine if input is substr of a command
		cin >> command;
		int multiply = 1;
		if (command.c_str()[0] >= '0' && command.c_str()[0] <= '9') multiply = atoi(command.c_str());

		while(command.c_str()[0] >= '0' && command.c_str()[0] <= '9') {
			command = command.substr(1);
		}

		int count = 0;
		if (!strncmp(left.c_str(), command.c_str(), strlen(command.c_str()))) count++;
		if (!strncmp(right.c_str(), command.c_str(), strlen(command.c_str()))) count++;
		if (!strncmp(down.c_str(), command.c_str(), strlen(command.c_str())))  count++;
		if (!strncmp(clockwise.c_str(), command.c_str(), strlen(command.c_str())))  count++;
		if (!strncmp(counterclockwise.c_str(), command.c_str(), strlen(command.c_str())))  count++;
		if (!strncmp(drop.c_str(), command.c_str(), strlen(command.c_str())))  count++;
		if (!strncmp(levelup.c_str(), command.c_str(), strlen(command.c_str())))  count++;
		if (!strncmp(leveldown.c_str(), command.c_str(), strlen(command.c_str())))  count++;
		if (!strncmp(restart.c_str(), command.c_str(), strlen(command.c_str())))  count++;
		if (!strncmp(rename.c_str(), command.c_str(), strlen(command.c_str())))  count++;

		if(count != 1) continue;
		success = 0;
		
		//Loop to perform action of command
		for(int i = 0; i < multiply; i++) {
			if(cin.eof()) {
				//Free memory on exit and break
				delete board;

				for(int i = 0; i < usedPieces; i++) {
						delete[] p[i];
				}

				for(int i = 0; i < usedPieces; i++) {
					delete oldPieces[i];
				}

				delete currentPiece;
				delete nextPiece;
				break;
			} else if (!strncmp(left.c_str(), command.c_str(), strlen(command.c_str()))) {
				//Unplace old position and replace new
				board->unplace(currentPiece, xPos, yPos);
				xPos--;
				success = board->place(currentPiece, xPos, yPos);
				
				for(int i = 0; i < 4; i++)
					currentPiece->getCoords()[i].setLocation(currentPiece->getCoords()[i].getX(), currentPiece->getCoords()[i].getY()-1);

				//If unsuccessful placement then add back the original placement
				if(success != 0 && success != 1) {
					xPos++;
					board->place(currentPiece, xPos, yPos);
					for(int i = 0; i < 4; i++)
						currentPiece->getCoords()[i].setLocation(currentPiece->getCoords()[i].getX(), currentPiece->getCoords()[i].getY()+1);
				}
			} else if (!strncmp(right.c_str(), command.c_str(), strlen(command.c_str()))) {
				//Unplace old position and replace new
				board->unplace(currentPiece, xPos, yPos);
				xPos++;
				success = board->place(currentPiece, xPos, yPos);
				cout << xPos << ", " << yPos << ";" << success << endl;
		
				for(int i = 0; i < 4; i++)
					currentPiece->getCoords()[i].setLocation(currentPiece->getCoords()[i].getX(), currentPiece->getCoords()[i].getY()+1);

				//If unsuccessful placement then add back the original placement
				if(success != 0 && success != 1) {
					xPos--;
					board->place(currentPiece, xPos, yPos);

					for(int i = 0; i < 4; i++)
						currentPiece->getCoords()[i].setLocation(currentPiece->getCoords()[i].getX(), currentPiece->getCoords()[i].getY()-1);

				}
			} else if (!strncmp(down.c_str(), command.c_str(), strlen(command.c_str()))) {
				//Unplace old position and replace new
				board->unplace(currentPiece, xPos, yPos);
				yPos++;
				success = board->place(currentPiece, xPos, yPos);

				for(int i = 0; i < 4; i++)
					currentPiece->getCoords()[i].setLocation(currentPiece->getCoords()[i].getX()+1, currentPiece->getCoords()[i].getY());

				//If unsuccessful placement then add back the original placement
				if(success != 0 && success != 1) {
					yPos--;
					board->place(currentPiece, xPos, yPos);
		
					for(int i = 0; i < 4; i++)
						currentPiece->getCoords()[i].setLocation(currentPiece->getCoords()[i].getX()-1, currentPiece->getCoords()[i].getY());

				}
			} else if (!strncmp(clockwise.c_str(), command.c_str(), strlen(command.c_str()))) { 

				//Unplace old position and replace new
				board->unplace(currentPiece, xPos, yPos);
				currentPiece->rotate();

				success = board->place(currentPiece, xPos, yPos);
				
				//If successful rotate then change coordinates otherwise dont change them
				if(success == 0){
					for(int i = 0; i < 4; i++)
						currentPiece->getCoords()[i].setLocation(yPos + currentPiece->getBody()[i].getY(), xPos + currentPiece->getBody()[i].getX());
				}
	
				//If unsuccessful placement then add back the original placement
				if(success > 1) {
					currentPiece->reverseRotate();
					success = board->place(currentPiece, xPos, yPos);

				}
			} else if (!strncmp(counterclockwise.c_str(), command.c_str(), strlen(command.c_str()))) {
				//Unplace old position and replace new
				board->unplace(currentPiece, xPos, yPos);
				currentPiece->reverseRotate();

				success = board->place(currentPiece, xPos, yPos);
				
				//If successful rotate then change coordinates otherwise dont change them
				if(success == 0){
					for(int i = 0; i < 4; i++)
						currentPiece->getCoords()[i].setLocation(yPos + currentPiece->getBody()[i].getY(), xPos + currentPiece->getBody()[i].getX());
				}
	
				//If unsuccessful placement then add back the original placement
				if(success > 1) {
					currentPiece->rotate();
					success = board->place(currentPiece, xPos, yPos);

				}
			} else if (!strncmp(drop.c_str(), command.c_str(), strlen(command.c_str()))) {
				//Reset success in preparation for drop
				success = 0;
				clear = false;

				//If flag for successful drop or row cleared then continue to attempt moving down
				while(success == 0 || success == 1) {
					//Unplace old position and replace new
					board->unplace(currentPiece, xPos, yPos);
					yPos++;
					success = board->place(currentPiece, xPos, yPos);

					if(success == 1) clear = true;

					for(int i = 0; i < 4; i++)
						currentPiece->getCoords()[i].setLocation(currentPiece->getCoords()[i].getX()+1, currentPiece->getCoords()[i].getY());

			
					//If unsuccessful placement then add back the original placement
					if(success != 0 && success != 1) {
						yPos--;
						board->place(currentPiece, xPos, yPos);

						for(int i = 0; i < 4; i++)
							currentPiece->getCoords()[i].setLocation(currentPiece->getCoords()[i].getX()-1, currentPiece->getCoords()[i].getY());

					}
				}				

				//Add current piece to old and increment used pieces
				oldPieces[usedPieces] = currentPiece;
				usedPieces++;

				//If row cleared flag then clear full rows
				if(true) {
					//Loop to go through every line and test if full then every used piece to set match blocks in that row to coord -1, -1
					for(int x = 0; x < board->getHeight();  x++){
						//Test for full row
						if(board->getRowWidth(x) >= board->getWidth()){
							for(int y = 0; y < usedPieces; y++){
								//Loop the bodies of used pieces
								for(int z = 0; z < 4; z++){
									//Make sure pieces haven't already been cleared
									if(oldPieces[y]->getCoords()[z].getX() != -1 && oldPieces[y]->getCoords()[z].getX() != -2){
										//Increment values above full line otherwise clear line
										if(oldPieces[y]->getCoords()[z].getX() < x){
											oldPieces[y]->getCoords()[z].setLocation(oldPieces[y]->getCoords()[z].getX()+1, oldPieces[y]->getCoords()[z].getY());
										}
										else if(oldPieces[y]->getCoords()[z].getX() == x){
											oldPieces[y]->getCoords()[z].setLocation(-1, -1);
										}
									}
								}//End of coords loop
							}//End of used pieces loop
						}//End of full line test
					}//End of height loop
							
					//Get number of rows cleared
					int rowsCleared = board->clearRows();
		
					//Add score for rows cleared
					if(rowsCleared > 0){
						score += ((rowsCleared + currentLevel)*(rowsCleared + currentLevel));
					}
					
					int counter = 0;

					//Loop to find if a piece has been fully cleared
					for(int x = 0; x < usedPieces; x++){
						counter = 0;
					
						//Loop to test if all coords are -1 -1 flag
						for(int y = 0; y < 4; y++){
							if(oldPieces[x]->getCoords()[y].getX() == -1 && oldPieces[x]->getCoords()[y].getY() == -1){
								counter++;
							}

							//If all 4 blocks have been cleared then add to score and remove the piece
							if(counter == 4){
								score += ((oldPieces[x]->getLevel()+1)*(oldPieces[x]->getLevel()+1));
								
								for(int z = 0; z < 4; z++)
									oldPieces[x]->getCoords()[z].setLocation(-2, -2);
							}//End of cleared block test
						}//End of coord loop
					}//End of used pieces loop			
 				}//End of row cleared test
			
				//Reset x and y position for next block
				xPos = 0;
				yPos = 0;
				
				//Set current to new and generate a next piece at the top
				currentPiece = nextPiece;
				nextPiece = nextPiece->generatePiece(p[currentLevel], currentLevel, zero, (usedPieces+1)%arraySize);
				success = board->place(currentPiece, xPos, yPos);

	
				//Restart game and save score
				if(success != 0 && success != 1) {
					//Clear board and current game trackers and make new game
					delete board;

					for(int i = 0; i < usedPieces; i++) {
						delete oldPieces[i];
					}
					delete currentPiece;
					delete nextPiece;

					if(score > highScore)
						highScore = score;

					score = 0;
					currentLevel = 0;
					usedPieces = 0;
					xPos = 0;
					yPos = 0;

					board = new Board(currentLevel, scriptFile);
					currentPiece = currentPiece->generatePiece(p[currentLevel], currentLevel, zero, 0%arraySize);
					nextPiece = nextPiece->generatePiece(p[currentLevel], currentLevel, zero, 1%arraySize);
					success = board->place(currentPiece, xPos, yPos);
				}
			} else if (!strncmp(levelup.c_str(), command.c_str(), strlen(command.c_str()))) {
				currentLevel++;
			} else if (!strncmp(leveldown.c_str(), command.c_str(), strlen(command.c_str()))) {
				currentLevel--;
				if(currentLevel < 0) currentLevel = 0;
			}  else if (!strncmp(restart.c_str(), command.c_str(), strlen(command.c_str()))) {
				//Clear board and current game trackers and make new game
				delete board;

				for(int i = 0; i < usedPieces; i++) {
					delete oldPieces[i];
				}

				cout << " I HAVEN'T SEG FAULTED YET" << endl;
				delete currentPiece;
				delete nextPiece;

				if(score > highScore)
					highScore = score;

				score = 0;
				currentLevel = 0;
				usedPieces = 0;
				xPos = 0;
				yPos = 0;

				board = new Board(currentLevel, scriptFile);
				currentPiece = currentPiece->generatePiece(p[currentLevel], currentLevel, zero, 0%arraySize);
				nextPiece = nextPiece->generatePiece(p[currentLevel], currentLevel, zero, 1%arraySize);
				success = board->place(currentPiece, xPos, yPos);
			} else if (!strncmp(rename.c_str(), command.c_str(), strlen(command.c_str()))) {
				//Ability to rename commands here even though unnecessary
				string oldCmd, newCmd;
				cin >> oldCmd >> newCmd;	

				if(oldCmd == left)              left = newCmd;
				if(oldCmd == right)             right = newCmd;
				if(oldCmd == down)              down = newCmd;
				if(oldCmd == clockwise)         clockwise = newCmd;
				if(oldCmd == counterclockwise)  counterclockwise = newCmd;
				if(oldCmd == levelup)           levelup = newCmd;
				if(oldCmd == leveldown)         leveldown = newCmd;
				if(oldCmd == restart)           restart = newCmd;
				if(oldCmd == rename)            rename = newCmd;
			}//End of command test
		}//End of action loop
	}//End of game loop

	return 0;
}//END OF MAIN