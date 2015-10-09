/*
  DMX_Master.ino - Example code for using the Conceptinetics DMX library
  Copyright (c) 2013 W.A. van der Meeren <danny@illogic.nl>.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <Conceptinetics.h>


//
// CTC-DRA-13-1 ISOLATED DMX-RDM SHIELD JUMPER INSTRUCTIONS
//
// If you are using the above mentioned shield you should 
// place the RXEN jumper towards pin number 2, this allows the
// master controller to put to iso shield into transmit 
// (DMX Master) mode 
//
//
// The !EN Jumper should be either placed in the G (GROUND) 
// position to enable the shield circuitry 
//   OR
// if one of the pins is selected the selected pin should be
// set to OUTPUT mode and set to LOGIC LOW in order for the 
// shield to work
//


//
// The master will control 100 Channels (1-100)
// 
// depending on the ammount of memory you have free you can choose
// to enlarge or schrink the ammount of channels (minimum is 1)
//
#define DMX_MASTER_CHANNELS   8 

//
// Pin number to change read or write mode on the shield
//
#define RXEN_PIN                2


// Configure a DMX master controller, the master controller
// will use the RXEN_PIN to control its write operation 
// on the bus
DMX_Master        dmx_master ( DMX_MASTER_CHANNELS, RXEN_PIN );


// Channel 1 = Temperature1
// Channel 2 = Intensity1
// Channel 3 = Temperature2
// Channel 4 = Intensity2
// Channel 5 = Temperature3
// Channel 6 = Intensity3
// Channel 7 = Temperature4
// Channel 8 = Intensity4

int dimmerVal = 0;
int tempVal = 0;

//Variable used for testing
//int dirMod = 1;

const int potNum = 2;
const int flipNum = 4;
const int channelNum = 10; 

int ledPin = 13; // select the pin for the LED
int sensorPins[potNum] = {A0,A1};    // select the input pin for the potentiometer
int sensorVals[potNum] = {0,0};       
int flipPins[flipNum] = {8,9,10,11};     // Select the pin for the flipSwitch
int flipStates[flipNum] = {1,1,1,1};

int sensorValue = 0;  // variable to store the value coming from the sensor

// the setup routine runs once when you press reset:
void setup() {             
  // Enable DMX master interface and start transmitting
  dmx_master.enable();  
  
  // Set channels 
  dmx_master.setChannelRange ( 1, 12, 200 );
  
  // Declare flip switch pins as INPUT_PULLUP
  for (int j = 0; j <= flipNum-1; j++){
    pinMode(flipPins[j], INPUT_PULLUP);
  }
  
}

// the loop routine runs over and over again forever:
void loop() 
{ 
  //Read potentiometer values and map to 0-255
  for (int i = 0; i <= potNum-1; i++) {
    sensorVals[i] = map(analogRead(sensorPins[i]), 0, 1023, 0, 255);  
  }

  //Read switch values
  for (int j = 0; j <= flipNum-1; j++) {
      flipStates[j] = digitalRead(flipPins[j]);
    }

  
  //Set channels to pot-values depending on which switches are on or off.
  for (int k = 0; k <= flipNum; k++) {
    if ( flipStates[k] == 1 ) {   
        dmx_master.setChannelValue ( k*2+1, sensorVals[0]);
        dmx_master.setChannelValue ( k*2+2, sensorVals[1]);
      } 
   }
    
  /* 
  //Test for checking lamps
  dmx_master.setChannelRange ( 3, 8, dimmerVal );  
  //dmx_master.setChannelValue ( 2, tempVal );

  dimmerVal += dirMod;
  
  if( dimmerVal >= 255 ) {
    dirMod = -1;
    }

   else if (dimmerVal <= 1) {
    dirMod = 1;      
      }*/
      
  delay(40); 
}
