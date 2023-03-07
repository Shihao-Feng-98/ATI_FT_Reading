#ifndef ATI_READING_H
#define ATI_READING_H

#include <arpa/inet.h>     // ip地址转换函数
#include <sys/types.h>     // 数据类型定义
#include <sys/socket.h>    // socket函数与数据结构
#include <netdb.h>         // 捕获和设置域名函数
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 49152 /* Port the Net F/T always uses */
#define COMMAND 2 /* Command code 2 starts streaming */
#define NUM_SAMPLES 1 /* Will send 1 sample before stopping */

/* Typedefs used so integer sizes are more explicit */
typedef unsigned int uint32;
typedef int int32;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned char byte;
typedef struct response_struct {
    uint32 rdt_sequence;
    uint32 ft_sequence;
    uint32 status;
    int32 FTData[6];
} RESPONSE;

typedef struct FTData_struct {
    float Fx, Fy, Fz;
    float Tx, Ty, Tz;
} FTData;

class AtiReading
{
private:
    int _socketHandle;			/* Handle to UDP socket used to communicate with Net F/T. */
    struct sockaddr_in _addr;	/* Address of Net F/T. */
    struct hostent *_he;		/* Host entry for Net F/T. */
    byte _request[8];			/* The request data sent to the Net F/T. */
    RESPONSE _resp;				/* The structured response received from the Net F/T. */
    byte _response[36];			/* The raw response data received from the Net F/T. */
    FTData _result;              /* The output data [Fx Fy Fz Tx Ty Tz] in N/Nm */

public:
    AtiReading(const char* ip_addr);
    // ~AtiReading();

    FTData send_recv();
};


#endif