#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "population.h"
#include "input.h"

static void handleReadError(const char *filename, int res)
{
    if (res != 0)
    {
        if (res == 1)
        {
            fprintf(stderr, "[ERROR] Unable to open file \"%s\"\n", filename);
        }
        else if (res == 2)
        {
            fprintf(stderr, "[ERROR] Read error in file \"%s\"\n", filename);
        }
        else
        {
            fprintf(stderr, "[ERROR] Unknown error in file \"%s\"\n", filename);
        }
        exit(res);
    }
}

int main(int argc, char **argv)
{
    WirelessNodes_t wnodes;
    Conns_t conns;
    population_t pop, child;
    double thres = 0.918621;
    unsigned int maxRetransmitTimes = 4; //首次嘗試 + 重傳3次
    unsigned int noImprovementThres = 64, c;
    unsigned int popSize = 1000;

    if (argc != 3)
    {
        fprintf(stderr, "%s [motes.bin] [conns.bin]\n", argv[0]);
        return 1;
    }

    wnode_init(&wnodes);
    conn_init(&conns);
    population_init(&pop, popSize);

    handleReadError(argv[1], file2wnodes(argv[1], &wnodes));
    handleReadError(argv[2], file2conns(argv[2], &wnodes, &conns));

    srand(time(0) + clock());
    population_firstGen(&pop, &wnodes, &conns, maxRetransmitTimes);
    printf("avg = %lf, max = %lf\n", population_avgScore(&pop), population_maxScore(&pop));
    c = 0;
    while (population_maxScore(&pop) < thres)
    {
        population_init(&child, popSize);
        population_nextGen(&pop, &child, &wnodes, &conns, maxRetransmitTimes);
        if (population_avgScore(&pop) < population_avgScore(&child))
        {
            population_destroy(&pop);
            memcpy(&pop, &child, sizeof(pop));
            c = 0;
        }
        else
        {
            population_destroy(&child);
            c += 1;
            if (c >= noImprovementThres)
                break;
        }

        printf("avg = %lf, max = %lf\n", population_avgScore(&pop), population_maxScore(&pop));
    }

    printChrom(population_maxScoreChrom(&pop));

    population_destroy(&pop);
    conn_destroy(&conns);
    wnode_init(&wnodes);
    return 0;
}
