#include <algorithm>
#include <iostream>

class Memory
{
  public:
    Memory();
    ~Memory();
    void initPage();
    void mem_init(size_t, size_t);
    void setPgDirItem(const int, const uint32_t);
    void setPgItem(const int, const int, const uint32_t);

    uint8_t *get_free_page();
    void free_page(size_t);
	uint8_t* put_page();
    void print();

  private:
    inline const size_t MAP_NR(size_t) const;
    inline uint8_t *MAP_N(size_t) const;

    static const int m_memoryPageDirectoryNum = 1;
    static const int m_memoryPageTableNum = 4;
    static const int m_memoryPageTableItemSize = 4 * 1024;                // 4KB
    static const int m_memorySize = 4 * 1024 * m_memoryPageTableItemSize; // 16MB
    static const int LOW_MEM = 0x100000;                                  // Lowest location of Main Memory
    static const int PAGING_MEMORY = (15 * 1024 * 1024);                  // Main Memory can occupy up to 15MB
    static const int PAGING_PAGES = (PAGING_MEMORY >> 12);
    static const int USED = 100;
    // length is PAGING_PAGES, set its address is m_memory + 5 * m_memoryPageTableItemSize
    // in fact, the address can be random.
    uint8_t *mem_map;
    // physical memory highest location
    long HIGH_MEMORY;

    int CR3;
    uint8_t *m_memory;
    uint8_t *m_pgDir;
    uint8_t *m_pg[4];
};
