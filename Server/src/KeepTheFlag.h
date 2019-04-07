#pragma once

#include "Protocol.h"
#include "Utils/MazeGenerator.h"
#include "GameObjects/GroundPlane.h"
#include "GameObjects/Wall.h"
#include "GameObjects/Flag.h"

class KeepTheFlag {
public:
	KeepTheFlag(int boardWidth, int boardHeight, 
		int mazeWidth, int mazeHeight, int mazeSpace);

	std::string GetGameMap() { return m_GameMap; }

private:
	MazeGenerator m_MazeGenerator;
	GroundPlane m_Floor;
	Flag* m_Flag;
	std::string m_GameMap;
};