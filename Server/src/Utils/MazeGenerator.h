#pragma once

#include <vector>
#include <map>

#include "GameObjects/Wall.h"

enum Cardinal;
struct Cell;
class MazeGenerator {
public:
	MazeGenerator(GroundPlane &gp, int width, int height);

	~MazeGenerator();

	void GenerateMaze();
	void CutLongerWalls(int wallLength);
	void PrintMaze();
	inline int GetWidth() { return m_Width; }
	inline int GetHeight() { return m_Height; }

	std::vector<Wall*> GetGameWalls();

private:
	Cardinal Opposite(Cardinal d);

	Cell *GotoUnvisitedNeighbor(const Cell from);
	void destroyWall(Cardinal d, Cell* cell);
	int m_Width, m_Height;
	GroundPlane &m_Ground;
	std::vector<Cell*> m_Maze;
};