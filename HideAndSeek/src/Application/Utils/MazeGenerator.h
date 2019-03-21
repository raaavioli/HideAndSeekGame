#pragma once

#include <vector>
#include <stack>
#include <map>
#include <stdlib.h>
#include <iostream>

#include "Engine/Log.h"

enum Direction {
	UNKNOWN = -1,
	NORTH,
	EAST,
	SOUTH,
	WEST,
};

struct Cell {
	int X, Y;
	std::map<Direction, Cell*> Neighbors;
	bool isVisited = false;
};

class MazeGenerator {
public:
	MazeGenerator(int width, int height);

	~MazeGenerator();

	void GenerateMaze();
	void CutLongerWalls(int wallLength);
	void PrintMaze();

private:
	Direction Opposite(Direction d) {
		switch (d) {
		case NORTH:
			return SOUTH;
		case SOUTH:
			return NORTH;
		case EAST:
			return WEST;
		case WEST:
			return EAST;
		default:
			return UNKNOWN;
		}
	}

	Cell &GotoUnvisitedNeighbor(const Cell from);

	void destroyWall(Direction d, Cell* cell);


private:
	int m_Width, m_Height, m_Size;
	std::vector<Cell*> m_Maze;
};