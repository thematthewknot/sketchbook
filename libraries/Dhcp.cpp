// DHCP Library v0.3 - April 25, 2009
// Author: Jordan Terrell - blog.jordanterrell.com

extern "C" {
  #include "types.h"
  #include "w5100.h"
  #include "sockutil.h"
  #include "socket.h"
  #include "spi.h"
}

#include <string.h>
#include <stdlib.h>
#include "Dhcp.h"
#include "wiring.h"

int DhcpClass::beginWithDHCP(uint8_t *mac, u_long timeout, u_long responseTimeout)
{
    uint8_t dhcp_state = STATE_DHCP_START;
    u_char messageType = 0;
  
    // zero out _dhcpMacAddr, _dhcpSubnetMask, _dhcpGatewayIp, _dhcpLocalIp, _dhcpDhcpServerIp, _dhcpDnsServerIp
    memset(_dhcpMacAddr, 0, 26); 

    memcpy((void*)_dhcpMacAddr, (void*)mac, 6);
  
    iinchip_init();
    setSHAR(_dhcpMacAddr);
    setSIPR(_dhcpLocalIp);
    
    sysinit(0x55, 0x55);
    if(socket(0, Sn_MR_UDP, DHCP_CLIENT_PORT, 0) <= 0)
    {
      return -1;
    }
    
    presend_DHCP();
    
    int result = 0;
    
    u_long startTime = millis();
    
    while(dhcp_state != STATE_DHCP_LEASED)
    {
        if(dhcp_state == STATE_DHCP_START)
        {
            _dhcpTransactionId++;
            
            send_DHCP_MESSAGE(DHCP_DISCOVER, ((millis() - startTime) / 1000));
            dhcp_state = STATE_DHCP_DISCOVER;
        }
        else if(dhcp_state == STATE_DHCP_DISCOVER)
        {
            messageType = parseDHCPResponse(responseTimeout);
            if(messageType == DHCP_OFFER)
            {
                send_DHCP_MESSAGE(DHCP_REQUEST, ((millis() - startTime) / 1000));
                dhcp_state = STATE_DHCP_REQUEST;
            }
        }
        else if(dhcp_state == STATE_DHCP_REQUEST)
        {
            messageType = parseDHCPResponse(responseTimeout);
            if(messageType == DHCP_ACK)
            {
                dhcp_state = STATE_DHCP_LEASED;
                result = 1;
            }
            else if(messageType == DHCP_NAK)
                dhcp_state = STATE_DHCP_START;
        }
        
        if(messageType == 255)
        {
            messageType = 0;
            dhcp_state = STATE_DHCP_START;
        }
        
        if(result != 1 && ((millis() - startTime) > timeout))
            break;
    }
    
    close(0);
    _dhcpTransactionId++;
    
    if(result == 1)
    {
        setSIPR(_dhcpLocalIp);
        setGAR(_dhcpGatewayIp);
        setSUBR(_dhcpSubnetMask);
    }
    
    return result;
}

void DhcpClass::presend_DHCP()
{
    uint16 port = DHCP_SERVER_PORT;

    IINCHIP_WRITE(Sn_DIPR0(0), 255);
    IINCHIP_WRITE((Sn_DIPR0(0) + 1), 255);
    IINCHIP_WRITE((Sn_DIPR0(0) + 2), 255);
    IINCHIP_WRITE((Sn_DIPR0(0) + 3), 255);

    IINCHIP_WRITE(Sn_DPORT0(0), (uint8)((port & 0xff00) >> 8));
    IINCHIP_WRITE((Sn_DPORT0(0) + 1), (uint8)(port & 0x00ff));  
}

void DhcpClass::send_DHCP_MESSAGE(uint8 messageType, uint16 secondsElapsed)
{
    uint16 ptr = 0;

    ptr = IINCHIP_READ(Sn_TX_WR0(0));
    ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_TX_WR0(0) + 1);

    uint8 *buffer = (uint8*) malloc(32);
    memset(buffer, 0, 32);

    buffer[0] = DHCP_BOOTREQUEST;   // op
    buffer[1] = DHCP_HTYPE10MB;     // htype
    buffer[2] = DHCP_HLENETHERNET;  // hlen
    buffer[3] = DHCP_HOPS;          // hops

    // xid
    unsigned long xid = htonl(_dhcpTransactionId);
    memcpy(buffer + 4, &(xid), 4);

    // 8, 9 - seconds elapsed
    buffer[8] = ((secondsElapsed & 0xff00) >> 8);
    buffer[9] = (secondsElapsed & 0x00ff);

    // flags
    unsigned short flags = htons(DHCP_FLAGSBROADCAST);
    memcpy(buffer + 10, &(flags), 2);

    // ciaddr: already zeroed
    // yiaddr: already zeroed
    // siaddr: already zeroed
    // giaddr: already zeroed

    //put data in W5100 transmit buffer
    write_data(0, buffer, (uint8 *)ptr, 28);
    ptr += 28;

    memcpy(buffer, _dhcpMacAddr, 6); // chaddr

    //put data in W5100 transmit buffer
    write_data(0, buffer, (uint8 *)ptr, 16);
    ptr += 16;

    memset(buffer, 0, 32); // clear local buffer

    // leave zeroed out for sname && file
    // put in W5100 transmit buffer x 6 (192 bytes)
  
    for(int i = 0; i < 6; i++) {
        write_data(0, buffer, (uint8 *)ptr, 32);
        ptr += 32;
    }
  
    // OPT - Magic Cookie
    buffer[0] = (uint8)((MAGIC_COOKIE >> 24)& 0xFF);
    buffer[1] = (uint8)((MAGIC_COOKIE >> 16)& 0xFF);
    buffer[2] = (uint8)((MAGIC_COOKIE >> 8)& 0xFF);
    buffer[3] = (uint8)(MAGIC_COOKIE& 0xFF);

    // OPT - message type
    buffer[4] = dhcpMessageType;
    buffer[5] = 0x01;
    buffer[6] = messageType; //DHCP_REQUEST;

    // OPT - client identifier
    buffer[7] = dhcpClientIdentifier;
    buffer[8] = 0x07;
    buffer[9] = 0x01;
    memcpy(buffer + 10, _dhcpMacAddr, 6);

    // OPT - host name
    buffer[16] = hostName;
    buffer[17] = strlen(HOST_NAME) + 3; // length of hostname + last 3 bytes of mac address
    strcpy((char*)&(buffer[18]), HOST_NAME);

    buffer[24] = _dhcpMacAddr[3];
    buffer[25] = _dhcpMacAddr[4];
    buffer[26] = _dhcpMacAddr[5];

    //put data in W5100 transmit buffer
    write_data(0, buffer, (uint8 *)ptr, 27);
    ptr += 27;

    if(messageType == DHCP_REQUEST)
    {
        buffer[0] = dhcpRequestedIPaddr;
        buffer[1] = 0x04;
        buffer[2] = _dhcpLocalIp[0];
        buffer[3] = _dhcpLocalIp[1];
        buffer[4] = _dhcpLocalIp[2];
        buffer[5] = _dhcpLocalIp[3];

        buffer[6] = dhcpServerIdentifier;
        buffer[7] = 0x04;
        buffer[8] = _dhcpDhcpServerIp[0];
        buffer[9] = _dhcpDhcpServerIp[1];
        buffer[10] = _dhcpDhcpServerIp[2];
        buffer[11] = _dhcpDhcpServerIp[3];

        //put data in W5100 transmit buffer
        write_data(0, buffer, (uint8 *)ptr, 12);
        ptr += 12;
    }
    
    buffer[0] = dhcpParamRequest;
    buffer[1] = 0x06;
    buffer[2] = subnetMask;
    buffer[3] = routersOnSubnet;
    buffer[4] = dns;
    buffer[5] = domainName;
    buffer[6] = dhcpT1value;
    buffer[7] = dhcpT2value;
    buffer[8] = endOption;
    
    //put data in W5100 transmit buffer
    write_data(0, buffer, (uint8 *)ptr, 9);
    ptr += 9;

    if(buffer)
        free(buffer);

    IINCHIP_WRITE(Sn_TX_WR0(0),(uint8)((ptr & 0xff00) >> 8));
    IINCHIP_WRITE((Sn_TX_WR0(0) + 1),(uint8)(ptr & 0x00ff));

    IINCHIP_WRITE(Sn_CR(0), Sn_CR_SEND);

    while( IINCHIP_READ(Sn_CR(0)) ) ;
}

u_char DhcpClass::parseDHCPResponse(u_long responseTimeout)
{
    uint16 ptr = 0;
     uint16 data_len = 0;
     uint16 port = 0;
     u_char type = 0;
     u_char svr_addr[4];
     u_char opt_len = 0;
     
    uint8* buffer = 0;

    u_long startTime = millis();

    while((IINCHIP_READ(Sn_RX_RSR0(0)) == 0x0000) && (IINCHIP_READ(Sn_RX_RSR0(0) + 1) == 0x0000))
    {
        if((millis() - startTime) > responseTimeout)
            return 255;
    }
  
    ptr = IINCHIP_READ(Sn_RX_RD0(0));
    ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_RX_RD0(0) + 1);

    // read UDP header
    buffer = (uint8*)malloc(8);
    read_data(0, (uint8 *)ptr, (uint8*)buffer, 0x08);
    ptr += 8;

    svr_addr[0] = buffer[0];
    svr_addr[1] = buffer[1];
    svr_addr[2] = buffer[2];
    svr_addr[3] = buffer[3];
    port = buffer[4];
    port = (port << 8) + buffer[5];
    data_len = buffer[6];
    data_len = (data_len << 8) + buffer[7];

    free(buffer);

    buffer = (uint8*) malloc(sizeof(RIP_MSG_FIXED));
    RIP_MSG_FIXED * pRMF = (RIP_MSG_FIXED*) buffer;

    read_data(0, (uint8 *)ptr, (uint8*)buffer, sizeof(RIP_MSG_FIXED));
  
    if(pRMF->op == DHCP_BOOTREPLY && port == DHCP_SERVER_PORT)
    {
        if(memcmp(pRMF->chaddr, _dhcpMacAddr, 6) != 0 || pRMF->xid != htonl(_dhcpTransactionId))
        {
            return 0;
        }

        memcpy(_dhcpLocalIp, pRMF->yiaddr, 4);
        free(buffer);

        uint16 optionLen = data_len - 240;
        buffer = (uint8*) malloc(optionLen);
        read_data(0, (uint8 *)ptr + 240, (uint8*)buffer, optionLen);


        uint8* p = buffer;
        uint8* e = p + optionLen;

        while ( p < e ) 
        {
            switch ( *p++ ) 
            {
                case endOption :
                    break;
                    
                case padOption :
                    break;
                    
                case dhcpMessageType :
                    opt_len = *p++;
                    type = *p;
                    break;
                    
                case subnetMask :
                    opt_len =* p++;
                    memcpy(_dhcpSubnetMask, p ,4);
                    break;
                    
                case routersOnSubnet :
                    opt_len = *p++;
                    memcpy(_dhcpGatewayIp, p, 4);
                    break;
                    
                case dns :
                    opt_len = *p++;
                    memcpy(_dhcpDnsServerIp, p, 4);
                    break;
                    
                case dhcpIPaddrLeaseTime :
                    opt_len = *p++;
                    break;

                case dhcpServerIdentifier :
                    opt_len = *p++;
                    if( *((u_long*)_dhcpDhcpServerIp) == 0 || 
                        *((u_long*)_dhcpDhcpServerIp) == *((u_long*)svr_addr) )
                    {
                        memcpy(_dhcpDhcpServerIp, p ,4);
                    }
                    break;
                    
                default :
                    opt_len = *p++;
                    break;
            }
          
            p += opt_len;
        }

        free(buffer);
    }

    ptr += data_len;

    IINCHIP_WRITE(Sn_RX_RD0(0),(uint8)((ptr & 0xff00) >> 8));
    IINCHIP_WRITE((Sn_RX_RD0(0) + 1),(uint8)(ptr & 0x00ff));

    IINCHIP_WRITE(Sn_CR(0),Sn_CR_RECV);

    while( IINCHIP_READ(Sn_CR(0)) );

    return type;
}

void DhcpClass::getMacAddress(uint8_t *dest)
{
    memcpy(dest, _dhcpMacAddr, 6);
}

void DhcpClass::getLocalIp(uint8_t *dest)
{
    memcpy(dest, _dhcpLocalIp, 4);
}

void DhcpClass::getSubnetMask(uint8_t *dest)
{
    memcpy(dest, _dhcpSubnetMask, 4);
}

void DhcpClass::getGatewayIp(uint8_t *dest)
{
    memcpy(dest, _dhcpGatewayIp, 4);
}

void DhcpClass::getDhcpServerIp(uint8_t *dest)
{
    memcpy(dest, _dhcpDhcpServerIp, 4);
}

void DhcpClass::getDnsServerIp(uint8_t *dest)
{
    memcpy(dest, _dhcpDnsServerIp, 4);
}

DhcpClass Dhcp;