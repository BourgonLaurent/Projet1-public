/**
 * Runs two programs chosen by pressing a button:
 *  - Detect   | Interrupt : Find poles and log them on a map.
 *  - Transmit | White     : Find the convex hull formed by the poles
 *                           and send a SVG through RS232.
 *  - Demo     | Both      : Same as transmit but with
 *                           a predefined set of points.
 *
 * Hardware Identification:
 *  NOTE: The data direction registers are set automatically.
 *
 *  INPUT:
 *    - IR Sensor: PA0
 *    - Interrupt Push Button: PD2
 *    - White Push Button: PC2
 *
 *  OUTPUT:
 *    - Bicolor LED connected plus to PB0 and minus to PB1.
 *    - Wheels
 *      - Left  | PD6 (enable) and PD4 (direction)
 *      - Right | PD7 (enable) and PD5 (direction)
 *    - Buzzer connected plus to PB3 and minus to PB2.
 *
 *  EEPROM:
 *    - SCL (PC0)
 *    - SDA (PC1)
 *
 *  USART:
 *    - RXD0 (PD0)
 *    - TXD0 (PD1)
 *
 *  TIMERS:
 *    - Timer 0 | Sound
 *    - Timer 1 | InterruptTimer & Flasher
 *    - Timer 2 | Wheels
 *
 * Team #4546
 *  \author Catalina Andrea Araya Figueroa
 *  \author Mehdi Benouhoud
 *  \author Laurent Bourgon
 *  \author Ihsane Majdoubi
 *
 * \date March 23, 2023
 */

#include <avr/io.h>

#include <lib/button.hpp>
#include <lib/communication.hpp>
#include <lib/debug.hpp>
#include <lib/flasher.hpp>
#include <lib/interruptButton.hpp>
#include <lib/interruptTimer.hpp>
#include <lib/irSensor.hpp>
#include <lib/led.hpp>

#include <app/detect/constants.hpp>
#include <app/detect/detect.hpp>
#include <app/detect/objectFinder.hpp>
#include <app/transmit/transmit.hpp>

ISR(InterruptTimer_vect)
{
    ObjectFinder::handleTimer();
    Flasher::handleFlash();
}

ISR(InterruptButton_vect)
{
    InterruptButton::waitForDebounce();
    Detect::handleButtonPress();
    InterruptButton::clear();
}

constexpr uint8_t WAIT_FOR_CLEAN_BUTTONS_MS = 200;

int main()
{
    Communication::initialize();
    debug::initialize();

    Led led = Led(&DDRB, &PORTB, PB0, PB1);
    Button white(&DDRC, &PINC, PC2, Button::ActiveMode::RELEASED);
    Button interrupt(&DDRD, &PIND, PD2);
    IrSensor irSensor(PA0, constants::CALIBRATION);

    bool whiteWasPressed = false;
    bool interruptWasPressed = false;

    while (interruptWasPressed = interrupt.isPressed(),
           whiteWasPressed = white.isPressed(),
           !interruptWasPressed && !whiteWasPressed) {}

    if (whiteWasPressed) {
        debug::send("Mode: transmit\n");
        Transmit::run(led, interrupt.isPressed() ? Transmit::Mode::DEMO
                                                 : Transmit::Mode::NORMAL);
    }
    else if (interruptWasPressed) {
        debug::send("Mode: detect\n");
        _delay_ms(WAIT_FOR_CLEAN_BUTTONS_MS);
        Detect::run(led, white, interrupt, irSensor);
    }
}
