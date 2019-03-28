#pragma once

#include <vector>
#include <stack>
#include <map>
#include <stdlib.h>
#include <iostream>
#include <ctime>

#include "../GameObjects/Wall.h"

enum Cardinal {
	UNKNOWN = -1,
	NORTH,
	EAST,
	SOUTH,
	WEST,
};

struct Cell {
	int X, Y;
	std::map<Cardinal, Cell*> Neighbors;
	bool isVisited = false;
};

class MazeGenerator {
public:
	MazeGenerator(int width, int height);

	~MazeGenerator();

	void GenerateMaze();
	void CutLongerWalls(int wallLength);
	void PrintMaze();

	std::vector<Wall*> GetGameWalls(GroundPlane &gp);

private:
	Cardinal Opposite(Cardinal d)
	{
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

	void destroyWall(Cardinal d, Cell* cell);


private:
	int m_Width, m_Height, m_Size;
	std::vector<Cell*> m_Maze;
};