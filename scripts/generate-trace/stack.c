#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#define PG_SIZE 4096
#define PG_START(_v) ((_v) & ~(unsigned long)(PG_SIZE-1))

// has to be aligned to page. otherwise it will be done for you.
#define ARRAY_ONE_BASE PG_START(0xffeffd000)
#define ARRAY_ONE_SIZE (4096 * 2 * sizeof(int))

unsigned long * allocateArray(unsigned long addr, size_t size) {

    int stack_prot =  PROT_READ | PROT_WRITE;
    int flags = MAP_SHARED | MAP_FIXED | MAP_ANONYMOUS;
    unsigned long *m_map;

    m_map = mmap((caddr_t)PG_START(addr), size, stack_prot, flags, -1, 0);

    // fail if mmap faield
    if (m_map == MAP_FAILED) {
        perror("mmap failed");
        abort();
    }

//    printf("Base address of allocated variable: %li\n", m_map);
    assert((void *)m_map == (void *)addr);

    return m_map;
}


int main(int argc, char *argv[]) {

    unsigned long *addrOne    = allocateArray(ARRAY_ONE_BASE, ARRAY_ONE_SIZE);

    return 0;
}
