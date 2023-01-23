/**
 * Implementation of the control of an LED.
 *
 * \author Mehdi Benouhoud
 * \author Laurent Bourgon
 * \date January 19, 2023
 */

#ifndef LED_H
#define LED_H

#include <avr/io.h>

#include <tp2/components/colors.hpp>

class LED
{
public:
    LED(
        volatile uint8_t *dataDirectionRegister,
        volatile uint8_t *port,
        const uint8_t plusBit,
        const uint8_t minusBit);

    void setColor(const Color &color);

private:
    volatile uint8_t *port_;
    const uint8_t plusBit_;
    const uint8_t minusBit_;
};

#endif