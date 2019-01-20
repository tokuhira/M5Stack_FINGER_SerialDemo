//
//  FPC1020.h
//  Created by Deray on 2015-10-07.
//

#ifndef FPC1020_h
#define FPC1020_h

//#ifdef __AVR__
//  #if ARDUINO >= 100
//    #include <SoftwareSerial.h>
//  #else
//    #include <NewSoftSerial.h>
//  #endif
//#endif

#define TRUE  0x01
#define FALSE  0x00

#define DATA_START          0xf5  // Data start
#define DATA_END            0xf5  // Data end

#define CMD_ENROLL1         0x01  // Add fingerprint 1st
#define CMD_ENROLL2         0x02  // Add fingerprint 2nd
#define CMD_ENROLL3         0x03  // Add fingerprint 3rd
#define CMD_DELETE          0x04  // Delete assigned user
#define CMD_CLEAR           0x05  // Delete all users
#define CMD_USERNUMB        0x09  // Get number of users
#define CMD_IDENTIFY        0x0b  // Fingerprint matching 1:1
#define CMD_SEARCH          0x0c  // Fingerprint matching 1:N
#define CMD_USERID          0x2b  // Get User ID and User Permission

#define ACK_SUCCESS         0x00  // Operate success
#define ACK_FAIL            0x01  // Operate filed
#define ACK_FULL            0x04  // Fingerprint database is full
#define ACK_NOUSER          0x05  // User do not exist
#define ACK_USER_OCCUPIED   0x06  // User ID already exists
#define ACK_USER_EXIST      0x07  // Fingerprint already exists
#define ACK_TIMEOUT         0x08  // Acuquisition timeout

class FPC1020
{
  public:

    FPC1020(HardwareSerial *ser);
    unsigned char Enroll(unsigned int u_id);
    unsigned char Clear(void);
    unsigned char Delete(unsigned int u_id);
    unsigned char Search(void);
    unsigned char Read(void);
    unsigned char PrintUserID(void);

  private:
    unsigned char WaitFpData(void);
    unsigned char CmdGenCHK(unsigned char wLen, unsigned char *ptr);
    void UART_SendPackage(unsigned char wLen, unsigned char *ptr);
    unsigned char Check_Package(unsigned char cmd);
    void FP_Identify(unsigned int u_id);
    void Enroll_Step1(unsigned int u_id);
    void Enroll_Step2(unsigned int u_id);
    void Enroll_Step3(unsigned int u_id);


};

#endif
