#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

// Mostra todas os periodicos do pesquisador
void printPeriodicsByClassification(researcherRoot_t *root)
{
    int i;
    periodicsNode_t *aux = root->allPeriodics->head;

    printf("==============================================================================\n");
    printf("  PRODUÇÃO SUMARIZADA POR ORDEM DE PERIÓDICOS DISCRIMINANDO OS ESTRATOS   \n");
    printf("==============================================================================\n\n");
    printf("+--------------+-------------+-----------------------------------------------------------------------------------+\n");
    printf("| Classif.     | Quant.      | Nome do periódico                                                                 |\n");
    printf("+--------------+-------------+-----------------------------------------------------------------------------------+\n");

    for (i = 1; i <= 9; i++)
    {
        while (aux)
        {
            if (aux->classificationInt == i)
                printf("| %-10s | %-5d | %-81s |\n", aux->classificationName, aux->numPublications, aux->title);
            aux = aux->next;
        }
        aux = root->allPeriodics->head;
    }
    printf("+--------------+-------------+-----------------------------------------------------------------------------------+\n");
}

// Mostra todas as conferencias do pesquisador
void printConfsByQualis(researcherRoot_t *root)
{
    int i;
    conferenceNode_t *aux = root->allEvents->head;

    printf("===============================================================================\n");
    printf("  PRODUÇÃO SUMARIZADA POR ORDEM DE CONFERÊNCIAS DISCRIMINANDO OS ESTRATOS\n");
    printf("===============================================================================\n\n");
    printf("+--------------+------------+------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("| Classif.     | Quant.     | Nome do evento                                                                                                                     |\n");
    printf("+--------------+------------+------------------------------------------------------------------------------------------------------------------------------------+\n");

    for (i = 1; i <= 9; i++)
    {
        while (aux)
        {
            if (i == aux->classificationInt)
                printf("| %-8s | %-6d | %-130s |\n", aux->classificationName, aux->numParticipations, aux->conference);
            aux = aux->next;
        }
        aux = root->allEvents->head;
    }
    printf("+--------------+------------+------------------------------------------------------------------------------------------------------------------------------------+\n");
}

int nameComparison(const void *a, const void *b)
{
    return strcmp((char *)a, (char *)b);
}

// Imprime a quantidade de periodicos e conferencias do pesquisador
void printByAuthor(researcherRoot_t *researchers)
{
    int i, j = 0;
    researcherNode_t *researcherAux = researchers->head;
    char classifications[10][3] = {"", "A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C"};

    char names[researchers->quantity][100];
    for (i = 0; i < researchers->quantity; i++)
    {
        strcpy(names[i], researcherAux->name);
        researcherAux = researcherAux->next;
    }
    qsort(names, researchers->quantity, 100, nameComparison);

    printf("==============================================\n");
    printf("      PRODUÇÃO SUMARIZADA POR PESQUISADOR\n");
    printf("==============================================\n");

    for (j = 0; j < researchers->quantity; j++)
    {
        researcherAux = researchers->head;
        while (strcmp(researcherAux->name, names[j]))
            researcherAux = researcherAux->next;
        printf("\nPesquisador: %s\n", researcherAux->name);
        printf("+----------+--------------+------------+\n");
        printf("| Classif. | Conferências | Periódicos |\n");
        printf("+----------+--------------+------------+\n");
        for (i = 1; i < 10; i++)
            printf("| %-7s | %-12d | %-10d |\n", classifications[i], researcherAux->quantityConfClassifications[i], researcherAux->quantityPerClassifications[i]);
        printf("+----------+--------------+------------+\n");
    }
}

// Pega a quantidade de periodicos do ano
int getPeriodicsByYear(researcherRoot_t *researchers, int qualisLevel, int year)
{
    researcherNode_t *researcherAux = researchers->head;
    periodicsNode_t *periodicAux;

    int count = 0;
    periodicAux = researchers->allPeriodics->head;

    while (periodicAux)
    {
        if (periodicAux->year == year && periodicAux->classificationInt == qualisLevel)
            count++;
        periodicAux = periodicAux->next;
    }

    researcherAux = researcherAux->next;

    return count;
}

// Pega a quantidade de conferencias do ano
int getConfesByYear(researcherRoot_t *researchers, int qualisLevel, int year)
{
    researcherNode_t *researcherAux = researchers->head;
    conferenceNode_t *eventAux;

    int count = 0;
    eventAux = researchers->allEvents->head;
    while (eventAux)
    {
        if (eventAux->year == year && eventAux->classificationInt == qualisLevel)
            count++;
        eventAux = eventAux->next;
    }
    researcherAux = researcherAux->next;

    return count;
}

// Imprime a quantidadde de periodicos e conferencias de um algomerado por ano
void printByYear(researcherRoot_t *researchers, int yearDelimiter)
{
    int min = researchers->minimumYear;
    int max = researchers->maximumYear;

    if (yearDelimiter != 0)
        min = yearDelimiter;

    int i, j;
    char classifications[10][3] = {"ND", "A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C"};

    printf("===========================================\n");
    printf("         PRODUÇÃO SUMARIZADA DO GRUPO POR ANO\n");
    printf("===========================================\n\n");

    for (i = max; i >= min; i--)
    {
        printf("Ano: %d\n", i);
        printf("+----------+--------------+------------+\n");
        printf("| Classif. | Conferências | Periódicos |\n");
        printf("+----------+--------------+------------+\n");
        for (j = 1; j < 10; j++)
        {
            printf("| %-8s | %-12d | %-10d |\n", classifications[j], getConfesByYear(researchers, j, i), getPeriodicsByYear(researchers, j, i));
        }
        printf("+----------+--------------+------------+\n\n");
    }
}

// Imprime os qualis C encotrados
void printAllC(researcherRoot_t *researchers)
{
    periodicsNode_t *periodicAux = researchers->allPeriodics->head;
    conferenceNode_t *eventAux = researchers->allEvents->head;

    printf("=================================================\n");
    printf("  LISTAGEM DE PERIÓDICOS E CONFERÊNCIAS NÍVEL C\n");
    printf("=================================================\n\n");

    printf("+--------------+\n");
    printf("|  Periódicos  |\n");
    printf("+--------------+\n");

    while (periodicAux)
    {
        if (periodicAux->classificationInt == 9)
            printf("- %s\n", periodicAux->title);
        periodicAux = periodicAux->next;
    }

    printf("\n+--------------+\n");
    printf("| Conferências |\n");
    printf("+--------------+\n");

    while (eventAux)
    {
        if (eventAux->classificationInt == 9)
            printf("- %s\n", eventAux->conference);
        eventAux = eventAux->next;
    }
}

// Imprime os ND/ nao classificados pela qualis
void printUnqualis(researcherRoot_t *researchers)
{
    periodicsNode_t *periodicAux = researchers->allPeriodics->head;
    conferenceNode_t *eventAux = researchers->allEvents->head;

    printf("=================================================================\n");
    printf("      LISTAGEM DE PERIÓDICOS E CONFERÊNCIAS NÃO CLASSIFICADOS  \n");
    printf("===================================================================\n\n");

    printf("+--------------+\n");
    printf("|  Periódicos  |\n");
    printf("+--------------+\n");

    while (periodicAux)
    {
        if (periodicAux->classificationInt == 0)
            printf("- %s\n", periodicAux->title);
        periodicAux = periodicAux->next;
    }

    printf("\n+--------------+\n");
    printf("| Conferências |\n");
    printf("+--------------+\n");

    while (eventAux)
    {
        if (eventAux->classificationInt == 0)
            printf("- %s\n", eventAux->conference);
        eventAux = eventAux->next;
    }
}

// Imprime os co-autores
void printCoauthors(researcherRoot_t *researchers)
{
    researcherNode_t *researcherAux = researchers->head;
    authorNode_t *authorAux;

    printf("=========================\n");
    printf("  LISTAGEM DE COAUTORES  \n");
    printf("=========================\n");

    while (researcherAux)
    {
        printf("\nPESQUISADOR: %s\n", researcherAux->name);
        authorAux = researcherAux->authors->head;
        while (authorAux)
        {
            printf("--- %s\n", authorAux->name);
            authorAux = authorAux->next;
        }
        researcherAux = researcherAux->next;
    }

    printf("---------------------------------------------\n");
}

// Imprime todos os periodicos da qualis do aglomerado
void printPeriodsByQualisClusters(clusters_t *clusters)
{
    int i, j = 0;
    researcherRoot_t *researchersAux = clusters->cluster1;
    periodicsNode_t *aux = researchersAux->allPeriodics->head;

    char *clusterName = clusters->cluster1_name;

    printf("=========================================================================\n");
    printf("  PRODUÇÃO SUMARIZADA POR ORDEM DE PERIÓDICOS DISCRIMINANDO OS ESTRATOS\n");
    printf("=========================================================================\n\n");

    for (j = 0; j < 2; j++)
    {
        printf("\nGRUPO: %s\n", clusterName);
        printf("+----------+--------+-----------------------------------------------------------------------------------+\n");
        printf("| Classif. | Quant. | Nome do periódico                                                                 |\n");
        printf("+----------+--------+-----------------------------------------------------------------------------------+\n");
        for (i = 1; i <= 9; i++)
        {
            while (aux)
            {
                if (aux->classificationInt == i)
                    printf("| %-8s | %-6d | %-81s |\n", aux->classificationName, aux->numPublications, aux->title);
                aux = aux->next;
            }
            aux = researchersAux->allPeriodics->head;
        }
        clusterName = clusters->cluster2_name;
        researchersAux = clusters->cluster2;
        aux = researchersAux->allPeriodics->head;
        printf("+----------+--------+-----------------------------------------------------------------------------------+\n");
    }
}

// Imprime todos os conferencias da qualis do aglomerado
void printConfsByQualisClusters(clusters_t *clusters)
{
    int i, j = 0;
    researcherRoot_t *researchersAux = clusters->cluster1;
    conferenceNode_t *aux = researchersAux->allEvents->head;
    char *clusterName = clusters->cluster1_name;

    printf("===============================================================================\n");
    printf("     PRODUÇÃO SUMARIZADA POR ORDEM DE CONFERÊNCIAS DISCRIMINANDO OS ESTRATOS\n");
    printf("===============================================================================\n");

    for (j = 0; j < 2; j++)
    {
        printf("\nGRUPO: %s\n", clusterName);
        printf("+----------+--------+------------------------------------------------------------------------------------------------------+\n");
        printf("| Classif. | Quant. | Nome do periódico                                                                                    |\n");
        printf("+----------+--------+------------------------------------------------------------------------------------------------------+\n");
        for (i = 1; i <= 9; i++)
        {
            while (aux)
            {
                if (aux->classificationInt == i)
                    printf("| %-8s | %-6d | %-100s |\n", aux->classificationName, aux->numParticipations, aux->conference);
                aux = aux->next;
            }
            aux = researchersAux->allEvents->head;
        }
        clusterName = clusters->cluster2_name;
        researchersAux = clusters->cluster2;
        aux = researchersAux->allEvents->head;

        printf("+----------+--------+------------------------------------------------------------------------------------------------------+\n");
    }
}

// Imprime a quantidade periodicos e conferencias de cada pesquisador por aglomerado
void printByAuthorClusters(clusters_t *clusters)
{
    int i, j, k = 0;
    researcherRoot_t *researchers = clusters->cluster1;
    researcherNode_t *researcherAux;

    char *clusterName = clusters->cluster1_name;
    char classifications[10][3] = {"", "A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C"};

    char names1[clusters->cluster1->quantity][100];
    char names2[clusters->cluster2->quantity][100];

    printf("==========================================\n");
    printf("  PRODUÇÃO SUMARIZADA POR PESQUISADOR\n");
    printf("==========================================");

    for (k = 0; k < 2; k++)
    {
        researcherAux = researchers->head;
        printf("\n\n------------ GRUPO: %s ------------\n", clusterName);
        if (k == 0)
        {
            for (i = 0; i < researchers->quantity; i++)
            {
                strcpy(names1[i], researcherAux->name);
                researcherAux = researcherAux->next;
            }
            qsort(names1, researchers->quantity, 150, nameComparison);
        }

        else
        {
            for (i = 0; i < researchers->quantity; i++)
            {
                strcpy(names2[i], researcherAux->name);
                researcherAux = researcherAux->next;
            }
            qsort(names2, researchers->quantity, 150, nameComparison);
        }

        for (j = 0; j < researchers->quantity; j++)
        {
            researcherAux = researchers->head;
            if (k == 0)
            {
                while (strcmp(researcherAux->name, names1[j]))
                    researcherAux = researcherAux->next;
            }
            else
            {
                while (strcmp(researcherAux->name, names2[j]))
                    researcherAux = researcherAux->next;
            }
            printf("\n ----------- Pesquisador: %s\n", researcherAux->name);

            printf("+----------+--------------+------------+\n");
            printf("| Classif. | Conferências | Periódicos |\n");
            printf("+----------+--------------+------------+\n");
            for (i = 1; i < 10; i++)
                printf("| %-8s | %-12d | %-10d |\n", classifications[i], researcherAux->quantityConfClassifications[i], researcherAux->quantityPerClassifications[i]);
            printf("+----------+--------------+------------+\n");
        }

        researchers = clusters->cluster2;
        clusterName = clusters->cluster2_name;
    }
}

// Imprime a quantidade periodicos e conferencias de cada pesquisador por ano
void printByYearClusters(clusters_t *clusters, int yearDelimiter)
{
    int min = 0;
    int max = 0;

    if (clusters->cluster1->maximumYear > clusters->cluster2->maximumYear)
        max = clusters->cluster1->maximumYear;
    else
        max = clusters->cluster2->maximumYear;
    if (clusters->cluster1->minimumYear < clusters->cluster2->minimumYear)
        min = clusters->cluster1->maximumYear;
    else
        min = clusters->cluster2->maximumYear;

    if (yearDelimiter != 0)
        min = yearDelimiter;
    int i, j;
    char classifications[10][3] = {"ND", "A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C"};

    printf("============================================\n");
    printf("    PRODUÇÃO SUMARIZADA DO GRUPO POR ANO\n");
    printf("============================================\n\n");

    for (i = max; i >= min; i--)
    {
        printf("Ano: %d\n", i);
        printf("%-44s%s\n", clusters->cluster1_name, clusters->cluster2_name);
        printf("+----------+--------------+------------+    +----------+--------------+------------+\n");
        printf("| Classif. | Conferências | Periódicos |    | Classif. | Conferências | Periódicos |\n");
        printf("+----------+--------------+------------+    +----------+--------------+------------+\n");
        for (j = 1; j < 10; j++)
        {
            printf("| %-8s | %-12d | %-10d |    ", classifications[j], getConfesByYear(clusters->cluster1, j, i), getPeriodicsByYear(clusters->cluster1, j, i));
            printf("| %-8s | %-12d | %-10d |\n", classifications[j], getConfesByYear(clusters->cluster2, j, i), getPeriodicsByYear(clusters->cluster2, j, i));
        }
        printf("+----------+--------------+------------+    +----------+--------------+------------+\n\n");
    }
}

// Imprime todos os qualis C do aglomerado
void printAllCClusters(clusters_t *clusters)
{
    researcherRoot_t *researchersAux = clusters->cluster1;
    periodicsNode_t *periodicAux = researchersAux->allPeriodics->head;
    conferenceNode_t *eventAux = researchersAux->allEvents->head;

    char *clusterName = clusters->cluster1_name;
    int i = 0;

    printf("=================================================\n");
    printf("  LISTAGEM DE PERIÓDICOS E CONFERÊNCIAS NÍVEL C\n");
    printf("=================================================\n\n");

    printf("+--------------+\n");
    printf("|  Periódicos  |\n");
    printf("+--------------+\n");

    for (i = 0; i < 2; i++)
    {
        printf("-------------- GRUPO: %s --------------\n", clusterName);
        while (periodicAux)
        {
            if (periodicAux->classificationInt == 9)
                printf("- %s\n", periodicAux->title);
            periodicAux = periodicAux->next;
        }
        researchersAux = clusters->cluster2;
        periodicAux = researchersAux->allPeriodics->head;
        clusterName = clusters->cluster2_name;

        printf("\n");
    }
    clusterName = clusters->cluster1_name;

    printf("\n+--------------+\n");
    printf("| Conferências |\n");
    printf("+--------------+\n");

    for (i = 0; i < 2; i++)
    {
        printf("-------------- GRUPO: %s --------------\n", clusterName);
        while (eventAux)
        {
            if (eventAux->classificationInt == 9)
                printf("- %s\n", eventAux->conference);
            eventAux = eventAux->next;
        }
        researchersAux = clusters->cluster2;
        eventAux = researchersAux->allEvents->head;
        clusterName = clusters->cluster2_name;

        printf("\n");
    }
}

// Imprime os ND/ nao classificados pela qualis do aglomerado
void printUnqualisclusters(clusters_t *clusters)
{
    researcherRoot_t *researchersAux = clusters->cluster1;
    periodicsNode_t *periodicAux = researchersAux->allPeriodics->head;
    conferenceNode_t *eventAux = researchersAux->allEvents->head;
    char *clusterName = clusters->cluster1_name;
    int i = 0;

    printf("===========================================================\n");
    printf("  LISTAGEM DE PERIÓDICOS E CONFERÊNCIAS NÃO CLASSIFICADOS\n");
    printf("===========================================================\n\n");

    printf("+--------------+\n");
    printf("|  Periódicos  |\n");
    printf("+--------------+\n");

    for (i = 0; i < 2; i++)
    {
        printf("GRUPO: %s\n", clusterName);
        while (periodicAux)
        {
            if (periodicAux->classificationInt == 0)
                printf("- %s\n", periodicAux->title);
            periodicAux = periodicAux->next;
        }
        researchersAux = clusters->cluster2;
        periodicAux = researchersAux->allPeriodics->head;
        clusterName = clusters->cluster2_name;
        printf("\n");
    }
    clusterName = clusters->cluster1_name;

    printf("\n+--------------+\n");
    printf("| Conferências |\n");
    printf("+--------------+\n");

    for (i = 0; i < 2; i++)
    {
        printf("GRUPO: %s\n", clusterName);
        while (eventAux)
        {
            if (eventAux->classificationInt == 0)
                printf("- %s\n", eventAux->conference);
            eventAux = eventAux->next;
        }
        researchersAux = clusters->cluster2;
        eventAux = researchersAux->allEvents->head;
        clusterName = clusters->cluster2_name;

        printf("\n");
    }
}

// Imprime os co-autores do aglomeros
void printCoauthorsCluster(clusters_t *clusters)
{
    researcherRoot_t *researchersAux = clusters->cluster1;
    researcherNode_t *researcherAux = researchersAux->head;
    authorNode_t *authorAux;
    char *clusterName = clusters->cluster1_name;
    int i = 0;

    printf("=========================\n");
    printf("  LISTAGEM DE COAUTORES  \n");
    printf("=========================\n");

    for (i = 0; i < 2; i++)
    {
        printf("\n-------- GRUPO: %s --------\n", clusterName);
        while (researcherAux)
        {
            printf("\nPESQUISADOR: %s\n", researcherAux->name);
            authorAux = researcherAux->authors->head;
            while (authorAux)
            {
                printf("--- %s\n", authorAux->name);
                authorAux = authorAux->next;
            }
            researcherAux = researcherAux->next;
        }
        researchersAux = clusters->cluster2;
        researcherAux = researchersAux->head;
        clusterName = clusters->cluster2_name;

        printf("---------------------------------------------\n\n");
    }
}