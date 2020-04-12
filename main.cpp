#include "Memory.hpp"

int main()
{
    Memory *memory = new Memory();
    memory->initPage();
    // Support 16MB
    static const int memory_end = (16 * 1024 * 1024);
    static const int buffer_memory_end = (4 * 1024 * 1024);
    static const int main_memory_start = buffer_memory_end;

    memory->mem_init(main_memory_start, memory_end);

    // memory->print();
    if (memory != nullptr)
    {
        delete memory;
        memory = nullptr;
    }

    return 0;
}
