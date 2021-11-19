#define DECODE_NEC
#include <IRremote.h>
#define IR_RECEIVE_PIN 2

// remote control codes
#define CHM 0x45
#define CH  0x46
#define CHP 0x47
#define LEFT 0x44
#define RIGHT 0x40
#define PLAY_PAUSE 0x43
#define MINUS 0x7
#define PLUS 0x15
#define EQ 0x9
#define ZERO 0x16
#define HUNDRED 0x19
#define HUNDRED2 0xD
#define ONE 0xC
#define TWO 0x18
#define THREE 0x5E
#define FOUR 0x8
#define FIVE 0x1C
#define SIX 0x5A
#define SEVEN 0x42
#define EIGHT 0x52
#define NINE 0x4A


void setup() {
  Serial.begin(115200);
  pinMode (13, OUTPUT); 
  digitalWrite(13, LOW);

  IrReceiver.begin(IR_RECEIVE_PIN);
}

void loop() {
  if (IrReceiver.decode()) {

    // Print a short summary of received data
    // IrReceiver.printIRResultShort(&Serial);
    // if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
    //     // We have an unknown protocol here, print more info
    //     IrReceiver.printIRResultRawFormatted(&Serial, true);
    // }
    /*
      * !!!Important!!! Enable receiving of the next value,
      * since receiving has stopped after the end of the current received data packet.
      */
    IrReceiver.resume(); // Enable receiving of the next value

    /*
      * Finally, check the received data and perform actions according to the received command
      */
    uint16_t command = IrReceiver.decodedIRData.command;
    switch(command) {
      case CHM:
        Serial.println("CH-");
        break;
      case CH:
        Serial.println("CH");
        break;
      case CHP:
        Serial.println("CHP");
        break;
      case LEFT:
        Serial.println("|<<");
        break;
      case RIGHT:
        Serial.println(">>|");
        break;
      case PLAY_PAUSE:
        Serial.println(">||");
        break;
      case MINUS:
        digitalWrite (13, LOW);
        Serial.println("-");
        break;
      case PLUS:
        digitalWrite (13, HIGH);
        Serial.println("+");
        break;
      case EQ:
        Serial.println("EQ");
        break;
      case ZERO:
        Serial.println("0");
        break;
      case HUNDRED:
        Serial.println("100+");
        break;
      case HUNDRED2:
        Serial.println("200+");
        break;
      case ONE:
        Serial.println("1");
        break;
      case TWO:
        Serial.println("2");
        break;
      case THREE:
        Serial.println("3");
        break;
      case FOUR:
        Serial.println("4");  
        break;
      case FIVE:
        Serial.println("5");
        break;
      case SIX:
        Serial.println("6");
        break;
      case SEVEN:
        Serial.println("7");
        break;
      case EIGHT:
        Serial.println("8");
        break;
      case NINE:
        Serial.println("9");
        break;
      default:
        Serial.print(command);
        Serial.println("?");
        break;
    }
  }
}
