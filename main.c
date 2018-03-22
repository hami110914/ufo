#include <pic32mx.h>
#include <stdint.h>
#include <stdlib.h>

#define DISPLAY_VDD PORTFbits.RF6
#define DISPLAY_VBATT PORTFbits.RF5
#define DISPLAY_COMMAND_DATA PORTFbits.RF4
#define DISPLAY_RESET PORTGbits.RG9
#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200

char textbuffer[4][16];
static const uint8_t const font[1024];

/* Tre olika väggar */
const uint8_t const wall[3][8] = {

	{0, 0, 255, 255, 255, 255, 255, 255},
	{255, 255, 0, 0, 0, 0, 255, 255},
	{255, 255, 255, 255, 0, 0, 0, 0},
};

/* UFO*/
const uint8_t const ufo[] = { 0x18,0x2C, 0x4A, 0x89, 0x89, 0x89, 0x4A, 0x2C, 0x18, 0x0};


/* Nummer mellan 0 till 9*/
const uint8_t const num[10][36] = {
		{
		254, 253, 123, 7, 7, 7, 7, 7, 7, 123, 253, 254,
		254, 126, 188, 192, 192, 192, 192, 192, 192, 188, 126, 254,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
		},

		{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 120, 252, 254,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 126, 254,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		},

		{
		1, 3, 7, 135, 135, 135, 135, 135, 135, 123, 253, 254,
		254, 126, 189, 195, 195, 195, 195, 195, 195, 193, 128, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
		},

		{
		1, 3, 7, 135, 135, 135, 135, 135, 135, 123, 253, 254,
		0, 128, 193, 195, 195, 195, 195, 195, 195, 189, 126, 254,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
		},

		{
		254, 252, 120, 128, 128, 128, 128, 128, 128, 120, 252, 254,
		0, 0, 1, 3, 3, 3, 3, 3, 3, 60, 126, 254,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		},

		{
		254, 253, 123, 135, 135, 135, 135, 135, 135, 7, 3, 1,
		0, 128, 193, 195, 195, 195, 195, 195, 195, 189, 126, 254,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
		},

		{
		254, 253, 123, 135, 135, 135, 135, 135, 135, 7, 3, 1,
		254, 126, 189, 195, 195, 195, 195, 195, 195, 189, 126, 254,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
		},

		{
		254, 253, 123, 7, 7, 7, 7, 7, 7, 123, 253, 254,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 126, 254,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		},

		{
		254, 253, 123, 135, 135, 135, 135, 135, 135, 123, 253, 254,
		254, 126, 189, 195, 195, 195, 195, 195, 195, 189, 126, 254,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
		},

		{
		254, 253, 123, 135, 135, 135, 135, 135, 135, 123, 253, 254,
		0, 0, 129, 195, 195, 195, 195, 195, 195, 189, 126, 254,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
		}
	};

	const uint8_t const icon[1][512] = { 								//score logo
		0xFF, 0xFF, 0xFF, 0x9F, 0x9F, 0x1F, 0x33, 0x03, 0x07, 0x0F,
    0x1F, 0x0F, 0x87, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x7F, 0x3F, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0x7F, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F,
    0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF0, 0xF0, 0xF0,
    0xF1, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x7C, 0xFE,
    0xFF, 0xFF, 0xFF, 0x8F, 0x07, 0x87, 0x87, 0xFF, 0xFF, 0xFF,
    0xFF, 0xDF, 0x03, 0xF9, 0xFC, 0x3E, 0x0E, 0x0E, 0x1E, 0xFE,
    0xFE, 0xFE, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0x4F, 0x3F, 0x7F,
    0xFF, 0xFF, 0xF9, 0x30, 0x01, 0x03, 0x03, 0x81, 0x80, 0xC0,
    0xE6, 0xE3, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
    0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x7F, 0xFF, 0xFF,
    0xBF, 0xBF, 0xBF, 0xDF, 0xDF, 0xCF, 0xC7, 0xC7, 0xC7, 0xCF,
    0xDE, 0xBF, 0xFF, 0xFF, 0x7E, 0x7E, 0x7F, 0x7F, 0x7F, 0xFF,
    0xBF, 0x9F, 0xDF, 0xEF, 0xF8, 0xF4, 0xE0, 0x3C, 0x3F, 0x0F,
    0x83, 0xE1, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xF0, 0xE1, 0xE1,
    0xE1, 0xE1, 0xE1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC3, 0xC3,
    0xC3, 0xE3, 0xE3, 0xF3, 0xF1, 0xF9, 0xFF, 0x7F, 0x3F, 0x0F,
    0x07, 0x03, 0x83, 0xC1, 0xF1, 0xF0, 0xF8, 0xFE, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
	};


	const uint8_t const logo[1][512] = {

		255, 255, 255, 255, 255, 255, 255, 255,
		0, 0, 0, 0, 0, 0, 0, 0,
		0x18,0x2C, 0x4A, 0x89, 0x89, 0x89, 0x4A, 0x2C, 0x18, 0x0,
		 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255,
		0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255,
		15, 15, 15, 15, 15, 15, 15, 15,
		15, 15, 15, 15, 15, 15, 15, 15,
		15, 15, 15, 15, 15, 15, 15, 15,
		255, 255, 255, 255, 255, 255, 255, 255,

		//
		255, 255, 255, 255, 255, 255, 255, 255,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255,
		0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255,
		0, 0, 0, 0, 0, 0, 0, 0,
		0x18,0x2C, 0x4A, 0x89, 0x89, 0x89, 0x4A, 0x2C, 0x18, 0x0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255,



		///
		255, 255, 255, 255, 255, 255, 255, 255,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255,
		0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255,

		////
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
	};

//Simple Delay
void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

//Aktiverar SPI porten
uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 0x01));
	return SPI2BUF;
}

//Funktion för att väcka skärmen
void display_init() {
	DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
	delay(10);
	DISPLAY_VDD_PORT &= ~DISPLAY_VDD_MASK;
	delay(1000000);

	spi_send_recv(0xAE);
	DISPLAY_RESET_PORT &= ~DISPLAY_RESET_MASK;
	delay(10);
	DISPLAY_RESET_PORT |= DISPLAY_RESET_MASK;
	delay(10);

	spi_send_recv(0x8D);
	spi_send_recv(0x14);

	spi_send_recv(0xD9);
	spi_send_recv(0xF1);

	DISPLAY_VBATT_PORT &= ~DISPLAY_VBATT_MASK;
	delay(10000000);

	spi_send_recv(0xA1);
	spi_send_recv(0xC8);

	spi_send_recv(0xDA);
	spi_send_recv(0x20);

	spi_send_recv(0xAF);
}

/* Skriver ut nummer från arrayen */
void display_number(int x, const uint8_t* data) {
int i, j;

	for(i = 0; i < 3; i++) {
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22); // = välj sida
		spi_send_recv(i + 1); // = vilken sida

		spi_send_recv(0x21); // välj kolumnaddress
		spi_send_recv(x & 0xF); // Kolumn (nere) lägsta 4 bits
		spi_send_recv(0x10 | ((x >> 4) & 0xF)); // uppe (övre) 4 bits

		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;

		for(j = 0; j < 12; j++)
			spi_send_recv(data[i*12 + j]);
	}
}

void display_icon(int x, const uint8_t* data) {		//Displaya ikon
	int i, j;

	for(i = 0; i < 4; i++) {
			DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
			spi_send_recv(0x22);
			spi_send_recv(i);

			spi_send_recv(0 & 0xF);
			spi_send_recv(0x10 | ((0 >> 4) & 0xF));

			DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;

			for(j = 0; j < 128; j++)
					spi_send_recv(~data[i*128 + j]);
	}
	}


	void display_logo(int x, const uint8_t* data) {		//Displaya ikon
		int i, j;

		for(i = 0; i < 4; i++) {
				DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
				spi_send_recv(0x22); //välj sida
				spi_send_recv(i);

				//spi_send_recv(0x22); //
				spi_send_recv(0 & 0xF); // Kolumn lägsta 4 bits
				spi_send_recv(0x10 | ((0 >> 4) & 0xF)); // uppe (övre) 4 bit

				DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;

				for(j = 0; j < 128; j++)
						spi_send_recv(data[i*128 + j]);
		}
	}


/* Skriver ut ufo:t på rätt ställe  */
void display_ufo(int x, const uint8_t *data) {
		int shf = 0;
		int p = 0;

		if (x == 1 | x == 3 | x == 5 | x == 7)
		{

		p = (x == 1)? 0 : (x == 3)? 1 : (x == 5)? 2 : (x == 7)? 3 : p;

		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22); // = välj sida
		spi_send_recv(p); // = vilken sida

		spi_send_recv(0x21); // välj kolumnaddress
		spi_send_recv(0x18 & 0xF); // kolumnens lägsta 4 bits
		spi_send_recv(0x10 | ((0x18 >> 4) & 0xF)); // uppe (övre) 4 bits

		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
		int j;
		for(j = 0; j < 11; j++)
			spi_send_recv((data[j] << 4));
		x++;
		shf = 4;
		}

		p = (x == 0)? 0 : (x == 2)? 1 : (x == 4)? 2 : (x == 6)? 3 : p;

		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22); // = välj sida
		spi_send_recv(p); // = vilken sida

		spi_send_recv(0x21); // välj kolumnaddress
		spi_send_recv(0x18 & 0xF); // kolumnens lägsta 4 bits
		spi_send_recv(0x10 | ((0x18 >> 4) & 0xF)); // uppe (övre) 4 bits

		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
		int j;
		for(j = 0; j < 11; j++)
			spi_send_recv(data[j] >> shf);
}

/* Skriver ut väggarna */
void display_wall(int x, const uint8_t *data) {
	int i, j;

	for(i = 0; i < 4; i++) {
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x21);
		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));

		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;

		for(j = 0; j < 2; j++)
			spi_send_recv(data[i*2 + j]);
	}
}

void display_update() {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;

		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;

			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

/*  Skriver ut poäng */
void print_max100 (uint8_t x) {

	uint8_t num_1, num_2;
	num_1 = num_2 = 0;

	uint8_t a, c, digits;
	a = 10;
	c = 1;

	if (x < 100)
	{
		digits = 2;

		if (x < 10)
			{
				digits = 1;
				num_1 = x;
				c = 0;
			}

		while (c)
		{
			if ((x >= a) & (x < (a + 10)))
			{
				num_2 = (a / 10);
				num_1 = (x - a);
				c = 0;
			}

			a = a + 10;
		}
	}
	else digits = 3;

	display_update();
	switch (digits)
	{
		case 1 :
		display_icon (0, icon[0]);
		display_number(88, num[num_1]);
		break;

		case 2 :
		display_icon (0, icon[0]);
		display_number(65, num[num_2]);
		display_number(80, num[num_1]);
		break;

		case 3 :
		display_number(43, num[1]);
		display_number(58, num[0]);
		display_number(73, num[0]);
		break;

		default :
		PORTE = 2;
		break;
	}
}


int main(void) {
	/* Fixa peripheral bussklocka */
	OSCCON &= ~0x180000;
	OSCCON |= 0x080000;

	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;

	/* Output pins för display signaler */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;

	/* Fixa input pins */
	TRISDSET = (3 << 7);
	TRISFSET = (1 << 1);

	/* Fixa SPI som master */
	SPI2CON = 0;
	SPI2BRG = 4;

	/* Ta bort SPIROV*/
	SPI2STATCLR &= ~0x40;
	/* Välj CKP = 1, MSTEN = 1; */
	SPI2CON |= 0x60;

	/* Sätt på SPI */
	SPI2CONSET = 0x8000;


	display_init();

	/* Spel parametrar*/
	uint8_t h, g, n, ufodrop, page, alive, button4, score, j, k, block;


	/* Spelet startas */
	while (1)
	{
		display_update();
		display_logo(0, logo[0]);
		delay (20000000);


	page = ufodrop = score = j = button4 = block = g = 0;
	k = 2;
	alive = 1;
	n = 128;

	/*
	alive : ufoliv status
	button4 : BTN4
	h : första väggens position
	n : andra väggens position
	g : vägg-offset nådd, tillåt skrivning 2:a väggen
	j : typ av vägg (vägg 1)
	k : typ av vägg (vägg 2)
	birddrop : counter för UFO:ts fallhöjd
	page : ufo:ts nuvarande vertikala position
	score : nuvarande poäng
	block : förhindra att knapp 4  får ett värde sin ej är bestämt
	*/

	while (alive) {
		for (h = 128; h > 0; h = h - 1)
		{
			PORTE = score;
			while (!((PORTD >> 8) & 1))
			PORTE += score;

		   //pausa spelet
			button4 = (PORTD >> 7) & 1;

			/* UFO:T hoppar uppåt när man trycker på knappen */
			if (button4 && (!block))
			{
				ufodrop = 0;
				if (page > 0)
					page--;

				block = 1;
			}
			else if (!button4) {
				block = 0;
			}

			/* Låt UFO:t falla  */
			ufodrop++;
			if (ufodrop == 6)   //tiden ms för UFO att falla
			{
				page = page + 1;
				ufodrop = 0;
			}

			/* UFO:t dör när det träffar marken */
			if (page == 7)
			{
				alive = 0;
				h = 0;
			}

			/* Så länge ufo:t lever, uppdatera skärmen  */
			if (alive)
			{
				/* Cycle rätt mellan väggar  */
				if (h == 126) {
					j++;
					if (j == 3)
						j = 0;
				}

				if (n == 126) {
					k++;
					if (k == 3)
						k = 0;
				}

				display_update();

				/* Gör en ny vägg när den har kommit förbi skärmen  */
				if ((h < 127) && (h > 1))
					display_wall(h, wall[j]);

				/* Ha två väggar på skärmen samtidigt, en offset på 64 px  */
				if (h == 64)
					g = 1;

				if (g) {
					if ((n < 127) && (n > 1))
						display_wall(n, wall[k]);
					n--;
				}

				if (n == 0)
					n = 128;

				display_ufo(page, ufo);
				delay(500000);

				/* Vägg detektor*/
				if ((h < 36) && (h > 23))
				{
					if ((j == 0) && (page > 2))
							alive = 0;

					if ((j == 1) && ((page < 3) || (page > 4)))
							alive = 0;

					if ((j == 2) && (page < 5))
							alive = 0;


					if (alive && (h == 24))
						score++;
				}

				if ((n < 36) && (n > 23))
				{
					if ((k == 0) && (page > 2))
							alive = 0;

					if ((k == 1) && ((page < 3) || (page > 4)))
							alive = 0;

					if ((k == 2) && (page < 5))
							alive = 0;


					if (alive && (n == 24))
						score++;
				}
			}

		}

	}

	/* Max poäng 100 */
	if (score > 100)
		score == 100;

	print_max100(score);

	/* Reset spelet genom att trycka på knapp 2 */
	while (!((PORTD >> 5) & 1));
	}
	return 0;
}
