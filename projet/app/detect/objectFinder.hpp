/**
 * Finds object.
 *
 * Hardware Identification:
 *  NOTE: The data direction registers are set automatically.
 *
 *  INPUT:
 *    - IR Sensor
 *
 *  OUTPUT:
 *    - Wheels
 *      - Left  | PD6 (enable) and PD4 (direction)
 *      - Right | PD7 (enable) and PD5 (direction)
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

    void run();

    void find(const Wheels::Side &side,
              const uint16_t timerLimit = constants::DELAY_FIND_MS);
    void search(const Wheels::Side &side, const uint16_t timerLimit,
                const uint8_t speed = constants::SPEED_TURN);
    void park(const Wheels::Side &side);

    bool isObjectFound();
    bool isObjectInFront(
        const Wheels::Side &side = Wheels::Side::RIGHT,
        const uint16_t firstDelay = constants::FIRST_DELAY_IS_IN_FRONT_MS,
        const uint16_t secondDelay = constants::SECOND_DELAY_IS_IN_FRONT_MS,
        const uint8_t speed = constants::SPEED_TURN);

    Point getLastPosition();
    void sendLastPosition();

    static void handleTimer();

private:
    static constexpr uint8_t N_FINDS_ON_BORDER = 2;
    static constexpr uint8_t N_FINDS_IN_MIDDLE = 4;

    static volatile bool timeOut_;
    static bool timerActive_;

    Border getBorder();

    void turnFind(const Wheels::Side &side);
    void findTurn(const Wheels::Side &side);
    void findLoop(const uint8_t nTurns, const Wheels::Side &side);

    PositionManager positionManager_;
    IrSensor* const irSensor_;

    bool objectFound_ = false;
};

#endif
