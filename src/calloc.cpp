#include <calloc.hpp>
#include <new>

#include <iostream>

static mem_block* HEAP_START = nullptr;

void* calloc(size_t size) {
	if (!HEAP_START) {
		HEAP_START = _callocVirtualMemory(nullptr, size);
		if (!HEAP_START) return nullptr;
	}

	mem_block* valid_block = getValidMemoryBlock(size);
	if (!valid_block) return nullptr;

	valid_block->Free = false;
	splitMemoryBlock(valid_block, size);

	return valid_block->Memory;
}

void cfree(void* pointer) {
	mem_block* block = findMemoryBlockAtAdress(pointer);
	if (!block) return;

	block->Free = true;
}

mem_block* _callocMemBlock(size_t size) {
	void* ptr = calloc(size);
	if (!ptr) return nullptr;

	return reinterpret_cast<mem_block*>(static_cast<std::byte*>(ptr) - MEM_BLOCK_SIZE);
}

mem_block* getValidMemoryBlock(size_t size) {
	mem_block* current_block = HEAP_START;

	while (	(!current_block->Free || size > current_block->Size) &&
			current_block->Next != nullptr) {
		current_block = current_block->Next;
	}

	if (!current_block->Free || size > current_block->Size) {
		mem_block* new_memory = _callocVirtualMemory(nullptr, size);

		current_block->Next = new_memory;
		current_block = new_memory;
	}

	return current_block;
}

mem_block* splitMemoryBlock(mem_block* adress, size_t size) {
	mem_block* old_adress_next_ptr = adress->Next;
	
	size_t new_block_size = adress->Size - size;
	if (new_block_size <= MEM_BLOCK_SIZE) return nullptr;

	mem_block* new_block = _allocateMemoryBlock(adress->Memory + size, new_block_size);

	if (!new_block) {
		return nullptr;
	}

	adress->Size = size;
	adress->Next = new_block;

	new_block->Next = old_adress_next_ptr;

	return new_block;
}

mem_block* findMemoryBlockAtAdress(void* adress) {
	mem_block* current_block = HEAP_START;

	while (current_block != nullptr) {
		if (current_block->Memory == adress) return current_block;

		current_block = current_block->Next;
	}

	return nullptr;
}

size_t getMemoryPageSize() {
	SYSTEM_INFO sysInfo{};
	GetSystemInfo(&sysInfo);
	return sysInfo.dwPageSize;
}

size_t MEM_PAGE_SIZE = 0;
mem_block* _callocVirtualMemory(LPVOID ipAdress, size_t dwSize) {

	if (MEM_PAGE_SIZE == 0) MEM_PAGE_SIZE = getMemoryPageSize();
	size_t aligned_size = ((dwSize + MEM_PAGE_SIZE - 1) / MEM_PAGE_SIZE) * MEM_PAGE_SIZE;

	LPVOID allocated_ptr = VirtualAlloc(ipAdress, aligned_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	if (!allocated_ptr) {
		return nullptr;
	}

	std::cout << "Allocating memory via Virtual, Allocated: " << aligned_size << " bytes.\n";
	
	mem_block* new_block = _allocateMemoryBlock(allocated_ptr, aligned_size);

	return new_block;
}

mem_block* _allocateMemoryBlock(void* ipAdress, size_t dwSize) {

	// Not enough size to allocate.
	if (dwSize < MEM_BLOCK_SIZE) {
		return nullptr;
	}

	mem_block* new_block = static_cast<mem_block*>(ipAdress);

	new_block->Free = true;
	new_block->Size = dwSize - MEM_BLOCK_SIZE;
	new_block->Next = nullptr;
	new_block->Memory = static_cast<std::byte*>(ipAdress) + MEM_BLOCK_SIZE;

	return new_block;
}