#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"
#include "chart.h"
#define MAX_QUALIS 18
#define MAX_INSTRUCTIONS 9

void setString(char *strA, char *strB, char strC[350], int position)
{
    strncpy(strC, strA, position);
    strC[position] = '\0';
    strcat(strC, strB);
    strcat(strC, strA + position);
}

// Plota um grafico com a comparação de periodos e conferencias QUALIS
void chart(researcherRoot_t *researchers)
{
    // Salva na array todos os comandos para gerar um grafico
    char commandsForGnuplot[MAX_INSTRUCTIONS][350] = {
        "set boxwidth 0.2 absolute",
        "set style fill solid 1.0",
        "set title \"Quantidade de Conf. e Perió. pesquisadores\"",
        "set ylabel \"Quantidade\"",
        "set xtics offset 3,0",
        "set xrange [0:20]",
        "set yrange [0:]",
        "set key at 0, 0",
        "plot 'DataPeriodics.out' using 1:2:xtic(3) with boxes fc rgb \"#953fff\" title \"\", \
              '' using 1:2:(strcol(2)) with labels offset 0,0.5 notitle, \
              'DataPeriodics.out' using 1:2 with boxes fc rgb \"#ffe13f\" title \"\", \
              '' using 1:2:(strcol(2)) with labels offset 0,0.5 notitle"};

    char respective[600];
    setString(commandsForGnuplot[8], "Periódicos", respective, 79);
    strcpy(commandsForGnuplot[8], respective);

    setString(commandsForGnuplot[8], "Conferências", respective, 243);
    strcpy(commandsForGnuplot[8], respective);

    FILE *DataPeriodics = fopen("DataPeriodics.out", "w");
    FILE *DataConfs = fopen("DataPeriodics.out", "w");

    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");

    int i;
    int position;
    char classifications[10][3] = {" ", "A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C"};
    
    for (i = 1; i <= MAX_QUALIS; i = i + 2)
    {
        position = i / 2 + 1;
        fprintf(DataPeriodics, "%-4d %-4d %s\n", i, researchers->allPeriodics->quantityClassifications[position], classifications[position]);
        fprintf(DataConfs, "%-4d %-4d %s\n", i + 1, researchers->allEvents->quantityClassifications[position], classifications[position]);
    }

    // Manda os comandos para o gnuplot
    for (i = 0; i < MAX_INSTRUCTIONS; i++)
    {
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]);
    }

    fclose(DataPeriodics);
    fclose(DataConfs);
    fclose(gnuplotPipe);
}