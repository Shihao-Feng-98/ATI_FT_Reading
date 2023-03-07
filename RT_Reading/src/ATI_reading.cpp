#include <ATI_reading.h>

AtiReading::AtiReading(const char* ip_addr)
{
    /* Calculate number of samples, command code, and open socket here. */
    _socketHandle = socket(AF_INET, SOCK_DGRAM, 0);
    if (_socketHandle == -1) {
        exit(1);
    }

    *(uint16*)&_request[0] = htons(0x1234);         /* standard header. */
    *(uint16*)&_request[2] = htons(COMMAND);        /* per table 9.1 in Net F/T user manual. */
    *(uint32*)&_request[4] = htonl(NUM_SAMPLES);    /* see section 9.1 in Net F/T user manual. */

    /* Sending the request. */
    _he = gethostbyname(ip_addr);
    memcpy(&_addr.sin_addr, _he->h_addr_list[0], _he->h_length);
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(PORT);

    int err = connect(_socketHandle, (struct sockaddr *)&_addr, sizeof(_addr));
    if (err == -1) {
        exit(2);
    }
}

FTData AtiReading::send_recv()
{
    send(_socketHandle, _request, 8, 0 );
    /* Receiving the response. */
    recv(_socketHandle, _response, 36, 0 );
    _resp.rdt_sequence = ntohl(*(uint32*)&_response[0]);
    _resp.ft_sequence = ntohl(*(uint32*)&_response[4]);
    _resp.status = ntohl(*(uint32*)&_response[8]);
    for(int i = 0; i < 6; i++) {
        _resp.FTData[i] = ntohl(*(int32*)&_response[12 + i * 4]);
    }
    // Extract data
    _result.Fx = _resp.FTData[0] / 1000000.0;
    _result.Fy = _resp.FTData[1] / 1000000.0;
    _result.Fz = _resp.FTData[2] / 1000000.0;
    _result.Tx = _resp.FTData[3] / 1000000.0;
    _result.Ty = _resp.FTData[4] / 1000000.0;
    _result.Tz = _resp.FTData[5] / 1000000.0;
    
    return _result;
}


