#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"
#include "chart.h"
#define NUM_LINES_RESEARCHERS 18
#define NUM_COMMANDS_RESEARCHERS 9

void insertSubstring(char *strA, char *strB, char strC[350], int pos)
{
    strncpy(strC, strA, pos);
    strC[pos] = '\0';
    strcat(strC, strB);
    strcat(strC, strA + pos);
}

// Plota um grafico com a comparação de periodos e conferencias QUALIS
void chart(researcherRoot_t *researchers)
{
    // Store the commands used in gnuplot
    char commandsForGnuplot[NUM_COMMANDS_RESEARCHERS][350] = {
        "set boxwidth 0.7 absolute",
        "set style fill solid 1.0",
        "set title \"Quantidade de Conf. e Perió. do grupo\"",
        "set ylabel \"Quantidade\"",
        "set xtics offset 2,0",
        "set xrange [0:20]",
        "set yrange [0:]",
        "set key at 0, 0",
        "plot 'periodicsData.temp' using 1:2:xtic(3) with boxes fc rgb \"#953fff\" title \"\", \
              '' using 1:2:(strcol(2)) with labels offset 0,0.5 notitle, \
              'eventsData.temp' using 1:2 with boxes fc rgb \"#ffe13f\" title \"\", \
              '' using 1:2:(strcol(2)) with labels offset 0,0.5 notitle"};

    // Insert labels into commands strings
    char res[600];
    insertSubstring(commandsForGnuplot[8], "Periódicos", res, 79);
    strcpy(commandsForGnuplot[8], res);
    insertSubstring(commandsForGnuplot[8], "Conferências", res, 243);
    strcpy(commandsForGnuplot[8], res);

    // Opens the temp files
    FILE *periodicsTemp = fopen("periodicsData.temp", "w");
    FILE *eventsTemp = fopen("eventsData.temp", "w");

    /*  Opens an interface that one can use to send commands as if they were typing into the
        gnuplot command line.  "The -persistent" keeps the plot open even after your
        C program terminates.
    */
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");

    int i;
    int pos;
    char classifications[10][3] = {" ", "A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C"};
    for (i = 1; i <= NUM_LINES_RESEARCHERS; i = i + 2)
    {
        // Write the data to a temporary file
        pos = i / 2 + 1;
        fprintf(periodicsTemp, "%-4d %-4d %s\n", i, researchers->allPeriodics->quantityClassifications[pos], classifications[pos]);
        fprintf(eventsTemp, "%-4d %-4d %s\n", i + 1, researchers->allEvents->quantityClassifications[pos], classifications[pos]);
    }

    // Send commands to gnuplot one by one.
    for (i = 0; i < NUM_COMMANDS_RESEARCHERS; i++)
    {
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]);
    }

    // Close files
    fclose(periodicsTemp);
    fclose(eventsTemp);
    fclose(gnuplotPipe);
}