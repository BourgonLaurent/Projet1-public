/**
 * Control of a infrared sensor.
 *
 * HARDWARE:
 * IR sensor to PA0
 *
 * Team #4546
 *  \author Catalina Andrea Araya Figueroa
 *  \author Mehdi Benouhoud
 *  \author Laurent Bourgon
 *  \author Ihsane Majdoubi
 *
 * \date March 28, 2023
 */

#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include "analogReader.hpp"

#include <avr/io.h>
#include <util/delay.h>

#include <lib/io.hpp>

class IrSensor
{
public:
    enum class Range
    {
        DIAGONAL,
        STRAIGHT,
    };

    struct Calibration {
        const uint8_t tenCm;
        const uint8_t fifteenCm;
        const uint8_t eightyCm;
    };

    IrSensor(io::Position pin, const Calibration &constants);

    uint16_t read();

    bool isInFront();
    bool isClose();
    bool isTooClose();

    bool isObjectDetected();
    void setObjectDetected(bool objectDetected);

    void setRange(IrSensor::Range range);

    Range getRange();

private:
    static constexpr uint8_t N_MEASURMENTS = 6;
    static constexpr uint8_t DELAY_BETWEEN_READS_MS = 5;

    static io::DataDirectionRegister dataDirection_;

    bool isInRange(uint8_t distance1, uint8_t distance2);

    AnalogReader reader_;
    const io::Position pin_;
    const Calibration calibration_;

    Range range_ = Range::STRAIGHT;

    bool isObjectDetected_ = false;
};

#endif
