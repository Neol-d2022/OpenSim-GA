#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "population.h"
#include "input.h"
#include "simulation.h"

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
    SimData_t SData;
    WirelessNodes_t wnodes;
    Conns_t conns;
    population_t *pop;
    const char *configFilename = 0;

    if (argc < 3)
    {
        fprintf(stderr, "%s [motes.bin] [conns.bin]\n", argv[0]);
        return 1;
    }
    else
    {
        configFilename = argv[3];
    }

    wnode_init(&wnodes);
    conn_init(&conns);

    handleReadError(argv[1], file2wnodes(argv[1], &wnodes));
    handleReadError(argv[2], file2conns(argv[2], &wnodes, &conns));

    SData.wnodes = &wnodes;
    SData.conns = &conns;
    input_getConfig(&SData, configFilename);
    simulation_start(&SData, 1);

    pop = SData.result;
    printChrom(population_maxScoreChrom(pop));

    population_destroy(pop);
    free(pop);
    conn_destroy(&conns);
    wnode_init(&wnodes);
    return 0;
}
