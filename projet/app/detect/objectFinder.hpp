/**
 * Finds object.
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

#ifndef OBJECT_FINDER_H
#define OBJECT_FINDER_H

#include <avr/io.h>
#include <util/delay.h>

#include <lib/analogReader.hpp>
#include <lib/io.hpp>
#include <lib/wheels.hpp>

#include <app/detect/border.hpp>
#include <app/detect/constants.hpp>
#include <app/detect/positionManager.hpp>

class ObjectFinder
{
public:
    ObjectFinder(IrSensor &irSensor);

    void finder(volatile bool &timeOut);

    void search(const Wheels::Side &side, volatile bool &timeOut,
                double timerLimit, uint8_t speed = constants::SPEED_TURN);
    void park(volatile bool &timeOut, const Wheels::Side &side);
    void find(const Wheels::Side &side, volatile bool &timeOut,
              double timerLimit = constants::DELAY_FIND_MS);

    void alertParked();
    void alertFoundNothing();
    bool isObjectFound();
    void sendLastPosition();

    // FIXME: 5 parameters is too much
    bool isObjectInFront(volatile bool &timeOut,
                         Wheels::Side side = Wheels::Side::RIGHT,
                         double delay1 = constants::FIRST_DELAY_IS_IN_FRONT_MS,
                         double delay2 = constants::SECOND_DELAY_IS_IN_FRONT_MS,
                         uint8_t speed = constants::SPEED_TURN);

    Point getLastPosition();

private:
    PositionManager positionManager_;
    IrSensor* const irSensor_;

    bool objectFound_ = false;

    Border getCardinal();

    void turnFind(const Wheels::Side &side, volatile bool &timeOut);
    void findTurn(const Wheels::Side &side, volatile bool &timeOut);
    void findLoop(uint8_t max, const Wheels::Side &side,
                  volatile bool &timeOut);
};

#endif
