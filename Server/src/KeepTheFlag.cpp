#include "KeepTheFlag.h"
#include <sstream>
#include <iostream>
#include <random>

#include "GameObjects/Collision/Collider.h"

KeepTheFlag::KeepTheFlag(int boardWidth, int boardHeight, 
	int mazeWidth, int mazeHeight, int mazeSpace)
	: m_Floor(GroundPlane(boardWidth, boardHeight)),
	m_MazeGenerator(mazeWidth, mazeHeight)
{
	Collider::Add(&m_Floor, MovementType::STATIC);
	m_GameMap.append(m_Floor.ToProtocolString());

	m_MazeGenerator.GenerateMaze();
	m_MazeGenerator.CutLongerWalls(mazeSpace);
	m_MazeGenerator.PrintMaze();

	for (Wall* w : m_MazeGenerator.GetGameWalls(m_Floor)) {
		Collider::Add(w, MovementType::STATIC);
		m_GameMap.append(w->ToProtocolString());
		w->Update();
	}

	m_Flag = new Flag(m_Floor, 1, (Flag::ACTIVE));
	m_GameMap.append(m_Flag->ToProtocolString());
	Collider::Add(m_Flag, MovementType::LOOTABLE);
	m_Flag->Update();

	m_CurrentTime = std::clock();
	m_TimeAccumulated = 0;
}

KeepTheFlag::~KeepTheFlag()
{
	for(auto& pair : m_Players)
	{
		delete pair.second;
	}
}

bool KeepTheFlag::Update()
{
	std::clock_t t = std::clock();
	m_TimeAccumulated += (t - m_CurrentTime);
	m_CurrentTime = t;

	Collider::Interact();

	if (m_TimeAccumulated > 1000)
	{
		m_TimeAccumulated %= 1000;
		for (auto[id, player] : m_Players)
		{
			if (player->HasItem(m_Flag))
			{
				player->AddScore(1);
			}
		}
	}
	return true; //Later changed to return status of game state
}

std::string KeepTheFlag::GetGameStatus()
{
	std::stringstream status;
	status << "SCORE" << std::endl;
	status << "---------------" << std::endl;
	for (auto [id, player] : m_Players)
	{
		status << "Player " << id << ": " << player->GetScore() << std::endl;
	}
	std::string s = status.str();
	if (s.size() > sizeof(pString))
		std::cout << "Error: GameStatus larger than pString size" << std::endl;
	else 
	{
		pString sc;
		std::strcpy(sc.Message, s.c_str());
		return Protocol::Stringify(InstructionType::MESSAGE, Attribute::STATUS, &sc);
	}
	return "";
}

Player* KeepTheFlag::GetPlayer(int id)
{
	auto p = m_Players.find(id);
	if (p != m_Players.end())
		return (*p).second;
	else {
		Player *player = new Player(
			id, 
			-m_Floor.GetWidth() / 2 + (m_Floor.GetWidth() / (m_MazeGenerator.GetWidth())) * ((std::rand()%m_MazeGenerator.GetWidth()) + 0.5),
			-m_Floor.GetHeight() / 2 + (m_Floor.GetHeight() / (2 * m_MazeGenerator.GetHeight())) * ((std::rand() % m_MazeGenerator.GetHeight()) + 0.5),
			3
		);
		Collider::Add(player, MovementType::DYNAMIC);
		m_Players.insert(std::make_pair(id, player));
		return player;
	}
}
