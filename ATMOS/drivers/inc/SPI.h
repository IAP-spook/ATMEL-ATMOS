/**
* \file SPI.h
*
* \brief SPI declaration
*
* Created: 8/9/2016 11:08:00
*  Author: C. Martin
*/

#ifndef SPI_H_
#define SPI_H_

void spi_init_master (void);
unsigned char spi_tranceiver (unsigned char data);

// below from 'Serialize.h'
#define SPI_SR1_WEL (1 << 1)

#define NULL_PTR 0x0 // a null pointer
#define DUMMY_BYTE 0x00 //dummy byte
typedef unsigned char	uint8;
typedef signed char		sint8;
typedef unsigned int	uint16;
typedef int				sint16;
typedef unsigned long	uint32;
typedef long			sint32;
typedef enum
{
	RetSpiError,
	RetSpiSuccess
} SPI_STATUS;
// Acceptable values for SPI master side configuration
typedef enum _SpiConfigOptions
{
	OpsNull,  			// do nothing
	OpsWakeUp,			// enable transfer
	OpsInitTransfer,
	OpsEndTransfer,

} SpiConfigOptions;


// char stream definition for
typedef struct _structCharStream
{
	uint8* pChar;                                // buffer address that holds the streams
	uint32 length;                               // length of the stream in bytes
} CharStream;

SPI_STATUS Serialize_SPI(const CharStream* char_stream_send,
                         CharStream* char_stream_recv,
                         SpiConfigOptions optBefore, SpiConfigOptions optAfter) ;

#endif /* SPI_H_ */