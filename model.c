#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "summarization.h"
#include "model.h"

// Inicializa uma lista de periodicos
periodicsRoot_t *createPeriodsList()
{
    int i;
    periodicsRoot_t *periodics = malloc(sizeof(periodicsRoot_t));
    if (!periodics)
        return NULL;
    periodics->quantityClassifications = malloc(sizeof(long) * 10);
    
    if (!periodics->quantityClassifications)
        return NULL;

    for (i = 1; i <= 10; i++)
        periodics->quantityClassifications[i] = 0;

    periodics->head = NULL;
    periodics->tail = NULL;
    periodics->quantity = 0;

    return periodics;
}

// Adiciona um novo periodicos
periodicsNode_t *createPeriodic(researcherRoot_t *researchers, char *title, int year, char *qualisLevel)
{
    periodicsNode_t *aux = researchers->allPeriodics->head;
    while (aux && strcmp(title, aux->title))
        aux = aux->next;

    if (aux)
    {
        researchers->allPeriodics->quantityClassifications[aux->classificationInt]++;
        researchers->tail->quantityPerClassifications[aux->classificationInt]++;
        aux->numPublications++;
        return aux;
    }

    periodicsNode_t *newPeriodic = malloc(sizeof(periodicsNode_t));
    if (!newPeriodic)
        return NULL;
    newPeriodic->title = malloc(sizeof(char) * strlen(title) + 1);
    if (!newPeriodic->title)
        return NULL;
    newPeriodic->classificationName = malloc(sizeof(char) * 3);
    if (!newPeriodic->classificationName)
        return NULL;

    strcpy(newPeriodic->title, title);
    updatePeriodicClassification(researchers, newPeriodic, qualisLevel);
    newPeriodic->year = year;

    if (researchers->allPeriodics->quantity == 0)
        researchers->allPeriodics->head = newPeriodic;
    else
        researchers->allPeriodics->tail->next = newPeriodic;
    researchers->allPeriodics->tail = newPeriodic;

    newPeriodic->numPublications = 1;
    newPeriodic->next = NULL;
    researchers->allPeriodics->quantity++;

    return newPeriodic;
}

// Cria uma nova lista conferencia
conferencesRoot_t *createConfsList()
{
    int i;
    conferencesRoot_t *events = malloc(sizeof(conferencesRoot_t));
    if (!events)
        return NULL;

    events->quantityClassifications = malloc(sizeof(long) * 10);
    if (!events->quantityClassifications)
        return NULL;

    for (i = 1; i <= 10; i++)
        events->quantityClassifications[i] = 0;

    events->quantity = 0;
    events->head = NULL;
    events->tail = NULL;
    return events;
}

// Adiciona uma nova conferencia
conferenceNode_t *createConference(researcherRoot_t *researchers, char *conference, int year, char *qualisLevel)
{
    conferenceNode_t *aux = researchers->allEvents->head;
    while (aux && strcmp(conference, aux->conference))
        aux = aux->next;

    if (aux)
    {
        aux->numParticipations++;
        researchers->allEvents->quantity++;
        researchers->allEvents->quantityClassifications[aux->classificationInt]++;
        researchers->tail->quantityConfClassifications[aux->classificationInt]++;
        return aux;
    }

    conferenceNode_t *newEvent = malloc(sizeof(conferenceNode_t));
    if (!newEvent)
        return NULL;
    newEvent->conference = malloc(sizeof(char) * strlen(conference) + 1);
    if (!newEvent->conference)
        return NULL;
    newEvent->classificationName = malloc(sizeof(char) * 3);
    if (!newEvent->classificationName)
        return NULL;

    strcpy(newEvent->conference, conference);
    newEvent->year = year;
    updateEventClassification(researchers, newEvent, qualisLevel);

    if (researchers->allEvents->quantity == 0)
        researchers->allEvents->head = newEvent;
    else
        researchers->allEvents->tail->next = newEvent;

    researchers->allEvents->tail = newEvent;
    newEvent->numParticipations = 1;
    newEvent->next = NULL;
    researchers->allEvents->quantity++;

    return newEvent;
}

// Cria uma lista de pesquisadores
researcherRoot_t *createResearchersList()
{
    int i;
    researcherRoot_t *researchers = malloc(sizeof(researcherRoot_t));
    if (!researchers)
        return NULL;
    researchers->totalConfClassifications = malloc(sizeof(long) * 10);
    if (!researchers->totalConfClassifications)
        return NULL;
    researchers->totalPerClassifications = malloc(sizeof(long) * 10);
    if (!researchers->totalPerClassifications)
        return NULL;

    researchers->allEvents = createConfsList();
    researchers->allPeriodics = createPeriodsList();

    for (i = 1; i <= 10; i++)
        researchers->totalConfClassifications[i] = 0;
    for (i = 1; i <= 10; i++)
        researchers->totalPerClassifications[i] = 0;

    researchers->maximumYear = 0;
    researchers->minimumYear = 0;
    researchers->quantity = 0;
    researchers->head = NULL;
    researchers->tail = NULL;
    return researchers;
}

// Adiciona um novo pesquisador
researcherNode_t *createResearcher(researcherRoot_t *researchers, char *name)
{
    int i;
    researcherNode_t *newResearcher = malloc(sizeof(researcherNode_t));
    if (!newResearcher)
        return NULL;
    newResearcher->name = malloc(sizeof(char) * strlen(name) + 1);
    if (!newResearcher->name)
        return NULL;
    newResearcher->quantityPerClassifications = malloc(sizeof(long) * 10);
    if (!newResearcher->quantityPerClassifications)
        return NULL;
    newResearcher->quantityConfClassifications = malloc(sizeof(long) * 10);
    if (!newResearcher->quantityConfClassifications)
        return NULL;

    strcpy(newResearcher->name, name);
    newResearcher->authors = createAuthorsList();

    if (researchers->quantity == 0)
        researchers->head = newResearcher;
    else
        researchers->tail->next = newResearcher;
    researchers->tail = newResearcher;

    for (i = 1; i <= 10; i++)
    {
        newResearcher->quantityPerClassifications[i] = 0;
        newResearcher->quantityConfClassifications[i] = 0;
    }

    researchers->quantity++;
    newResearcher->next = NULL;

    return newResearcher;
}

// Cria uma nova lista de autores
authorsRoot_t *createAuthorsList()
{
    authorsRoot_t *authors = malloc(sizeof(authorsRoot_t));
    if (!authors)
        return NULL;

    authors->head = NULL;
    authors->tail = NULL;
    authors->quantity = 0;

    return authors;
}

// Adiciona um novo autor
authorNode_t *createAuthor(researcherNode_t *researcher, char *name)
{
    authorNode_t *authorAux = researcher->authors->head;
    while (authorAux && strcmp(authorAux->name, name))
        authorAux = authorAux->next;

    // Confere se autor já pertence a lista
    if (authorAux || !strcmp(name, researcher->name))
        return NULL;

    // Cria um novo autor
    authorNode_t *newAuthor = malloc(sizeof(authorNode_t));
    if (!newAuthor)
        return NULL;
    newAuthor->name = malloc(sizeof(char) * strlen(name) + 1);

    if (!newAuthor->name)
        return NULL;

    strcpy(newAuthor->name, name);
    newAuthor->next = NULL;
    if (researcher->authors->quantity == 0)
        researcher->authors->head = newAuthor;
    else
        researcher->authors->tail->next = newAuthor;

    researcher->authors->tail = newAuthor;
    researcher->authors->quantity++;

    return newAuthor;
}

// Inicializa um aglomerado de pesquisadores
clusters_t *createClusters()
{
    clusters_t *clusters = malloc(sizeof(clusters_t));
    if (!clusters)
        return NULL;

    clusters->cluster1_name = NULL;
    clusters->cluster2_name = NULL;

    clusters->cluster1 = NULL;
    clusters->cluster2 = NULL;

    return clusters;
}

// Adiciona um novo aglomerado de pesuiqsadores
void addCluster(clusters_t *clusters, researcherRoot_t *researchers, char *clusterName)
{

    // Cria o primeiro aglomerado
    if (!clusters->cluster1)
    {
        clusters->cluster1 = researchers;
        clusters->cluster1_name = malloc(sizeof(char) * strlen(clusterName) + 1);
        if (!clusters->cluster1_name)
            return;
        strcpy(clusters->cluster1_name, clusterName);
    }
    // Cria segundo aglomerado
    else
    {
        clusters->cluster2 = researchers;
        clusters->cluster2_name = malloc(sizeof(char) * strlen(clusterName) + 1);
        if (!clusters->cluster2_name)
            return;
        strcpy(clusters->cluster2_name, clusterName);
    }
}

// Converção de string qualis para um inteiro
int getClassificationInt(char *string)
{
    if (strstr(string, "ND"))
        return 0;
    else if (strstr(string, "A1"))
        return 1;
    else if (strstr(string, "A2"))
        return 2;
    else if (strstr(string, "A3"))
        return 3;
    else if (strstr(string, "A4"))
        return 4;
    else if (strstr(string, "B1"))
        return 5;
    else if (strstr(string, "B2"))
        return 6;
    else if (strstr(string, "B3"))
        return 7;
    else if (strstr(string, "B4"))
        return 8;
    else
        return 9;
}

// Atualiza a lista de quantidade de qualis da conferencia
void updateEventClassification(researcherRoot_t *researchers, conferenceNode_t *conference, char *qualisLevel)
{
    int classificationInt = 0;
    researcherNode_t *researcherAux = researchers->tail;

    // salva na struct o nome da qualis
    conference->classificationName[0] = qualisLevel[0];
    conference->classificationName[1] = qualisLevel[1];
    conference->classificationName[2] = '\0';

    // salva o inteiro qualis respectivo ao nome
    classificationInt = getClassificationInt(qualisLevel);

    conference->classificationInt = classificationInt;
    researchers->totalConfClassifications[classificationInt]++;
    researchers->allEvents->quantityClassifications[classificationInt]++;

    researcherAux->quantityConfClassifications[classificationInt]++;
}

// Atualiza a lista de quantidade de qualis do periodico
void updatePeriodicClassification(researcherRoot_t *researchers, periodicsNode_t *periodic, char *qualisLevel)
{
    int classificationInt = 0;
    researcherNode_t *researcherAux = researchers->tail;

    // salva na struct o nome da qualis
    periodic->classificationName[0] = qualisLevel[0];
    periodic->classificationName[1] = qualisLevel[1];
    periodic->classificationName[2] = '\0';

    // salva o inteiro qualis respectivo ao nome
    classificationInt = getClassificationInt(qualisLevel);

    periodic->classificationInt = classificationInt;
    researchers->totalPerClassifications[classificationInt]++;
    researchers->allPeriodics->quantityClassifications[classificationInt]++;

    researcherAux->quantityPerClassifications[classificationInt]++;
}

// Limpa memoria alocada dos eventos
void destroyConfs(conferencesRoot_t *root)
{
    conferenceNode_t *next = root->head;
    conferenceNode_t *aux = next;

    while (next)
    {
        aux = next;
        next = aux->next;
        free(aux->conference);
        free(aux->classificationName);
        free(aux);
    }

    free(root->quantityClassifications);
    free(root);
}

// Limpa memoria alocada dos eventos
void destroyPeriodics(periodicsRoot_t *root)
{
    periodicsNode_t *next = root->head;
    periodicsNode_t *aux = next;

    while (next)
    {
        aux = next;
        next = aux->next;
        free(aux->title);
        free(aux->classificationName);
        free(aux);
    }

    free(root->quantityClassifications);
    free(root);
}

// Clean coauthors allocated memory
void destroyAuthors(authorsRoot_t *authors)
{
    authorNode_t *next = authors->head;
    authorNode_t *aux = next;

    while (next)
    {
        aux = next;
        next = aux->next;
        free(aux->name);
        free(aux);
    }

    free(authors);
}

// Limpa a memoria da lista de pesquisadores
void destroyResearchers(researcherRoot_t *root)
{
    researcherNode_t *next = root->head;
    researcherNode_t *aux = next;

    while (next)
    {
        aux = next;
        next = aux->next;
        free(aux->name);
        free(aux->quantityConfClassifications);
        free(aux->quantityPerClassifications);

        destroyAuthors(aux->authors);
        free(aux);
    }

    free(root->totalConfClassifications);
    free(root->totalPerClassifications);

    destroyConfs(root->allEvents);
    destroyPeriodics(root->allPeriodics);

    free(root);
}

// Limpa toda memoria alocada
void destroyAllStructs(researcherRoot_t *researchers, clusters_t *clusters)
{
    if (clusters)
    {
        destroyResearchers(clusters->cluster1);
        destroyResearchers(clusters->cluster2);

        free(clusters->cluster1_name);
        free(clusters->cluster2_name);
    }
    else
        destroyResearchers(researchers);
}
