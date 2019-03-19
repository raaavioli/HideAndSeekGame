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
	int X, Y, NeighborCount;
	std::map<Direction, Cell*> Neighbors;
	bool isVisited = false;
};

class MazeGenerator {
public:
	MazeGenerator(int width, int height)
		: m_Width(width), m_Height(height), m_Size(width*height)
	{	
		m_Maze.reserve(width * height);

		for(int y = 0; y < m_Height; y++)
		{
			for (int x = 0; x < m_Width; x++) {
				Cell *c = new Cell();
				c->X = x;
				c->Y = y;
				c->NeighborCount = 0;
				if (x > 0) {
					c->Neighbors.insert(std::pair<Direction, Cell*>(WEST, m_Maze.at(y*m_Width + x - 1)));
					c->NeighborCount++;
				}
				else {
					c->Neighbors.insert(std::pair<Direction, Cell*>(WEST, nullptr));
				}
				if (y > 0) {
					c->Neighbors.insert(std::pair<Direction, Cell*>(NORTH, m_Maze.at((y - 1)*m_Width + x)));
					c->NeighborCount++;
				}
				else {
					c->Neighbors.insert(std::pair<Direction, Cell*>(NORTH, nullptr));
				}
				m_Maze.push_back(c);
			}
		}

		//Insert cell neighbors south and east which were not yet
		//declared during last iteration
		for (int y = 0; y < m_Height; y++)
		{
			for (int x = 0; x < m_Width; x++) {
				if (x < m_Width - 1) {
					m_Maze.at(y*m_Width + x)->Neighbors.insert(std::pair<Direction, Cell*>(EAST, m_Maze.at(y*m_Width + x + 1)));
					m_Maze.at(y*m_Width + x)->NeighborCount++;
				}
				else {
					m_Maze.at(y*m_Width + x)->Neighbors.insert(std::pair<Direction, Cell*>(EAST, nullptr));
				}
				if (y < m_Height - 1) {
					m_Maze.at(y*m_Width + x)->Neighbors.insert(std::pair<Direction, Cell*>(SOUTH, m_Maze.at((y + 1)*m_Width + x)));
					m_Maze.at(y*m_Width + x)->NeighborCount++;
				}
				else {
					m_Maze.at(y*m_Width + x)->Neighbors.insert(std::pair<Direction, Cell*>(SOUTH, nullptr));
				}
			}
		}
		
	};

	~MazeGenerator()
	{
		for (auto cell : m_Maze)
			delete cell;
	}

	void GenerateMaze() {
		std::stack<Cell> queue;
		std::srand(std::time(0));
		int xStart = rand() % m_Width;
		int yStart = rand() % m_Height;

		Cell current = *m_Maze.at(yStart*m_Width + xStart);
		m_Maze.at(yStart*m_Width + xStart)->isVisited = true;

		queue.push(current);

		while (queue.size() > 0)
		{
			Cell& neighbor = GotoUnvisitedNeighbor(current);
			if (neighbor.isVisited)
			{
				//All valid neighbors for current were visited
				queue.pop();
				if (queue.size() > 0) {
					current = *m_Maze.at(queue.top().Y * m_Width + queue.top().X);
				}
			}
			else
			{
				m_Maze.at(neighbor.Y * m_Width + neighbor.X)->isVisited = true;
				current = neighbor;
				queue.push(current);
			}
		}
	}

	void CutLongerWalls(int wallLength) {
		std::vector<int> columns(m_Width, 1);
		for (int y = 0; y < m_Height; y++)
		{
			int inRow = 1;
			for (int x = 0; x < m_Width; x++)
			{
				//Calculate number of walls in a row vertically on column x
				//if nr of walls in a row is larger than wallLength, remove 1 of them at random.
				if (x < m_Width-1 && m_Maze.at(y*m_Width + x)->Neighbors.at(EAST) != nullptr) 
				{
					if (columns.at(x) < wallLength) columns.at(x)++;
					else
					{
						int random = rand() % wallLength;
						destroyWall(EAST, m_Maze.at((y - random)*m_Width + x));
					}
				}
				else columns.at(x) = 1;

				//Calculate number of walls in a row horizontally on a row y
				//if nr of walls in a row is larger than wallLength, remove 1 of them at random.
				if (y < m_Height-1 && m_Maze.at(y*m_Width + x)->Neighbors.at(SOUTH) != nullptr)
				{
					if (inRow < wallLength) inRow++;
					else
					{
						int random = rand() % wallLength;
						destroyWall(SOUTH, m_Maze.at(y*m_Width + x - random));
					}
				}
				else inRow = 1;	
			}
		}
	}

	void PrintMaze() {
		for (int y = 0; y < m_Height; y++) {
			std::cout << "|";
			for (int x = 0; x < m_Width; x++) {
				Cell &c = *m_Maze.at(y*m_Width + x);
				bool south = false;
				bool east = false;

				if (c.Neighbors.at(SOUTH) != nullptr) {
					south = true;
				}
				if (c.Neighbors.at(EAST) != nullptr) {
					east = true;
				}
				
				if (south && east)
					std::cout << "_|";
				else if (south)
					std::cout << "__";
				else if (east)
					std::cout << " |";
				else
					std::cout << "  ";
			}
			std::cout << "|\n";
		}
	}

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

	Cell &GotoUnvisitedNeighbor(const Cell from) 
	{
		static Direction dirs[4] = { NORTH, EAST, SOUTH, WEST };
		
		//Get random start direction
		int random = rand() % 4;
		for (int i = 0; i < 4; i++) {
			random += i;
			random %= 4;
			//Check if neighbor in random direction is not null
			if (from.Neighbors.at(dirs[random]) != nullptr) {
				Direction dir = dirs[random];
				Cell *neighbor = from.Neighbors.at(dir);
				//if neighbor wasn't null, it must not be visited
				if (!neighbor->isVisited)
				{
					//Break wall between 'from' and the non visited neighbor
					//return the neighbor and use it as the new 'current'
					destroyWall(dir, m_Maze.at(from.Y*m_Width + from.X));
					return *neighbor;
				}
			}
			//Otherwise, try next direction if that one is valid
		}
		//If no directions were valid, return invalid cell which is already "visited"
		return Cell{-1,-1, 0, std::map<Direction, Cell*>(), true};
	}

	void destroyWall(Direction d, Cell* cell) 
	{
		if(cell->Neighbors.at(d) != nullptr)
		{
		cell->Neighbors.at(d)->Neighbors.at(Opposite(d)) = nullptr;
		cell->Neighbors.at(d) = nullptr;
		}
	}


private:
	int m_Width, m_Height, m_Size;
	std::vector<Cell*> m_Maze;
};