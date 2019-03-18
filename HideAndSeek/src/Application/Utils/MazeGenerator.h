#pragma once

#include <vector>
#include <stack>
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
	std::vector<Direction> Walls;
	bool isVisited = false;
	int findWallIndex(Direction d) 
	{ 
		for (int i = 0; i < Walls.size(); i++) {
			if (Walls.at(i) == d) {
				return i;
			}
		}
		return -1;
	}
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
				if (x > 0)
					c->Walls.push_back(WEST);
				if (y > 0)
					c->Walls.push_back(NORTH);
				if (x < m_Width-1)
					c->Walls.push_back(EAST);
				if (y < m_Height-1)
					c->Walls.push_back(SOUTH);

				m_Maze.push_back(c);
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
		int xStart = rand() % m_Width;
		int yStart = rand() % m_Height;
		APP_INFO("Maze starts at: ({0},{1})", xStart, yStart);

		Cell current = *m_Maze.at(yStart*m_Width + xStart);
		m_Maze.at(yStart*m_Width + xStart)->isVisited = true;

		queue.push(current);

		while (queue.size() > 0)
		{
			Cell& neighbor = GotoUnvisitedNeighbor(current);
			if (neighbor.isVisited)
			{
				//No unvisited neighbors existed for 
				queue.pop();
				if(queue.size() > 0)
					current = *m_Maze.at(queue.top().Y * m_Width + queue.top().X);
			}
			else
			{
				neighbor.isVisited = true;
				current = neighbor;
				queue.push(current);
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
				for (int i = 0; i < c.Walls.size(); i++) {
					if (c.Walls.at(i) == SOUTH) {
						south = true;
					}
					if (c.Walls.at(i) == EAST) {
						east = true;
					}
				}
				if (south && east)
					std::cout << "_|";
				else if (south)
					std::cout << "_";
				else if (east)
					std::cout << " |";
				else
					std::cout << " ";
			}
			std::cout << "|\n";
		}
	}

private:
	Cell &GotoUnvisitedNeighbor(const Cell from) 
	{
		int random = 0;
		if(from.Walls.size() > 0)
			random = rand() % from.Walls.size();
		
		for (int i = 0; i < from.Walls.size(); i++) 
		{
			//If first random value is already visited, we want to continue through all walls
			random += i;
			random %= from.Walls.size();

			Direction dir = from.Walls.at(random);
			Cell *neighbor;
			if(dir == NORTH) {
				neighbor = m_Maze.at((from.Y - 1)*m_Width + from.X);
				if (!neighbor->isVisited)
				{
					neighbor->Walls.erase(neighbor->Walls.begin() + neighbor->findWallIndex(SOUTH));
					Cell *current = m_Maze.at(from.Y*m_Width + from.X);
					current->Walls.erase(current->Walls.begin() + random);
					return *neighbor;
				}
			}	
			else if (dir == EAST) {
				neighbor = m_Maze.at((from.Y)*m_Width + from.X + 1);
				if (!neighbor->isVisited)
				{
					neighbor->Walls.erase(neighbor->Walls.begin() + neighbor->findWallIndex(WEST));
					Cell *current = m_Maze.at(from.Y*m_Width + from.X);
					current->Walls.erase(current->Walls.begin() + random);
					return *neighbor;
				}
			}
			else if (dir == SOUTH) {
				neighbor = m_Maze.at((from.Y + 1)*m_Width + from.X);
				if (!neighbor->isVisited)
				{
					neighbor->Walls.erase(neighbor->Walls.begin() + neighbor->findWallIndex(NORTH));
					Cell *current = m_Maze.at(from.Y*m_Width + from.X);
					current->Walls.erase(current->Walls.begin() + random);
					return *neighbor;
				}
			}
			else if (dir == WEST) {
				neighbor = m_Maze.at((from.Y)*m_Width + from.X - 1);
				if (!neighbor->isVisited)
				{
					neighbor->Walls.erase(neighbor->Walls.begin() + neighbor->findWallIndex(EAST));
					Cell *current = m_Maze.at(from.Y*m_Width + from.X);
					current->Walls.erase(current->Walls.begin() + random);
					return *neighbor;
				}
			}
		}
		return Cell{-1,-1, std::vector<Direction>(), true};
	}


private:
	int m_Width, m_Height, m_Size;
	std::vector<Cell*> m_Maze;
};