#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "summarization.h"
#include "model.h"

// ======================= STRUCTS MAINTENANCE FUNCTIONS =======================

// Return a pointer to an eventsRoot
conferencesRoot_t *createConferencesList()
{
    int i;
    conferencesRoot_t *events = malloc(sizeof(conferencesRoot_t));
    if (!events)
        return NULL;

    events->quantityClassifications = malloc(sizeof(long) * 10);
    if (!events->quantityClassifications)
        return NULL;

    // Add the info and inicialize variables
    for (i = 1; i <= 10; i++)
        events->quantityClassifications[i] = 0;
    events->quantity = 0;
    events->head = NULL;
    events->tail = NULL;
    return events;
}

// Create a new eventNode or add +1 in quantity variable if is already on the list
conferenceNode_t *createConference(researcherRoot_t *researchers, char *conference, int year, char *qualisLevel)
{
    // Search for event existence
    conferenceNode_t *aux = researchers->allEvents->head;
    while (aux && strcmp(conference, aux->conference))
        aux = aux->next;

    // If event already exist
    if (aux)
    {
        aux->numParticipations++;
        researchers->allEvents->quantity++;
        researchers->allEvents->quantityClassifications[aux->classificationInt]++;
        researchers->tail->quantityConfClassifications[aux->classificationInt]++;
        return aux;
    }

    // Create a new event
    conferenceNode_t *newEvent = malloc(sizeof(conferenceNode_t));
    if (!newEvent)
        return NULL;
    newEvent->conference = malloc(sizeof(char) * strlen(conference) + 1);
    if (!newEvent->conference)
        return NULL;
    newEvent->classificationName = malloc(sizeof(char) * 3);
    if (!newEvent->classificationName)
        return NULL;

    // Add the info and inicialize variables
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

// Return a pointer to a periodicsRoot
periodicsRoot_t *createPeriodicsList()
{
    int i;
    periodicsRoot_t *periodics = malloc(sizeof(periodicsRoot_t));
    if (!periodics)
        return NULL;
    periodics->quantityClassifications = malloc(sizeof(long) * 10);
    if (!periodics->quantityClassifications)
        return NULL;

    // Add the info and inicialize variables
    for (i = 1; i <= 10; i++)
        periodics->quantityClassifications[i] = 0;
    periodics->head = NULL;
    periodics->tail = NULL;
    periodics->quantity = 0;

    return periodics;
}

periodicsNode_t *createPeriodic(researcherRoot_t *researchers, char *title, int year, char *qualisLevel)
{
    // Search for periodic existence
    periodicsNode_t *aux = researchers->allPeriodics->head;
    while (aux && strcmp(title, aux->title))
        aux = aux->next;

    // If periodic already exist
    if (aux)
    {
        researchers->allPeriodics->quantityClassifications[aux->classificationInt]++;
        researchers->tail->quantityPerClassifications[aux->classificationInt]++;
        aux->numPublications++;
        return aux;
    }

    // Create a new periodic
    periodicsNode_t *newPeriodic = malloc(sizeof(periodicsNode_t));
    if (!newPeriodic)
        return NULL;
    newPeriodic->title = malloc(sizeof(char) * strlen(title) + 1);
    if (!newPeriodic->title)
        return NULL;
    newPeriodic->classificationName = malloc(sizeof(char) * 3);
    if (!newPeriodic->classificationName)
        return NULL;

    // Add the info and inicialize variables
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

// Return a pointer to a researcherRoot
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

    // Add the info and inicialize variables
    researchers->allEvents = createConferencesList();
    researchers->allPeriodics = createPeriodicsList();
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

// Create a new researcherNode
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

    // Add the info and inicialize variables
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

// Return a pointer to an authorsRoot
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

// Create a new author
authorNode_t *createAuthor(researcherNode_t *researcher, char *name)
{
    authorNode_t *authorAux = researcher->authors->head;
    while (authorAux && strcmp(authorAux->name, name))
        authorAux = authorAux->next;

    // If coauthor is already on the list, return NULL
    if (authorAux || !strcmp(name, researcher->name))
        return NULL;

    // Create a newAuthor
    authorNode_t *newAuthor = malloc(sizeof(authorNode_t));
    if (!newAuthor)
        return NULL;
    newAuthor->name = malloc(sizeof(char) * strlen(name) + 1);
    if (!newAuthor->name)
        return NULL;

    // Add the info and inicialize variables
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

// Return a pointer to a groups structure
groups_t *createGroups()
{
    groups_t *groups = malloc(sizeof(groups_t));
    if (!groups)
        return NULL;
    groups->group1_name = NULL;
    groups->group2_name = NULL;
    groups->group1 = NULL;
    groups->group2 = NULL;

    return groups;
}

// Add a new group in groups
void addGroup(groups_t *groups, researcherRoot_t *researchers, char *groupName)
{

    // First group added
    if (!groups->group1)
    {
        groups->group1 = researchers;
        groups->group1_name = malloc(sizeof(char) * strlen(groupName) + 1);
        if (!groups->group1_name)
            return;
        strcpy(groups->group1_name, groupName);
    }
    // Second group added
    else
    {
        groups->group2 = researchers;
        groups->group2_name = malloc(sizeof(char) * strlen(groupName) + 1);
        if (!groups->group2_name)
            return;
        strcpy(groups->group2_name, groupName);
    }
}

// Convert string qualisLevel into int qualisLevel
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

// Update the qualisLevel informations of an conference
void updateEventClassification(researcherRoot_t *researchers, conferenceNode_t *conference, char *qualisLevel)
{
    int classificationInt = 0;
    researcherNode_t *researcherAux = researchers->tail;
    // Get classificationName and update structures
    conference->classificationName[0] = qualisLevel[0];
    conference->classificationName[1] = qualisLevel[1];
    conference->classificationName[2] = '\0';

    // Get classificationInt and update structures
    classificationInt = getClassificationInt(qualisLevel);
    conference->classificationInt = classificationInt;
    researchers->totalConfClassifications[classificationInt]++;
    researchers->allEvents->quantityClassifications[classificationInt]++;
    researcherAux->quantityConfClassifications[classificationInt]++;
}

// Update the qualisLevel informations of a periodic
void updatePeriodicClassification(researcherRoot_t *researchers, periodicsNode_t *periodic, char *qualisLevel)
{

    int classificationInt = 0;
    researcherNode_t *researcherAux = researchers->tail;
    // Get classificationName and update structures
    periodic->classificationName[0] = qualisLevel[0];
    periodic->classificationName[1] = qualisLevel[1];
    periodic->classificationName[2] = '\0';

    // Get classificationInt and update structures
    classificationInt = getClassificationInt(qualisLevel);
    periodic->classificationInt = classificationInt;
    researchers->totalPerClassifications[classificationInt]++;
    researchers->allPeriodics->quantityClassifications[classificationInt]++;
    researcherAux->quantityPerClassifications[classificationInt]++;
}

// ======================= DESTROY FUNCTIONS =======================

// Clean events allocated memory
void destroyEvents(conferencesRoot_t *root)
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

// Clean periodics allocated memory
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
void destroyCoauthors(authorsRoot_t *authors)
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

// Clean researchers list allocated memory
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
        destroyCoauthors(aux->authors);
        free(aux);
    }
    free(root->totalConfClassifications);
    free(root->totalPerClassifications);
    destroyEvents(root->allEvents);
    destroyPeriodics(root->allPeriodics);
    free(root);
}

// Clean all allocated memory
void destroyAllStructs(researcherRoot_t *researchers, groups_t *groups)
{

    if (groups)
    {
        destroyResearchers(groups->group1);
        destroyResearchers(groups->group2);
        free(groups->group1_name);
        free(groups->group2_name);
    }
    else
        destroyResearchers(researchers);
}
