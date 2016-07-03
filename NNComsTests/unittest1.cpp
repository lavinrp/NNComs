#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\Code\src\Radio.h"
#include "../Code/src/GameDataReader.h"
#include "../Code/src/Point3D.h"

#include<Windows.h>
#include "FakeTs3Functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NNComsTests
{		
	TEST_CLASS(GameDataReaderTest)
	{
	public:
		
		TEST_METHOD(limitedConstructor) {
			Assert assert;

			GameDataReader gdr(GetTs3Functions());

			assert.AreEqual(gdr.isConnected(), false);
			assert.AreEqual(gdr.getContinueDataCollection(), true);
		}

		TEST_METHOD(beginTest) {
			GameDataReader gdr(GetTs3Functions());
			gdr.begin();

			sendFakeGameData();

			Assert selfIDCheck;
			selfIDCheck.AreEqual(1, (int)gdr.getSelfGameID());

			Assert playerCountTest;

			if (!gdr.getPlayer(51) || gdr.getPlayer(52)) {
				playerCountTest.Fail;
			}
		}

	private:

		//creates a pipe and sends fake game data across that pipe
		void sendFakeGameData() {
			//pipe
			LPTSTR pipeName = TEXT("\\\\.\\pipe\\gamePipe");
			HANDLE hPipe;

			hPipe = CreateNamedPipe(pipeName, PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE, 1, 0, 0, 0, NULL);

			//handle bad pipe creation
			if (!hPipe || hPipe == INVALID_HANDLE_VALUE) {
				Assert invalidHandle;
				invalidHandle.Fail;
			}

			//Connect to consumer
			bool successfulConnection = ConnectNamedPipe(hPipe, NULL);

			//handle bad connection
			Assert connectionSuccess;
			connectionSuccess.IsTrue(successfulConnection);


			//Send Game ID
			double gameIdData[1];
			gameIdData[0] = 1;
			DWORD bytesWritten;
			bool succcessfulWrite = WriteFile(
				hPipe,
				gameIdData,
				sizeof(gameIdData),
				&bytesWritten,
				NULL);
			Assert idSent;
			idSent.IsTrue(succcessfulWrite);

			//send players and radios
			sendPlayerAndRadioData(hPipe);

			//close pipe
			CloseHandle(hPipe);
		}


		//sends fake player and radio data across a passed pipe
		void sendPlayerAndRadioData(HANDLE& hPipe) {

			const unsigned int playerCount = 2;
			const unsigned int radioCount = 2;

			//send number of voice sources
			double voiceSourceCountsData[2] = {playerCount, radioCount};
			DWORD bytesWritten;
			bool succcessfulWrite = WriteFile(
				hPipe,
				voiceSourceCountsData,
				sizeof(voiceSourceCountsData),
				&bytesWritten,
				NULL);
			Assert voiceSourceCountSent;
			voiceSourceCountSent.IsTrue(succcessfulWrite);


			//send radio data
			//position changes with each radio. First radio is off. all others are on.
			for (unsigned int i = 0; i < radioCount; i++) {
				double radioData[8] = { i + 1, i + 2, i + 3, 1, 1, 1, i, i };

				//write data
				bool succcessfulWrite = WriteFile(
					hPipe,
					radioData,
					sizeof(radioData),
					&bytesWritten,
					NULL);
			}


			//send player data
			//position changes with each player. ID starts at 50 then moves to 50+i
			for (unsigned int i = 0; i < playerCount; i++) {
				double playerData[6] = { i + 1, i + 2, i + 3, 1, -1, i + 50 };

				bool succcessfulWrite = WriteFile(
					hPipe,
					playerData,
					sizeof(playerData),
					&bytesWritten,
					NULL);
			}

		}

	};
}