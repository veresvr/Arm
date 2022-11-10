/*			discription:
    List of errors, which chemical barrel returns to PC.
*/

#define OK              	0x00
#define NO_ERROR	      	0x00
#define START_STOP_UNCORR       0x02
#define CRC_UNCORR      	0x03
#define UNDEF_COMMAND   	0x04
#define COMMAND_LESS_3_CHAR     0x05
#define EXCESS_NUMB     	0x06
#define EXCESS_DELAY    	0x07
#define X_SENSOR_MAX    	0x08
#define X_SENSOR_MIN    	0x09
#define Y_SENSOR_MAX    	0x0A
#define Y_SENSOR_MIN    	0x0B
#define Z_SENSOR_MAX    	0x0C
#define Z_SENSOR_MIN    	0x0D
#define SH_CANT_ON		0x0E
#define SH_OVERCURRENT	 	0x0F
#define LASER_CANT_START 	0x10
#define ONE_WIRE_NO_PRESENCE 	0x11
#define ONE_WIRE_IS_PRESENCE 	0x12
