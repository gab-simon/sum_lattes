#include "model.h"
#ifndef SUMMARIZATION_H
#define SUMMARIZATION_H

// Imprime todas os periodicos do pesquisador
void printPeriodicsByClassification(researcherRoot_t *root);

// Imprime todas as conferencias do pesquisador
void printConfsByQualis(researcherRoot_t *root);

// Imprime a quantidade de periodicos e conferencias do pesquisador
void printByAuthor(researcherRoot_t *researchers);

// Imprime a quantidadde de periodicos e conferencias de um algomerado por ano
void printByYear(researcherRoot_t *researchers, int yearDelimiter);

// Imprime os qualis C encotrados
void printAllC(researcherRoot_t *researchers);

// Imprime os ND/ nao classificados pela qualis
void printUnqualis(researcherRoot_t *researchers);

// Imprime os co-autores
void printCoauthors(researcherRoot_t *researchers);

// Imprime todos os periodicos da qualis do aglomerado
void printPeriodsByQualisClusters(clusters_t *clusters);

// Imprime todos os conferencias da qualis do aglomerado
void printConfsByQualisClusters(clusters_t *clusters);

// Imprime a quantidade periodicos e conferencias de cada pesquisador por aglomerado
void printByAuthorClusters(clusters_t *clusters);

// Imprime a quantidade periodicos e conferencias de cada pesquisador por ano
void printByYearClusters(clusters_t *clusters, int yearDelimiter);

// Imprime todos os qualis C do aglomerado
void printAllCClusters(clusters_t *clusters);

// Imprime os ND/ nao classificados pela qualis do aglomerado
void printUnqualisclusters(clusters_t *clusters);

// Imprime os co-autores do aglomeros
void printCoauthorsCluster(clusters_t *clusters);

#endif
