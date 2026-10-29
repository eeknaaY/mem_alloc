#include <calloc.hpp>

int main()
{
    // Allocates memory from VirtualAllocation(), minimum at 1 page. (4096B, depending on system)
    void* ptr = calloc(64);
    cfree(ptr);

    // Can allocate all these without requesting new memory.
    for (int i = 0; i < 32; i++) ptr = calloc(128);
    // (Yes I know it's a memory leak.)
    
    // Allocates more raw memory.
    ptr = calloc(1);

    return 0;
}
