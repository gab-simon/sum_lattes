#include "model.h"
#ifndef SUMMARIZATION_H
#define SUMMARIZATION_H

// ======================= INDIVIDUAL PRINTING ======================= 

// Print all periodics of a researcher
void printPeriodicsByClassification(researcherRoot_t *root);

// Print all events of a researcher
void printEventsByClassification(researcherRoot_t *root);

// Print quantity of periodics and events by author
void printByAuthor(researcherRoot_t *researchers);

// Print quantity of periodics and events of the group by year 
void printByYear(researcherRoot_t*researchers, int yearDelimiter);

// Print all events and periodics with C qualisLevel
void printAllC(researcherRoot_t *researchers);

// Print all events and periodics unclassified
void printUnclassified(researcherRoot_t *researchers);

// Print all coauthors of each researcher
void printCoauthors(researcherRoot_t *researchers);




// ======================= GROUP PRINTING ======================= 

// Print all periodics discriminated by qualisLevel of each group
void printPeriodicsByClassificationGroups(groups_t *groups);

// Print all events discriminated by qualisLevel of each group
void printEventsByClassificationGroups(groups_t *groups);

// Print the quantity of periodics and events of each researcher by group 
void printByAuthorGroups(groups_t *groups);

// Print the quantity of periodics and events of each year by groups
void printByYearGroups(groups_t *groups, int yearDelimiter);

// Print all events and periodics with C qualisLevel
void printAllCGroups(groups_t *groups);

// Print all events and periodics unclassified by group
void printUnclassifiedGroups(groups_t *groups);

// Print all coauthors of each researcher by group
void printCoauthorsGroup(groups_t *groups);

#endif
