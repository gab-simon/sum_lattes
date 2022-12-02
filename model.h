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

typedef struct clusters {
    researcherRoot_t *cluster1;
    researcherRoot_t *cluster2;
    char *cluster1_name;
    char *cluster2_name;
} clusters_t;

// Cria uma lista de periodicos
periodicsRoot_t *createPeriodsList();

// Adiciona um novo periodicos
periodicsNode_t *createPeriodic(researcherRoot_t *researchers, char *title, int year, char *qualisLevel);

// Cria uma nova lista conferencia
conferencesRoot_t * createConfsList();

// Adiciona uma nova conferencia
conferenceNode_t *createConference(researcherRoot_t *researchers, char *conference, int year, char *qualisLevel);

// Cria uma lista de pesquisadores
researcherRoot_t *createResearchersList();

// Adiciona um novo pesquisador
researcherNode_t *createResearcher(researcherRoot_t *researchers, char *name);

// Cria uma nova lista de autores
authorsRoot_t *createAuthorsList();

// Adiciona um novo autor
authorNode_t *createAuthor(researcherNode_t *researcher, char *name);

// Inicializa um aglomerado de pesquisadores
clusters_t *createClusters();

// Adiciona um novo aglomerado de pesuiqsadores
void addCluster(clusters_t *clusters, researcherRoot_t *researchers, char *clusterName);

// Atualiza a lista de quantidade de qualis da conferencia
void updateEventClassification(researcherRoot_t *researchers, conferenceNode_t *conference, char *qualisLevel);

// Atualiza a lista de quantidade de qualis do periodico
void updatePeriodicClassification(researcherRoot_t *researchers, periodicsNode_t *periodic, char *qualisLevel);

// Converção de string qualis para um inteiro
int getClassificationInt(char *string);

// Limpa a memoria alocada das conferencias
void destroyConfs(conferencesRoot_t *root);

// Limpa a memoria da lista de co-autores
void destroyAuthors(authorsRoot_t *authors);

// Limpa a memoria alocada dos periodicos
void destroyPeriods(periodicsRoot_t *root);

// Limpa a memoria da lista de pesquisadores
void destroyResearchers(researcherRoot_t *root);

// Limpa toda memoria alocada
void destroyAllStructs(researcherRoot_t *root, clusters_t *clusters);

#endif
