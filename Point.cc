//Include guard
#ifndef __POINT__
#define __POINT__

//Point class
class Point{
	//Declare private fields
	int x, y;

	public:
		//Constuctor given point p
		Point(Point &p){
			x = p.x;
			y = p.y;
		}
		
		//Default destructor
		~Point(){
		}
		
		//Default constructor
		Point() {
			this->x = 0;
			this->y = 0;
		}
	
		//Constructor given x, y
		Point(int x, int y){
			this->x = x;
			this->y = y;
		}
		
		//X accessor
		int getX(){
			return x;
		}
		
		//Y accessor
		int getY(){
			return y;
		}
		
		//Set point location given coordinates
		void setLocation(int x, int y){
			this->x = x;
			this->y = y;
		}
		
		//Return true for point obj..wtf
		bool equals(Point *obj) {
			return true;
   		}
};

#endif
