/** 
	Name: Noah Baker
	Course: Data Structures
	Instructor: Terry Hostetler
	Term: Fall 2019

	MazeSolver.cpp : This program will attempt to solve a maze that is given to it. 
		The format of the file is as follows:
			NumberOfRows NumberOfColumns
			AMaze
		A maze has an assortment of periods and astericks that corrospond to open and closed spaces (walls), with open spaces being periods and walls being astericks.
		For a maze to be solveable, the bottom right corner must have a route of open spaces to the top left corner, only using cardinal directions. 
			This can be explained more logically as a route from (0,0) to (NumberOfRows-1,NumberOfColumns-1).
			With this program, however, the maze will be shifted by (+1,+1) as a border will be added to surround the maze. This is to help with preventing indexOutOfBounds errors.
		A maze is unsolveable when a route of open spaces does not exist between these two points, or either the top corner or bottom corner is a wall.
		The program will also find the shortest route.

		When a solution is found, the program will then find the shortest route by setting a max length for the route, and upping this max until a route is discovered.
		After this route is discovered, the program will output the maze with letters showing what path was taken, specfically with 'a' being the start, 
			unless steps are taken over 'z', in which the program will wrap back to 'a'.
		If a solution is not found, the program will output that "No solution found!"


**/

#include <iostream>
#include <fstream>

using namespace std;

const int NUM_ROWS = 12; // used so that the max length of a maxe can fit with a border
const int NUM_COLS = 12; // used so that the max length of a maxe can fit with a border

typedef char TableType[NUM_ROWS][NUM_COLS]; // This is used so that passing the maze to a function results in less errors (seen in textbook)

void solveMaze(TableType maze, int NR, int NC); // A non-recursive function to  make sure that the starting position and end position is even available. It will also print if the maze is completed or not
bool solveMazeRecursive(TableType maze, int x, int y, int NR, int NC, char nextChar); // This will be ran through multiple times to see what our shortest length can be
bool shortestPathRecursive(TableType maze, int x, int y, int NR, int NC, char nextChar, int max, int num); // this is only ran if a path exists


int main()
{
	TableType maze;
	ifstream file;
	int NR, NC;

	// this whole block imports the maze into a TableType so that it can be worked on by the functions
	file.open("maze.in");
	if (!file) {
		cout << "File Error!" << endl;
		return -1;
	}

	file >> NR >> NC;

	for (int r = 0; r < NR; r++) {
		for (int c = 0; c < NC; c++) {
			char value = file.get();
			if (value == '\n') // because we are reading char by char, we have to make this check
				value = file.get();
			maze[r][c] = value;
		}
	}
	
	solveMaze(maze, NR, NC);

	file.close();
    return 0;
}

// Pre: maze is initialized and filled with the maze
// Post: prints the maze with the shortest solution if solvable, and prints "No Solution found!" if there is no solution possible
// This method is the backbone behind solving a maze. It will run the recursive methond when it has to.
void solveMaze(TableType maze, int NR, int NC) {
	TableType newMaze; // This is the maze that will be recursively solved. It will be the same as maze, but with a border
	TableType mazeShort; // Mzze for the smallest possible route

	// This block copies the maze and adds a border of walls around it
	for (int r = 0; r < NR + 2; r++) {
		for (int c = 0; c < NC + 2; c++){
			if (r == 0 || r == NR + 1 || c == 0 || c == NC + 1) { // essentially, if the row is the first one, or the last,and same with column, it will create a wall (our border)
				newMaze[r][c] = '*';
				mazeShort[r][c] = '*';
			}
			else {
				newMaze[r][c] = maze[r - 1][c - 1];
				mazeShort[r][c] = maze[r - 1][c - 1];
			}
		}
	}

	if (newMaze[NR][NC] == '*' || newMaze[1][1] == '*') { // if the starting position is a wall, or the ending is a wall, it will cancel out the program
		cout << "No Solution found!" << endl;
	}
	else {
		if (solveMazeRecursive(newMaze, 1, 1, NR, NC, 'a')) {
			int maxPath = NR + NC - 1;

			while (!(shortestPathRecursive(mazeShort, 1, 1, NR, NC, 'a', maxPath, 0))) { // this loop will keep increasing the max path until one is discovered
				maxPath++;
			}

			for (int r = 1; r < NR + 1; r++) {
				for (int c = 1; c < NC + 1; c++) {
					cout << mazeShort[r][c];
				}
				cout << endl;
			}
				
		}
		else {
			cout << "No Solution found!" << endl;
		}
	}

	
}

// Pre: maze is intiallized and has a border of wall
// Post: true is returned if a solution is found. It will also modify maze with a map of the solution
// This method will attempt to solve a maze recursively. If a solution exists, it will return false if a solution does not exist
bool solveMazeRecursive(TableType maze, int x, int y, int NR, int NC, char nextChar) {

	if (nextChar == '{') // if the char goes over z, it will loop back to a
		nextChar = 'a';
	else {
		if (maze[x][y] == '.') {
			maze[x][y] = nextChar;
			if (x == NR && y == NC) {
				return true;
			}
			else {
				if (solveMazeRecursive(maze, x, y + 1, NR, NC, nextChar + 1)) // attempts to run right
					return true;
				else if (solveMazeRecursive(maze, x + 1, y, NR, NC, nextChar + 1)) // attempts to run downward
					return true;
				else if (solveMazeRecursive(maze, x - 1, y, NR, NC, nextChar + 1)) // attempts to run upward
					return true;
				else if (solveMazeRecursive(maze, x, y - 1, NR, NC, nextChar + 1)) // attempts to run left
					return true;
				
				else {
					maze[x][y] = '^'; // changes space to a placeholder char as this space cannot be proceeded to
					return false;
				}
			}
		}
		else {
			return false;
		}
	}
	
}



// Pre: maze has been initialized
// Post: retruns true when a solution is found. It will also modify maze with a map of the solution
// This method will solve a maze recursively while also solving for its shortest path. If a path does not exist, it will return false
bool shortestPathRecursive(TableType maze, int x, int y, int NR, int NC, char nextChar, int max, int num) {

	
	if (nextChar == '{') // if the char goes over z, it will loop back to a
		nextChar = 'a';
	if (num == max) {
		return false;
	}
	else {
		if (maze[x][y] == '.') {
			maze[x][y] = nextChar;
			if (x == NR && y == NC) {
				return true;
			}
			else {
				if (shortestPathRecursive(maze, x, y - 1, NR, NC, nextChar + 1, max, num + 1)) // attempts to run left
					return true;
				else if (shortestPathRecursive(maze, x + 1, y, NR, NC, nextChar + 1, max, num + 1)) // attempts to run downward
					return true;
				else if (shortestPathRecursive(maze, x - 1, y, NR, NC, nextChar + 1, max, num + 1)) // attempts to run upward
					return true;
				else if (shortestPathRecursive(maze, x, y + 1, NR, NC, nextChar + 1, max, num + 1)) // attempts to run right
					return true;
				else {
					maze[x][y] = '.'; // fixes space back to open as this space cannot be proceeded to
					return false;
				}
			}
		}
		else {
			return false;
		}
	}

}
