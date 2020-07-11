/* GameLord.h of "DENYALORD" | slipchux@gmail.com */
/* SLIPCHUK START BLOCK */
#pragma once
#include <windows.h>


class GameLord
{
public:
	// Constructor by Default 
	GameLord();

	// Constructor with parameters
	GameLord(bool isActiveWall, bool isActiveTrigger, bool isActiveFlash);

	// Destrcutor
	virtual ~GameLord();

	// Event Tick()
	virtual void Tick();

private:
	// Read process memory
	// Input: address - address wich we want to read from the game
	// Output: buffer with data
	template<typename T>
	T RPM(SIZE_T address);

	// Write process memory
	// Input: address - where want writting; buffer - what want writting
	template<typename T>
	void WPM(SIZE_T address, T buffer);

	// Returns base adress of a module by name
	// Input: moduleName - name of the module
	// Output: address of module 
	uintptr_t GetModuleBaseAddress(const char* moduleName);

	// Returns address of local player
	uintptr_t GetLocalPlayer();

	// Returns player by index
	// Input: index - index of player
	uintptr_t GetPlayer(const int index);

	// Returns ID of player's team by index
	// Input: player - address of player
	int GetTeam(const uintptr_t player);

private:
	HWND hWND;
	HANDLE hProcess;
	DWORD processID;
	uintptr_t moduleBase;

	const char* windowName;
	const char* moduleName;

	bool isActiveWall;
	bool isActiveTrigger;
	bool isActiveFlash;
};
/* SLIPCHUK END BLOCK */