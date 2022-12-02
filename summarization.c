#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

// ======================= INDIVIDUAL PRINTING =======================

// Print all periodics of a researcher
void printPeriodicsByClassification(researcherRoot_t *root)
{

    int i;
    periodicsNode_t *aux = root->allPeriodics->head;

    // Header
    printf("=========================================================================\n");
    printf("  PRODUÇÃO SUMARIZADA POR ORDEM DE PERIÓDICOS DISCRIMINANDO OS ESTRATOS\n");
    printf("=========================================================================\n\n");
    printf("+----------+--------+-----------------------------------------------------------------------------------+\n");
    printf("| Classif. | Quant. | Nome do periódico                                                                 |\n");
    printf("+----------+--------+-----------------------------------------------------------------------------------+\n");

    // Content
    for (i = 1; i <= 9; i++)
    {
        while (aux)
        {
            if (aux->classificationInt == i)
                printf("| %-8s | %-6d | %-81s |\n", aux->classificationName, aux->numPublications, aux->title);
            aux = aux->next;
        }
        aux = root->allPeriodics->head;
    }
    printf("+----------+--------+-----------------------------------------------------------------------------------+\n");
}

// Print all events of a researcher
void printEventsByClassification(researcherRoot_t *root)
{

    int i;
    conferenceNode_t *aux = root->allEvents->head;

    // Header
    printf("===========================================================================\n");
    printf("  PRODUÇÃO SUMARIZADA POR ORDEM DE CONFERêNCIAS DISCRIMINANDO OS ESTRATOS\n");
    printf("===========================================================================\n\n");
    printf("+----------+--------+------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("| Classif. | Quant. | Nome do evento                                                                                                                     |\n");
    printf("+----------+--------+------------------------------------------------------------------------------------------------------------------------------------+\n");

    // Content
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
    printf("+----------+--------+------------------------------------------------------------------------------------------------------------------------------------+\n");
}

// Comparison function for printByAuthor
int nameComparison(const void *a, const void *b)
{

    return strcmp((char *)a, (char *)b);
}

// Print quantity of periodics and events by author
void printByAuthor(researcherRoot_t *researchers)
{

    int i, j = 0;
    researcherNode_t *researcherAux = researchers->head;
    char classifications[10][3] = {"", "A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C"};
    // Create an array with names for sorting
    char names[researchers->quantity][100];
    for (i = 0; i < researchers->quantity; i++)
    {
        strcpy(names[i], researcherAux->name);
        researcherAux = researcherAux->next;
    }
    qsort(names, researchers->quantity, 100, nameComparison);

    // Header
    printf("==========================================\n");
    printf("  PRODUÇÃO SUMARIZADA POR PESQUISADOR\n");
    printf("==========================================\n");

    // Content
    for (j = 0; j < researchers->quantity; j++)
    {
        researcherAux = researchers->head;
        // Search for the next researcher in alpha order
        while (strcmp(researcherAux->name, names[j]))
            researcherAux = researcherAux->next;
        printf("\nPesquisador: %s\n", researcherAux->name);
        printf("+----------+--------------+------------+\n");
        printf("| Classif. | Conferências | Periódicos |\n");
        printf("+----------+--------------+------------+\n");
        for (i = 1; i < 10; i++)
            printf("| %-8s | %-12d | %-10d |\n", classifications[i], researcherAux->quantityConfClassifications[i], researcherAux->quantityPerClassifications[i]);
        printf("+----------+--------------+------------+\n");
    }
}

// Return the quantity of periodics from a given year
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

// Return the quantity of events from a given year
int getEventsByYear(researcherRoot_t *researchers, int qualisLevel, int year)
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

// Print quantity of periodics and events of the group by year
void printByYear(researcherRoot_t *researchers, int yearDelimiter)
{

    int min = researchers->minimumYear;
    int max = researchers->maximumYear;
    if (yearDelimiter != 0)
        min = yearDelimiter;
    int i, j;
    char classifications[10][3] = {"ND", "A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C"};

    // Header
    printf("========================================\n");
    printf(" PRODUÇÃO SUMARIZADA DO GRUPO POR ANO\n");
    printf("========================================\n\n");

    // Content
    for (i = max; i >= min; i--)
    {
        printf("Ano: %d\n", i);
        printf("+----------+--------------+------------+\n");
        printf("| Classif. | Conferências | Periódicos |\n");
        printf("+----------+--------------+------------+\n");
        for (j = 1; j < 10; j++)
        {
            printf("| %-8s | %-12d | %-10d |\n", classifications[j], getEventsByYear(researchers, j, i), getPeriodicsByYear(researchers, j, i));
        }
        printf("+----------+--------------+------------+\n\n");
    }
}

// Print all events and periodics with C qualisLevel
void printAllC(researcherRoot_t *researchers)
{

    periodicsNode_t *periodicAux = researchers->allPeriodics->head;
    conferenceNode_t *eventAux = researchers->allEvents->head;

    // Header
    printf("=================================================\n");
    printf("  LISTAGEM DE PERIÓDICOS E CONFERÊNCIAS NÍVEL C\n");
    printf("=================================================\n\n");

    printf("+--------------+\n");
    printf("|  Periódicos  |\n");
    printf("+--------------+\n");

    // Content
    while (periodicAux)
    {
        if (periodicAux->classificationInt == 9)
            printf("- %s\n", periodicAux->title);
        periodicAux = periodicAux->next;
    }

    // Header
    printf("\n+--------------+\n");
    printf("| Conferências |\n");
    printf("+--------------+\n");

    // Content - events
    while (eventAux)
    {
        if (eventAux->classificationInt == 9)
            printf("- %s\n", eventAux->conference);
        eventAux = eventAux->next;
    }
}

// Print all events and periodics unclassified
void printUnclassified(researcherRoot_t *researchers)
{

    periodicsNode_t *periodicAux = researchers->allPeriodics->head;
    conferenceNode_t *eventAux = researchers->allEvents->head;

    // Header
    printf("===========================================================\n");
    printf("  LISTAGEM DE PERIÓDICOS E CONFERÊNCIAS NÃO CLASSIFICADOS  \n");
    printf("===========================================================\n\n");

    printf("+--------------+\n");
    printf("|  Periódicos  |\n");
    printf("+--------------+\n");

    // Content - periodics
    while (periodicAux)
    {
        if (periodicAux->classificationInt == 0)
            printf("- %s\n", periodicAux->title);
        periodicAux = periodicAux->next;
    }

    // Header
    printf("\n+--------------+\n");
    printf("| Conferências |\n");
    printf("+--------------+\n");

    // Content - events
    while (eventAux)
    {
        if (eventAux->classificationInt == 0)
            printf("- %s\n", eventAux->conference);
        eventAux = eventAux->next;
    }
}

// Print all coauthors of each researcher
void printCoauthors(researcherRoot_t *researchers)
{

    researcherNode_t *researcherAux = researchers->head;
    authorNode_t *authorAux;

    // Header
    printf("=========================\n");
    printf("  LISTAGEM DE COAUTORES  \n");
    printf("=========================\n");

    // Content
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

// ======================= GROUP PRINTING =======================

// Print all periodics discriminated by qualisLevel of each group
void printPeriodicsByClassificationGroups(groups_t *groups)
{

    int i, j = 0;
    researcherRoot_t *researchersAux = groups->group1;
    periodicsNode_t *aux = researchersAux->allPeriodics->head;
    char *groupName = groups->group1_name;
    // Header
    printf("=========================================================================\n");
    printf("  PRODUÇÃO SUMARIZADA POR ORDEM DE PERIÓDICOS DISCRIMINANDO OS ESTRATOS\n");
    printf("=========================================================================\n\n");

    // Content
    for (j = 0; j < 2; j++)
    {
        printf("\nGRUPO: %s\n", groupName);
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
        groupName = groups->group2_name;
        researchersAux = groups->group2;
        aux = researchersAux->allPeriodics->head;
        printf("+----------+--------+-----------------------------------------------------------------------------------+\n");
    }
}

// Print all events discriminated by qualisLevel of each group
void printEventsByClassificationGroups(groups_t *groups)
{

    int i, j = 0;
    researcherRoot_t *researchersAux = groups->group1;
    conferenceNode_t *aux = researchersAux->allEvents->head;
    char *groupName = groups->group1_name;
    // Header
    printf("===========================================================================\n");
    printf("  PRODUÇÃO SUMARIZADA POR ORDEM DE CONFERÊNCIAS DISCRIMINANDO OS ESTRATOS\n");
    printf("===========================================================================\n");

    // Content
    for (j = 0; j < 2; j++)
    {
        printf("\nGRUPO: %s\n", groupName);
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
        groupName = groups->group2_name;
        researchersAux = groups->group2;
        aux = researchersAux->allEvents->head;
        printf("+----------+--------+------------------------------------------------------------------------------------------------------+\n");
    }
}

// Print the quantity of periodics and events of each researcher by group
void printByAuthorGroups(groups_t *groups)
{

    int i, j, k = 0;
    researcherRoot_t *researchers = groups->group1;
    researcherNode_t *researcherAux;
    char *groupName = groups->group1_name;
    char classifications[10][3] = {"", "A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C"};
    char names1[groups->group1->quantity][100];
    char names2[groups->group2->quantity][100];

    // Header
    printf("==========================================\n");
    printf("  PRODUÇÃO SUMARIZADA POR PESQUISADOR\n");
    printf("==========================================");
    // Print the groups
    for (k = 0; k < 2; k++)
    {
        researcherAux = researchers->head;
        printf("\n\n-------- GRUPO: %s --------\n", groupName);
        // Create an array with names for sorting
        if (k == 0)
        {
            for (i = 0; i < researchers->quantity; i++)
            {
                strcpy(names1[i], researcherAux->name);
                researcherAux = researcherAux->next;
            }
            qsort(names1, researchers->quantity, 100, nameComparison);
        }
        else
        {
            for (i = 0; i < researchers->quantity; i++)
            {
                strcpy(names2[i], researcherAux->name);
                researcherAux = researcherAux->next;
            }
            qsort(names2, researchers->quantity, 100, nameComparison);
        }
        // Content
        for (j = 0; j < researchers->quantity; j++)
        {
            researcherAux = researchers->head;
            // Search for the next researcher in alpha order
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
            printf("\nPesquisador: %s\n", researcherAux->name);
            printf("+----------+--------------+------------+\n");
            printf("| Classif. | Conferências | Periódicos |\n");
            printf("+----------+--------------+------------+\n");
            for (i = 1; i < 10; i++)
                printf("| %-8s | %-12d | %-10d |\n", classifications[i], researcherAux->quantityConfClassifications[i], researcherAux->quantityPerClassifications[i]);
            printf("+----------+--------------+------------+\n");
        }
        researchers = groups->group2;
        groupName = groups->group2_name;
    }
}

// Print the quantity of periodics and events of each year by groups
void printByYearGroups(groups_t *groups, int yearDelimiter)
{

    int min = 0;
    int max = 0;

    // Get maximumYear and minimumYear between the groups
    if (groups->group1->maximumYear > groups->group2->maximumYear)
        max = groups->group1->maximumYear;
    else
        max = groups->group2->maximumYear;
    if (groups->group1->minimumYear < groups->group2->minimumYear)
        min = groups->group1->maximumYear;
    else
        min = groups->group2->maximumYear;

    if (yearDelimiter != 0)
        min = yearDelimiter;
    int i, j;
    char classifications[10][3] = {"ND", "A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C"};

    // Header
    printf("========================================\n");
    printf(" PRODUÇÃO SUMARIZADA DO GRUPO POR ANO\n");
    printf("========================================\n\n");

    // Content
    for (i = max; i >= min; i--)
    {
        printf("Ano: %d\n", i);
        printf("%-44s%s\n", groups->group1_name, groups->group2_name);
        printf("+----------+--------------+------------+    +----------+--------------+------------+\n");
        printf("| Classif. | Conferências | Periódicos |    | Classif. | Conferências | Periódicos |\n");
        printf("+----------+--------------+------------+    +----------+--------------+------------+\n");
        for (j = 1; j < 10; j++)
        {
            printf("| %-8s | %-12d | %-10d |    ", classifications[j], getEventsByYear(groups->group1, j, i), getPeriodicsByYear(groups->group1, j, i));
            printf("| %-8s | %-12d | %-10d |\n", classifications[j], getEventsByYear(groups->group2, j, i), getPeriodicsByYear(groups->group2, j, i));
        }
        printf("+----------+--------------+------------+    +----------+--------------+------------+\n\n");
    }
}

// Print all events and periodics with C qualisLevel
void printAllCGroups(groups_t *groups)
{

    researcherRoot_t *researchersAux = groups->group1;
    periodicsNode_t *periodicAux = researchersAux->allPeriodics->head;
    conferenceNode_t *eventAux = researchersAux->allEvents->head;
    char *groupName = groups->group1_name;
    int i = 0;

    // Header
    printf("=================================================\n");
    printf("  LISTAGEM DE PERIÓDICOS E CONFERÊNCIAS NÍVEL C\n");
    printf("=================================================\n\n");

    printf("+--------------+\n");
    printf("|  Periódicos  |\n");
    printf("+--------------+\n");

    // Content - periodics
    for (i = 0; i < 2; i++)
    {
        printf("GRUPO: %s\n", groupName);
        while (periodicAux)
        {
            if (periodicAux->classificationInt == 9)
                printf("- %s\n", periodicAux->title);
            periodicAux = periodicAux->next;
        }
        researchersAux = groups->group2;
        periodicAux = researchersAux->allPeriodics->head;
        groupName = groups->group2_name;
        printf("\n");
    }
    groupName = groups->group1_name;

    // Header
    printf("\n+--------------+\n");
    printf("| Conferências |\n");
    printf("+--------------+\n");

    // Content - events
    for (i = 0; i < 2; i++)
    {
        printf("GRUPO: %s\n", groupName);
        while (eventAux)
        {
            if (eventAux->classificationInt == 9)
                printf("- %s\n", eventAux->conference);
            eventAux = eventAux->next;
        }
        researchersAux = groups->group2;
        eventAux = researchersAux->allEvents->head;
        groupName = groups->group2_name;
        printf("\n");
    }
}

// Print all events and periodics unclassified by group
void printUnclassifiedGroups(groups_t *groups)
{

    researcherRoot_t *researchersAux = groups->group1;
    periodicsNode_t *periodicAux = researchersAux->allPeriodics->head;
    conferenceNode_t *eventAux = researchersAux->allEvents->head;
    char *groupName = groups->group1_name;
    int i = 0;

    // Header
    printf("===========================================================\n");
    printf("  LISTAGEM DE PERIÓDICOS E CONFERÊNCIAS NÃO CLASSIFICADOS\n");
    printf("===========================================================\n\n");

    printf("+--------------+\n");
    printf("|  Periódicos  |\n");
    printf("+--------------+\n");

    // Content - periodics
    for (i = 0; i < 2; i++)
    {
        printf("GRUPO: %s\n", groupName);
        while (periodicAux)
        {
            if (periodicAux->classificationInt == 0)
                printf("- %s\n", periodicAux->title);
            periodicAux = periodicAux->next;
        }
        researchersAux = groups->group2;
        periodicAux = researchersAux->allPeriodics->head;
        groupName = groups->group2_name;
        printf("\n");
    }
    groupName = groups->group1_name;

    // Header
    printf("\n+--------------+\n");
    printf("| Conferências |\n");
    printf("+--------------+\n");

    // Content - events
    for (i = 0; i < 2; i++)
    {
        printf("GRUPO: %s\n", groupName);
        while (eventAux)
        {
            if (eventAux->classificationInt == 0)
                printf("- %s\n", eventAux->conference);
            eventAux = eventAux->next;
        }
        researchersAux = groups->group2;
        eventAux = researchersAux->allEvents->head;
        groupName = groups->group2_name;
        printf("\n");
    }
}

// Print all coauthors of each researcher by group
void printCoauthorsGroup(groups_t *groups)
{

    researcherRoot_t *researchersAux = groups->group1;
    researcherNode_t *researcherAux = researchersAux->head;
    authorNode_t *authorAux;
    char *groupName = groups->group1_name;
    int i = 0;

    // Header
    printf("=========================\n");
    printf("  LISTAGEM DE COAUTORES  \n");
    printf("=========================\n");

    // Content
    for (i = 0; i < 2; i++)
    {
        printf("\n-------- GRUPO: %s --------\n", groupName);
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
        researchersAux = groups->group2;
        researcherAux = researchersAux->head;
        groupName = groups->group2_name;
        printf("---------------------------------------------\n\n");
    }
}
