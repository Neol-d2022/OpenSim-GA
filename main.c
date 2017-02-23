#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "population.h"

int main(int argc, char **argv)
{
    char buf[64];
    FILE *nodeFile;
    WirelessNodes_t wnodes;
    WirelessNode_t nodeNew;
    Conns_t conns;
    population_t pop;
    unsigned int maxRetransmitTimes = 4; //首次嘗試 + 重傳3次
    unsigned int popSize = 100;

    if (argc != 3)
    {
        return 1;
    }

    // Round 1
    nodeFile = fopen(argv[1], "rb");
    if (!nodeFile)
    {
        return 2;
    }

    wnode_init(&wnodes);
    while (fread(buf, 1, ADDR_LENGTH + 1, nodeFile) == ADDR_LENGTH + 1)
    {
        memcpy(nodeNew.addr, buf, ADDR_LENGTH);
        if (buf[ADDR_LENGTH])
            nodeNew.isRoot = 1;
        else
            nodeNew.isRoot = 0;

        wnode_addNode(&nodeNew, &wnodes);
    }
    if (!feof(nodeFile))
    {
        return 3;
    }
    fclose(nodeFile);

    nodeFile = fopen(argv[2], "rb");
    if (!nodeFile)
    {
        return 4;
    }

    // Round 2
    conn_init(&conns);
    while (fread(buf, 1, ADDR_LENGTH * 2 + sizeof(double), nodeFile) == (ADDR_LENGTH * 2 + sizeof(double)))
    {
        conn_setPdr(&conns, &wnodes, (unsigned char *)(buf + 0), (unsigned char *)(buf + ADDR_LENGTH), *(double *)(buf + ADDR_LENGTH * 2));
    }
    if (!feof(nodeFile))
    {
        return 6;
    }
    fclose(nodeFile);

    srand(time(0) + clock());
    population_init(&pop, popSize);
    population_firstGen(&pop, &wnodes, &conns, maxRetransmitTimes);

    printf("%lf\n", population_avgScore(&pop));

    conn_destroy(&conns);
    wnode_init(&wnodes);
    return 0;
}
