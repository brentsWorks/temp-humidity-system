#ifndef DATA_ANALYSIS_H_INCLUDED
#define DATA_ANALYSIS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>

#define MAX_QUERY_LEN 1024

void calculateTempStatistics(MYSQL *conn, char startTime[], char endTime[]);
void calculateHumiStatistics(MYSQL *conn, char startTime[], char endTime[]);

#endif // DATA_ANALYSIS_H_INCLUDED
