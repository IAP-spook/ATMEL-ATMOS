/**
* \file SPI.c
*
* \brief SPI implementation for Flash Memory
*
* Created: 8/9/2016 11:02:00
*  Author: C. Martin
*/
#include <drivers/inc/SPI.h>
#include <avr/io.h>

//Initialize SPI Master Device
void spi_init_master (void)
{
	DDRB &= ~((1<<PORTB2)|(1<<PORTB3)|(1<<PORTB1)|(1<<PORTB0));
    DDRB |= (1<<PORTB1)|(1<<PORTB2)|(1<<PORTB0);              //Set MOSI, SCK as Output
    SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR1)|(1<<SPR0); //Enable SPI, Set as Master
                                       //Prescaler: Fosc/16, Enable Interrupts
}
//Function to send and receive data
unsigned char spi_tranceiver (unsigned char data)
{
    SPDR = data;                       //Load data into the buffer
    while(!(SPSR & (1<<SPIF) ));       //Wait until transmission complete
    return(SPDR);                      //Return received data
}

//generic function based off of Serialize.c
SPI_STATUS Serialize_SPI(const CharStream* char_stream_send,
                         CharStream* char_stream_recv,
                         SpiConfigOptions optBefore,
                         SpiConfigOptions optAfter
                        )
{
	uint8 *char_send, *char_recv;
	uint16 rx_len = 0, tx_len = 0;
	
	int i;
	printf("\nSEND:");
	for(i=0; i<char_stream_send->length; i++)
		printf(" 0x%x \n",char_stream_send->pChar[i]);

	tx_len = char_stream_send->length;
	char_send = char_stream_send->pChar;

	if (NULL_PTR != char_stream_recv)
	{
		rx_len = char_stream_recv->length;
		char_recv = char_stream_recv->pChar;
	}



	//ConfigureSpi(optBefore);

	while (tx_len-- > 0)
	{
		spi_tranceiver (*(char_send++));
	}
	
	while (rx_len-- > 0)
	{
		*(char_recv++) = spi_tranceiver (0xff);
		//printf("%x\n",spi_tranceiver(0xff));
	}

	//ConfigureSpi(optAfter);

printf("\nREC:");
for(i=0; i<char_stream_recv->length; i++)
printf(" 0x%x \n",char_stream_recv->pChar[i]);

	return RetSpiSuccess;
}