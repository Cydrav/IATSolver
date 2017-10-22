// by jasonfish4
#include <windows.h>

struct IMP_AT
{
	DWORD Size;
	PVOID Address;
};

IMP_AT GetIAT(LPCSTR ModuleName);

DWORD CalculateVirtualPageCount(IMP_AT IAT);

void ResolveIAT(LPCSTR FirstModule, LPCSTR CopyModule);

