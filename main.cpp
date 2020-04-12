#include "Memory.cpp"
#include "stdio.h"

int main()
{
	Memory *memory = new Memory();
	memory->initPage();
	memory->print();
	if (memory != nullptr)
	{
		delete memory;
		memory = nullptr;
	}

	return 0;
}
