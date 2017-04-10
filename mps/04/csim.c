/*
 *  MPS/04 - CACHE SIMULATOR
 *
 *  NAME: Mayank Bansal
 *  EMAIL: mbansal5@hawk.iit.edu
 *
 *  WARNING: The code in this repo can potentially make you cry,
 *  Wilbur the safety pig is provided below for your benefit.
 *  Copy Wilbur the safety pig, wrap him in a comment and hope all goes well.
 *                             _
 *     _._ _..._ .-',     _.._(`))
 *    '-. `     '  /-._.-'    ',/
 *       )         \            '.
 *      / _    _    |             \
 *     |  a    a    /              |
 *     \   .-.                     ;
 *      '-('' ).-'       ,'       ;
 *         '-;           |      .'
 *            \           \    /
 *            | 7  .__  _.-\   \
 *            | |  |  ``/  /`  /
 *           /,_|  |   /,_/   /
 *              /,_/      '`-'
 *
 */

#include "cachelab.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef unsigned long long int ULLI;

typedef struct {
    int used, valid;
    ULLI tag;
} set_line_t;

typedef struct {
    set_line_t *lines;
} cache_set_t;

typedef struct {
    cache_set_t *sets;
} cache_t;

int hitCount = 0, missCount = 0, evictionCount = 0;
int lineCount, blockBitCount, setIndexBitCount;
cache_t cache;

void simulate(ULLI address) {

    int cacheFull = 1, LRU = 0, MRU = 0;

    // FIND SET INDEX & TAG INDEX
    int tagSize = (64 - (setIndexBitCount + blockBitCount));
    ULLI newTag = address >> (64 - tagSize);
    ULLI setIndex = address << tagSize >> (tagSize + blockBitCount);
    cache_set_t set = cache.sets[setIndex];

    // LOOK THROUGH SET FOR VALID RECORD WITH SEARCH TAG
    for (int i = 0; i < lineCount; i++) {
        if (!set.lines[i].valid) cacheFull = 0;
        else if (set.lines[i].tag == newTag) {
            set.lines[i].used++;
            hitCount++;
            return;
        }

        // MEANWHILE FIND LEAST & MOST RECENTLY USED RECORD
        if (set.lines[LRU].used > set.lines[i].used)
            LRU = i;
        else if (set.lines[MRU].used < set.lines[i].used)
            MRU = i;
    }

    /*********************************
     * CACHE MISS | TAG NOT IN CACHE *
     *********************************/

    //INIT NEW LINE TO INSERT WITH USED COUNT HIGHER THAN MOST RECENTLY USED
    set_line_t newLine = (set_line_t) {
            .tag = newTag, .valid = 1, .used = set.lines[MRU].used + 1
    };

    // IF CACHE IS FULL THEN EVICT LEAST RECENTLY USED
    if (cacheFull) {
        set.lines[LRU] = newLine;
        evictionCount++;
    } else
        //OTHERWISE FIND FIRST INVALID RECORD IN SET
        for (int i = 0; i < lineCount; i++)
            if (!set.lines[i].valid) {
                set.lines[i] = newLine;
                break;
            }

    missCount++;
}

int main(int argc, char **argv) {

    setIndexBitCount = atoi(argv[2]);
    lineCount = atoi(argv[4]);
    blockBitCount = atoi(argv[6]);
    FILE *parseFile = fopen(argv[argc - 1], "r");

    char instruction, buffer[256];
    ULLI address, setCount = pow(2, setIndexBitCount);
    int size;

    if (!parseFile) exit(0);

    /***************************************
     * ALLOCATE MEMORY FOR CACHE STRUCTURE *
     ***************************************/

    cache.sets = malloc(sizeof(cache_set_t) * setCount);

    for (int i = 0; i < setCount; i++) {
        cache.sets[i].lines = malloc(sizeof(set_line_t) * lineCount);
        for (int j = 0; j < lineCount; j++)
            cache.sets[i].lines[j] = (set_line_t) {
                    .valid = 0, .tag = 0, .used = 0
            };
    }

    /*******************************************
     * PARSE TRACE FILE AND EXECUTE SIMULATION *
     *******************************************/

    while (fgets(buffer, sizeof(buffer), parseFile))
        if (sscanf(buffer, " %c %llx,%d", &instruction, &address, &size) == 3) {
            if (instruction == 'L' || instruction == 'S')
                simulate(address);
            else if (instruction == 'M') {
                simulate(address);
                simulate(address);
            }
        }

    fclose(parseFile);

    /****************************
     * FREE THE CACHE STRUCTURE *
     ****************************/

    for (int i = 0; i < setCount; i++)
        free(cache.sets[i].lines);
    free(cache.sets);

    printSummary(hitCount, missCount, evictionCount);
    return 0;
}
