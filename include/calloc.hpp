#ifndef CALLOC_HPP
#define CALLOC_HPP

#include <Windows.h>
#include <cstddef>
#include <memoryapi.h>

typedef struct memory_block {
public:
	bool Free = false;
	size_t Size;

	memory_block* Next = nullptr;

	std::byte* Memory = nullptr;
} mem_block;

constexpr size_t MEM_BLOCK_SIZE = sizeof(mem_block);

/*
 * Allocates a block of memory.
 *
 * @param size Number of bytes to allocate.
 * 
 * @return Pointer to the allocated memory, or nullptr if allocation fails.
 */
void* calloc(size_t size);

/*
* Deallocates the given object.
* 
* @param pointer Adress of the object to be deallocated.
*/
void cfree(void* pointer);

/*
* Returns the memory block of the allocated memory.
* 
* @param size Number of bytes to allocate.
* 
* @return Pointer to memory block of the allocated memory.
*/
mem_block* _callocMemBlock(size_t size);

/*
* Gets the first memory block with sufficient storage.
* 
* @param size Size of required storage.
* 
* @return Pointer to a valid memory block.
*/
mem_block* getValidMemoryBlock(size_t size);

/*
* Splits the given memory block.
* 
* @param adress Adress of the memory block to be split
* @param size Size of required space for the memory block about to be split.
* 
* @return Pointer to a fresh memory block.
*/
mem_block* splitMemoryBlock(mem_block* adress, size_t size);

/*
* Finds the memory block at given adress.
* 
* @param adress Adress of the memory block.
* 
* @return Pointer to memory block, null if not found.
*/
mem_block* findMemoryBlockAtAdress(void* adress);

/*
* Retrieves the memory page size.
* 
* @return Memory page size.
*/
size_t getMemoryPageSize();

/*
 * Allocates a block of memory via VirtualMemory.
 *
 * @param ipAdress Location to allocate, use nullptr to handle automatically.
 * @param dwSize Number of bytes to allocate.
 * @param flAllocationType Memory allocation type.
 * @param flProtect Memory protection type.
 *
 * @return Pointer to new memory block, or nullptr if allocation fails.
 */
mem_block* _callocVirtualMemory(LPVOID ipAdress, size_t dwSize);

/*
* Creates a memory block from raw memory.
* 
* @param ipAdress Pointer to memory.
* @param dwSize Size of the allocated memory.
* 
* @return Pointer to a memory block.
*/
mem_block* _allocateMemoryBlock(void* ipAdress, size_t dwSize);


#endif