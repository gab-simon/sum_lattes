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
char *getContentXML(char *string, char content[BUFFER], FILE *curriculum)
{
    char *tag_xml;
    tag_xml = strtok(string, "\"");
    tag_xml = strtok(NULL, "=");
    while (!strstr(tag_xml, "\""))
    {
        strcat(content, tag_xml);
        strcat(content, " ");
        fscanf(curriculum, "%s", tag_xml);
    }
    strtok(tag_xml, "\"");
    strcat(content, tag_xml);
    return content;
}

// Encontra a lista de conferencias do aquivo qualis.....txt
char *findConf(FILE *qualisConf, char *qualisLevel, char *confTitle)
{
    char string[BUFFER];
    int distance;
    int minimalDistance = -1;

    // Deixa a string em minuscula para comparação 
    char lowerConf[BUFFER] = "";
    char lowerString[BUFFER] = "";
    convertLowerCase(confTitle, lowerConf);

    // Acha a conferencia compativel
    while (!feof(qualisConf))
    {
        fgets(string, BUFFER, qualisConf);

        // Deixa a classificação qualis em minuscula para comparação
        convertLowerCase(string, lowerString); 
        distance = levenshteinDistance(lowerString, lowerConf);

        // Copy into qualisLevel the string line with min distance
        if (((minimalDistance == -1) || distance < minimalDistance) && (distance - 9 < strlen(lowerConf) / 2.5))
        {
            strcpy(confTitle, string);
            qualisLevel[0] = confTitle[strlen(confTitle) - 3];
            qualisLevel[1] = confTitle[strlen(confTitle) - 2];
            qualisLevel[2] = '\0';
            confTitle[strlen(confTitle) - 3] = '\0';

            minimalDistance = distance;
        }
    }

    // Registra as não encontradas no arquivo .txt
    if (minimalDistance == -1)
        strcpy(qualisLevel, "ND\n");

    return qualisLevel;
}

// Encontra a lista de periodicas do aquivo qualis.....txt
char *findPeriodics(FILE *qualisPer, char *qualisLevel, char *periodicTitle)
{
    char string[BUFFER];
    int distance;
    int minimalDistance = -1;

    // Deixa a string em minuscula para comparação 
    char lowerPeriodic[BUFFER] = "";
    char lowerString[BUFFER] = "";
    convertLowerCase(periodicTitle, lowerPeriodic);

    // Acha o periodico compativel
    while (!feof(qualisPer))
    {
        fgets(string, BUFFER, qualisPer);

        // Deixa a classificação qualis em minuscula para comparação
        convertLowerCase(string, lowerString);                  
        distance = levenshteinDistance(lowerString, lowerPeriodic);

        // Copy into qualisLevel the string line with min distance
        if (((minimalDistance == -1) || distance < minimalDistance) && (distance - 3 < strlen(lowerPeriodic) / 3))
        {
            strcpy(periodicTitle, string);
            qualisLevel[0] = periodicTitle[strlen(periodicTitle) - 3];
            qualisLevel[1] = periodicTitle[strlen(periodicTitle) - 2];
            qualisLevel[2] = '\0';
            periodicTitle[strlen(periodicTitle) - 3] = '\0';

            minimalDistance = distance;
        }
    }

    // Registra as não encontradas no arquivo txt
    if (minimalDistance == -1)
        strcpy(qualisLevel, "ND\n");

    return qualisLevel;
}

// Pega o subdiretorio e coloca em uma array
void getSubdirent(char *path, char new_path[2][BUFFER], char subdirectories[2][BUFFER])
{
    DIR *dirent = opendir(path);
    struct dirent *file;
    file = readdir(dirent);

    int i = 0;
    while (file)
    {
        if (strcmp(file->d_name, ".") && strcmp(file->d_name, ".."))
        {
            strcpy(new_path[i], path);
            strcat(new_path[i], "/");
            strcat(new_path[i], file->d_name);
            strcpy(subdirectories[i], file->d_name);

            i++;
        }
        file = readdir(dirent);
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

    DIR *dirent;

    struct dirent *dirFile;

    FILE *curriculum;
    FILE *qualisConf = fopen(path_qualisConf, "r");
    FILE *qualisPer = fopen(path_qualisPer, "r");

    char string[BUFFER] = "";
    char new_path[2][BUFFER] = {"", ""};
    char subdirectories[2][BUFFER] = {"", ""};
    char periodicTitle[BUFFER] = "";
    char confTitle[BUFFER] = "";
    char name[BUFFER] = "";
    char coauthor[BUFFER] = "";
    char year[BUFFER] = "";
    char qualisLevel[BUFFER] = "";
    int nameVerified;

    researcherRoot_t *researchers = NULL;
    researcherNode_t *researcherAux = NULL;
    groups_t *groups = NULL;

    if (flag_subdirectory)
    {
        getSubdirent(path_subdirectory, new_path, subdirectories);
        groups = createGroups();
    }

    // If subdiretory flag is actated, the program will get the information of two researchers
    for (int i = 0; i <= flag_subdirectory; i++)
    {
        researchers = createResearchersList();
        // Abre o diretorio ou um sub diretorio
        if (flag_subdirectory)
        {
            strcpy(path_dirent, new_path[i]);
            printf("-------------------------GRUPO-------------------------%s\n", subdirectories[i]);
        }
        dirent = opendir(path_dirent);
        
        // Leitura de arquivo xml
        for (;;)
        {
            dirFile = readdir(dirent);
            if (!dirFile)
                break;
            if (dirFile->d_type != 8)
                continue;

            printf("Processsando arquivos ---> %s\n", dirFile->d_name);

            // Get full path_file
            path_file[0] = '\0';
            strcat(path_file, path_dirent);
            strcat(path_file, "/");
            strcat(path_file, dirFile->d_name);

            curriculum = fopen(path_file, "r");

            // ==== Read curriculum and get name, events and periodics informations ====
            nameVerified = 0;
            name[0] = '\0';
            while (!feof(curriculum))
            {
                fscanf(curriculum, "%s", string);
                // --- Get researcher name ---
                if (strstr(string, "DADOS-GERAIS") && !nameVerified)
                {
                    fscanf(curriculum, "%s", string); // get NOME-COMPLETO tag
                    getContentXML(string, name, curriculum);
                    researcherAux = createResearcher(researchers, name);
                    nameVerified = 1;
                }
                // --- Get events ---
                else if (strstr(string, "<TRABALHO-EM-EVENTOS"))
                {
                    fscanf(curriculum, "%s", string);
                    // Search for the next ocorrency of tag TRABALHO-EM-EVENTOS
                    while (!strstr(string, "<AUTORES"))
                    {
                        // Search for de subtag ANO-DO-TRABALHO
                        if (strstr(string, "ANO-DO-TRABALHO=") || strstr(string, "ANO="))
                        {
                            getContentXML(string, year, curriculum);
                            if (atoi(year) < researchers->minimumYear || researchers->minimumYear == 0)
                                researchers->minimumYear = atoi(year);
                            else if (atoi(year) > researchers->maximumYear || researchers->maximumYear == 0)
                                researchers->maximumYear = atoi(year);
                        }
                        // Search for the subtag NOME-DO-EVENTO and create a new event instance
                        else if (strstr(string, "NOME-DO-EVENTO=\""))
                        {
                            rewind(qualisConf);
                            getContentXML(string, confTitle, curriculum);
                            findConf(qualisConf, qualisLevel, confTitle); // Search the event name and qualisLevel in qualisConf
                            createConference(researchers, confTitle, atoi(year), qualisLevel);
                            qualisLevel[0] = '\0';
                            confTitle[0] = '\0';
                            year[0] = '\0';
                        }
                        fscanf(curriculum, "%s", string);
                    }
                }
                // --- Get coauthors ---
                if (strstr(string, "NOME-COMPLETO-DO-AUTOR="))
                {
                    getContentXML(string, coauthor, curriculum);
                    createAuthor(researcherAux, coauthor);
                    coauthor[0] = '\0';
                }
                // --- Get periodics ---
                // Search for the next ocorrency of tag ANO-DO-ARTIGO
                else if (strstr(string, "ANO-DO-ARTIGO="))
                    getContentXML(string, year, curriculum);
                // Search for the next ocorrency of tag TÍTULO-DO-PERIODICO-OU-REVISTA and create a new periodic instance
                else if (strstr(string, "TITULO-DO-PERIODICO-OU-REVISTA"))
                {
                    rewind(qualisPer);
                    getContentXML(string, periodicTitle, curriculum);
                    findPeriodics(qualisPer, qualisLevel, periodicTitle); // Search the periodic title and qualisLevel in qualisPer
                    createPeriodic(researchers, periodicTitle, atoi(year), qualisLevel);
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
        printf("\n\n");
        printEventsByClassificationGroups(groups);
        printf("\n\n");
        printByAuthorGroups(groups);
        printf("\n\n");
        printByYearGroups(groups, limite_year);
        printf("\n\n");
        printAllCGroups(groups);
        printf("\n\n");
        printUnclassifiedGroups(groups);
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
        chart(researchers);
    }

    // Clean allocated memory
    fclose(qualisConf);
    fclose(qualisPer);
    closedir(dirent);
    destroyAllStructs(researchers, groups);
    exit(0);
    return 0;
}
