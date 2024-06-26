/**
 * Emit MIDI notes using piezoelectric buzzer and hardware PWM.
 *
 * LIMITATIONS: Supports only notes from 45 to 81.
 *
 * Hardware Identification
 *  WARNING: Data Direction Register will be set automatically.
 *  TIMER: Timer 0 (OC0A)
 *  OUTPUT: Buzzer, connected plus to PB3 and minus to PB2.
 *
 * USAGE:
 *  Sound::intialize();
 *  Sound::playNote(45);
 *  _delay_ms(500);
 *  Sound::stop();
 *  _delay_ms(500);
 *  Sound::start();
 *
 * Team #4546
 *  \author Catalina Andrea Araya Figueroa
 *  \author Mehdi Benouhoud
 *  \author Laurent Bourgon
 *  \author Ihsane Majdoubi
 *
 * \date March 14, 2023
 */

#include "sound.hpp"

#include <math.h>

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

io::DataDirectionRegister Sound::dataDirectionRegister_ = &DDRB;
io::Port buzzerPort = &PORTB;

io::Position Sound::buzzerMinusPosition_ = PB2;
io::Position Sound::buzzerPlusPosition_ = PB3;

void Sound::initialize()
{
    io::setOutput(dataDirectionRegister_, buzzerPlusPosition_);
    io::setOutput(dataDirectionRegister_, buzzerMinusPosition_);

    io::clear(buzzerPort, buzzerMinusPosition_);

    // (p.103) PWM, CTC
    io::clear(&TCCR0A, WGM00);
    io::setActive(&TCCR0A, WGM01);
    io::clear(&TCCR0B, WGM02);

    // (p.104) Prescaler of 256
    io::clear(&TCCR0B, CS00);
    io::clear(&TCCR0B, CS01);
    io::setActive(&TCCR0B, CS02);
}

void Sound::playNote(uint8_t midiNote)
{
    if (midiNote < MINIMUM_NOTE || midiNote > MAXIMUM_NOTE) {
        return;
    }

    uint16_t frequency = getFrequency(midiNote);

    // (p.96) Get OCR0A value from frequency
    OCR0A = (F_CPU / (2 * uint32_t(frequency) * PRESCALER_VALUE)) - 1;

    start();
}

void Sound::start()
{
    // (p.101) Clear OC0A on set to toggle
    io::setActive(&TCCR0A, COM0A0);
    io::clear(&TCCR0A, COM0A1);
}

void Sound::stop()
{
    // (p.101) Disconnect OC0A
    io::clear(&TCCR0A, COM0A0);
    io::clear(&TCCR0A, COM0A1);
    OCR0A = 0;
}

uint16_t Sound::getFrequency(uint8_t midiNote)
{
    return 440 * pow(2, (double(midiNote) - 69) / 12);
}
