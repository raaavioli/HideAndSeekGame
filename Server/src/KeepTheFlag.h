#pragma once
#include <map>
#include <ctime>

#include "Protocol.h"
#include "Utils/MazeGenerator.h"
#include "GameObjects/GroundPlane.h"
#include "GameObjects/Wall.h"
#include "GameObjects/Flag.h"
#include "GameObjects/Player.h"

class KeepTheFlag {
public:
	KeepTheFlag(int boardWidth, int boardHeight, 
		int mazeWidth, int mazeHeight, int mazeSpace);

	~KeepTheFlag();

	bool Update();
	std::string GetGameMap() { return m_GameMap; }
	std::string GetGameStatus();
	Player* GetPlayer(int id);

private:
	MazeGenerator m_MazeGenerator;
	GroundPlane m_Floor;
	Flag* m_Flag;
	std::map<int, Player*> m_Players;
	std::string m_GameMap;

	long m_CurrentTime;
	long m_TimeAccumulated;
};