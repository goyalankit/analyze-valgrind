#include "/work/02681/ankitg/workspace/valgrind/gleipnir/gleipnir.h"
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
#define ARRAY_TWO_BASE PG_START(PG_START(0xffeffd000) + ARRAY_ONE_SIZE + 1)

#define BIT_MASK(a, b) (size_t)(((size_t) -1 >> ((sizeof(size_t)*8) \
                             - (b))) & ~((1U << (a)) - 1))

#define CACHE_LINE(addr) (addr >> 6)

unsigned address_to_set(size_t address) {
    return ((address & BIT_MASK(6, 12)) >> 6);
}

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

    printf("Base address of allocated variable: %li\n", m_map);
    assert((void *)m_map == (void *)addr);

    return m_map;
}

void printSetAndCacheLine(int **array) {
    int i;
    for (i = 0; i < 4096; ++i) {
        printf("Set: %d, CL: %li\n", address_to_set(&array[i]), CACHE_LINE((size_t)&array[i]));
    }
}

void iterateToProduceTrace(int *array) {
GL_GLOBAL_START_INSTR;
    int i;
    for (i = 0; i < 4096; ++i) {
        array[i] = i;
    }
GL_GLOBAL_STOP_INSTR;
}

int main(int argc, char *argv[]) {

    unsigned long *addrOne    = allocateArray(ARRAY_ONE_BASE, ARRAY_ONE_SIZE);
  //  unsigned long *addrSecond = allocateArray(ARRAY_TWO_BASE, ARRAY_ONE_SIZE);

    //int *firstArray = (int *)addrOne;
    //int *secondArray = (int *)addrSecond;

    //printf("varinfo:a:4096:%08lx:%lu\n",(void *) &firstArray[0], sizeof(firstArray[0]));

//    iterateToProduceTrace(firstArray);
 //   iterateToProduceTrace(secondArray);
    //printf("---First Array\n---");
    //printSetAndCacheLine(firstArray);

    //printf("---Second Array\n---");
    //printSetAndCacheLine(secondArray);

    return 0;
}
