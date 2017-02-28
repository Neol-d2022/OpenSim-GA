#include "input.h"

#include <stdio.h>
#include <string.h>

int file2wnodes(const char *filename, WirelessNodes_t *wnodes)
{
    char buf[64];
    WirelessNode_t nodeNew;
    FILE *nodeFile;

    // Round 1
    nodeFile = fopen(filename, "rb");
    if (!nodeFile)
    {
        return 1;
    }

    while (fread(buf, 1, ADDR_LENGTH + 1, nodeFile) == ADDR_LENGTH + 1)
    {
        memcpy(nodeNew.addr, buf, ADDR_LENGTH);
        if (buf[ADDR_LENGTH])
            nodeNew.isRoot = 1;
        else
            nodeNew.isRoot = 0;

        wnode_addNode(&nodeNew, wnodes);
    }
    if (!feof(nodeFile))
    {
        return 2;
    }
    fclose(nodeFile);

    return 0;
}

int file2conns(const char *filename, WirelessNodes_t *wnodes, Conns_t *conns)
{
    char buf[64];
    FILE *nodeFile;

    // Round 2
    nodeFile = fopen(filename, "rb");
    if (!nodeFile)
    {
        return 1;
    }

    while (fread(buf, 1, ADDR_LENGTH * 2 + sizeof(double), nodeFile) == (ADDR_LENGTH * 2 + sizeof(double)))
    {
        conn_setPdr(conns, wnodes, (unsigned char *)(buf + 0), (unsigned char *)(buf + ADDR_LENGTH), *(double *)(buf + ADDR_LENGTH * 2));
    }
    if (!feof(nodeFile))
    {
        return 2;
    }
    fclose(nodeFile);

    return 0;
}

void input_getConfig(SimData_t *SData)
{
    // Default values
    double thres = 0.918621;
    unsigned int maxRetransmitTimes = 4; //首次嘗試 + 重傳3次
    unsigned int noImprovementThres = 64;
    unsigned int popSize = 1000;

    SData->thres = thres;
    SData->popSize = popSize;
    SData->maxRetransmitTimes = maxRetransmitTimes;
    SData->noImprovementThres = noImprovementThres;

    return;
}