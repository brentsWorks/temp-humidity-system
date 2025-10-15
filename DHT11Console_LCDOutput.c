#include <stdint.h>
#include "sql.h"
#include "I2C-1.h"
#define MAXTIMINGS 85
#define DHTPIN 7
int dht11_dat[5] = { 0, 0, 0, 0, 0 };

float fahrenheit=0.0;
float humidity=0.0;

void read_dht11_dat()
{
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay( 18 );
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    pinMode( DHTPIN, INPUT );
    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( digitalRead( DHTPIN ) == laststate )
        {
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = digitalRead( DHTPIN );
        if ( counter == 255 )
            break;
        if ( (i >= 4) && (i % 2 == 0) )
        {
            dht11_dat[j / 8] <<= 1;
            if ( counter > 16 )
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }
    if ( (j >= 40) &&
            (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
    {
        // read into SQL as Temperature (Fahrenheit)
        fahrenheit = dht11_dat[2] * 9. / 5. + 32;
        humidity = dht11_dat[0] + (dht11_dat[1]);
        printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
                dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], fahrenheit );
        // query values into sql table
        // printf("Humidity and Temperature values have been queried into SQL database");
    }
    else
    {
        printf( "Data not good, skip\n" );
    }
}
int main( void )
{
    /*MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW;*/

    char tempBuff[8];
    char humiBuff[8];

    printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
    if ( wiringPiSetup() == -1 )
        exit( 1 );

   /* conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    */

    fd = wiringPiI2CSetup(LCDAddr);
    init();

    while ( 1 )
    {
        read_dht11_dat();

        /*sprintf(query, "insert into conditions (temperature, humidity) values ('%f', '%f')", fahrenheit, humidity);

        if (mysql_query(conn, query)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            mysql_close(conn);
            exit(1);
        }
        delay( 1000 );*/
        gcvt(fahrenheit, 4, tempBuff);
        gcvt(humidity, 4, humiBuff);

        write(0, 0, tempBuff);
        write(1, 1, humiBuff);

        delay(4000);
    }
    clear();
    return(0);
}

void write_word(int data){
	int temp = data;
	if ( BLEN == 1 )
		temp |= 0x08;
	else
		temp &= 0xF7;
	wiringPiI2CWrite(fd, temp);
}

void send_command(int comm){
	int buf;
	// Send bit7-4 firstly
	buf = comm & 0xF0;
	buf |= 0x04;			// RS = 0, RW = 0, EN = 1
	write_word(buf);
	delay(2);
	buf &= 0xFB;			// Make EN = 0
	write_word(buf);

	// Send bit3-0 secondly
	buf = (comm & 0x0F) << 4;
	buf |= 0x04;			// RS = 0, RW = 0, EN = 1
	write_word(buf);
	delay(2);
	buf &= 0xFB;			// Make EN = 0
	write_word(buf);
}

void send_data(int data){
	int buf;
	// Send bit7-4 firstly
	buf = data & 0xF0;
	buf |= 0x05;			// RS = 1, RW = 0, EN = 1
	write_word(buf);
	delay(2);
	buf &= 0xFB;			// Make EN = 0
	write_word(buf);

	// Send bit3-0 secondly
	buf = (data & 0x0F) << 4;
	buf |= 0x05;			// RS = 1, RW = 0, EN = 1
	write_word(buf);
	delay(2);
	buf &= 0xFB;			// Make EN = 0
	write_word(buf);
}

void init(){
	send_command(0x33);	// Must initialize to 8-line mode at first
	delay(5);
	send_command(0x32);	// Then initialize to 4-line mode
	delay(5);
	send_command(0x28);	// 2 Lines & 5*7 dots
	delay(5);
	send_command(0x0C);	// Enable display without cursor
	delay(5);
	send_command(0x01);	// Clear Screen
	wiringPiI2CWrite(fd, 0x08);
}

void clear(){
	send_command(0x01);	//clear Screen
}

void write(int x, int y, char data[]){
	int addr, i;
	int tmp;
	if (x < 0)  x = 0;
	if (x > 15) x = 15;
	if (y < 0)  y = 0;
	if (y > 1)  y = 1;

	// Move cursor
	addr = 0x80 + 0x40 * y + x;
	send_command(addr);

	tmp = strlen(data);
	for (i = 0; i < tmp; i++){
		send_data(data[i]);
	}
}
