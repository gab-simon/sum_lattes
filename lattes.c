#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <dirent.h>
#include "model.h"
#include "summarization.h"
#include "math.h"
#include "chart.h"
#define BUFFER 1024


// Valor do conteudo da tag
char *getContentXML(char *string, char content[BUFFER], FILE *resume)
{
    char *tag_xml;
    tag_xml = strtok(string, "\"");
    tag_xml = strtok(NULL, "=");
    // Get all the words until find the next quotes
    while (!strstr(tag_xml, "\""))
    {
        strcat(content, tag_xml);
        strcat(content, " ");
        fscanf(resume, "%s", tag_xml);
    }
    strtok(tag_xml, "\"");
    strcat(content, tag_xml);
    return content;
}

// Encontra a lista de eventos do aquivo .txt
char *findEvents(FILE *qualisConf, char *classification, char *conferenceTitle)
{
    int distance;
    char string[BUFFER];
    int minDistance = -1;
    // Transform event name in lowercase for comparison
    char lowerEvent[BUFFER] = "";
    char lowerString[BUFFER] = "";
    convertLowerCase(conferenceTitle, lowerEvent);
    // Search the match event in qualisConf
    while (!feof(qualisConf))
    {
        fgets(string, BUFFER, qualisConf);
        convertLowerCase(string, lowerString); // Transform qualisConf line in lowercase for comparison
        distance = levenshteinDistance(lowerString, lowerEvent);
        // Copy into classification the string line with min distance
        if (((minDistance == -1) || distance < minDistance) && (distance - 9 < strlen(lowerEvent) / 2.5))
        {
            strcpy(conferenceTitle, string);
            classification[0] = conferenceTitle[strlen(conferenceTitle) - 3];
            classification[1] = conferenceTitle[strlen(conferenceTitle) - 2];
            classification[2] = '\0';
            conferenceTitle[strlen(conferenceTitle) - 3] = '\0';
            minDistance = distance;
        }
    }
    // If the event is not in qualisConf, it's unclassified
    if (minDistance == -1)
        strcpy(classification, "NC\n");

    return classification;
}

// Search the match of an event or periodic into files .txt
char *findPeriodics(FILE *qualisPer, char *classification, char *periodicTitle)
{
    char string[BUFFER];
    int distance;
    int minDistance = -1;
    // Transform periodic name in lowercase for comparison
    char lowerPeriodic[BUFFER] = "";
    char lowerString[BUFFER] = "";
    convertLowerCase(periodicTitle, lowerPeriodic);

    while (!feof(qualisPer))
    {
        fgets(string, BUFFER, qualisPer);
        convertLowerCase(string, lowerString);                  
        distance = levenshteinDistance(lowerString, lowerPeriodic);
        // Copy into classification the string line with min distance
        if (((minDistance == -1) || distance < minDistance) && (distance - 3 < strlen(lowerPeriodic) / 3))
        {
            strcpy(periodicTitle, string);
            classification[0] = periodicTitle[strlen(periodicTitle) - 3];
            classification[1] = periodicTitle[strlen(periodicTitle) - 2];
            classification[2] = '\0';
            periodicTitle[strlen(periodicTitle) - 3] = '\0';
            minDistance = distance;
        }
    }
    // If the event is not in qualisConf, it's unclassified
    if (minDistance == -1)
        strcpy(classification, "NC\n");

    return classification;
}

// Search for subdirectories and put the names inside an array of strings
void getSubdirectories(char *path, char newPaths[2][BUFFER], char subdirectories[2][BUFFER])
{
    DIR *dir = opendir(path);
    struct dirent *file;
    file = readdir(dir);
    int i = 0;
    while (file)
    {
        if (strcmp(file->d_name, ".") && strcmp(file->d_name, ".."))
        {
            strcpy(newPaths[i], path);
            strcat(newPaths[i], "/");
            strcat(newPaths[i], file->d_name);
            strcpy(subdirectories[i], file->d_name);
            i++;
        }
        file = readdir(dir);
    }
}

int main(int argc, char **argv)
{
    int option = 0;
    int flag_subdirectory = 0;
    int limite_year = 0;
    char path_subdirectory[BUFFER] = "";
    char path_file[BUFFER] = "";
    char path_dirent[BUFFER] = "";
    char path_qualisPer[BUFFER] = "";
    char path_qualisConf[BUFFER] = "";

    while ((option = getopt(argc, argv, "d:c:p:y:s:")) != -1)
        switch (option)
        {
        case 'd':
            strcpy(path_dirent, optarg);
            break;
        case 'c':
            strcpy(path_qualisConf, optarg);
            break;
        case 'p':
            strcpy(path_qualisPer, optarg);
            break;
        case 'y':
            limite_year = atoi(optarg);
            break;
        case 's':
            flag_subdirectory = 1;
            strcpy(path_subdirectory, optarg);
            break;
        default:
            printf("Falta argumentos!\n");
            exit(1);
        }

    if (argc == 1)
    {
        printf("Falta argumentos!\n");
        exit(1);
    }

    // Create necessary structs
    researcherRoot_t *researchers = NULL;
    researcherNode_t *researcherAux = NULL;
    groups_t *groups = NULL;

    // Create necessary variables
    DIR *dir;
    struct dirent *dirFile;
    FILE *resume;
    FILE *qualisConf = fopen(path_qualisConf, "r");
    FILE *qualisPer = fopen(path_qualisPer, "r");
    char string[BUFFER] = "";
    char newPaths[2][BUFFER] = {"", ""};
    char subdirectories[2][BUFFER] = {"", ""};
    char periodicTitle[BUFFER] = "";
    char conferenceTitle[BUFFER] = "";
    char name[BUFFER] = "";
    char coauthor[BUFFER] = "";
    char year[BUFFER] = "";
    char classification[BUFFER] = "";
    int nameVerified;
    int i = 0;

    if (flag_subdirectory)
    {
        getSubdirectories(path_subdirectory, newPaths, subdirectories);
        groups = createGroups();
    }

    // If subdiretory flag is actated, the program will get the information of two researchers
    for (i = 0; i <= flag_subdirectory; i++)
    {
        researchers = createResearchersList();
        // Open directory or flag_subdirectory
        if (flag_subdirectory)
        {
            strcpy(path_dirent, newPaths[i]);
            printf("-------------------------GRUPO-------------------------%s\n", subdirectories[i]);
        }
        dir = opendir(path_dirent);
        // Read xml files inside directory
        for (;;)
        {
            dirFile = readdir(dir);
            if (!dirFile)
                break;
            if (dirFile->d_type != 8)
                continue;
            printf("Carregando informações do arquivo %s...\n", dirFile->d_name);

            // Get full path_file
            path_file[0] = '\0';
            strcat(path_file, path_dirent);
            strcat(path_file, "/");
            strcat(path_file, dirFile->d_name);
            resume = fopen(path_file, "r");

            // ==== Read resume and get name, events and periodics informations ====
            nameVerified = 0;
            name[0] = '\0';
            while (!feof(resume))
            {
                fscanf(resume, "%s", string);
                // --- Get researcher name ---
                if (strstr(string, "DADOS-GERAIS") && !nameVerified)
                {
                    fscanf(resume, "%s", string); // get NOME-COMPLETO tag
                    getContentXML(string, name, resume);
                    researcherAux = createResearcher(researchers, name);
                    nameVerified = 1;
                }
                // --- Get events ---
                else if (strstr(string, "<TRABALHO-EM-EVENTOS"))
                {
                    fscanf(resume, "%s", string);
                    // Search for the next ocorrency of tag TRABALHO-EM-EVENTOS
                    while (!strstr(string, "<AUTORES"))
                    {
                        // Search for de subtag ANO-DO-TRABALHO
                        if (strstr(string, "ANO-DO-TRABALHO=") || strstr(string, "ANO="))
                        {
                            getContentXML(string, year, resume);
                            if (atoi(year) < researchers->minimumYear || researchers->minimumYear == 0)
                                researchers->minimumYear = atoi(year);
                            else if (atoi(year) > researchers->maximumYear || researchers->maximumYear == 0)
                                researchers->maximumYear = atoi(year);
                        }
                        // Search for the subtag NOME-DO-EVENTO and create a new event instance
                        else if (strstr(string, "NOME-DO-EVENTO=\""))
                        {
                            rewind(qualisConf);
                            getContentXML(string, conferenceTitle, resume);
                            findEvents(qualisConf, classification, conferenceTitle); // Search the event name and classification in qualisConf
                            createConference(researchers, conferenceTitle, atoi(year), classification);
                            classification[0] = '\0';
                            conferenceTitle[0] = '\0';
                            year[0] = '\0';
                        }
                        fscanf(resume, "%s", string);
                    }
                }
                // --- Get coauthors ---
                if (strstr(string, "NOME-COMPLETO-DO-AUTOR="))
                {
                    getContentXML(string, coauthor, resume);
                    createAuthor(researcherAux, coauthor);
                    coauthor[0] = '\0';
                }
                // --- Get periodics ---
                // Search for the next ocorrency of tag ANO-DO-ARTIGO
                else if (strstr(string, "ANO-DO-ARTIGO="))
                    getContentXML(string, year, resume);
                // Search for the next ocorrency of tag TÍTULO-DO-PERIODICO-OU-REVISTA and create a new periodic instance
                else if (strstr(string, "TITULO-DO-PERIODICO-OU-REVISTA"))
                {
                    rewind(qualisPer);
                    getContentXML(string, periodicTitle, resume);
                    findPeriodics(qualisPer, classification, periodicTitle); // Search the periodic title and classification in qualisPer
                    createPeriodic(researchers, periodicTitle, atoi(year), classification);
                    periodicTitle[0] = '\0';
                    year[0] = '\0';
                }
            }
        }

        // Add the researchers in groups
        if (flag_subdirectory)
        {
            addGroup(groups, researchers, subdirectories[i]);
            printf("\n");
        }
    }

    // Subdirectories printing
    if (flag_subdirectory)
    {
        printPeriodicsByClassificationGroups(groups);
        printf("\n\n\n\n");
        printEventsByClassificationGroups(groups);
        printf("\n\n\n\n");
        printByAuthorGroups(groups);
        printf("\n\n\n\n");
        printByYearGroups(groups, limite_year);
        printf("\n\n\n\n");
        printAllCGroups(groups);
        printf("\n\n\n\n");
        printUnclassifiedGroups(groups);
        printCoauthorsGroup(groups);
        groupsGraph(groups);
    }
    // Directory printing
    else
    {
        printf("\n\n");
        printPeriodicsByClassification(researchers);
        printf("\n\n");
        printEventsByClassification(researchers);
        printf("\n\n");
        printByAuthor(researchers);
        printf("\n\n");
        printByYear(researchers, limite_year);
        printf("\n\n");
        printAllC(researchers);
        printf("\n\n");
        printUnclassified(researchers);
        printf("\n\n");
        printCoauthors(researchers);
        researchersGraph(researchers);
    }

    // Clean allocated memory
    fclose(qualisConf);
    fclose(qualisPer);
    closedir(dir);
    destroyAllStructs(researchers, groups);
    exit(0);
    return 0;
}
