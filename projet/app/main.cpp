/**
 * Runs two programs chosen by pressing a button:
 *  - Detect   | Interrupt : Find poles and log them on a map.
 *  - Transmit | White)    : Find the convex hull formed by the poles
 *                           and send a SVG through RS232.
 *
 * Hardware Identification:
 *  NOTE: The data direction registers are set automatically.
 *
 *  INPUT:
 *    - Interrupt Push Button: D2
 *    - White Push Button: C2
 *
 *  OUTPUT:
 *    - Bicolor LED connected plus to PB0 and minus to PB1.
 *
 *  EEPROM:
 *    - SCL (C0)
 *    - SDA (C1)
 *
 *  USART:
 *    - RXD0 (D0)
 *    - TXD0 (D1)
 *
 *  TIMERS:
 *    - Timer 1 | Flasher
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
#include <app/transmit/run.hpp>

ISR(InterruptTimer_vect)
{
    Flasher::handleFlash();
    Detect::checkTimerValue();
}

ISR(InterruptButton_vect)
{
    InterruptButton::waitForDebounce();
    Detect::setStateISR();
    InterruptButton::clear();
}

int main()
{
    Communication::initialize();
    debug::initialize();

    Led led = Led(&DDRB, &PORTB, PB0, PB1);
    Button white(&DDRC, &PINC, PC2, Button::ActiveMode::RELEASED);
    Button interrupt(&DDRD, &PIND, PD2);
    IrSensor irSensor(constants::SENSOR);
    Map map;

    bool whiteWasPressed = false;
    bool interruptWasPressed = false;

    while (interruptWasPressed = interrupt.isPressed(),
           whiteWasPressed = white.isPressed(),
           !interruptWasPressed && !whiteWasPressed) {}

    if (interruptWasPressed) {
        debug::send("Mode: detect\n");
        _delay_ms(1000);
        Detect::run(led, white, interrupt, irSensor, map);
    }
    else if (whiteWasPressed) {
        debug::send("Mode: transmit\n");
        transmit::run(led);
    }
}
