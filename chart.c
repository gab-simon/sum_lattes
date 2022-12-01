#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"
#include "chart.h"
#define NUM_LINES_RESEARCHERS 18
#define NUM_LINES_GROUPS 18
#define NUM_COMMANDS_RESEARCHERS 9
#define NUM_COMMANDS_GROUPS 13

// ======================= GRAPH FUNCTIONS =======================

void insertSubstring(char *strA, char *strB, char strC[600], int pos)
{
    strncpy(strC, strA, pos);
    strC[pos] = '\0';
    strcat(strC, strB);
    strcat(strC, strA + pos);
}

// Plot a comparasion graph of periodics and events quantities
void researchersGraph(researcherRoot_t *researchers)
{
    // Store the commands used in gnuplot
    char commandsForGnuplot[NUM_COMMANDS_RESEARCHERS][600] = {
        "set title \"QUANTIDADE DE PERIÓDICOS/CONFERÊNCIAS DO GRUPO\"",
        "set boxwidth 0.7 absolute",
        "set style fill solid 1.0",
        "set ylabel \"QUANTIDADE\"",
        "set xtics offset 2,0",
        "set xrange [0:19]",
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
    char classifications[10][3] = {"", "A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C"};
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

void groupsGraph(groups_t *groups)
{
    // Store the commands used in gnuplot
    char commandsForGnuplot[NUM_COMMANDS_GROUPS][600] = {
        "set multiplot layout 1,2",
        "set title \"Histograma quantidade de periódicos dos grupos\"",
        "set boxwidth 0.3 absolute",
        "set style fill solid 1.0",
        "set ylabel \"QUANTIDADE\"",
        "set xtics offset 1,0",
        "set xrange [0:25]",
        "set yrange [0:]",
        "set key at 0, 0",
        "plot 'periodicsData1.temp' using 1:2:xtic(3) with boxes fc rgb \"#953fff\" title \"\", \
              '' using 1:2:(strcol(2)) with labels offset 0,0.5 notitle, \
              'periodicsData2.temp' using 1:2:xtic(3) with boxes fc rgb \"#ffe13f\" title \"\", \
              '' using 1:2:(strcol(2)) with labels offset 0,0.5 notitle",
        "set title \"QUANTIDADE DE EVENTOS DOS GRUPOS\"",
        "unset key",
        "plot 'eventsData1.temp' using 1:2:xtic(3) with boxes fc rgb \"#953fff\", \
              '' using 1:2:(strcol(2)) with labels offset 0,0.5, \
              'eventsData2.temp' using 1:2:xtic(3) with boxes fc rgb \"#ffe13f\", \
              '' using 1:2:(strcol(2)) with labels offset 0,0.5"};

    // Insert labels into commands strings
    char res[600];
    insertSubstring(commandsForGnuplot[9], groups->group1_name, res, 80);
    strcpy(commandsForGnuplot[9], res);
    insertSubstring(commandsForGnuplot[9], groups->group2_name, res, 252);
    strcpy(commandsForGnuplot[9], res);

    // Opens the temp files
    FILE *periodicsTemp1 = fopen("periodicsData1.temp", "w");
    FILE *periodicsTemp2 = fopen("periodicsData2.temp", "w");
    FILE *eventsTemp1 = fopen("eventsData1.temp", "w");
    FILE *eventsTemp2 = fopen("eventsData2.temp", "w");
    FILE *stats = fopen("stats.temp", "w");

    /*  Opens an interface that one can use to send commands as if they were typing into the
        gnuplot command line.  "The -persistent" keeps the plot open even after your
        C program terminates.
    */
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    char xtic[100] = "";
    char classifications[10][3] = {"", "A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C"};
    int i;
    int pos;
    for (i = 1; i <= NUM_LINES_GROUPS; i = i + 2)
    {
        // Write the data to periodicsData1.temp
        strcpy(xtic, classifications[(i + 1) / 2]);
        pos = i / 2 + 1;
        fprintf(periodicsTemp1, "%-4d %-4d %s\n", i, groups->group1->allPeriodics->quantityClassifications[pos], xtic);
        // Write the data to periodicsData2.temp
        fprintf(periodicsTemp2, "%-4d %-4d %s\n", i + 1, groups->group2->allPeriodics->quantityClassifications[pos], "");
        // Write the data to eventsData1.temp
        fprintf(eventsTemp1, "%-4d %-4d %s\n", i, groups->group1->allEvents->quantityClassifications[pos], xtic);
        // Write the data to eventsData2.temp
        fprintf(eventsTemp2, "%-4d %-4d %s\n", i + 1, groups->group2->allEvents->quantityClassifications[pos], "");
        // Write the data in stats.temp
        fprintf(stats, "%d\n", groups->group1->allPeriodics->quantityClassifications[pos]);
        fprintf(stats, "%d\n", groups->group2->allPeriodics->quantityClassifications[pos]);
        fprintf(stats, "%d\n", groups->group1->allEvents->quantityClassifications[pos]);
        fprintf(stats, "%d\n", groups->group2->allEvents->quantityClassifications[pos]);
    }

    // Send commands to gnuplot one by one.
    for (i = 0; i < NUM_COMMANDS_GROUPS; i++)
    {
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]);
    }

    // Close files
    fclose(periodicsTemp1);
    fclose(periodicsTemp2);
    fclose(eventsTemp1);
    fclose(eventsTemp2);
    fclose(gnuplotPipe);
}
