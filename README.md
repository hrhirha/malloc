# Malloc

## What is malloc?

`malloc()` is a function that is used to dynamically allocate a block of memory with the specified size at run time. It returns a pointer of type void which can then be typecasted into any type of pointer.

Malloc uses a `Linked List` to store blocks of memory and keep track of their state.

## What malloc() does:

1. Memory Management and Segmentation: create blocks of memory.
2. Linear Search and Allocation: find free blocks.
3. Intelligent Memory Optimization: split blocks to reduse waste.
4. Pointer Provision: provide a precise address.

## What free() does:

1. Block Identification: find block to free.
2. Memory Merging: solve fragmentation problem.
3. validate the pointer (was the pointer malloced?)
  * mallocfreefree(): invalid pointer
4. manage double free
  * mallocfreefree(): double free detected.
5. invalid header (size is corrupted / not 16 aligned)
  * munmap_chunk(): invalid pointer


## Placement strategies: Finding available chunk of memory:

1. First Fit : The first big enough block.
2. Best Fit  : Traverse the whole list and find the most suitable block.
3. Next Fit  : Same as `First Fit` but remembers where it stoped the last time.

[The knapsack problem](https://en.wikipedia.org/wiki/Knapsack_problem)


## Pre-allocated zones

Tiny heap [1-32]

* TINY_ZONE_SIZE  = 32
* pre-allocate: 1 * PAGE_SIZE

Small heap [32-128]

* SMALL_ZONE_SIZE = 128
* pre-allocate: 4 * PAGE_SIZE

## Algorithm

### malloc

Align size to 16.
loop(zones with the apropriate type):
	if (free block with anough room):
		if (size can hold more then on block):
			split block
	elif (free space in zone): create new block
if (no zones or zones with the apropriate type are full):
	create new zone
	create new block

### free

if (ptr is null):
	do nothing
find the apropriate zone
if (ptr out of range):
	error()
set the block as free and set prev and next pointers
if (the previous block or the next block are free):
	merge free blocks
if (zone is empty):
	unmap it

### realloc

if (ptr in null):
	malloc()
if (new size > size and enough space):
	expand block
if (new size < size and enough space for another block):
	split block
malloc new block woth enough space
copy data
free old block
return new block

## Off topic

Page tables (9 bits each):

1. PGD (0xac)  : Page Global Directory.
2. PUD (0x117) : Page Upper Directory.
3. PMD (0xf1)  : Page Middle Directory.
4. PTE (0x194) : Page Table Enteries.
