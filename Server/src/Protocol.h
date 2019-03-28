#pragma once

#include <string>

enum ObjectType {
	PLAYER = 0,
	PLANE,
	WALL,
	ITEM,
	NUMOBJTYPES,
	OBJERROR,
};

enum Attribute {
	ID = 0,
	POSITION,
	SCALE,
	DIRECTION,
	STATUS,
	NUMATTRIBS,
	ATTRIBERROR,
};

//Requires 4*3 bytes of memory
struct Position
{
	float X = 0;
	float Y = 0;
	float Z = 0;
};

//Requires 4 bytes of memory
struct Id
{
	int Value = 0;
};

//Requires 4*3 bytes of memory
struct Scale
{
	float X = 1; 
	float Y = 1; 
	float Z = 1;
};

//Requires 4*3 bytes of memory
struct Direction
{
	float X = 0;
	float Y = 0;
	float Z = 0;
};

//Requires 64 bytes of memory
struct Status
{
	char Message[64]; 
};

class Protocol {

public:
	Protocol(std::string* string)
		:m_BeingParsed(string) 
	{
		m_DataPointer = 0;
	};

	bool Next();
	ObjectType GetObjectType();
	Attribute GetAttribute();
	int GetData(void* out_data);

	static std::string Stringify(ObjectType, Attribute, void* data);
private:
	int m_DataPointer;
	std::string *m_BeingParsed;

	int getObjectSize();

};