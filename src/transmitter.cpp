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
//                            When chosing your LCD Display, make sure it uses the least amount of pins.
//                            Say you have the commonly used LCD 16x2, make sure it has i2c module attached to it.
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
#define CHANNEL_MAX_VALUE           4095

#define NUMBER_OF_CHANNELS          3       // how many different channels can you switch between.
                                            // channels (frequencies) are determined in a way that sets them the furthest
                                            // apart from each other.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Errors defines
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PACKET_LOST         "Packet lost!"
#define FATAL_PACKET_LOST   "Too many packets lost!"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Uncomment any of these lines for specific behavior
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define DEBUG_MODE                          // useful debuging information will be shown
#define LCD_MSG_TO_SERIAL                   // messages that are printed to the LCD will be shown in the serial monitor



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  LCD-Related functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LCD_PrintErrorMsg(const char* error_string);
void LCD_PrintValuesToSend(const int16_t* values_to_send);
void LCD_PrintCurrentChannel(const uint8_t current_channel);
void LCD_PrintSensitivity(const int16_t sensitivity_value, bool inPercentage = true);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Transmitter-related functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UpdateChannel();                         // reads channel input from potentiometer

RF24      transmitter(CE, CSN);               // check link for class methods: https://nrf24.github.io/RF24/classRF24.html

uint8_t   address[] = "00001";                // logical address: this address must be the same on the receiving end
int16_t   values_to_send[4];                  // [VX0, VY0, VX1, VY1]

uint8_t   current_channel_index;              // index of current RF channel
uint16_t  nbr_packets_lost = 0;               // this holds the overal number of packets lost SINCE the first successful transmition
uint16_t  nbr_packets_lost_suc = 0;           // nuber of packets lost successively.

void setup() {

  #if defined (DEBUG_MODE) || defined (LCD_MSG_TO_SERIAL)
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

  // reads values from the two joysticks + sensitivity
  values_to_send[0] = static_cast<int16_t>(map(analogRead(VX0), 0, 4095, 180, 0) * ((float)sensibilityValue / SENSIBILITY_MAX_VALUE));
  values_to_send[1] = static_cast<int16_t>(map(analogRead(VY0), 0, 4095, 180, 0) * ((float)sensibilityValue / SENSIBILITY_MAX_VALUE));
  values_to_send[2] = static_cast<int16_t>(map(analogRead(VX1), 0, 4095, 180, 0) * ((float)sensibilityValue / SENSIBILITY_MAX_VALUE));
  values_to_send[3] = static_cast<int16_t>(map(analogRead(VY1), 0, 4095, 180, 0) * ((float)sensibilityValue / SENSIBILITY_MAX_VALUE));

  #ifdef DEBUG_MODE || 
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

void LCD_PrintErrorMsg(const char* error_string)
{

}

void LCD_PrintValuesToSend(const int16_t* values_to_send)
{

}

void LCD_PrintCurrentChannel(const uint8_t current_channel)
{

}

void LCD_PrintSensitivity(const int16_t sensitivity_value, bool inPercentage = true)
{

}

void UpdateChannel()
{
  int16_t channel_value = analogRead(CHANNEL_POTENTIOMETER);
  if ( ( float(channel_value) / (float(CHANNEL_MAX_VALUE) / NUMBER_OF_CHANNELS) ) != current_channel_index)
  {
    current_channel_index = float(channel_value) / (float(CHANNEL_MAX_VALUE) / NUMBER_OF_CHANNELS);
    transmitter.setChannel(100);
  }
}