#include "Main.h"
#include "../RL/ReflectiveLoader.h"
#include  "atltime.h"

//#define Customer 252551516
// (252551516 is the hwid of the use)
//[enc_string_enable /]
//[junk_enable /]
DWORD WINAPI CheatEntry(LPVOID lpThreadParameter)
{
	HMODULE hModule = (HMODULE)lpThreadParameter;

	if (Engine::Initialize())
	{
		return 0;
	}

	return 0;
}

//UCHAR szFileSys[255], szVolNameBuff[255];
//DWORD dwMFL, dwSysFlags;
//DWORD dwSerial;
//LPCTSTR szHD = "C:\\";

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{

	if (fdwReason == DLL_PROCESS_ATTACH)

		//to use HWID protect remove the line beginnings '//' in each line of this .cpp file. Then use AVOZ HWID finder and replace 'putHWIDhere' with your HWID or multiple HWIDs
	{
		//CTime expirydatatime(2030, 0, 0, 0, 0, 0); //(year, month, days, hours, minutes, seconds)
		//CTime currentdatatime = CTime::GetCurrentTime();

		//if (currentdatatime >= expirydatatime)
		//{
		//	MessageBox(NULL, "Subscription Ended", "Box name", MB_OK);
		//	exit(0);
		//}

		//GetVolumeInformation(szHD, (LPTSTR)szVolNameBuff, 255, &dwSerial, &dwMFL, &dwSysFlags, (LPTSTR)szFileSys, 255);

		//if (dwSerial == HWID1, HWID2, HWID3)
		//{
		//	Sleep(100);
		//}
		//else
		//{
		//	// when HWID rejected
		//	MessageBox(NULL, "HWID doesn't match!", "fawn.ow", MB_OK);
		//	exit(0);
		//}


		DisableThreadLibraryCalls(hinstDLL);

		Client::BaseDir = CSX::Utils::GetHackWorkingDirectory().c_str();

		if (Client::BaseDir.size() < 1)
		{
			Client::BaseDir = CSX::Utils::GetModuleBaseDir(hinstDLL);
		}

#if ENABLE_DEBUG_FILE == 1
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);

		Client::LogFile = Client::BaseDir + "\\debug.log";
		DeleteFileA(Client::LogFile.c_str());
		CSX::Log::LogFile = Client::LogFile;
		printf("Client::BaseDir = %s\nClient::LogFile = %s\n", Client::BaseDir.c_str(), Client::LogFile.c_str());
		CSX::Log::Add("::Init::");

#endif

		CreateThread(0, 0, CheatEntry, hinstDLL, 0, 0);
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{
		Engine::Shutdown();
	}

	return TRUE;
}