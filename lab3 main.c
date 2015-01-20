#include <iostream>
#include <cstdlib>  // atoi
#include <stack>
#include <climits>
#include "Maze.h"
#include "MazeDefinitions.h"
#include "PathFinder.h"
using namespace std;

const int SIZE = 16;

/**
* Demo of a PathFinder implementation.
*
* Do not use a left/right wall following algorithm, as most
* Micromouse mazes are designed for such algorithms to fail.
*/

/*
class LeftWallFollower : public PathFinder {
public:
LeftWallFollower(bool shouldPause = false) : pause(shouldPause) {
shouldGoForward = false;
visitedStart = false;
}

MouseMovement nextMovement(unsigned x, unsigned y, const Maze &maze) {
const bool frontWall = maze.wallInFront();
const bool leftWall  = maze.wallOnLeft();

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
if(isAtCenter(x, y)) {
std::cout << "Found center! Good enough for the demo, won't try to get back." << std::endl;
return Finish;
}

// If we hit the start of the maze a second time, then
// we couldn't find the center and never will...
if(x == 0 && y == 0) {
if(visitedStart) {
std::cout << "Unable to find center, giving up." << std::endl;
return Finish;
} else {
visitedStart = true;
}
}

// If we have just turned left, we should take that path!
if(!frontWall && shouldGoForward) {
shouldGoForward = false;
return MoveForward;
}

// As long as nothing is in front and we have
// a wall to our left, keep going forward!
if(!frontWall && leftWall) {
shouldGoForward = false;
return MoveForward;
}

// If our forward and left paths are blocked
// we should try going to the right!
if(frontWall && leftWall) {
shouldGoForward = false;
return TurnClockwise;
}

// Lastly, if there is no left wall we should take that path!
if(!leftWall) {
shouldGoForward = true;
return TurnCounterClockwise;
}

// If we get stuck somehow, just terminate.
std::cout << "Got stuck..." << std::endl;
return Finish;
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
};
*/

/////////////
//Floodfill//
/////////////

const int N = 1;
const int S = 2;
const int E = 3;
const int W = 4;

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

int heading = 0;
unsigned char dist[16][16];
wall walls[SIZE][SIZE];


//void setBit(int a[], int k)

class Floodfill : public PathFinder {
public:
	Floodfill(bool shouldPause = false) : pause(shouldPause) {
		heading = N;
		shouldGoForward = false;
		visitedStart = false;
		//initialize dist 2d array
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

	void runFloodfill(unsigned x, unsigned y){
		//x = col, y = row
		stack<coord> costack;
		bool processed[SIZE][SIZE];
		coord cur(x, y);
		costack.push(cur);
		while(!costack.empty()){
			cur = costack.top();
			unsigned curx = cur.x;
			unsigned cury = cur.y;
			processed[16-cury][curx] = true;
			if(dist[16-cury][curx] == 0)
				continue;
			unsigned shortest = UINT_MAX;
			//for all directions NSEW

			//north
			coord n(x, y+1);
			if(!walls[16-n.y-1][n.x].north){
				if(dist[16-n.y][n.x] < shortest)
					shortest = dist[16-n.y][n.x];
				if(!processed[16-n.y][n.x])
					costack.push(n);
			}
			//south
			coord s(x, y-1);
			if(!walls[16-s.y+1][s.x].south){
				if(dist[16-s.y][s.x] < shortest)
					shortest = dist[16-s.y][s.x];
				if(!processed[16-s.y][s.x])
					costack.push(s);
			}
			//east
			coord e(x+1, y);
			if(!walls[16-e.y][e.x-1].north){
				if(dist[16-e.y][e.x] < shortest)
					shortest = dist[16-e.y][e.x];
				if(!processed[16-e.y][e.x])
					costack.push(e);
			}
			//west
			coord w(x-1, y);
			if(!walls[16-w.y][w.x+1].north){
				if(dist[16-w.y][w.x] < shortest)
					shortest = dist[16-w.y][w.x];
				if(!processed[16-w.y][w.x])
					costack.push(w);
			}

			if(shortest == UINT_MAX)
				continue;
			if(dist[16-cury][curx] == shortest+1)
				continue;
			dist[16-cury][curx] = shortest+1;
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
			if(cury != 0){
				neighbor.x = curx-1;
				neighbor.y = cury;
				costack.push(neighbor);
			}			
		}
	}

	MouseMovement nextMovement(unsigned x, unsigned y, const Maze &maze) {
		const bool frontWall = maze.wallInFront();
		const bool leftWall  = maze.wallOnLeft();

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
		if(isAtCenter(x, y)) {
			std::cout << "Found center! Good enough for the demo, won't try to get back." << std::endl;
			return Finish;
		}

		// If we hit the start of the maze a second time, then
		// we couldn't find the center and never will...
		if(x == 0 && y == 0) {
			if(visitedStart) {
				std::cout << "Unable to find center, giving up." << std::endl;
				return Finish;
			} else {
				visitedStart = true;
			}
		}

		// If we have just turned left, we should take that path!
		if(!frontWall && shouldGoForward) {
			shouldGoForward = false;
			return MoveForward;
		}

		// As long as nothing is in front and we have
		// a wall to our left, keep going forward!
		if(!frontWall && leftWall) {
			shouldGoForward = false;
			return MoveForward;
		}

		// If our forward and left paths are blocked
		// we should try going to the right!
		if(frontWall && leftWall) {
			shouldGoForward = false;
			return TurnClockwise;
		}

		// Lastly, if there is no left wall we should take that path!
		if(!leftWall) {
			shouldGoForward = true;
			return TurnCounterClockwise;
		}

		// If we get stuck somehow, just terminate.
		std::cout << "Got stuck..." << std::endl;
		return Finish;
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

	void updateWall(unsigned x, unsigned y, int heading, bool frontWall, bool leftWall, bool rightWall){
		//NORTH
		if (heading == N){
			if(frontWall){
				walls[SIZE-y][x].north = true;
				if (y < SIZE-1)
					walls[SIZE-y-1][x].south = true;
			} 
			if(leftWall){
				walls[SIZE-y][x].west = true;
				if (x > 0)
					walls[SIZE-y][x-1].east = true; 
			} 
			if(rightWall){
				walls[SIZE-y][x].east = true;
				if (x < SIZE-1)
					walls[SIZE-y][x+1].west = true; 
			}  
		}
		//EAST
		else if (heading == E){
			if(frontWall){
				walls[SIZE-y][x].east = true; 
				if(x<SIZE-1)
					walls[SIZE-y][x+1].west = true;
			}
			if(leftWall){
				walls[SIZE-y][x].north = true; 
				if(y<SIZE-1)
					walls[SIZE-y-1][x].south = true;
			}
			if(rightWall) {
				walls[SIZE-y][x].south = true;
				if(y>0)
					walls[SIZE-y+1][x].north = true;
			}
		}
		// WEST
		else if (heading == W){
			if(frontWall){
				walls[SIZE-y][x].west = true;
				if(x>0)
					walls[SIZE][x-1].east = true;  
			}
			if(leftWall){
				walls[SIZE-y][x].south = true;
				if(y>0){
					walls[SIZE-y+1][x].north = true;
				}
				if(rightWall){
					walls[SIZE-y][x].north = true;
					if(y<SIZE-1)
						walls[SIZE-y-1][x].south = true;
				}
			}
			//SOUTH
			else{
				if(frontWall){
					walls[SIZE-y][x].south = true;
					if(y>0)
						walls[SIZE-y+1][x].north = true;
				}
				if(leftWall){
					walls[SIZE-y][x].east = true;
					if(x<SIZE-1)
						walls[SIZE-y][x+1].west = true;
				}       
				if(rightWall) {
					walls[SIZE-y][x].west = true;
					if(x>0)
						walls[SIZE-y][x-1].east = true;
				}
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
}
