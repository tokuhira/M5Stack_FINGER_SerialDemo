//
//  Demo code for FPC1020 Fingerprint Sensor
//  Created by Deray on 2015-10-07.
//  Modified by Tokuhira on 2019-01-20 for M5Stack.
//

#include <M5Stack.h>
#include "FPC1020.h"

#define FINGER_UART 2        // UART for FINGER unit
#define ID_INPUT_DELAY 1000  // Wait for incomming digits

extern unsigned char l_ucFPID;
extern unsigned char rBuf[192];  //Receive return data
//#define sw_serial_rx_pin 2 //  Connect this pin to TX on the FPC1020
//#define sw_serial_tx_pin 3 //  Connect this pin to RX on the FPC1020

HardwareSerial hwSerial(FINGER_UART);
FPC1020 Finger(&hwSerial);

void setup() {
  M5.begin();
  Serial.begin(19200);
  //pinMode(4, INPUT); // IRQ

  Serial.println("Fingerprint Test ! ");
}

void loop() {
  unsigned int User_ID = 0;
  unsigned char incomingNub;
  unsigned int  matchUserID = 0;
  unsigned char rtf = 0;

  while (1) {
    Serial.println("============== Menu ================");
    Serial.println("Add a New User ----------------- 1");
    Serial.println("Fingerprint Matching --------------- 2");
    Serial.println("Get User Number and Print All User ID ------ 3 ");
    Serial.println("Delete Assigned User --------- 4");
    Serial.println("Delete All User ---------- 5");
    Serial.println("============== End =================");

    unsigned char  MODE = 0;

    while (Serial.available() <= 0);

    MODE = Serial.read() - 0x30;

    switch (MODE) {
      case 0:  // Null
        break;

      case 1:   // Fingerprint Input and Add a New User
        MODE = 0;
        User_ID = 0;

        Serial.println("Please input the new user ID (0 ~ 99).");
        while (Serial.available() <= 0);
        delay(ID_INPUT_DELAY);
        incomingNub = Serial.available();
        for (char i = incomingNub; i >= 1; i--) {
          User_ID = User_ID + (Serial.read() - 0x30) * pow(10, (i - 1));
        }
        if (User_ID > 99) {
          Serial.print("Invalid User ID read: ");
          Serial.println( User_ID , DEC);
          break;
        }

        Serial.println("Add Fingerprint, please put your finger on the Fingerprint Sensor.");
        rtf = Finger.Enroll(User_ID);

        if (rtf == TRUE) {
          Serial.print("Success, your User ID is: ");
          Serial.println( User_ID , DEC);
        }
        else if (rtf == FALSE) {
          Serial.println("Failed, please try again.");
        }
        else if ( rtf == ACK_USER_OCCUPIED) {
          Serial.println("Failed, this User ID already exsits.");
        }
        else if ( rtf == ACK_USER_EXIST) {
          Serial.println("Failed, this fingerprint already exsits.");
        }
        delay(2000);
        break;

      case 2:  // Fingerprint Matching
        MODE = 0 ;
        Serial.println("Mctch Fingerprint, please put your finger on the Sensor.");

        if ( Finger.Search()) {
          Serial.print("Success, your User ID is: ");
          Serial.println( l_ucFPID, DEC);
        }
        else {
          Serial.println("Failed, please try again.");
        }
        delay(1000);
        break;

      case 3:   // Print all user ID
        MODE = 0;
        if (Finger.PrintUserID()) {
          Serial.print("Number of Fingerprint User is:");
          unsigned char UserNumb;
          UserNumb = (l_ucFPID - 2) / 3;

          Serial.println(UserNumb, DEC);
          Serial.println("Print all the User ID:");

          for (char i = 0; i < UserNumb; i++) {
            Serial.println(rBuf[12 + i * 3], DEC);
          }
        }
        else {
          Serial.println("Print User ID Fail!");
        }
        delay(1000);
        break;

      case 4:   // Delete Assigned User ID
        MODE = 0;
        User_ID = 0;
        Serial.println("Please input the user ID(0 ~ 99) you want to delecte.");
        while (Serial.available() <= 0);
        delay(ID_INPUT_DELAY);
        incomingNub = Serial.available();
        for (char i = incomingNub; i >= 1; i--) {
          User_ID = User_ID + (Serial.read() - 0x30) * pow(10, (i - 1));
        }
        if (User_ID > 99) {
          Serial.print("Invalid User ID read: ");
          Serial.println( User_ID , DEC);
          break;
        }

        if (Finger.Delete(User_ID)) {
          Serial.println("Delete Fingerprint User Success!");
        }
        else {
          Serial.println("Delete Fingerprint User Fail!");
        }
        delay(1000);
        break;

      case 5:  // Delete All User ID
        MODE = 0;
        unsigned char DeleteFlag = 0;

        Serial.println("Delete All Users, Y/N ?");

        for (unsigned char i = 200; i > 0; i--) //wait response info
        {
          delay(20);
          if (Serial.available() > 0)
          {
            DeleteFlag = Serial.read();
            break;
          }
        }

        if (DeleteFlag == 'Y' || DeleteFlag == 'y') {
          if (Finger.Clear()) {
            Serial.println("Delete All Fingerprint User Success!");
          }
          else {
            Serial.println("Delete All Fingerprint User Fail!");
          }
        }
        delay(500);
        break;
    }
  }
}
