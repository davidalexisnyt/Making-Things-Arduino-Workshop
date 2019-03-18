/*
 *  DOORBELL UNIT - NRF24L01+
 *  ---------------------------------------------------------------------------
 *  This version of the doorbell uses the NRF24L01+ 2.4GHz radio to receive
 *  ring notifications from the doorbell transmitter unit.
 *
 *  ATTRIBUTIONS
 *
 *  Uses the excellent NRFLite library from  dparson55. https://github.com/dparson55/NRFLite
 *  ---------------------------------------------------------------------------
 */

#include <avr/sleep.h>
#include <avr/power.h>
#include <SPI.h>
#include <NRFLite.h>

// Define pins used by NRF24L01+ for chip select and chip enable
#define CE_PIN   9
#define CSN_PIN  10

// This is the radio ID of the receiver.
const static uint8_t DESTINATION_RADIO_ID  = 0;

// This is our transmitter ID. If we have more than one transmitter, each should have its own ID.
const static uint8_t RADIO_ID           = 1;

const static uint8_t RADIO_CE_PIN   = 9;
const static uint8_t RADIO_CSN_PIN  = 10;

const static int BUTTON = 3;
const static int LED = 4;
const static int DEBOUNCED_ELAY = 500;
const static int ACTIVITY_TIMEOUT = 10000;

struct RadioPacket // Any packet up to 32 bytes can be sent.
{
    uint8_t FromRadioId;
    uint32_t OnTimeMillis;
    uint32_t FailedTxCount;
};

NRFLite radio;
RadioPacket radioData;

volatile bool ringRequested = false;
volatile int lastPressed = 0;

bool ledOn = false;
bool startingUp;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Initialize NRF24L01+ radio to enable communication.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void initializeComm()
{
    if (!radio.init(RADIO_ID, RADIO_CE_PIN, RADIO_CSN_PIN))
    {
        #ifdef DEBUGMODE
        Serial.println("Cannot communicate with radio");
        #endif

        // Loop forever, rapidly flashing the LED in error mode
        while (1) 
        {
            flashLed(1);
        }
    }
    
    radioData.FromRadioId = RADIO_ID;

    delay(50);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void sendNotify()
{
    radioData.OnTimeMillis = millis();

    #ifdef DEBUGMODE
    Serial.print("Sending ");
    Serial.print(radioData.OnTimeMillis);
    Serial.println(" ms");
    #endif

    if (radio.send(DESTINATION_RADIO_ID, &radioData, sizeof(radioData)))
    {
        #ifdef DEBUGMODE
        Serial.println("Successfully sent data to receiver");
        #endif
    }
    else
    {
        #ifdef DEBUGMODE
        Serial.println("Failed to send data to receiver!");
        #endif

        radioData.FailedTxCount++;
    }

    flashLed(4);
}

/* ------------------- MAIN HANDLERS & LOOP -------------------------------- */

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void flashLed(int repeat)
{
    for (byte i = 0; i < repeat; i++)
    {
        digitalWrite(LED, LOW);
        delay(100);
        digitalWrite(LED, HIGH);
        delay(100);
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void buttonPressed()
{
    // Handle button debouncing by not handling a button press within 1 second of the previous press.
    if ( !startingUp && !ringRequested && (millis() - lastPressed > DEBOUNCED_ELAY) )
    {
        ringRequested = true;
        ledOn = true;
        lastPressed = millis();
    }

    startingUp = false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void wakeUp()
{
    sleep_disable();

    // When we wake up, we want to trigger the button press event
    // Remove the wakeUp interrupt handler
    detachInterrupt(INT1);

    #ifdef DEBUGMODE
    Serial.println("wake!");
    #endif

    // Restore AVR peripherals used elsewhere in the code
    power_usart0_enable(); // Enable Serial
    power_timer0_enable(); // Enable millis() etc.
    power_spi_enable();    // Enable SPI

    // Turn on the NRF24L01+ radio.
    // radio.powerUp();

    // Set up button to trigger an interrupt.
    attachInterrupt(INT1, buttonPressed, FALLING);

    delay(50);

    // Simulate button press to set the correct flags
    buttonPressed();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// goToSleep()
// Powers down communication module and microcontroller. The normal button
// interrupt handler is disconnected, and another handler that handles the
// wakup is attached.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void goToSleep(void)
{
    noInterrupts();
    detachInterrupt(INT1);
    delay(50);
    attachInterrupt(INT1, wakeUp, LOW);
    interrupts();
    delay(50);

    digitalWrite(LED, LOW);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();

    radio.powerDown();

    power_spi_disable();    // Disable SPI
    delay(50);              // Stall for last serial output
    power_timer0_disable(); // Disable millis() etc.
    power_usart0_disable(); // Disable Serial

    // Go to sleep
    sleep_mode();

    /* The program will continue from here. */
    // ....
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Setup
// Initializes the communication module, and configures the LED and button.
// The button presses are handled with an interrupt handler rather than
// polling.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void setup()
{
    startingUp = true;
    ringRequested = false;

    #ifdef DEBUGMODE
    Serial.begin(115200);
    delay(10);
    Serial.println();
    Serial.println();
    Serial.print("Configuring...");
    #endif

    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(LED, OUTPUT);

    initializeComm();

    flashLed(3);
    ledOn = true;

    // Set up button to trigger an interrupt.
    noInterrupts();
    attachInterrupt(INT1, buttonPressed, FALLING);
    interrupts();
    delay(50);

    startingUp = false;

    #ifdef DEBUGMODE
        Serial.println();
        Serial.print("Done.");
    #endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Main loop
// Sends the wireless notification to the base unit if someone presses the
// doorbell.  Flashes the LED then leaves it solid on for 20 seconds.  If
// the bell is not pressed again in that time, the device is put to sleep.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void loop()
{
    if ( ringRequested )
    {
        #ifdef DEBUGMODE
            Serial.println( "Button pressed" );
            Serial.println( "Sending message to base station ..." );
        #endif

        sendNotify();

        // Reset the ring request flag so button presses are allowed again.
        ringRequested = false;
    }

    // Let's check if the unit has been awake longer than the activity timeout
    // since the last time the button was pressed.  If so, put the unit to sleep.

    int timePassed = millis() - lastPressed;

    if ( timePassed > ACTIVITY_TIMEOUT )
    {
        flashLed(4);
        digitalWrite(LED, LOW);
        ledOn = false;

        goToSleep();
    }
    else
    {
        // When the button is pressed, flash the LED every half a second for 2 seconds
        // to let the user know that something is happening.
        if ( timePassed < 2000 && (0 < (timePassed % 500) < 10) )
        {
            flashLed(1);
        }
    }
}
