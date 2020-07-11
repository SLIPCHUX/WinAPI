/* GameLord.cpp of "DENYALORD" | slipchux@gmail.com */
/* SLIPCHUK START BLOCK */

// Including the header of the class
#include "GameLord.h"
// Helper includes
#include <TlHelp32.h>
#include "ObjectsOffsets.h"
#include "Glow.h"
// Defines
#define COUNT_PLAYERS 32
#define PLAYER_HEALTH 100

// Constructor by Default 
GameLord::GameLord()
	: windowName("Counter-Strike: Global Offensive"), moduleName("client.dll"), isActiveWall(false), isActiveTrigger(false), isActiveFlash(false)
{
	hWND = FindWindowA(NULL, windowName);
	GetWindowThreadProcessId(hWND, &processID);
	moduleBase = GetModuleBaseAddress(moduleName);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, processID);
}

// Constructor with parameters
GameLord::GameLord(bool isActiveWall, bool isActiveTrigger, bool isActiveFlash)
	: GameLord()
{
	this->isActiveWall = isActiveWall;
	this->isActiveTrigger = isActiveTrigger;
	this->isActiveFlash = isActiveFlash;

	Tick();
}

// Destrcutor
GameLord::~GameLord(){ }

// Event Tick()
void GameLord::Tick()
{
	while (!GetAsyncKeyState(VK_END))												// Working while END-key isn't pressed
	{
		// WHALLHACK BLOCK
		if (isActiveWall)													
		{
			uintptr_t dwGlowManager = RPM<uintptr_t>(moduleBase + dwGlowObjectManager);	// Getting address of Game's GlowManager
			int teamID = RPM<int>(GetLocalPlayer() + m_iTeamNum);						// Getting ID of player's team 

			for (size_t i = 0; i < COUNT_PLAYERS; ++i)
			{
				uintptr_t dwEntity = RPM<uintptr_t>(moduleBase + dwEntityList + i * 0x10);
				int iGlowIndx = RPM<int>(dwEntity + m_iGlowIndex);
				int EnmHealth = RPM<int>(dwEntity + m_iHealth); if (EnmHealth < 1 || EnmHealth > PLAYER_HEALTH) continue;
				int Dormant = RPM<int>(dwEntity + m_bDormant); if (Dormant) continue;
				int EntityTeam = RPM<int>(dwEntity + m_iTeamNum);


				if (teamID == EntityTeam)
				{
					WPM<glowStructLocal>(dwGlowManager + (iGlowIndx * 0x38) + 0x4, glowLocal);
				}
				else if (teamID != EntityTeam)
				{
					WPM<glowStructEnemy>(dwGlowManager + (iGlowIndx * 0x38) + 0x4, glowEnm);
				}
			}
		}
		// TRIGGERBOT BLOCK
		if (isActiveTrigger)
		{
			int crosshairID = RPM<int>(GetLocalPlayer() + m_iCrosshairId);
			int crosshairTeam = GetTeam(GetPlayer(crosshairID - 1));
			int teamID = RPM<int>(GetLocalPlayer() + m_iTeamNum);

			if (crosshairID > 0 && crosshairID < COUNT_PLAYERS && teamID != crosshairTeam)
			{
				if (GetAsyncKeyState(VK_MENU))
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
					Sleep(100);
				}
			}
		}
		// ANTI-FLASH BLOCK
		if (isActiveFlash)
		{
			int flashDuration = RPM<int>(GetLocalPlayer() + m_flFlashDuration);

			if (flashDuration > 0)
			{
				WPM<int>(GetLocalPlayer() + m_flFlashDuration, 0);
			}
		}

	}
}

// Returns base adress of a module by name
// Input: moduleName - name of the module
// Output: address of module 
uintptr_t GameLord::GetModuleBaseAddress(const char* moduleName)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->processID);
	if (hSnap != INVALID_HANDLE_VALUE) 
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) 
		{
			do 
			{
				if (!strcmp(modEntry.szModule, moduleName)) 
				{
					CloseHandle(hSnap);
					return (uintptr_t)modEntry.modBaseAddr;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
}

// Returns address of local player
uintptr_t GameLord::GetLocalPlayer()
{
	return RPM<uintptr_t>(moduleBase + dwLocalPlayer);
}

uintptr_t GameLord::GetPlayer(const int index)
{
	return RPM<uintptr_t>(moduleBase + dwEntityList + index * 0x10);
}

int GameLord::GetTeam(const uintptr_t player)
{
	return RPM<int>(player + m_iTeamNum);
}

// Read process memory (getting game's data by address)
// Input: address - address wich we want to read from the game
// Output: buffer with data
template<typename T>
T GameLord::RPM(SIZE_T address)
{
	T buffer;
	ReadProcessMemory(this->hProcess, (LPCVOID)address, &buffer, sizeof(T), NULL);
	return buffer;
}

template<typename T>
void GameLord::WPM(SIZE_T address, T buffer)
{
	WriteProcessMemory(hProcess, (LPVOID)address, &buffer, sizeof(buffer), NULL);
}

/* SLIPCHUK END BLOCK */