/**
 * \file BMP280.c
 *
 * \brief BMP280 Pressure Sensor Library implementation
 * 
 * This library is intended only to work on the REV1 base station, check BMP280.h for address defines and the like
 *
 * Created: 2/10/2015 20:24:55
 *  Author: Camden Miller
 *  Modified: Zidu Zhang, 04/06/2016
 */ 

//Includes//
#include "devices/old-sensor/BMP280.h"
#include "drivers/inc/TWI.h"
#include "drivers/inc/usart0.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>

static int dig_T2 , dig_T3 , dig_P2 , dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9, dig_H2, dig_H4, dig_H5; //!<Calibration values from the BMP280
static unsigned int dig_T1, dig_P1; //!<Calibration values from the BMP280
static unsigned char dig_H1, dig_H3;
static signed char dig_H6;

/*initialization*/
void BME280_ClearCalibration(void)
{
	dig_T1 = 0;
	dig_T2 = 0;
	dig_T3 = 0;
	dig_P1 = 0;
	dig_P2 = 0;
	dig_P3 = 0;
	dig_P4 = 0;
	dig_P5 = 0;
	dig_P6 = 0;
	dig_P7 = 0;
	dig_P8 = 0;
	dig_P9 = 0;
	dig_H1 = 0;
	dig_H2 = 0;
	dig_H3 = 0;
	dig_H4 = 0;
	dig_H5 = 0;
	dig_H6 = 0;
}
/****************/


static short oversampling, oversampling_t; //!<Oversampling sertings
static long signed int t_fine; 
static char error, status; //1<Error and status codes

//static function prototypes//
static char BMP280_ReadInt(char, int *);
static char BMP280_ReadInt4(char, int *);
static char BMP280_ReadInt5(char, int *);
static char BMP280_ReadUInt(char, unsigned int *);
static char BMP280_ReadChar(char, int *);
static char BMP280_ReadUChar(char, unsigned int *);
static char BMP280_ReadBytes(unsigned char *, char);
static char BMP280_WriteBytes(unsigned char *, char);

/*************************************************************************//**
  @brief Initializes the BMP280 and reads the calibration data from the device
  @return status (zero on failure, nonzero otherwise)
*****************************************************************************/
char BMP280_Init(void){
	// Initialize the TWI library at 200kHz
	//TWI_Init(200000);

	// The BMP280 includes factory calibration data stored on the device.
	// Each device has different numbers, these must be retrieved and
	// used in the calculations when taking measurements.

	// Retrieve calibration data from device:
	BME280_ClearCalibration();
	
	if (    
		BMP280_ReadUInt(0x88, &dig_T1)   &&
		BMP280_ReadInt(0x8A, &dig_T2)    &&
		BMP280_ReadInt(0x8C, &dig_T3)    &&
		BMP280_ReadUInt(0x8E, &dig_P1)   &&
		BMP280_ReadInt(0x90, &dig_P2)    &&
		BMP280_ReadInt(0x92, &dig_P3)    &&
		BMP280_ReadInt(0x94, &dig_P4)    &&
		BMP280_ReadInt(0x96, &dig_P5)    &&
		BMP280_ReadInt(0x98, &dig_P6)    &&
		BMP280_ReadInt(0x9A, &dig_P7)    &&
		BMP280_ReadInt(0x9C, &dig_P8)    &&
		BMP280_ReadInt(0x9E, &dig_P9)    &&
		BMP280_ReadUChar(0xA1, &dig_H1)  &&
		BMP280_ReadInt(0xE1, &dig_H2)    &&
		BMP280_ReadUChar(0xE3, &dig_H3)  &&
		BMP280_ReadInt4(0xE4, &dig_H4)   &&
		BMP280_ReadInt5(0xE5, &dig_H5)	 &&
		BMP280_ReadChar(0xE7, &dig_H6)    )
	{
		printf("\nT: %i ,%i ,%i P: %i ,%i ,%i ,%i ,%i ,%i ,%i ,%i ,%i \n",dig_T1,dig_T2,dig_T3,dig_P1,dig_P2,dig_P3,dig_P4,dig_P5,dig_P6,dig_P7,dig_P8,dig_P9,dig_H1,dig_H2,dig_H3,dig_H4,dig_H5,dig_H6); //%i = 16 bit
		return (1);
	}
	else 
		return (0);
}

/*************************************************************************//**
  @brief Reads an int from the BMP280
  @param[in] address The register address of the first byte of the uint
  @param[out] val* A pointer to a uint to store the received data to
  @return status (zero on failure, nonzero otherwise)
*****************************************************************************/
static char BMP280_ReadInt(char address, int *val){
	//printf("\nBMP280_ReadInt");
	unsigned char data[2];	//char is 4 bits, 1 byte
	data[0] = address;
	
	if (BMP280_ReadBytes(&data[0],2)){
		*val = (((int)data[1]<<8)|(int)data[0]);
		return(1);
	}
	*val = 0;
	return(0);
}

static char BMP280_ReadInt4(char address, int *val){
	//printf("\nBMP280_ReadInt");
	int *val1, *val2;
	unsigned char data[2];	//char is 4 bits, 1 byte
	data[0] = address;
	
	if ( (BMP280_ReadUChar(&data[0], *val1)) && (BMP280_ReadUChar(&data[1], *val2)) )
	{
		*val = (*val1<<4)|(*val2 & 0x0F);
		return(1);
	}
	/*
	if (BMP280_ReadBytes(&data[0],2)){
		*val = ( ((int)data[0]<<4)|((int)data[1]&(0x0F)) );
		return(1);
	}
	*val = 0;*/
	*val = 0;
	return(0);
}


static char BMP280_ReadInt5(char address, int *val){
	//printf("\nBMP280_ReadInt");
	int *val1, *val2;
	unsigned char data[2];	//char is 4 bits, 1 byte
	data[0] = address;
	
	if ( (BMP280_ReadUChar(&data[0], *val1)) && (BMP280_ReadUChar(&data[1], *val2)) )
	{
		*val = (*val2<<4)|(*val1>>4);
		return(1);
	}
	*val = 0;
	return(0);
}


/*************************************************************************//**
  @brief Reads an unsigned int from the BMP280
  @param[in] address The register address of the first byte of the uint
  @param[out] val* A pointer to a uint to store the received data to
  @return status (zero on failure, nonzero otherwise)
*****************************************************************************/
static char BMP280_ReadUInt(char address, unsigned int *val){
	//printf("\nBMP280_ReadUInt");
	unsigned char data[2];	//4 bits
	data[0] = address;
	
	if (BMP280_ReadBytes(&data[0],2)){
		*val = (((unsigned int)data[1]<<8)|(unsigned int)data[0]);
		return(1);
	}
	*val = 0;
	return(0);
}

/*************************************************************************//**
Has no buffer overrun protection
 @brief Read signed bytes from BMP280
 @param[in] *values pointer to an array to store the bytes, put the starting register in values[0]
 @param[in] length how many bytes to read
 @return status (zero on failure, non zero otherwise)
*****************************************************************************/
static char BMP280_ReadChar(char address, int *val){
	unsigned char data[1];
	data[0] = address;
	
	if (BMP280_ReadBytes(&data[0],1)){
		*val = (int)data[0];
		return(1);
	}
	*val = 0;
	return(0);
}

/*************************************************************************//**
Has no buffer overrun protection
 @brief Read unsigned bytes from BMP280
 @param[in] *values pointer to an array to store the bytes, put the starting register in values[0]
 @param[in] length how many bytes to read
 @return status (zero on failure, non zero otherwise)
*****************************************************************************/
static char BMP280_ReadUChar(char address, unsigned int *val){
	unsigned char data[1];
	data[0] = address;
	
	if (BMP280_ReadBytes(&data[0],1)){
		*val = (unsigned int)data[0];
		return(1);
	}
	*val = 0;
	return(0);
}

/*************************************************************************//**
Has no buffer overrun protection
  @brief Reads some bytes from the BMP280
  @param[in] *values pointer to an array to store the bytes, put the starting register in values[0]
  @param[in] length how many bytes to read
  @return status (zero on failure, nonzero otherwise)
*****************************************************************************/
static char BMP280_ReadBytes(unsigned char *values, char length){
	//printf("BMP280_ReadBytes");
	//printf("0x%1x",(unsigned)TWI_BeginWrite(BMP280_ADDR));
	TWI_BeginWrite(BMP280_ADDR);
	//printf("0x%1x",(unsigned)TWI_WriteByte(values[0]));
	TWI_WriteByte(values[0]);//Write the register address
	status = TWI_BeginRead(BMP280_ADDR); //Send a repeated start
	//printf("0x%1x",status);
	if (status == TWI_SLAR_ACK){
		if(((TWI_Read(&values[0],length,false)&TWSR_MASK) == TWI_REC_NACK) && (TWI_Stop() != 0)) return(1); //Receive bytes, send a STOP bit, and check for success
	}
	return(0);
}

/*************************************************************************//**
Has no buffer overrun protection
  @brief Reads some bytes from the BMP280
  @param[in] *values pointer to an array to send, put the starting register in values[0]
  @param[in] length how many bytes to write (including the starting register ie. sizeof(values))
  @return status (zero on failure, nonzero otherwise)
*****************************************************************************/
static char BMP280_WriteBytes(unsigned char *values, char length){
	TWI_BeginWrite(BMP280_ADDR);
	if((TWI_Write(&values[0],length) == TWI_SENT_ACK) && (TWI_Stop() != 0)) return 1; //Send bytes, a STOP bit, and check for success
	return(0);
}

/*************************************************************************//**
  @brief Gets the oversampling setting for the library
  @return oversampling
*****************************************************************************/
short BMP280_GetOversampling(void){
	return oversampling;
}

/*************************************************************************//**
  @brief Sets the oversampling setting for the library
  @param[in] oss Oversampling setting
  @return 1
*****************************************************************************/
char BMP280_SetOversampling(short oss){
	oversampling = oss;
	return (1);
}

/*************************************************************************//**
  @brief Starts a measurement
  @return time to wait for result (in ms)
*****************************************************************************/
char BMP280_StartMeasurment(void){
	unsigned char data[2], result, delay;
	
	data[0] = BMP280_REG_CONTROL;

	switch (oversampling){
		case 0:
		data[1] = BMP280_COMMAND_PRESSURE0;
		oversampling_t = 1;
		delay = 8;
		break;
		case 1:
		data[1] = BMP280_COMMAND_PRESSURE1;
		oversampling_t = 1;
		delay = 10;
		break;
		case 2:
		data[1] = BMP280_COMMAND_PRESSURE2;
		oversampling_t = 1;
		delay = 15;
		break;
		case 3:
		data[1] = BMP280_COMMAND_PRESSURE3;
		oversampling_t = 1;
		delay = 24;
		break;
		case 4:
		data[1] = BMP280_COMMAND_PRESSURE4;
		oversampling_t = 1;
		delay = 45;
		break;
		default:
		data[1] = BMP280_COMMAND_PRESSURE0;
		delay = 9;
		break;
	}
	result = BMP280_WriteBytes(&data[0], 2);
	if (result) // good write?
	return(delay); // return the delay in ms (rounded up) to wait before retrieving data
	else
	return(-1); // or return -1 if there was a problem communicating with the BMP
	//printf("failure on writeInt");
}

/*************************************************************************//**
  @brief Gets the uncalibrated temperature and pressure and humidity data
  @param[out] pointer to a place to store the pressure data
  @param[out] pointer to a place to store the temperature data
  @return status
*****************************************************************************/
char BMP280_GetUnPTH(double *uP, double *uT, double *uH){
	unsigned char data[9];
	char result;
	
	data[0] = BMP280_REG_RESULT_PRESSURE; //0xF7

	result = BMP280_ReadBytes(&data[0], 8); // 0xF7; xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE
	if (result){ // good read
		double factor = pow(2, 4);
		*uP = ( (data[0] *256.0) + data[1] + (data[2]/256.0) ) * factor ;	//20bit UP
		*uT = ( (data[3] *256.0) + data[4] + (data[5]/256.0) ) * factor ;	//20bit UT
		*uH = ( (data[6] *256.0) + data[7] ) * factor ;	//16bit UH
		printf("\nH: %i ,%i ,%i ,%i ,%i ,%i \n",dig_H1,dig_H2,dig_H3,dig_H4,dig_H5,dig_H6);
	}
	return(result);
}

/*************************************************************************//**
  @brief Gets temperature and pressure
  @param[out] pointer to a place to store the pressure data
  @param[out] pointer to a place to store the temperature data
  @return status
*****************************************************************************/
char BMP280_GetTPH(double *T, double *P, double *H){
	double uP,uT,uH ;
	char result = BMP280_GetUnPTH(&uP, &uT, &uH);
	if(result!=0){
		// calculate the temperature
		result = BMP280_CalcTemperature(T,&uT);
		if(result){
			// calculate the pressure
			result = BMP280_CalcPressure(P,&uP);
			if(result){
				result = BMP280_CalcHumidity(H,&uH); 	
				if(result)return (1);
				else error = 4 ;	// humidity error ;
				return (0);
			}else
			error = 3;	// pressure error ;
		}else
		error = 2; // temperature error
	}
	else
	error = 1;
	return (0);
}


/*************************************************************************//**
  @brief Calculates temperature
  @param[out] pointer to a place to store the temperature data
  @param[in] pointer to the uncalibrated temperature data
  @return status
*****************************************************************************/
char BMP280_CalcTemperature(double *T, double *uT){
	double adc_T = *uT ;
	//printf("adc_T = %d",adc_T);
	
	double var1 = (((double)adc_T)/16384.0-((double)dig_T1)/1024.0)*((double)dig_T2);
	double var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0)*(((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0))*((double)dig_T3);
	t_fine = (long signed int)(var1+var2);
	
	*T = (var1+var2)/5120.0;
	
	if(*T>100.0 || *T <(-100.0))return 0;
	
	return (1);
}

/*************************************************************************//**
  @brief Calculates pressure
  @param[out] pointer to a place to store the pressure data
  @param[in] pointer to the uncalibrated pressure data
  @return status
*****************************************************************************/
char BMP280_CalcPressure(double *P,double *uP){
	//char result;
	double var1 , var2 ;
	
	var1 = ((double)t_fine/2.0) - 64000.0;
	//Serial.print("var1 = ");Serial.println(var1,2);
	var2 = var1 * (var1 * ((double)dig_P6)/32768.0);	//not overflow
	//Serial.print("var2 = ");Serial.println(var2,2);
	var2 = var2 + (var1 * ((double)dig_P5)*2.0);	//overflow
	//Serial.print("var2 = ");Serial.println(var2,2);
	
	var2 = (var2/4.0)+(((double)dig_P4)*65536.0);
	//Serial.print("var2 = ");Serial.println(var2,2);
	
	var1 = (((double)dig_P3) * var1 * var1/524288.0 + ((double)dig_P2) * var1) / 524288.0;
	//Serial.print("var1 = ");Serial.println(var1,2);
	
	
	//Serial.print("(32768.0 + var1) = ");Serial.println((32768.0 + var1),5);
	
	//double t_var = (32768.0 + var1)/32768.0;
	//Serial.print("((32768.0 + var1)/32768.0) = "); Serial.println(t_var,5);
	//Serial.print("dig_P1 = ");Serial.println(dig_P1);
	//Serial.print("dig_P1 = ");Serial.println((double)dig_P1,5);
	//double tt_var = t_var * (double)dig_P1;
	
	//Serial.print("mulipication = "); Serial.println(tt_var,5);
	
	var1 = ((32768.0 + var1)/32768.0)*((double)dig_P1);
	//Serial.print("var1 = ");Serial.println(var1,2);
	
	double p = 1048576.0- (double)*uP;
	//Serial.print("p = ");Serial.println(p,2);
	
	p = (p-(var2/4096.0))*6250.0/var1 ;	//overflow
	//Serial.print("p = ");Serial.println(p,2);
	
	var1 = ((double)dig_P9)*p*p/2147483648.0;	//overflow
	
	var2 = p*((double)dig_P8)/32768.0;
	//Serial.print("var1 = ");Serial.println(var1,2);
	p = p + (var1+var2+((double)dig_P7))/16.0;
	//Serial.print("p = ");Serial.println(p,2);
	
	*P = p/100.0 ;
	
	if((*P>1200.0) || (*P < 800.0))return (0);
	return (1);
}

/*************************************************************************//**
  @brief Calculates Humidity
  @param[out] pointer to a place to store the humidity
  @param[in] pointer to the uncalibrated humidity data
  @return status
*****************************************************************************/
char BMP280_CalcHumidity(double *H,double *uH)
{
	double adc_H = *uH;
	double var1;
	
	var1 = (double)t_fine - 76800.0;
	
	if (var1 != 0)
	{
		var1 = (((((adc_H * 16384) - ((double)dig_H4 * 1048576) - ((double)dig_H5 * var1)) + 16384 ) / 32768 ) * (((((((var1 * (double)dig_H6) / 1024) * (((var1 * (double)dig_H3) / 2048) + 32768)) / 1024) + 2097152) * ((double)dig_H2 + 8192) / 16384)));
		/*var1 = (adc_H - (((double)dig_H4) * 64.0 + ((double)dig_H5) / 16384.0 * var1)) *
				(((double)dig_H2) / 65536.0 * (1.0 + ((double)dig_H6) / 67108864.0 * 
				var1 * (1.0 + ((double)dig_H3) / 67108864.0 * var1))); */
		*H = var1;
	}
	else return (0);
	var1 = (var1 - (((((var1 / 32768) * (var1 / 32768)) / 128) * (double)dig_H1) / 16));
	//var1 = var1 * (1.0 - ((double)dig_H1) * var1 / 524288.0);
	if (var1 > 100.0) {var1 = 100.0; *H = var1;}
	else if (var1 < 0.0) {var1 = 0.0; *H = var1;}
	return (1);
}


/*************************************************************************//**
  @brief Calculates pressure at sea level given an altitude
  @param[in] pressure reading
  @param[in] altitude
  @return the corrected reading
*****************************************************************************/
double BMP280_Sealevel(double P, double A){
// Given a pressure P (mb) taken at a specific altitude (meters),
// return the equivalent pressure (mb) at sea level.
// This produces pressure readings that can be used for weather measurements.
	return(P/pow(1-(A/44330.0),5.255));
}

/*************************************************************************//**
  @brief Calculates altitude
  @param[in] pressure reading
  @param[in] sea level pressure
  @return the corrected reading
*****************************************************************************/
double BMP280_Altitude(double P, double P0){
// Given a pressure measurement P (mb) and the pressure at a baseline P0 (mb),
// return altitude (meters) above baseline.
	return(44330.0*(1-pow(P/P0,1/5.255)));
}

/*************************************************************************//**
  @brief Returns the internal library error value
  @param[in] pressure reading
  @param[in] sea level pressure
  @return error value
*****************************************************************************/
char BMP280_GetError(void){
// If any library command fails, you can retrieve an extended
// error code using this command. Errors are from the wire library:
// 0 = Success
// 1 = Data too long to fit in transmit buffer
// 2 = Received NACK on transmit of address
// 3 = Received NACK on transmit of data
// 4 = Other error
	return(error);
}
