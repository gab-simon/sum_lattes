#ifndef MODEL_H
#define MODEL_H


typedef struct authorNode {
    char *name;
    struct authorNode *next;
} authorNode_t;

typedef struct authorsRoot {
    int quantity;
    authorNode_t *head;
    authorNode_t *tail;
} authorsRoot_t;

typedef struct eventNode {
    char *conference;
    char *classificationName;
    int classificationInt;
    int numParticipations;
    int year;
    struct eventNode *next;
} conferenceNode_t;

typedef struct eventsRoot {
    int quantity;
    int *quantityClassifications;
    conferenceNode_t *head;
    conferenceNode_t *tail;
} conferencesRoot_t;

typedef struct periodicsNode {
    char *title;
    char *classificationName;
    int classificationInt;
    int numPublications;
    int year;
    struct periodicsNode *next;
} periodicsNode_t;

typedef struct periodicsRoot {
    int quantity;
    int *quantityClassifications;
    periodicsNode_t *head;
    periodicsNode_t *tail;
} periodicsRoot_t;

typedef struct researcherNode {
    char *name;
    int *quantityPerClassifications;
    int *quantityConfClassifications;
    authorsRoot_t *authors;
    struct researcherNode *next;
} researcherNode_t;

typedef struct researcherRoot {
    int quantity;
    int minimumYear;
    int maximumYear;
    int *totalConfClassifications;
    int *totalPerClassifications;
    periodicsRoot_t *allPeriodics;
    conferencesRoot_t *allEvents;
    researcherNode_t *head;
    researcherNode_t *tail;
} researcherRoot_t;

typedef struct groups {
    researcherRoot_t *group1;
    researcherRoot_t *group2;
    char *group1_name;
    char *group2_name;
} groups_t;

// Cria uma lista de periodos
periodicsRoot_t *createPeriodicsList();

// Cria um novo periodo
periodicsNode_t *createPeriodic(researcherRoot_t *researchers, char *title, int year, char *classification);

// Cria uma lista de pesquisadores
researcherRoot_t *createResearchersList();

// Cria um novo pesquisador
researcherNode_t *createResearcher(researcherRoot_t *researchers, char *name);

// Cria uma nova lista conferencia
conferencesRoot_t * createConferencesList();

// Cria uma nova conferencia
conferenceNode_t *createConference(researcherRoot_t *researchers, char *conference, int year, char *classification);

// Cria uma nova lista de autores
authorsRoot_t *createAuthorsList();

// Cria um novo autor
authorNode_t *createAuthor(researcherNode_t *researcher, char *name);

// Return a pointer to a groups structure
groups_t *createGroups();

// Add a new group in groups
void addGroup(groups_t *groups, researcherRoot_t *researchers, char *groupName);

// Update the classification informations of an conference
void updateEventClassification(researcherRoot_t *researchers, conferenceNode_t *conference, char *classification);

// Update the classification informations of a periodic
void updatePeriodicClassification(researcherRoot_t *researchers, periodicsNode_t *periodic, char *classification);

// Limpa toda memoria alocada
void destroyAllStructs(researcherRoot_t *root, groups_t *groups);

#endif
