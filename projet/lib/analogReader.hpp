/**
 * Read analog values.
 *
 * Code modified from can (2006, Matthew Khouzam & Jerome Collin)
 *
 * Hardware Identification
 *  WARNING: A read pin needs to be on PORTA and set as input.
 *
 * USAGE:
 *  io::setInput(PA0);
 *  AnalogReader reader;
 *  uint8_t value = reader.read(PA0);
 *
 * Team #4546
 *  \author Catalina Andrea Araya Figueroa
 *  \author Mehdi Benouhoud
 *  \author Laurent Bourgon
 *  \author Ihsane Majdoubi
 *
 * \date March 2, 2023
 */

#ifndef ANALOG_READER_H
#define ANALOG_READER_H

#include <avr/io.h>

#include <lib/io.hpp>

class AnalogReader
{
public:
    AnalogReader();
    ~AnalogReader();

    uint8_t read(io::Position position);
};

#endif
