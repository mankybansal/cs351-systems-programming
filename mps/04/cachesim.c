#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
    int num_lines = atoi(argv[1]),
        lines_per_set = atoi(argv[2]),
        bytes_per_block = atoi(argv[3]);

    char line[80];

    long addr_req;

    printf("Simulating cache with:\n");
    printf(" - Total lines   = %d\n", num_lines);
    printf(" - Lines per set = %d\n", lines_per_set);
    printf(" - Block size    = %d bytes\n", bytes_per_block);

    int n = 0;
    while (fgets(line, 80, stdin)) {
        addr_req = strtol(line, NULL, 0);
        n++;

        /* simulate cache fetch with address `addr_req` */
        printf("Processing request: 0x%lX\n", addr_req);
    }
    return 0;
}
