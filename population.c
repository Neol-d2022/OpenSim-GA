#include "population.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int __cmp_f(void *a, void *b)
{
    scored_chrom_t *c, *d;

    c = (scored_chrom_t *)a;
    d = (scored_chrom_t *)b;

    if (c->score > d->score)
        return 1;
    else if (c->score < d->score)
        return -1;
    else
        return memcmp(c->chrom, d->chrom, c->chrom->length * sizeof(*(c->chrom->p)));
}

static void __releaser(void *ptr)
{
    scored_chrom_t *a;

    a = (scored_chrom_t *)ptr;
    chrom_destroy(a->chrom);
    free(a->chrom);

    free(ptr);
}

void population_init(population_t *population, unsigned int size)
{
    population->s_chroms = AVL_Create(__cmp_f, __releaser);
    population->size = size;
}

void population_destroy(population_t *population)
{
    AVL_Destroy(population->s_chroms);
}

void population_addChrom(population_t *population, scored_chrom_t *sch)
{
    AVL_Insert(population->s_chroms, sch);
}

void population_firstGen(population_t *population, WirelessNodes_t *wnodes, Conns_t *conns, unsigned int maxRetransmitTimes)
{
    Chromo_t *ch;
    scored_chrom_t *sch;
    unsigned int i, j, k, l, m, n;

    n = wnode_getNodesCount(wnodes);
    m = population->size;
    for (l = 0; l < m; l += 1)
    {
        ch = malloc(sizeof(*ch));
        sch = malloc(sizeof(*sch));
        chrom_init(ch, n);
        do
        {
            for (i = 0; i < n; i += 1)
            {
                j = (unsigned int)(rand() % n);
                k = 0;
                while (conn_getPdr(conns, i, j) == 0.0 || i == j)
                {
                    j = (j + 1) % n;
                    k += 1;
                    if (k >= n)
                    {
                        fprintf(stderr, "[ERROR] abandoned node.\n");
                        chrom_destroy(ch);
                        free(ch);
                        free(sch);
                        return;
                    }
                }
                (ch->p)[i] = j;
            }
        } while ((sch->score = fitness_score(ch, conns, wnodes, maxRetransmitTimes)) <= 0);
        sch->chrom = ch;
        population_addChrom(population, sch);
    }
}

static void _traveserSum(void *ptr, void *param)
{
    scored_chrom_t *a;
    double *d = (double *)param;

    a = (scored_chrom_t *)ptr;
    *d += a->score;
}

double population_avgScore(population_t *population)
{
    double sum = 0.0;

    AVL_Traverse(population->s_chroms, &sum, _traveserSum);
    return sum / AVL_Count(population->s_chroms);
}