#include <iostream>
#include <cstdlib>  // atoi
#include <stack>
#include <climits>
#include "Maze.h"
#include "MazeDefinitions.h"
#include "PathFinder.h"
using namespace std;

const int SIZE = 16;
/////////////
//Floodfill//
/////////////

const int N = 1;
const int E = 2;
const int S = 3;
const int W = 0;

const char toCenter = 'c';
const char toStart = 's';
char dest = toCenter;

struct wall{
	bool north;
	bool south;
	bool east;
	bool west;
};

//upper left corner = (0,0)
struct coord{
	coord(unsigned sx, unsigned sy){
		x = sx;
		y = sy;
	}
	unsigned x;
	unsigned y;
};

int orient = 0;
unsigned char dist[16][16];
wall walls[SIZE][SIZE];
//unsigned char path[256];
bool centerFlag = 0;

void initDistToCenter(){
	for(int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			if(i <= 7 && j <= 7)
				dist[i][j] = 14 - j - i;
			else if(i <= 7)
				dist[i][j] = j - i - 1;
			else if(j <= 7)
				dist[i][j] = i - j - 1;
			else
				dist[i][j] = i + j - 16;
			//cout<< (int)dist[i][j] << ' ';
		}
		//cout << endl;
	}
}

void initDistToStart(){
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
           dist[i][j] = -i + j +15;
        }
    }
}

//void setBit(int a[], int k)
void printArray(unsigned char dist[][SIZE]){
	for(int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			cout << (int)dist[i][j]<<'\t';
		}
		cout << endl;
	}
}

class Floodfill : public PathFinder {
public:
	Floodfill(bool shouldPause = false) : pause(shouldPause) {
		orient = N;
		shouldGoForward = false;
		visitedStart = false;
		//initialize dist 2d array
		initDistToCenter();
	}

	void runFloodfill(unsigned x, unsigned y){
		//x = col, y = row
		stack<coord> costack;
		bool processed[SIZE][SIZE];
		memset(processed, false, 256);
		coord cur(x, y);
		costack.push(cur);
		while(!costack.empty()){
			cur = costack.top();
			costack.pop();
			unsigned curx = cur.x;
			unsigned cury = cur.y;  
			processed[SIZE-1-cury][curx] = true;
			if(dist[SIZE-1-cury][curx] == 0)
				continue;
			unsigned shortest = UINT_MAX;
			//for all directions NSEW

			//north
			//coord n(x, y+1);
			if(cury < SIZE-1 && !walls[SIZE-1-cury][curx].north){
				if(dist[SIZE-1-cury-1][curx] < shortest)
					shortest = dist[SIZE-1-cury-1][curx];
				if(!processed[SIZE-1-cury-1][curx]){
					coord n(curx, cury+1);
					costack.push(n);
				}
			}
			//south
			if(cury > 0 && !walls[SIZE-1-cury][curx].south){
				if(dist[SIZE-1-cury+1][curx] < shortest)
					shortest = dist[SIZE-1-cury+1][curx];
				if(!processed[SIZE-1-cury+1][curx]){
					coord s(curx, cury-1);
					costack.push(s);
				}
			}
			//east			
			if(curx < SIZE-1 && !walls[SIZE-1-cury][curx].east){
				if(dist[SIZE-1-cury][curx+1] < shortest)
					shortest = dist[SIZE-1-cury][curx+1];
				if(!processed[SIZE-1-cury][curx+1]){
					coord e(curx+1, cury);
					costack.push(e);
				}
			}
			//west			
			if(curx > 0 && !walls[SIZE-1-cury][curx].west){
				if(dist[SIZE-1-cury][curx-1] < shortest)
					shortest = dist[SIZE-1-cury][curx-1];
				if(!processed[SIZE-1-cury][curx-1]){
					coord w(curx-1, cury);
					costack.push(w);
				}
			}

			if(shortest == UINT_MAX)
				continue;
			if(dist[SIZE-1-cury][curx] == shortest+1)
				continue;
			dist[SIZE-1-cury][curx] = shortest+1;
			//push all neighbors on costack
			coord neighbor(0, 0);

			//TODO
			//north neighbor
			if(cury != 15){
				neighbor.x = curx;
				neighbor.y = cury+1;
				costack.push(neighbor);
			}
			//south neighbor
			if(cury != 0){
				neighbor.x = curx;
				neighbor.y = cury-1;
				costack.push(neighbor);
			}
			//east neighbor
			if(curx != 15){
				neighbor.x = curx+1;
				neighbor.y = cury;
				costack.push(neighbor);
			}
			//west neighbor
			if(curx != 0){
				neighbor.x = curx-1;
				neighbor.y = cury;
				costack.push(neighbor);
			}			
		}
		//printArray(dist);
	}


	MouseMovement nextMovement(unsigned x, unsigned y, const Maze &maze) {
		const bool frontWall = maze.wallInFront();
		const bool leftWall  = maze.wallOnLeft();
		const bool rightWall  = maze.wallOnRight();
		updateWall(x, y, orient, frontWall, leftWall, rightWall);

		// Pause at each cell if the user requests it.
		// It allows for better viewing on command line.
		if(pause) {
			std::cout << "Hit enter to continue..." << std::endl;
			std::cin.ignore(10000, '\n');
			std::cin.clear();
		}

		std::cout << maze.draw(5) << std::endl << std::endl;

		// If we somehow miraculously hit the center
		// of the maze, just terminate and celebrate!
		if(isAtCenter(x, y) && dest == toCenter) {
			std::cout << "Found center! Good enough for the demo, won't try to get back." << std::endl;
			initDistToStart();
			runFloodfill(x, y);
			return Finish;
		}
		
		if(isAtStart(x, y) && dest == toStart){
		    std::cout << "Found start!" << std::endl; 
			initDistToCenter();
			runFloodfill(x, y);
		    return Finish;
		}

		unsigned char cur = dist[SIZE-1-y][x];
        //deadend, turn 180
        if (frontWall && leftWall && rightWall){			
            orient = (orient + 2) % 4;
			runFloodfill(x, y);
            return TurnAround;
        }
     
        //determine lowest weight and direction 
        unsigned char lowest = cur;
        unsigned char east, west, north, south; 
        east = west = north = south = UCHAR_MAX;
        char dir = 'c';    //c for cur  
		//EAST
        if((x < SIZE-1) && !walls[SIZE-1-y][x].east)
            east = dist[SIZE-1-y][x+1];
        if(lowest > east){
            lowest = east;
            dir = 'e';
        }
		//WEST
        if((x > 0) && !walls[SIZE-1-y][x].west)
            west = dist[SIZE-1-y][x-1];       
        if(lowest > west){
            lowest = west;
            dir = 'w';
        }
		//SOUTH
        if((y > 0) && !walls[SIZE-1-y][x].south)
            south = dist[SIZE-1-y+1][x];
        if(lowest > south){
            lowest = south;
            dir = 's';
        }
		//NORTH
        if((y < SIZE-1) && !walls[SIZE-1-y][x].north)    
            north = dist[SIZE-1-y-1][x];
        if(lowest > north){
           lowest = north;
           dir = 'n';
        }              
        //Check which direction to move next
        if (dir == 'c'){
			cout << "about to call floodfill bc stuck!\n";
            runFloodfill(x, y);
            return Stay;
        }        
        else if (orient == N){   
			switch(dir){
				 case 'n':
					 orient = N;
					 return MoveForward;
					 break;
				 case 's':
					 orient = S;
					 return TurnAround;
					 break;
				 case 'e':
					 orient = E;
					 return TurnClockwise;
					 break;    
				 case 'w':
					 orient = W;
					 return TurnCounterClockwise;
					 break;
				 default:
					 return Stay; 
					 break;
			}                
        }
        else if (orient == E){
			switch(dir){
				 case 'n':
					 orient = N;
					 return TurnCounterClockwise;
					 break;
				 case 's':
					 orient = S;
					 return TurnClockwise;
					 break;
				 case 'e':
					 orient = E;
					 return MoveForward;
					 break;    
				 case 'w':
					 orient = W;
					 return TurnAround;
					 break;
				 default:
					 return Stay; 
					 break;
			}
        }
        else if (orient == W){
            switch(dir){
				 case 'n':
					 orient = N;
					 return TurnClockwise;
					 break;
				 case 's':
					 orient = S;
					 return TurnCounterClockwise;
					 break;
				 case 'e':
					 orient = E;
					 return TurnAround;
					 break;    
				 case 'w':
					 orient = W;
					 return MoveForward;
					 break;
				 default:
					 return Stay; 
					 break;
			}
        }
        else if (orient == S){
            switch(dir){
				 case 'n':
					 orient = N;
					 return TurnAround;
					 break;
				 case 's':
					 orient = S;
					 return MoveForward;
					 break;
				 case 'e':
					 orient = E;
					 return TurnCounterClockwise;
					 break;    
				 case 'w':
					 orient = W;
					 return TurnClockwise;
					 break;
				 default:
					 return Stay; 
					 break;
			}
        }
		return Stay;		
	}

protected:
	// Helps us determine that we should go forward if we have just turned left.
	bool shouldGoForward;

	// Helps us determine if we've made a loop around the maze without finding the center.
	bool visitedStart;

	// Indicates we should pause before moving to next cell.
	// Useful for command line usage.
	const bool pause;

	bool isAtCenter(unsigned x, unsigned y) const {
		unsigned midpoint = MazeDefinitions::MAZE_LEN / 2;

		if(MazeDefinitions::MAZE_LEN % 2 != 0) {
			return x == midpoint && y == midpoint;
		}

		return  (x == midpoint     && y == midpoint    ) ||
			(x == midpoint - 1 && y == midpoint    ) ||
			(x == midpoint     && y == midpoint - 1) ||
			(x == midpoint - 1 && y == midpoint - 1);
	}

	bool isAtStart(unsigned x, unsigned y) const {
        return (x == 0 && y == 0);
    }

	void updateWall(unsigned x, unsigned y, int orient, bool frontWall, bool leftWall, bool rightWall){
		//NORTH
		if (orient == N){
			if(frontWall){
				walls[SIZE-1-y][x].north = true;
				if (y < SIZE-1)
					walls[SIZE-1-y-1][x].south = true;
			} 
			if(leftWall){
				walls[SIZE-1-y][x].west = true;
				if (x > 0)
					walls[SIZE-1-y][x-1].east = true; 
			} 
			if(rightWall){
				walls[SIZE-1-y][x].east = true;
				if (x < SIZE-1)
					walls[SIZE-1-y][x+1].west = true; 
			}  
		}
		//EAST
		else if (orient == E){
			if(frontWall){
				walls[SIZE-1-y][x].east = true; 
				if(x<SIZE-1)
					walls[SIZE-1-y][x+1].west = true;
			}
			if(leftWall){
				walls[SIZE-1-y][x].north = true; 
				if(y<SIZE-1)
					walls[SIZE-1-y-1][x].south = true;
			}
			if(rightWall) {
				walls[SIZE-1-y][x].south = true;
				if(y>0)
					walls[SIZE-1-y+1][x].north = true;
			}
		}
		// WEST
		else if (orient == W){
			if(frontWall){
				walls[SIZE-1-y][x].west = true;
				if(x>0)
					walls[SIZE-1][x-1].east = true;  
			}
			if(leftWall){
				walls[SIZE-1-y][x].south = true;
				if(y>0){
					walls[SIZE-1-y+1][x].north = true;
				}
				if(rightWall){
					walls[SIZE-1-y][x].north = true;
					if(y<SIZE-1)
						walls[SIZE-1-y-1][x].south = true;
				}
			}
		}
		//SOUTH
		else{
			if(frontWall){
				walls[SIZE-1-y][x].south = true;
				if(y>0)
					walls[SIZE-1-y+1][x].north = true;
			}
			if(leftWall){
				walls[SIZE-1-y][x].east = true;
				if(x<SIZE-1)
					walls[SIZE-1-y][x+1].west = true;
			}       
			if(rightWall) {
				walls[SIZE-1-y][x].west = true;
				if(x>0)
					walls[SIZE-1-y][x-1].east = true;
			}
		}
	}
};



int main(int argc, char * argv[]) {
	MazeDefinitions::MazeEncodingName mazeName = MazeDefinitions::MAZE_CAMM_2012;
	bool pause = true;

	// Since Windows does not support getopt directly, we will
	// have to parse the command line arguments ourselves.

	// Skip the program name, start with argument index 1
	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-m") == 0 && i+1 < argc) {
			int mazeOption = atoi(argv[++i]);
			if(mazeOption < MazeDefinitions::MAZE_NAME_MAX && mazeOption > 0) {
				mazeName = (MazeDefinitions::MazeEncodingName)mazeOption;
			}
		} else if(strcmp(argv[i], "-p") == 0) {
			pause = true;
		} else {
			std::cout << "Usage: " << argv[0] << " [-m N] [-p]" << std::endl;
			std::cout << "\t-m N will load the maze corresponding to N, or 0 if invalid N or missing option" << std::endl;
			std::cout << "\t-p will wait for a newline in between cell traversals" << std::endl;
			return -1;
		}
	}

	//LeftWallFollower leftWallFollower(pause);
	Floodfill floodfill(pause);
	Maze maze(mazeName, &floodfill);
	std::cout << maze.draw(5) << std::endl << std::endl;
	maze.start();
	dest = toStart;
	maze.start();
	dest = toCenter;
	printArray(dist);
	maze.start();
	dest = toStart;
	maze.start();
}
