/**
 * Finds object.
 *
 * HARDWARE:
 * IR sensor to PA6
 *
 * Team #4546
 *  \author Catalina Andrea Araya Figueroa
 *  \author Mehdi Benouhoud
 *  \author Laurent Bourgon
 *  \author Ihsane Majdoubi
 *
 * \date March 28, 2023
 */

#include "lib/analogReader.hpp"

#include <avr/io.h>
#include <util/delay.h>

#include <lib/io.hpp>
#include <lib/irSensor.hpp>
#include <lib/led.hpp>
#include <lib/wheels.hpp>

class ObjectFinder
{
public:
    ObjectFinder(Led &led, IrSensor &irSensor);

    void park();
    void find(Wheels::Side side = Wheels::Side::RIGHT);
    void alertParked();
    void alertFoundNothing();

private:
    const io::Position SENSOR = PA6;
    static constexpr uint16_t DELAY_FOUND_NOTHING_MS = 2000;
    static constexpr uint16_t DELAY_ALERT_PARKED_MS = 300;
    static constexpr uint8_t DELAY_TURN_MIDDLE_OBJECT_MS = 100;
    static constexpr uint8_t HIGH_NOTE = 78;
    static constexpr uint8_t LOW_NOTE = 45;
    static constexpr uint16_t DELAY_TURNOFF_MS = 500;
    Led &led_;
    IrSensor &irSensor_;
    bool objectFound_ = 0;
};