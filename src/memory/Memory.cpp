/*
* Polygon-4 Engine
* Copyright (C) 2015 lzwdgc
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Polygon4/Memory.h>

//#include <tbb/scalable_allocator.h>

#include <memory>

/** Value we fill a memory block with after it is free, in UE_BUILD_DEBUG **/
#define DEBUG_FILL_FREED (0xdd)

/** Value we fill a new memory block with, in UE_BUILD_DEBUG **/
#define DEBUG_FILL_NEW (0xcd)

#ifdef _WIN32

decltype(malloc) *dll_alloc = nullptr;
decltype(free) *dll_free = nullptr;

/*void* Malloc(size_t Size)
{
    void* NewPtr = scalable_malloc(Size);
    memset(NewPtr, DEBUG_FILL_NEW, Size);
    return NewPtr;
}

void Free(void* Ptr)
{
    if (!Ptr)
        return;
    memset(Ptr, DEBUG_FILL_FREED, scalable_msize(Ptr));
    scalable_free(Ptr);
}*/


void* alloc_internal(size_t size)
{
    //return Malloc(size);
    return _aligned_malloc(size, size >= 16 ? 16 : 8);
    //return malloc(size);

    if (dll_alloc)
        return dll_alloc(size);
    return malloc(size);
}

void free_internal(void* ptr)
{
    //return Free(ptr);
    return _aligned_free(ptr);
    //return free(ptr);

    if (dll_free)
        return dll_free(ptr);
    free(ptr);
}

void* operator new      (size_t size)                                   { return alloc_internal(size); }
void* operator new[]    (size_t size)                                   { return alloc_internal(size); }
void* operator new      (size_t size, const std::nothrow_t&) throw()    { return alloc_internal(size); }
void* operator new[]    (size_t size, const std::nothrow_t&) throw()    { return alloc_internal(size); }
void  operator delete   (void* ptr)                                     { free_internal(ptr); }
void  operator delete[] (void* ptr)                                     { free_internal(ptr); }
void  operator delete   (void* ptr, const std::nothrow_t&) throw()      { free_internal(ptr); }
void  operator delete[] (void* ptr, const std::nothrow_t&) throw()      { free_internal(ptr); }

#define POLYGON4_INIT_MEMORY_FUNCTION Polygon4InitMemory
#define POLYGON4_INIT_MEMORY_FUNCTION_NAME "Polygon4InitMemory"

using POLYGON4_INIT_MEMORY_FUNCTION = void(__cdecl *)(void **, void **);

static const char *InitMemoryName = POLYGON4_INIT_MEMORY_FUNCTION_NAME;

#include <Windows.h>
#include <psapi.h>

POLYGON4_INIT_MEMORY_FUNCTION find_proc(const char *module)
{
    auto lib = LoadLibrary(module);
    if (!lib)
        return 0;
    return (POLYGON4_INIT_MEMORY_FUNCTION)GetProcAddress(lib, InitMemoryName);
}

bool try_load_mm()
{
    HMODULE hMods[1024];
    auto hProcess = GetCurrentProcess();
    DWORD cbNeeded;

    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        for (DWORD i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            TCHAR szModName[MAX_PATH];
            if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
            {
                auto proc = find_proc(szModName);
                if (proc)
                {
                    //proc((void **)&dll_alloc, (void **)&dll_free);
                    return true;
                }
            }
        }
    }
    return false;
}

// try to set name that will preceed any other symbol
// during initialization
#define LONG_UNDERSCORE __________________aaaaa_
#define UNDERSCORE(x) LONG_UNDERSCORE ## x

static
struct UNDERSCORE(init)
{
    UNDERSCORE(init)()
    {
        //try_load_mm();

        // Enable low fragmentation heap - http://msdn2.microsoft.com/en-US/library/aa366750.aspx
        intptr_t	CrtHeapHandle = _get_heap_handle();
        ULONG		EnableLFH = 2;
        HeapSetInformation((void*)CrtHeapHandle, HeapCompatibilityInformation, &EnableLFH, sizeof(EnableLFH));
    }
} UNDERSCORE(init);

/*__declspec(dllexport)
void __my_memory_manager()
{
}*/

#endif
