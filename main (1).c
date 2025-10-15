#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "sql.h"
#include "data_analysis.h"

void (*filterFunction[2])(MYSQL*, char[], char[]) = {calculateTempStatistics, calculateHumiStatistics};

int main () {
    MYSQL *conn;
    int choice=0;
    char startTime[20], endTime[20];

    printf("Enter a number to choose your function:\n");
    printf("%d Calculate Minimum, Maximum, Average Temperature Statistics\n", 0);
    printf("%d Calculate Minimum, Maximum, Average Humidity Statistics\n", 1);
    printf("%d Exit program\n", 2);

    do {
        printf("=> => ");
        scanf("%d", &choice);
        getchar(); // consume leftover newline char before if statement
    } while (choice < 0 || choice > 2);

    if (choice != 2) {
        printf("Enter a time range to start your data collection from (YYYY-MM-DD HH:MM:SS)\n => => ");
        fgets(startTime, sizeof(startTime), stdin);
        startTime[strcspn(startTime, "\n")] = '\0'; // remove trailing newline

        getchar(); // consume newline char again
        printf("Enter a time you'd like to conclude your data collection at (YYYY-MM-DD HH:MM:SS)\n => => ");
        fgets(endTime, sizeof(endTime), stdin);
        endTime[strcspn(endTime, "\n")] = '\0';

        getchar(); // consume newline char

        conn = mysql_init(NULL);

        if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }
        filterFunction[choice](conn, startTime, endTime);
    }

    return 0;
}



void calculateTempStatistics(MYSQL *conn, char startTime[], char endTime[]) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    // min temp
    sprintf(query, "select min(temperature) from conditions where time >= '%s' and time <= '%s'", startTime, endTime);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    res = mysql_use_result(conn);
    if ((row = mysql_fetch_row(res)) != NULL) {
        printf("Minimum Temperature: %2f Fahrenheit\n", strtod(row[0], NULL));
    }
    mysql_free_result(res);

    // max temp
    sprintf(query, "select max(temperature) from conditions where time >= '%s' and time <= '%s'", startTime, endTime);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    res = mysql_use_result(conn);
    if ((row = mysql_fetch_row(res)) != NULL) {
        printf("Maximum Temperature: %2f Fahrenheit\n", strtod(row[0], NULL));
    }
    mysql_free_result(res);

    // avg temp
    sprintf(query, "select avg(temperature) from conditions where time >= '%s' and time <= '%s'", startTime, endTime);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    res = mysql_use_result(conn);
    if ((row = mysql_fetch_row(res)) != NULL) {
        printf("Average Temperature: %2f Fahrenheit\n", strtod(row[0], NULL));
    }
    mysql_free_result(res);

    return;
}

void calculateHumiStatistics(MYSQL *conn, char startTime[], char endTime[]) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    // min temp
    sprintf(query, "select min(humidity) from conditions where time >= '%s' and time <= '%s'", startTime, endTime);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    res = mysql_use_result(conn);
    if ((row = mysql_fetch_row(res)) != NULL) {
        printf("Minimum Humidity: %2f %%\n", strtod(row[0], NULL));
    }
    mysql_free_result(res);

    // max temp
    sprintf(query, "select max(humidity) from conditions where time >= '%s' and time <= '%s'", startTime, endTime);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    res = mysql_use_result(conn);
    if ((row = mysql_fetch_row(res)) != NULL) {
        printf("Maximum Humidity: %2f %%\n", strtod(row[0], NULL));
    }
    mysql_free_result(res);

    // avg temp
    sprintf(query, "select avg(humidity) from conditions where time >= '%s' and time <= '%s'", startTime, endTime);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    res = mysql_use_result(conn);
    if ((row = mysql_fetch_row(res)) != NULL) {
        printf("Average Humidity: %2f %%\n", strtod(row[0], NULL));
    }
    mysql_free_result(res);

    return;
}
