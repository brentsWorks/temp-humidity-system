#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <string.h>

int LCDAddr = 0x27;
int BLEN = 1;
int fd;

void write_word(int);
void send_command(int);
void send_data(int);
void init();
void clear();
void write(int, int, char[]);


