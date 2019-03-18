/*
 *  DOORBELL RECEIVER UNIT - NRF24L01+
 *  ---------------------------------------------------------------------------
 *  This version of the doorbell uses the NRF24L01+ 2.4GHz radio to receive
 *  ring notifications from the doorbell transmitter unit.
 *
 *  ATTRIBUTIONS
 *
 *  Uses the excellent NRFLite library from  dparson55. https://github.com/dparson55/NRFLite
 *  ---------------------------------------------------------------------------
 */

/*
    NRF24L01+ Wiring
    --------------------------------
    CE    -> 9
    CSN   -> 10 (Hardware SPI SS)
    MOSI  -> 11 (Hardware SPI MOSI)
    MISO  -> 12 (Hardware SPI MISO)
    SCK   -> 13 (Hardware SPI SCK)
    IRQ   -> No connection
    VCC   -> No more than 3.6 volts
    GND   -> GND

    Speaker and LED
    -------------------------------
    Speaker +ve -> pin 5
    Speaker -ve -> GND
    LED +ve     -> resistor -> pin 7
    LED -ve     -> GND
*/
 
#include <SPI.h>
#include <NRFLite.h>
#include "notes.h"

// Our radio's id.  The transmitter will send to this id.
const static uint8_t RADIO_ID       = 0;

const static uint8_t RADIO_CE_PIN   = 9;
const static uint8_t RADIO_CSN_PIN  = 10;

const static int     SPEAKER_PIN    = 5;
const static int     LED_PIN        = 7;

// Define the data structure for our data packet
struct RadioPacket
{
    uint8_t FromRadioId;
    uint32_t OnTimeMillis;
    uint32_t FailedTxCount;
};

NRFLite     radio;
RadioPacket radioData;


void setup()
{
    pinMode(SPEAKER_PIN, OUTPUT );
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    if (!radio.init(RADIO_ID, RADIO_CE_PIN, RADIO_CSN_PIN))
    {
        // Loop forever, rapidly flashing the LED in error mode
        while (1) 
        {
            flashLed(50);
        }
    }

    // If we got there, then the radio was successfully initialized.
    // Flash the LED to let the user know that the doorbell is ready.
    flashLed(500);
    flashLed(500);
    flashLed(500);
}

void loop()
{
    while (radio.hasData())
    {
        // Read the data that was sent by the transmitter unit.
        // We're not using this data here, but we could use it to do something interesting
        // like play a different ringtone for each transmitter.
        radio.readData(&radioData);

        ringBell();

        // Let's wait half a second before checking for more signals from our transmitter(s).
        // This adds a little delay between rings.
        delay(500);
    }
}

// This function performs our "ring" action, which involves playing a ringtone and flashing the LED.
void ringBell()
{
    flashLed(50);
    flashLed(50);

    playRingtone();

    flashLed(50);
    flashLed(50);
}

// Play a ringtone
void playRingtone()
{
   tone(SPEAKER_PIN, NOTE_A5, 800);
   delay(800);
   tone(SPEAKER_PIN, NOTE_F5, 800);
   delay(800);

   noTone(SPEAKER_PIN);
   digitalWrite(SPEAKER_PIN, LOW);
}

// Flashed the specified LED for a specified duration
void flashLed(int duration) {
    digitalWrite(LED_PIN, HIGH);
    delay(duration); 
    digitalWrite(LED_PIN, LOW);
    delay(duration);
}