/**
 * Detect program of the project
 *
 * HARDWARE:
 * IR sensor to PA0
 * Led to PB0(+) and PB1(-) // à vérifier
 * InterruptButton to PD2
 * White Button to PC2
 *
 * Team #4546
 *  \author Catalina Andrea Araya Figueroa
 *  \author Mehdi Benouhoud
 *  \author Laurent Bourgon
 *  \author Ihsane Majdoubi
 *
 * \date April 17, 2023
 */

#ifndef DETECT_H
#define DETECT_H

#include <avr/io.h>

#include <app/misc/map/map.hpp>
#include <lib/button.hpp>
#include <lib/interruptButton.hpp>
#include <lib/irSensor.hpp>
#include <lib/led.hpp>

class Detect
{
public:
    enum class States
    {
        SET_DIRECTION,
        RIGHT,
        FROM_RIGH_UP,
        UP,
        FIND_OBJECT,
        FOUND_NOTHING,
        FOUND_OBJECT,
        WAIT_NEXT_DETECTION

    };
    static void checkTimerValue();
    static void setStateISR();
    static int run(Led &led, Button &whiteButton, Button &interruptButton,
                   IrSensor &irSensor, Map &map);

private:
    static volatile States state_;
    static volatile bool timeOut_;
    static void initialize();

}; // namespace detect

#endif