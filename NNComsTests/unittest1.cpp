#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\Code\src\Radio.h"
#include "../Code/src/GameDataReader.h"
#include "../Code/src/Point3D.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NNComsTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Point3D testPoint;
			Assert assert;
			assert.AreEqual(testPoint.getX(), 0.0);
		}

	};
}