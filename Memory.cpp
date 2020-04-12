#include "Memory.hpp"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

Memory::Memory()
{
    m_memory = new uint8_t[m_memorySize];
    m_pgDir = m_memory;
    m_pg[0] = m_memory + 1 * m_memoryPageTableItemSize;
    m_pg[1] = m_memory + 2 * m_memoryPageTableItemSize;
    m_pg[2] = m_memory + 3 * m_memoryPageTableItemSize;
    m_pg[3] = m_memory + 4 * m_memoryPageTableItemSize;
    mem_map = m_memory + 5 * m_memoryPageTableItemSize;
}
Memory::~Memory()
{
    if (m_memory != nullptr)
    {
        delete[] m_memory;
        m_memory = nullptr;
    }
}

void Memory::initPage()
{
    // clear the memory of 5 Pages
    fill_n(m_memory, 5 * m_memoryPageTableItemSize, 0x00);
    // fill in 4 Page directory table items
    setPgDirItem(0, 0x00001007);
    setPgDirItem(1, 0x00002007);
    setPgDirItem(2, 0x00003007);
    setPgDirItem(3, 0x00004007);
    // fill in Page table
    int page_ord = 3, ord = 1023;
    int32_t addr = 0xFFF007;
    while (page_ord >= 0)
    {
        while (ord >= 0)
        {
            setPgItem(page_ord, ord, addr);
            addr -= 0x1000;
            if (addr < 0)
                break;
            ord--;
        }
        if (addr < 0)
            break;
        ord = 1023;
        page_ord--;
    }
    fill_n(mem_map, PAGING_PAGES, 0x00);
}

void Memory::mem_init(size_t start_mem, size_t end_mem)
{
    int i;
    HIGH_MEMORY = end_mem;
    for (i = 0; i < PAGING_PAGES; ++i)
        mem_map[i] = USED;
    i = MAP_NR(start_mem);

    end_mem -= start_mem;
    end_mem >>= 12;
    while (end_mem-- > 0)
        mem_map[i++] = 0;
}

// this address is real address, you can access the address by this pointer
uint8_t *Memory::get_free_page()
{
    int i = PAGING_PAGES - 1;
    // Find empty page in reverse
    for (i = PAGING_PAGES - 1; i >= 0; --i)
    {
        if (mem_map[i] == 0)
            break;
    }
    // No empty page found
    if (i == 0)
        return nullptr;
    // Get real physical address
    uint8_t *real_physics_address = MAP_N(i);
    // Empty memory in reverse
    for (int j = 4096 - 1; j >= 0; --j)
        real_physics_address[j] = 0x00;
    return real_physics_address;
}

// this addr is an addr relative to the m_memory, which is linear address
void Memory::free_page(size_t addr)
{
    if (addr < LOW_MEM)
    {
        return;
    }
    if (addr >= HIGH_MEMORY)
    {
        std::out_of_range("trying to free nonexistent page");
    }
    addr -= LOW_MEM;
    addr >>= 12;
	if (mem_map[addr] != 0)
	{
		mem_map[addr]--;
		return;
	}
	mem_map[addr] = 0;
	std::out_of_range("trying to free free page");
}

// 4 Bytes
void Memory::setPgDirItem(const int idx, const uint32_t value)
{
    *((uint32_t *) (m_pgDir) + idx) = (uint32_t) value;
}

// 4 Byte
void Memory::setPgItem(const int page_ord, const int ord, const uint32_t value)
{
    if (page_ord >= m_memoryPageTableNum)
        return;
    *((uint32_t *) (m_pg[page_ord]) + ord) = (uint32_t) value;
}

void Memory::print()
{
    ofstream outfile;
    outfile.open("file.dat", ios::out | ios::trunc);
    for (int i = 0; i < m_memorySize / 4; ++i)
        outfile << ((uint32_t *) m_memory)[i] << endl;
    outfile.close();
}

inline const size_t Memory::MAP_NR(size_t addr) const
{
    return ((addr - LOW_MEM) >> 12);
}

inline uint8_t *Memory::MAP_N(size_t idx) const
{
    if (idx >= 3840)
        throw std::out_of_range("mem_map index out of range");
    return LOW_MEM + (idx << 12) + m_memory;
}
