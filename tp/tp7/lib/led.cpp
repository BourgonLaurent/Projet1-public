/**
 * Control of an LED.
 *
 * Hardware Identification:
 * /!\ The Data Direction Register (`dataDirectionRegister`)
 *     will be set automatically.
 * OUTPUT: Bicolor LED connected plus to `plusBit` and minus to `minusBit`.
 *
 * Team #45
 * \author Mehdi Benouhoud
 * \author Laurent Bourgon
 * \date January 19, 2023
 */

#include "led.hpp"

#include <util/delay.h>

#include <lib/colors.hpp>
#include <lib/io.hpp>

LED::LED(io::DataDirectionRegister dataDirectionRegister, io::Port port,
         const io::PinPosition plus, const io::PinPosition minus)
    : port_(port), plus_(plus), minus_(minus)
{
    io::setOutput(dataDirectionRegister, plus_);
    io::setOutput(dataDirectionRegister, minus_);
};

void LED::setColor(const Color &color)
{
    switch (color) {
        case Color::OFF :
            io::clear(port_, minus_);
            io::clear(port_, plus_);
            break;

        case Color::GREEN :
            io::setActive(port_, minus_);
            io::clear(port_, plus_);
            break;

        case Color::RED :
            io::setActive(port_, plus_);
            io::clear(port_, minus_);
            break;
    }
}

void LED::setAmberForMs(const uint16_t durationMs)
{
    for (uint16_t i = 0; i < durationMs; i++) {
        this->setColor(Color::GREEN);
        _delay_us(LED::AMBER_DELAY_US);
        this->setColor(Color::RED);
        _delay_us(LED::AMBER_DELAY_US);
    }
}
