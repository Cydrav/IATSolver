#include "iatresolve.h"

struct IMP_AT
{
	DWORD Size;
	PVOID Address;
};

IMP_AT GetIAT(LPCSTR ModuleName)
{
	HMODULE mod = GetModuleHandleA(ModuleName);

	PIMAGE_DOS_HEADER img_dos_headers = (PIMAGE_DOS_HEADER)mod;
	PIMAGE_NT_HEADERS img_nt_headers = (PIMAGE_NT_HEADERS)((BYTE*)img_dos_headers + img_dos_headers->e_lfanew);

	IMP_AT Retn;
	Retn.Size = (img_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size);
	Retn.Address = (PVOID)((DWORD)mod + img_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress);
	return Retn;
}

DWORD CalculateVirtualPageCount(IMP_AT IAT)
{
	MEMORY_BASIC_INFORMATION MemoryBasicInformation;
	VirtualQuery(IAT.Address, &MemoryBasicInformation, sizeof(MemoryBasicInformation));

	DWORD Precedent = MemoryBasicInformation.RegionSize;

	if (IAT.Size < Precedent)
		return 1;

	return (IAT.Size / Precedent);
}


void ResolveIAT(LPCSTR FirstModule, LPCSTR CopyModule)
{
	IMP_AT OriginalModuleIAT = GetIAT(FirstModule);
	IMP_AT CopyModuleIAT = GetIAT(CopyModule);
	DWORD VPC = CalculateVirtualPageCount(OriginalModuleIAT);
	DWORD OldProtect;

	VirtualProtect(CopyModuleIAT.Address, VPC, PAGE_EXECUTE_READWRITE, &OldProtect);
	memcpy(CopyModuleIAT.Address, OriginalModuleIAT.Address, OriginalModuleIAT.Size);
	VirtualProtect(CopyModuleIAT.Address, VPC, OldProtect, &OldProtect);
}
