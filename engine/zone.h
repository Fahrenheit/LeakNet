#ifndef ZONE_H
#define ZONE_H
#pragma once

/*
 memory allocation


H_??? The hunk manages the entire memory block given to quake.  It must be
contiguous.  Memory can be allocated from either the low or high end in a
stack fashion.  The only way memory is released is by resetting one of the
pointers.

Hunk allocations should be given a name, so the Hunk_Print () function
can display usage.

Hunk allocations are guaranteed to be 16 byte aligned.

The video buffers are allocated high to avoid leaving a hole underneath
server allocations when changing to a higher video mode.

Cache_??? Cache memory is for objects that can be dynamically loaded and
can usefully stay persistant between levels.  The size of the cache
fluctuates from level to level.

To allocate a cachable object


Temp_??? Temp memory is used for file loading and surface caching.  The size
of the cache memory is adjusted so that there is a minimum of 512k remaining
for temp memory.


------ Top of Memory -------

high hunk allocations

<--- high hunk reset point held by vid

video buffer

z buffer

surface cache

<--- high hunk used

cachable memory

<--- low hunk used

client and server low hunk allocations

<-- low hunk reset point held by host

startup hunk allocations

Zone block

----- Bottom of Memory -----



*/

void Memory_Init (void);
void Memory_Shutdown( void );

void *Hunk_Alloc (int size);		// returns 0 filled memory
void *Hunk_AllocName (int size, const char *name);

void *Hunk_HighAllocName (int size, char *name);

int	Hunk_LowMark (void);
void Hunk_FreeToLowMark (int mark);

int	Hunk_HighMark (void);
void Hunk_FreeToHighMark (int mark);

void *Hunk_TempAlloc (int size);

void Hunk_Check (void);

int MEM_Summary_Console( void );

#endif // ZONE_H


