//Stolen entirely from https://github.com/Source2ZE/MovementUnlocker
//Edited for unlocking Lua
//With lots of help from Vauff & tilgep

#include <stdio.h>
#include "LuaUnlocker.h"
#include <sh_memory.h>
#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <dlfcn.h>
#endif

LuaUnlocker g_LuaUnlocker;

#ifdef _WIN32
const unsigned char *pPatchSignature = (unsigned char *)"\xBE\x01\x2A\x2A\x2A\x2B\xD6\x74\x2A\x3B\xD6";
const char *pPatchPattern = "xx???xxx?xx";
int offset = 2;
#elif __linux__
const unsigned char * pPatchSignature = (unsigned char *)"\x83\xFE\x01\x0F\x84\x2A\x2A\x2A\x2A\x83";
const char* pPatchPattern = "xxxxx????x";
int offset = 3;
#endif

// From https://git.botox.bz/CSSZombieEscape/sm-ext-PhysHooks
uintptr_t FindPattern(uintptr_t BaseAddr, const unsigned char* pData, const char* pPattern, size_t MaxSize, bool Reverse)
{
	unsigned char* pMemory;
	uintptr_t PatternLen = strlen(pPattern);

	pMemory = reinterpret_cast<unsigned char*>(BaseAddr);

	if (!Reverse)
	{
		for (uintptr_t i = 0; i < MaxSize; i++)
		{
			uintptr_t Matches = 0;
			while (*(pMemory + i + Matches) == pData[Matches] || pPattern[Matches] != 'x')
			{
				Matches++;
				if (Matches == PatternLen)
					return (uintptr_t)(pMemory + i);
			}
		}
	}
	else
	{
		for (uintptr_t i = 0; i < MaxSize; i++)
		{
			uintptr_t Matches = 0;
			while (*(pMemory - i + Matches) == pData[Matches] || pPattern[Matches] != 'x')
			{
				Matches++;
				if (Matches == PatternLen)
					return (uintptr_t)(pMemory - i);
			}
		}
	}

	return 0x00;
}

PLUGIN_EXPOSE(LuaUnlocker, g_LuaUnlocker);
bool LuaUnlocker::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

	char pBinPath[MAX_PATH];
#ifdef _WIN32
	V_snprintf(pBinPath, MAX_PATH, "%s%s", Plat_GetGameDirectory(), "/bin/win64/vscript.dll");
	auto *pBin = LoadLibrary(pBinPath);
#elif __linux__
	V_snprintf(pBinPath, MAX_PATH, "%s%s", Plat_GetGameDirectory(), "/bin/linuxsteamrt64/libvscript.so");
	auto *pBin = dlopen(pBinPath, RTLD_NOW);
#endif

	if (!pBin)
	{
		snprintf(error, maxlen, "Could not open %s", pBinPath);
		return false;
	}

#ifdef _WIN32
	uintptr_t pPatchAddress = (uintptr_t)GetProcAddress(pBin, "CreateInterface");
#elif __linux__
	uintptr_t pPatchAddress = (uintptr_t)dlsym(pBin, "CreateInterface");
#endif

	pPatchAddress = FindPattern(pPatchAddress, pPatchSignature, pPatchPattern, ULLONG_MAX, true);

	if (!pPatchAddress)
	{
		snprintf(error, maxlen, "Could not find VScript patch signature!");
		return false;
	}

	//Patch
	SourceHook::SetMemAccess((void*)(pPatchAddress + offset), 1, SH_MEM_READ | SH_MEM_WRITE | SH_MEM_EXEC);
	*(unsigned char*)(pPatchAddress + offset) = ((unsigned char*)"\x02")[0];
	SourceHook::SetMemAccess((void*)(pPatchAddress + offset), 1, SH_MEM_READ | SH_MEM_EXEC);

	META_CONPRINTF( "[Lua Unlocker] Successfully patched Lua Unlocker!\n" );

	return true;
}

bool LuaUnlocker::Unload(char *error, size_t maxlen)
{
	return true;
}

void LuaUnlocker::AllPluginsLoaded()
{
}

bool LuaUnlocker::Pause(char *error, size_t maxlen)
{
	return true;
}

bool LuaUnlocker::Unpause(char *error, size_t maxlen)
{
	return true;
}

const char * LuaUnlocker::GetLicense()
{
	return "GNU General Public License v3.0";
}

const char * LuaUnlocker::GetVersion()
{
	return "1.0";
}

const char * LuaUnlocker::GetDate()
{
	return __DATE__;
}

const char * LuaUnlocker::GetLogTag()
{
	return "LUAUNLOCKER";
}

const char * LuaUnlocker::GetAuthor()
{
	return "Hichatu";
}

const char * LuaUnlocker::GetDescription()
{
	return "Enables the use of the Lua VScripting language in CS2";
}

const char * LuaUnlocker::GetName()
{
	return "Lua Unlocker";
}

const char * LuaUnlocker::GetURL()
{
	return "https://github.com/Source2ZE/LuaUnlocker";
}
