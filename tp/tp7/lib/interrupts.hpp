/**
 * Interrupts manipulation.
 *
 * Team #4546
 * \author Catalina Andrea Araya Figueroa
 * \author Mehdi Benouhoud
 * \author Laurent Bourgon
 * \author Ihsane Majdoubi
 *
 * \date February 2, 2023
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <avr/interrupt.h>
#include <avr/io.h>

#include "io.hpp"

namespace interrupts {
    static inline void startCatching()
    {
        sei();
    }

    static inline void stopCatching()
    {
        cli();
    }
} // namespace interrupts

#endif