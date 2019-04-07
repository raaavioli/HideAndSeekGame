#pragma once

#include <string>

enum InstructionType {
	OBJERROR = -1,
	PLAYER,
	PLANE,
	WALL,
	ITEM,
	PICKUP,
	DROP,
	NUMOBJTYPES,
};

enum Attribute {
	ATTRIBERROR = -1,
	ID,
	POSITION,
	SCALE,
	DIRECTION,
	STATUS,
	MODEL,
	NUMATTRIBS,
};

//Requires 4*3 bytes of memory
struct pVector3
{
	float X = 0;
	float Y = 0;
	float Z = 0;
};

//Requires 4 bytes of memory
struct pInt
{
	int Value = 0;
};

//Requires 1 byte of memory
struct pChar
{
	char Value = 0;
};

//Requires 64 bytes of memory
struct pString
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
	bool HasNext();
	InstructionType GetInstructionType();
	Attribute GetAttribute();
	int GetData(void* out_data);

	static std::string Stringify(InstructionType, Attribute, void* data);
private:
	int m_DataPointer;
	std::string *m_BeingParsed;

	int getObjectSize();

};