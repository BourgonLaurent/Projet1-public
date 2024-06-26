/**
 * Problem 2:
 *  Reflex game.
 *
 * Team #45
 * \author Mehdi Benouhoud
 * \author Laurent Bourgon
 * \date February 2, 2023
 *
 * Hardware Identification
 * INPUT: Push button connected to D2 with a jumper.
 * OUTPUT: Bicolor LED connected plus to A0 and minus to A1.
 *
 */

#define F_CPU 8000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include <tp2/components/colors.hpp>
#include <tp2/components/led.hpp>

#include <tp4/components/interruptButton.hpp>
#include <tp4/components/interruptTimer.hpp>
#include <tp4/components/interrupts.hpp>

constexpr uint8_t TIMER_DURATION_S = 1;
constexpr uint8_t FLASH_DURATION_MS = 100;
constexpr uint16_t WAIT_DURATION_MS = 10000;

volatile bool waitForUser = false;
volatile bool userWon = false;

void InterruptTimer::whenFinished()
{
    if (::waitForUser) {
        ::userWon = false;
        ::waitForUser = false;
    }
}

void InterruptButton::whenPressed()
{
    if (::waitForUser) {
        ::userWon = true;
        ::waitForUser = false;
    }
}

int main()
{
    LED led = LED(&DDRA, &PORTA, PORTA0, PORTA1);

    interrupts::stopCatching();

    InterruptTimer::initialize();
    InterruptTimer::setMode(InterruptTimer::Mode::CTC);
    InterruptTimer::setPrescaleMode(InterruptTimer::PrescaleMode::CLK1024);
    InterruptTimer::setSeconds(TIMER_DURATION_S);

    InterruptButton::initialize();
    InterruptButton::setMode(InterruptButton::Mode::RISING);

    interrupts::startCatching();

    _delay_ms(WAIT_DURATION_MS);

    led.setColor(Color::RED);
    _delay_ms(FLASH_DURATION_MS);
    led.setColor(Color::OFF);

    InterruptTimer::start();
    ::waitForUser = true;

    while (::waitForUser) {}

    interrupts::stopCatching();
    InterruptTimer::stop();

    led.setColor(::userWon ? Color::GREEN : Color::RED);

    return 0;
}
