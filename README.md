# IATSolver
Resolves the correct Import Address Table for copy modules

For specific reasons, reverse engineers and others may want to load an identical library/module into a process.

```
(Process.exe)
(Process2.exe) - Same file as Process.exe

Inject Process2.exe -> Process.exe
```

One reason for doing this may be to bypass memory integrity checks where:

```
Process.function_one:
push ebp -> (change to: jmp Process2.function_one)
mov ebp, esp
...code... w/ call dword ptr [xxxxxxxx]
mov esp, ebp
pop ebp
retn
```
```
Process2.function_one:
push ebp
mov ebp, esp
...code... w/ call dword ptr [xxxxxxxx]
mov esp, ebp
pop ebp
retn
```
This jump or "hook" jumps Process2.function_one which is an identical function; however, the injection of Process2 loads its own Import Address Table and doesn't resolve correctly, making calls direct to invalid or unmapped memory locations. This set of functions fixes that problem.

**struct IMP_AT** - 
Contains the size of the Import Address table and the base address of it

**IMP_AT GetIAT(LPCSTR ModuleName)** -
Returns two elements of information about the Import Address Table in a struct of the module given, the size of the table and the base address of it.

**DWORD CalculateVirtualPageCount(IMP_AT IAT)** - 
Calculates the number of virtual memory pages (or regions) that the size in bytes the Import Address takes up.

**void ResolveIAT(LPCSTR FirstModule, LPCSTR CopyModule)** -
Resolves/corrects the Import Address Table for the CopyModule.
