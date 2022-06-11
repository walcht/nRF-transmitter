///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Essential Libraries\Defines
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <SPI.h>        // Arduino Serial Peripheral Interface protocol library
#include <RF24.h>       // RC transceiver module libraries
#include <nRF24L01.h>

#define CE    5         // necessary for RC tranceiver
#define CSN   4      

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Joysticks \ Potentiometers \ etc... pin defines
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
//              ||
//              ||
//        ______||______
//      / *            * \ 
//     / X _   LCD    _ X \ 
//    /___/ |________| \___\ 
//
//  Left joystick:            VX0 for horizontal axis => Yaw
//                            VY0 for vertical axis   => Throttle
//          
//  Right joystick:           VX1 for horizontal axis => Roll
//                            VY1 for vertical axis   => Pitch
//
//
//  Top left potentiometer:   SENSIBILITY_POTENTIOMETER for changing the sensibility of ALL joysticks.
//                            This could be useful in situations where subtle and careful changes in values
//                            is needed to control the RC module.
//
//  Top right potentiometer:  CHANNEL_POTENTIOMETER for changing which channel the transmitter is currently on.
//                            This could be useful in situations where you want to rapidly switch between controlling
//                            different RC modules; say a robot and an arm attached to it.
//                            We set this to allow to change between 3 channels.
//                            You can change how many channels this potentiometer will allow you to change to.
//                            We decided to go with 3 channels because it is easy switch between just 3 different zones
//                            for the potentiometer.
//
//  LCD Display:              Currently we haven't yet added LCD functionalities, but doing so is straight forward.
//                            This could be useful in showing the user:
//                            - what values are going to be sent
//                            - what is the CURRENT channel
//                            - error messages
//                            - etc...
//

#define VX0   33        // left joystick input
#define VY0   32
#define VX1   35        // right joystick input
#define VY1   34

#define SENSIBILITY_POTENTIOMETER   25  
#define CHANNEL_POTENTIOMETER       26
#define SENSIBILITY_MAX_VALUE       4095    // for ESP32 boards, the maximum value detected by an ADC is 4095.
                                            // for Arduino boards, make sure to change this value according to the
                                            // resolution of the ADC.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Uncomment any of these lines for specific behavior
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define DEBUG_MODE




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  LCD-Related functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LCD_PrintErrorMsg();
void LCD_PrintValuesToSend();
void LCD_PrintCurrentChannel();


RF24    transmitter(CE, CSN);               // check link for class methods: https://nrf24.github.io/RF24/classRF24.html

uint8_t address[] = "00001";                // logical address: this address must be the same on the receiving end
int16_t values_to_send[4];                  // [VX0, VY0, VX1, VY1]

void setup() {

  #ifdef DEBUG_MODE
    Serial.begin(115200);
  #endif

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  RF Module initilaization and settings
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  transmitter.begin();                      // setting-up nRF module configuration
  transmitter.setAutoAck(true);
  transmitter.setPALevel(RF24_PA_MIN);      // you can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  transmitter.stopListening();              // set RC module to transmitter mode
  transmitter.openWritingPipe(address);     // which address to write to?
  
}

void loop() {

  int16_t sensibilityValue = analogRead(SENSIBILITY_POTENTIOMETER);

  Serial.print("SENSIBILITY: "); Serial.println(sensibilityValue);

  values_to_send[0] = static_cast<int16_t>(map(analogRead(VX0), 0, 4095, 180, 0) * ((float)sensibilityValue / SENSIBILITY_MAX_VALUE));
  values_to_send[1] = static_cast<int16_t>(map(analogRead(VY0), 0, 4095, 180, 0) * ((float)sensibilityValue / SENSIBILITY_MAX_VALUE));
  values_to_send[2] = static_cast<int16_t>(map(analogRead(VX1), 0, 4095, 180, 0) * ((float)sensibilityValue / SENSIBILITY_MAX_VALUE));
  values_to_send[3] = static_cast<int16_t>(map(analogRead(VY1), 0, 4095, 180, 0) * ((float)sensibilityValue / SENSIBILITY_MAX_VALUE));

  #ifdef DEBUG_MODE
    Serial.print("VX0: ");
    Serial.print(values_to_send[0]);
    Serial.print("\t");

    Serial.print("VY0: ");
    Serial.print(values_to_send[1]);
    Serial.print("\t");

    Serial.print("VX1: ");
    Serial.print(values_to_send[2]);
    Serial.print("\t");
    
    Serial.print("VY1: ");
    Serial.println(values_to_send[3]);
  #endif
  
  // if (packet didn't reach the receiver)
  if (!transmitter.write(values_to_send, sizeof(int16_t)*4))
  {
    #ifdef DEBUG_MODE
      Serial.print("\t");
      Serial.print("packet lost!");
    #endif
    // some code to inform the user of packet loss
  }
  
}