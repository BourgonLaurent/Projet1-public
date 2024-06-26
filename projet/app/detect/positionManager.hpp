/**
 * Management of current position and next position.
 *
 * Team #4546
 *  \author Catalina Andrea Araya Figueroa
 *  \author Mehdi Benouhoud
 *  \author Laurent Bourgon
 *  \author Ihsane Majdoubi
 *
 * \date April 5, 2023
 */

#ifndef POSITION_MANAGER_H
#define POSITION_MANAGER_H

#include <lib/irSensor.hpp>
#include <lib/wheels.hpp>

#include <app/misc/map/map.hpp>
#include <app/misc/point.hpp>

class PositionManager
{
public:
    enum Quadrant
    {
        TOP_RIGHT = 0,
        BOTTOM_RIGHT = 1,
        BOTTOM_LEFT = 2,
        TOP_LEFT = 3
    };

    Point getLastPosition();

    void setNextPositionObject(const IrSensor::Range &range);

    enum class Distance
    {
        FAR,
        CLOSE,
    };
    void setDistance(const Distance &distance);

    uint8_t getQuadrant();
    void resetQuadrant();
    void updateQuadrant(const Wheels::Side &side);

private:
    static constexpr uint8_t DIFFERENCE_WITH_NEW_POSITION_FAR = 2;
    static constexpr uint8_t DIFFERENCE_WITH_NEW_POSITION_CLOSE = 1;

    void setPositionDiagonal(uint8_t difference, uint8_t quadrant);
    void setPositionStraight(uint8_t difference, uint8_t quadrant);

    Point lastPosition_ = {0, 0};
    uint8_t quadrant_ = 0;
    Distance distance_ = Distance::CLOSE;
};

#endif
