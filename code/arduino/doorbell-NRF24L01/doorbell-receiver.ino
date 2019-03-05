#include <SPI.h>

#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>



/*
 *  DOORBELL RECEIVER UNIT - NRF24L01+
 *  ---------------------------------------------------------------------------
 *  This version of the doorbell unit uses the NRF24L01+ 2.4GHz radio to receive
 *  ring notifications from the doorbell unit.
 *  This implementation is the first version of the doorbell, with limitted
 *  functionality - i.e. Just a doorbell.  The initial vision was for a full
 *  door monitoring system with a motion sensor to wake the device, camera and
 *  audio interaction with the person at the door, a weather station capturing
 *  temperature, humidity, and barometric pressure.
 *
 *  ATTRIBUTIONS
 *
 *  Uses the optimized RF24 library from TMRh20. https://github.com/TMRh20/RF24
 *  ---------------------------------------------------------------------------
 */

/*
 *  9 -> brownf
 * 11 -> green
 * 12 -> yellow
 * 13 -> blue
 */
 
#define CE_PIN  9
#define CSN_PIN 10

//#define speakerPin 5

const uint64_t pipe = 0xE8E8F0F0E1LL;
const int led = 7;

int message[2];

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

void setup()
{
     
    Serial.begin(9600);
    delay(100);

    
    //pinMode( speakerPin, OUTPUT );
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    
    radio.begin();
    radio.openReadingPipe(1, pipe);
    
    radio.startListening();

    ringBell();

  radio.printDetails();
}

void loop()
{
//    ringBell();
    Serial.print(".");
    delay(100);
    
    if ( radio.available() )
    {
        Serial.println("radio available");
        ringBell();
        // Read the data payload until we've received everything
        //bool done = false;

        while ( radio.available() )
        {
            Serial.println("getting data");
            ringBell();
            radio.read( &message, sizeof(message) );
        }

        Serial.println("got data");
        
        //if ( message[0] == 517 )
        //{
            ringBell();
        //}
    }
}

void ringBell()
{
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
//    digitalWrite(led, HIGH);
//    delay(50);
//    digitalWrite(led, LOW);
//    delay(50);
}
//void ringBell()
//{
//    tone(speakerPin, NOTE_A5, 800);
//    delay(800);
//    tone(speakerPin, NOTE_F5, 800);
//    delay(800);
//
//    noTone(speakerPin);
//    digitalWrite(speakerPin, LOW);
//}
