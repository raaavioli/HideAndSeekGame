#include "KeepTheFlag.h"

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
}
