#include "stdafx.h"

#include "CppUnitTest.h"

#include "../../HideAndSeek/vendor/glm/glm/glm.hpp"
#include "../../Server/src/Protocol.cpp"

#include "GameObjects/Wall.h"
#include "GameObjects/Wall.cpp"
#include "GameObjects/GroundPlane.h"
#include "GameObjects/GroundPlane.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ServerTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(AssertObjectTypeAfterProtocolConversion)
		{
			//Setup
			ObjectType expected = ObjectType::PLANE;		
			Attribute attr = Attribute::POSITION;	
			Position p{ 1.0f, 1.0f, 1.0f };			
			std::string string = Protocol::Stringify(expected, attr, &p);
			Protocol protocol(&string);

			//Calculate
			ObjectType actual = protocol.GetObjectType();

			//Assert
			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(AssertObjectAttributeAfterConversion)
		{
			//Setup
			ObjectType ot = ObjectType::PLANE;		
			Attribute expected = Attribute::POSITION;
			Position p{ 1.0f, 1.0f, 1.0f };			
			std::string string = Protocol::Stringify(ot, expected, &p);
			Protocol protocol(&string);

			//Calculate
			Attribute actual = protocol.GetAttribute();

			//Assert
			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(AssertObjectIdAfterConversion)
		{
			//Setup
			ObjectType ot = ObjectType::PLANE;		
			Attribute expectedAttr = Attribute::ID;	
			Id expected{ 1 };		
			int expectedSize = sizeof(expected);
			std::string string = Protocol::Stringify(ot, expectedAttr, &expected);
			Protocol protocol(&string);

			//Calculate
			Id actual;
			int actualSize = protocol.GetData(&actual);

			//Assert
			Assert::IsTrue(actual.Value == expected.Value);
			Assert::IsTrue(actualSize == expectedSize);
		}

		TEST_METHOD(AssertObjectIntPositionAfterConversion)
		{
			//Setup
			ObjectType ot = ObjectType::PLANE;			
			Attribute expectedAttr = Attribute::POSITION;
			Position expected{ 3.0f, 4.0f, 5.0f };		
			int expectedSize = sizeof(expected);
			std::string string = Protocol::Stringify(ot, expectedAttr, &expected);
			Protocol protocol(&string);

			//Calculate
			Position actual;
			int actualSize = protocol.GetData(&actual);

			//Assert
			Assert::IsTrue(actual.X == expected.X);
			Assert::IsTrue(actual.Y == expected.Y);
			Assert::IsTrue(actual.Z == expected.Z);
			Assert::IsTrue(actualSize == expectedSize);
		}

		TEST_METHOD(AssertObjectFloatPositionAfterConversion)
		{
			//Setup
			ObjectType ot = ObjectType::PLANE;
			Attribute expectedAttr = Attribute::POSITION;
			Position expected{ 3.9999999999f, 4.23232323f, 1.323123123f };
			int expectedSize = sizeof(expected);
			std::string string = Protocol::Stringify(ot, expectedAttr, &expected);
			Protocol protocol(&string);

			//Calculate
			Position actual;
			int actualSize = protocol.GetData(&actual);

			//Assert
			Assert::IsTrue(actual.X == expected.X);
			Assert::IsTrue(actual.Y == expected.Y);
			Assert::IsTrue(actual.Z == expected.Z);
			Assert::IsTrue(expectedSize == actualSize);
		}

		TEST_METHOD(AssertObjectStatusAfterConversion)
		{
			//Setup
			ObjectType ot = ObjectType::PLANE;
			Attribute expectedAttr = Attribute::STATUS;
			Status expected{"123qweasd123qweasd123qweasd123qweasd123"};
			int expectedSize = sizeof(expected);
			std::string string = Protocol::Stringify(ot, expectedAttr, &expected);
			Protocol protocol(&string);

			//Calculate
			Status actual;
			int actualSize = protocol.GetData(&actual);

			//Assert
			Assert::AreEqual(std::string(expected.Message), std::string(actual.Message));
			Assert::IsTrue(expectedSize == actualSize);
		}

		TEST_METHOD(AssertTwoAttribProtocolIsCorrectlyParsed)
		{
			//Setup
			ObjectType ot = ObjectType::PLANE;
			Attribute expectedAttr1 = Attribute::STATUS;
			Status expected1{ "123qweasd123qweasd123qweasd123qweasd123" };

			Attribute expectedAttr2 = Attribute::POSITION;
			Position expected2{ 2.0f, 1.0f, 2.0f };

			std::string string = Protocol::Stringify(ot, expectedAttr1, &expected1);
			string.append(Protocol::Stringify(ot, expectedAttr2, &expected2));
			Protocol protocol = Protocol(&string);

			//Calculate
			Status actual1;
			protocol.GetData(&actual1);
			bool firstNext = protocol.Next();
			
			Position actual2;
			protocol.GetData(&actual2);
			bool secondNext = protocol.Next();
			bool thirdNext = protocol.Next();

			//Assert
			Assert::AreEqual(std::string(expected1.Message), std::string(actual1.Message));
			Assert::IsTrue(actual2.X == expected2.X);
			Assert::IsTrue(actual2.Y == expected2.Y);
			Assert::IsTrue(actual2.Z == expected2.Z);
			Assert::IsTrue(firstNext);
			Assert::IsTrue(secondNext);
			Assert::IsFalse(thirdNext);
		}

		// For some reason this just cannot be run. Linker errors all over the place.

		//TEST_METHOD(AssertNumAttribsCorrectForWall)
		//{
		//	//Setup
		//	GroundPlane gp(10, 10);
		//	Wall wall(gp, 0,9, glm::vec3(2,3,4));
		//	std::string protocolstring = wall.ToProtocolString();
		//	char expectedCount = 2;
		//	char actual = 0;
		//	Protocol p(&protocolstring);

		//	//Calculate
		//	Numattribs numAttribs;
		//	p.GetData(&numAttribs);
		//	actual = p.Next() ? actual + 1 : actual;
		//	actual = p.Next() ? actual + 1 : actual;

		//	//Assert
		//	Assert::AreEqual(expectedCount, numAttribs.Value);
		//	Assert::AreEqual(expectedCount, actual);
		//	actual = p.Next() ? actual + 1 : actual;
		//	Assert::AreEqual(numAttribs.Value, actual);
		//	actual = p.Next() ? actual + 1 : actual;
		//	Assert::AreEqual(numAttribs.Value, actual);
		//}

	};
}