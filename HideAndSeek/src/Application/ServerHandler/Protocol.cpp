#include "Protocol.h"

std::string Protocol::Stringify(InstructionType object, Attribute attrib, void *data)
{
	std::string ret = "";
	if (object < NUMOBJTYPES && attrib <= NUMATTRIBS) {
		int val = (char)object + ((char)attrib << 8);
		char* start = (char*)&val;
		ret = std::string(start, 2);

		switch (attrib)
		{
		case ID:
		{
			pInt *id = ((pInt*)data);
			ret.append((char*)&(id->Value), 4);
			break;

		}
		case POSITION:
		case DIRECTION:
		case SCALE:
		case COLOR:
		case ROTATION:
		{
			pVector3 *pos = ((pVector3*)data);
			ret.append((char*)&pos->X, 4);
			ret.append((char*)&pos->Y, 4);
			ret.append((char*)&pos->Z, 4);
			break;
		}
		case STATUS:
		{
			pString512 *stat = ((pString512*)data);
			ret.append(stat->Message, sizeof(pString512));
			break;
		}
		case MODEL:
		{
			pString64 *stat = ((pString64*)data);
			ret.append(stat->Message, sizeof(pString64));
			break;
		}
		case NUMATTRIBS:
		{
			pChar *stat = ((pChar*)data);
			int s = sizeof(pChar);
			ret.append((char*)&stat->Value, sizeof(pChar));
			break;
		}
		default:
			ret = "";
		};
	}
	return ret;
}

bool Protocol::Next()
{
	int objectSize = getObjectSize();
	if (objectSize > 0)
	{
		m_DataPointer += objectSize;
		return m_DataPointer < m_BeingParsed->size();
	}
	return false;
}

bool Protocol::HasNext()
{
	int objectSize = getObjectSize();
	if (objectSize > 0)
	{
		return (m_DataPointer + objectSize) <= m_BeingParsed->size();
	}
	return false;
}

InstructionType Protocol::GetInstructionType()
{
	if (m_BeingParsed->size() > m_DataPointer) {
		char obj = m_BeingParsed->at(m_DataPointer);
		if (0 <= obj && obj < NUMOBJTYPES)
			return (InstructionType)obj;
	}
	return OBJERROR;
}

Attribute Protocol::GetAttribute()
{
	if (m_BeingParsed->size() > m_DataPointer) {
		char obj = m_BeingParsed->at(m_DataPointer + 1);
		if (0 <= obj && obj <= NUMATTRIBS)
			return (Attribute)obj;
	}
	return ATTRIBERROR;
}


int Protocol::GetData(void *out_data)
{
	Attribute attrib = GetAttribute();
	int size;

	switch (attrib)
	{
	case ID:
	{
		*((pInt*)out_data) = *((pInt*)&m_BeingParsed->at(m_DataPointer + 2));
		size = sizeof(pInt);
		break;
	}
	case POSITION:
	case SCALE:
	case DIRECTION:
	case COLOR:
	case ROTATION:
	{
		*((pVector3*)out_data) = *((pVector3*)&m_BeingParsed->at(m_DataPointer + 2));
		size = sizeof(pVector3);
		break;
	}
	case STATUS:
	{
		*((pString512*)out_data) = *((pString512*)&m_BeingParsed->at(m_DataPointer + 2));
		size = sizeof(pString512);
		break;
	}
	case MODEL:
	{
		*((pString64*)out_data) = *((pString64*)&m_BeingParsed->at(m_DataPointer + 2));
		size = sizeof(pString64);
		break;
	}
	case NUMATTRIBS:
	{
		*((pChar*)out_data) = *((pChar*)&m_BeingParsed->at(m_DataPointer + 2));
		size = sizeof(pChar);
		break;
	}
	default:
		return ATTRIBERROR;
	};

	return size;
}

/*
* Returns the size of the currently pointed object in m_BeingParsed
* Size is size of data + 2 bytes for the object type and the attrib type
*/
int Protocol::getObjectSize()
{
	Attribute attrib = GetAttribute();

	switch (attrib)
	{
	case ID:
	{
		return sizeof(pInt) + 2;
	}
	case POSITION:
	case SCALE:
	case DIRECTION:
	case COLOR:
	case ROTATION:
	{
		return sizeof(pVector3) + 2;
	}
	case STATUS:
	{
		return sizeof(pString512) + 2;
	}
	case MODEL:
	{
		return sizeof(pString64) + 2;
	}
	case NUMATTRIBS:
	{
		return sizeof(pChar) + 2;
	}
	default:
		return ATTRIBERROR;
	};
}