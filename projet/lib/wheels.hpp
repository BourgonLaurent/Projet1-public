/**
 * Control left and right wheels using hardware PWM.
 *
 * Hardware Identification
 *  WARNING: The Data Direction Register will be set automatically.
 *  TIMER: Timer 2.
 *  OUTPUT: H-bridge, connected left  to PD6 (enable) and PD4 (direction)
 *                    connected right to PD7 (enable) and PD5 (direction)
 *
 * USAGE:
 *  Wheels::initialize();
 *  Wheels::setDirection(Direction::BACKWARD, Wheels::Side::RIGHT);
 *  Wheels::setSpeed(0.2, Wheels::Side::LEFT);
 *  Wheels::setSpeed(0.6, Wheels::Side::RIGHT);
 *  Wheels::invertDirection(Side::LEFT);
 *  Wheels::rotate(Side::LEFT);
 *  turnOff(Side::RIGHT)
 *
 * Team #4546
 *  \author Catalina Andrea Araya Figueroa
 *  \author Mehdi Benouhoud
 *  \author Laurent Bourgon
 *  \author Ihsane Majdoubi
 *
 * \date February 2, 2023
 */

#ifndef Wheel_H
#define Wheel_H

#include <avr/io.h>

#include <lib/io.hpp>

class Wheels
{
public:
    enum class Side
    {
        LEFT,
        RIGHT,
        BOTH
    };

    enum class Direction
    {
        FORWARD,
        BACKWARD
    };

    static void initialize(const Side &side = Side::BOTH);

    static void setDirection(const Direction &direction,
                             const Side &side = Side::BOTH);
    static void invertDirection(const Side &side);

    static void turn(const Side &side);

    static void rotate(const Side &side, const uint8_t speed);
    static void stopRotating(const Side &side);

    static void setSpeed(const uint8_t speed, const Side &side = Side::BOTH);
    static void turnOff(const Side &side = Side::BOTH);

private:
    static void setDirectionOfPin(const Direction &direction,
                                  const io::Position pin);
    static void configureOutputPins(const Side &side);

    static constexpr uint8_t TOP_VALUE = 0xFF; // (p. 153)

    static constexpr uint16_t TURN_DURATION_MS = 2250;
    static constexpr uint8_t TURN_TIMEOUT_MS = 125;
    static constexpr uint8_t SPEED_TURN_90 = 48;
    static constexpr uint8_t SPEED_DIFFERENCE_BETWEEN_WHEELS = 5;

    static io::DataDirectionRegister dataDirectionRegister_;
    static io::Port port_;
    static io::Position leftEnable_;
    static io::Position leftDirection_;
    static io::Position rightEnable_;
    static io::Position rightDirection_;
};

constexpr Wheels::Side operator!(const Wheels::Side &side)
{
    return side == Wheels::Side::LEFT ? Wheels::Side::RIGHT
                                      : Wheels::Side::LEFT;
}

#endif
