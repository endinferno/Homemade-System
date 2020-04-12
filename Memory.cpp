#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Memory
{
	public:
		Memory()
		{
			m_memory = new uint8_t[m_memorySize];
			m_pgDir = m_memory;
			m_pg[0] = m_memory + m_memoryPageTableItemSize;
			m_pg[1] = m_memory + 2 * m_memoryPageTableItemSize;
			m_pg[2] = m_memory + 3 * m_memoryPageTableItemSize;
			m_pg[3] = m_memory + 4 * m_memoryPageTableItemSize;
		}
		~Memory()
		{
			if (m_memory != nullptr)
			{
				delete[] m_memory;
				m_memory = nullptr;
			}
		}

		void initPage()
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
			cout << addr << endl;
		}

		// 4 Bytes
		void setPgDirItem(const int ord, const uint32_t value)
		{
			*((uint32_t *) (m_pgDir) + ord) = (uint32_t) value;
		}
		// 4 Byte
		void setPgItem(const int page_ord, const int ord, const uint32_t value)
		{
			if (page_ord >= m_memoryPageTableNum)
				return;
			*((uint32_t *) (m_pg[page_ord]) + ord) = (uint32_t) value;
		}

		void print()
		{
			ofstream outfile;
			outfile.open("file.dat", ios::out | ios::trunc);
			for (int i=0;i<m_memorySize / 4;++i)
				outfile << ((uint32_t*)m_memory)[i] << endl;
			outfile.close();
		}

	private:
		static const int m_memoryPageDirectoryNum = 1;
		static const int m_memoryPageTableNum = 4;
		static const int m_memoryPageTableItemSize = 4 * 1024;                // 4KB
		static const int m_memorySize = 4 * 1024 * m_memoryPageTableItemSize; // 16MB
		static const int LOW_MEM = 0x100000;
		int CR3;
		uint8_t *m_memory;
		uint8_t *m_pgDir;
		uint8_t *m_pg[4];
};
