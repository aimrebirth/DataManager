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

#include <memory>

#if defined(_WIN32) && defined(DATA_MANAGER_ALIGNED_ALLOCATOR)

void* alloc_internal(size_t size)
{
    return _aligned_malloc(size, size >= 16 ? 16 : 8);
}

void free_internal(void* ptr)
{
    return _aligned_free(ptr);
}

void* operator new      (size_t size)                                   { return alloc_internal(size); }
void* operator new[]    (size_t size)                                   { return alloc_internal(size); }
void* operator new      (size_t size, const std::nothrow_t&) throw()    { return alloc_internal(size); }
void* operator new[]    (size_t size, const std::nothrow_t&) throw()    { return alloc_internal(size); }
void  operator delete   (void* ptr)                                     { free_internal(ptr); }
void  operator delete[] (void* ptr)                                     { free_internal(ptr); }
void  operator delete   (void* ptr, const std::nothrow_t&) throw()      { free_internal(ptr); }
void  operator delete[] (void* ptr, const std::nothrow_t&) throw()      { free_internal(ptr); }

// try to set name that will preceed any other symbol
// during initialization
#define LONG_UNDERSCORE __________________aaaaa_
#define UNDERSCORE(x) LONG_UNDERSCORE ## x

static
struct UNDERSCORE(init)
{
    UNDERSCORE(init)()
    {
        // Enable low fragmentation heap - http://msdn2.microsoft.com/en-US/library/aa366750.aspx
        intptr_t	CrtHeapHandle = _get_heap_handle();
        ULONG		EnableLFH = 2;
        HeapSetInformation((void*)CrtHeapHandle, HeapCompatibilityInformation, &EnableLFH, sizeof(EnableLFH));
    }
} UNDERSCORE(init);

#endif
