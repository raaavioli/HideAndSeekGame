#include "Protocol.h"

std::string Protocol::Stringify(ObjectType object, Attribute attrib, void *data)
{
	std::string ret = "";
	if (object < NUMOBJTYPES && attrib < NUMATTRIBS) {
		int val = (char)object + ((char)attrib << 8);
		char* start = (char*)&val;
		ret = std::string(start, 2);

		switch (attrib)
		{
			case ID: 
			{
				Id *id = ((Id*)data);
				ret.append((char*)&(id->Value), 4);
				break;

			}
			case POSITION:
			{
				Position *pos = ((Position*)data);
				ret.append((char*)&pos->X, 4);
				ret.append((char*)&pos->Y, 4);
				ret.append((char*)&pos->Z, 4);
				break;
			}
			case DIRECTION:
			{
				Direction *dir = ((Direction*)data);
				ret.append((char*)&dir->X, 4);
				ret.append((char*)&dir->Y, 4);
				ret.append((char*)&dir->Z, 4);
				break;
			}
			case SCALE:
			{
				Scale *scale = ((Scale*)data);
				ret.append((char*)&scale->X, 4);
				ret.append((char*)&scale->Y, 4);
				ret.append((char*)&scale->Z, 4);
				break;
			}
			case STATUS:
			{
				Status *stat = ((Status*)data);
				ret.append(stat->Message, sizeof(Status));
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
	m_DataPointer += objectSize;

	return objectSize > 0 && m_DataPointer <= m_BeingParsed->size();
}

ObjectType Protocol::GetObjectType()
{
	if (m_BeingParsed->size() > m_DataPointer) {
		char obj = m_BeingParsed->at(m_DataPointer);
		if (0 <= obj && obj < NUMOBJTYPES)
			return (ObjectType)obj;
	}
	return OBJERROR;
}

Attribute Protocol::GetAttribute()
{	
	if (m_BeingParsed->size() > m_DataPointer) {
		char obj = m_BeingParsed->at(m_DataPointer + 1);
		if (0 <= obj && obj < NUMATTRIBS)
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
			*((Id*)out_data) = *((Id*)&m_BeingParsed->at(m_DataPointer + 2));
			size = sizeof(Id);
			break;
		}
		case POSITION:
		{
			*((Position*)out_data) = *((Position*)&m_BeingParsed->at(m_DataPointer + 2));
			size = sizeof(Position);
			break;
		}
		case SCALE:
		{
			*((Scale*)out_data) = *((Scale*)&m_BeingParsed->at(m_DataPointer + 2));
			size = sizeof(Scale);
			break;
		}
		case DIRECTION:
		{
			*((Direction*)out_data) = *((Direction*)&m_BeingParsed->at(m_DataPointer + 2));
			size = sizeof(Direction);
			break;
		}
		case STATUS:
		{
			*((Status*)out_data) = *((Status*)&m_BeingParsed->at(m_DataPointer + 2));
			size = sizeof(Status);
			break;
		}
		default:
			return -1;
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
		return sizeof(Id) + 2;
	}
	case POSITION:
	{
		return sizeof(Position) + 2;
	}
	case SCALE:
	{
		return sizeof(Scale) + 2;
	}
	case DIRECTION:
	{
		return sizeof(Direction) + 2;
	}
	case STATUS:
	{
		return sizeof(Status) + 2;
	}
	default:
		return -1;
	};
}