#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef unsigned long long int mem_addr_t;

typedef struct {
    int valid;
    mem_addr_t tag;
    int timestamp;
} line_st;

typedef struct {
    line_st *lines;
} cache_set;

typedef struct {
    cache_set *sets;
} cache_t;

int main(int argc, char *argv[]) {

    /* PRINT DEBUG */
    printf("Arguments: %d\n", argc);

    int setIndexBits = atoi(argv[2]);
    int associativity = atoi(argv[4]);
    int blockBits = atoi(argv[6]);

    int hit_count = 0;
    int miss_count = 0;
    int eviction_count = 0;

    cache_t cache;

    /* PRINT DEBUG */
    printf("Set Index Bits: %d\n", setIndexBits);
    printf("Associativity: %d\n", associativity);
    printf("Block Bits: %d\n", blockBits);

    /* ALLOCATE MEMORY FOR CACHE LINES & SETS */
    cache.sets = malloc(setIndexBits * sizeof(cache_set));
    int i =0;
    while(setIndexBits--)
        cache.sets[i].lines = malloc(sizeof(line_st) * associativity);

    printf("%lu",sizeof(cache.sets));

    char *parseFileName = argv[argc-1];
    FILE *parseFile = fopen(parseFileName,"r");

    if(!parseFile) {
        printf("File could not be opened");
        exit(0);
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), parseFile)) {
        unsigned int address;
        char instr;
        int mem;

        sscanf(buffer, "%c", &instr);

        if(instr!=' ')
            sscanf(buffer, " %04x,%d", &address, &mem);
        else
            sscanf(buffer, " %c %04x,%d", &instr, &address, &mem);

        printf("%c %04x,%d\n", instr, address, mem);
    }

    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}
