#ifndef VENDOTEK_H__
#define VENDOTEK_H__

#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>
#include <wiringSerial.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "settings.h"
#include "dfplayer.h"

#define BUFFER_SIZE 1024
#define BUFFER_SIZE1 1024

// FORFORMAT
#define VTK_START_BYTE 0x1f // START_BYTE
#define VTK_BASE_VMC_L 0x96 // FROM VMC
#define VTK_BASE_VMC_R 0xFB
#define VTK_BASE_POS_L 0x97 // FROM POS
#define VTK_BASE_POS_R 0xFB

// PARAMETERS
#define VTK_MESSAGE_NAME 0x01           // ASCII 3 VMC, POS
#define VTK_OPERATION_NUMBER 0x03       // Decimal in ASCII 8 VMC, POS
#define VTK_AMOUNT 0x04                 // Decimal in ASCII 12 VMC, POS
#define VTK_KEEPALIVE_INTERVAL 0x05     // Decimal in ASCII 3 POS
#define VTK_OPERATION_TIMEOUT 0x06      // Decimal in ASCII 3 POS
#define VTK_EVENT_NAME 0x07             // ASCII 5 VMC, POS
#define VTK_EVENT_NUMBER 0x08           // Decimal in ASCII 8 VMC, POS
#define VTK_PRODUCT_ID 0x09             // Decimal in ASCII 6 VMC, POS
#define VTK_QR_CODE_DATA 0x0A           // ASCII No limit VMC
#define VTK_TCP_IP_DESTINATION 0x0B     // Binary No limit VMC, POS
#define VTK_OUTGOING_BYTE_COUNTER 0x0C  // Decimal in ASCII 12 VMC
#define VTK_SIMPLE_DATA_BLOCK 0x0D      // Binary No limit VMC, POS
#define VTK_CONFIRMABLE_DATA_BLOCK 0x0E // Binary No limit POS
#define VTK_PRODUCT_NAME 0x0F           // ASCII No limit VMC
#define VTK_POS_MANAGEMENT_DATA 0x10    // ASCII No limit VMC, POS
#define VTK_LOCAL_TIME 0x11             // ASCII No limit VMC, POS
#define VTK_SYSTEM_INFORMATION 0x12     // ASCII No limit POS
#define VTK_BANKING_RECEIPT 0x13        // UTF-8 No limit POS
#define VTK_DISPLAY_TIME_MS 0x14        // Decimal in ASCII 8 VMC
#define VTK_IMAGE_NAME 0x15             // Decimal in ASCII 8 VMC

// MESSAGE NAMES
#define VTK_MESSAGE_LENHG 0x03
#define VTK_IDL "IDL" // IDLE state handshake
#define VTK_DIS "DIS" // DISABLED state handshake VMC, POS
#define VTK_STA "STA" // Start of session, if required from POS POS
#define VTK_VRP "VRP" // Vend request positive VMC, POS
#define VTK_FIN "FIN" // Finalization of operation VMC, POS
#define VTK_ABR "ABR" // Abort of operation VMC
#define VTK_CDP "CDP" // Cash deposit VMC, POS
#define VTK_MFR "MFR" // MIFARE card detected POS
#define VTK_CON "CON" // Connect message VMC, POS
#define VTK_DAT "DAT" // Data message VMC, POS
#define VTK_DSC "DSC" // Disconnect message VMC, POS
#define VTK_PMD "20240119T144724+0300"
// REQ

#define VTK_EMPTY "empty"
#define VTK_OWNER "owner"
#define VTK_TAKE "take"

typedef struct vtc_request
{
  uint16_t len;
  uint8_t data[];
} vtc_request;

// static vtc_request req;
//  CRC-16/CCITT-FALSE

static const uint16_t crc16_ccitt_table[256] = {
    0x0000,
    0x1021,
    0x2042,
    0x3063,
    0x4084,
    0x50A5,
    0x60C6,
    0x70E7,
    0x8108,
    0x9129,
    0xA14A,
    0xB16B,
    0xC18C,
    0xD1AD,
    0xE1CE,
    0xF1EF,
    0x1231,
    0x0210,
    0x3273,
    0x2252,
    0x52B5,
    0x4294,
    0x72F7,
    0x62D6,
    0x9339,
    0x8318,
    0xB37B,
    0xA35A,
    0xD3BD,
    0xC39C,
    0xF3FF,
    0xE3DE,
    0x2462,
    0x3443,
    0x0420,
    0x1401,
    0x64E6,
    0x74C7,
    0x44A4,
    0x5485,
    0xA56A,
    0xB54B,
    0x8528,
    0x9509,
    0xE5EE,
    0xF5CF,
    0xC5AC,
    0xD58D,
    0x3653,
    0x2672,
    0x1611,
    0x0630,
    0x76D7,
    0x66F6,
    0x5695,
    0x46B4,
    0xB75B,
    0xA77A,
    0x9719,
    0x8738,
    0xF7DF,
    0xE7FE,
    0xD79D,
    0xC7BC,
    0x48C4,
    0x58E5,
    0x6886,
    0x78A7,
    0x0840,
    0x1861,
    0x2802,
    0x3823,
    0xC9CC,
    0xD9ED,
    0xE98E,
    0xF9AF,
    0x8948,
    0x9969,
    0xA90A,
    0xB92B,
    0x5AF5,
    0x4AD4,
    0x7AB7,
    0x6A96,
    0x1A71,
    0x0A50,
    0x3A33,
    0x2A12,
    0xDBFD,
    0xCBDC,
    0xFBBF,
    0xEB9E,
    0x9B79,
    0x8B58,
    0xBB3B,
    0xAB1A,
    0x6CA6,
    0x7C87,
    0x4CE4,
    0x5CC5,
    0x2C22,
    0x3C03,
    0x0C60,
    0x1C41,
    0xEDAE,
    0xFD8F,
    0xCDEC,
    0xDDCD,
    0xAD2A,
    0xBD0B,
    0x8D68,
    0x9D49,
    0x7E97,
    0x6EB6,
    0x5ED5,
    0x4EF4,
    0x3E13,
    0x2E32,
    0x1E51,
    0x0E70,
    0xFF9F,
    0xEFBE,
    0xDFDD,
    0xCFFC,
    0xBF1B,
    0xAF3A,
    0x9F59,
    0x8F78,
    0x9188,
    0x81A9,
    0xB1CA,
    0xA1EB,
    0xD10C,
    0xC12D,
    0xF14E,
    0xE16F,
    0x1080,
    0x00A1,
    0x30C2,
    0x20E3,
    0x5004,
    0x4025,
    0x7046,
    0x6067,
    0x83B9,
    0x9398,
    0xA3FB,
    0xB3DA,
    0xC33D,
    0xD31C,
    0xE37F,
    0xF35E,
    0x02B1,
    0x1290,
    0x22F3,
    0x32D2,
    0x4235,
    0x5214,
    0x6277,
    0x7256,
    0xB5EA,
    0xA5CB,
    0x95A8,
    0x8589,
    0xF56E,
    0xE54F,
    0xD52C,
    0xC50D,
    0x34E2,
    0x24C3,
    0x14A0,
    0x0481,
    0x7466,
    0x6447,
    0x5424,
    0x4405,
    0xA7DB,
    0xB7FA,
    0x8799,
    0x97B8,
    0xE75F,
    0xF77E,
    0xC71D,
    0xD73C,
    0x26D3,
    0x36F2,
    0x0691,
    0x16B0,
    0x6657,
    0x7676,
    0x4615,
    0x5634,
    0xD94C,
    0xC96D,
    0xF90E,
    0xE92F,
    0x99C8,
    0x89E9,
    0xB98A,
    0xA9AB,
    0x5844,
    0x4865,
    0x7806,
    0x6827,
    0x18C0,
    0x08E1,
    0x3882,
    0x28A3,
    0xCB7D,
    0xDB5C,
    0xEB3F,
    0xFB1E,
    0x8BF9,
    0x9BD8,
    0xABBB,
    0xBB9A,
    0x4A75,
    0x5A54,
    0x6A37,
    0x7A16,
    0x0AF1,
    0x1AD0,
    0x2AB3,
    0x3A92,
    0xFD2E,
    0xED0F,
    0xDD6C,
    0xCD4D,
    0xBDAA,
    0xAD8B,
    0x9DE8,
    0x8DC9,
    0x7C26,
    0x6C07,
    0x5C64,
    0x4C45,
    0x3CA2,
    0x2C83,
    0x1CE0,
    0x0CC1,
    0xEF1F,
    0xFF3E,
    0xCF5D,
    0xDF7C,
    0xAF9B,
    0xBFBA,
    0x8FD9,
    0x9FF8,
    0x6E17,
    0x7E36,
    0x4E55,
    0x5E74,
    0x2E93,
    0x3EB2,
    0x0ED1,
    0x1EF0,
};

static vtc_request req;
static char buf[BUFFER_SIZE1];

uint16_t get_crc(const uint8_t *data, uint16_t size);
uint32_t power(uint16_t x, uint8_t n);
void delay(unsigned ms);
void clearR(vtc_request *r);
void gen_head_mes(vtc_request *r);
void add_param(vtc_request *r, char mes[BUFFER_SIZE1], uint16_t len);
void gen_end_mes(vtc_request *r);
uint8_t chek_param(uint8_t mes[], uint16_t len, char chek_mes[BUFFER_SIZE1], uint16_t lenchek);
uint32_t get_num_value_param(vtc_request *r, char chek_mes);
void gen_IDL(vtc_request *r);
void gen_DIS(vtc_request *r);
void gen_ABR(vtc_request *r);
void gen_VRP(vtc_request *r, uint16_t ammount, uint16_t prod_ID, char *name);
void gen_IDL_QR(vtc_request *r, char *qr);
void gen_IDL_SCREEN_EMPTY(vtc_request *r);
void gen_IDL_SCREEN_OWNER(vtc_request *r);
void gen_IDL_SCREEN_TAKE(vtc_request *r);
void gen_FIN(vtc_request *r, uint16_t ammount, uint16_t prod_ID, char *name);
bool chekSEAL(vtc_request *r);
void sendUART(vtc_request *r, int fd);
int readUART(vtc_request *r, int fd);
bool chekCART(vtc_request *r);
bool get_brecpt(char *bufer, vtc_request *r);

// bool readUART_IT(vtc_request *r);
bool chekFromUart(vtc_request *r);
// bool readUART_IT_f(vtc_request *r);

void gen_test_VTK_BANKING_RECEIPT(vtc_request *r, char *qr);
// static char buf[BUFFER_SIZE1];

int read_from_UART(vtc_request *r, int fd);

static uint32_t oper_num = 0;

#endif // VENDOTEK_H__
