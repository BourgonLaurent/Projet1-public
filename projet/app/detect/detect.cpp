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
#include "app/detect/detect.hpp"
#include <app/detect/objectFinder.hpp>
#include <app/misc/map/map.hpp>
#include <app/misc/map/mapManager.hpp>
#include <lib/debug.hpp>
#include <lib/interruptTimer.hpp>
#include <lib/interrupts.hpp>
#include <lib/irSensor.hpp>
#include <lib/sound.hpp>
#include <lib/wheels.hpp>
#include <util/delay.h>

volatile bool Detect::timeOut_ = false;
volatile Detect::States Detect::state_ = States::SET_DIRECTION;
void Detect::initialize()
{
    Wheels::initialize();
    Sound::initialize();
    InterruptButton::initialize(InterruptButton::Mode::ANY);
    InterruptTimer::initialize(InterruptTimer::Mode::NORMAL,
                               constants::DELAY_TURN_90_MS);
}
void Detect::checkTimerValue()
{
    if (OCR1A < TCNT1)
        Detect::timeOut_ = true;
}
void Detect::setStateISR()
{
    Detect::state_ = Detect::States::FIND_OBJECT;
}

int Detect::run(Led &led, Button &whiteButton, Button &interruptButton,
                IrSensor &irSensor, Map &map)
{
    initialize();
    ObjectFinder finder(led, irSensor);
    interrupts::stopCatching();
    while (true) {
        switch (state_) {
            case States::SET_DIRECTION :
                led.setAmberForMs(constants::DELAY_LED_AMBER_MS);

                if (!whiteButton.isPressed())
                    state_ = States::RIGHT;
                else if (interruptButton.isPressed())
                    state_ = States::UP;

                break;

            case States::RIGHT :
                led.setColor(Led::Color::RED);
                _delay_ms(2000);
                led.setColor(Led::Color::OFF);
                state_ = States::FROM_RIGH_UP;
                break;

            case States::FROM_RIGH_UP :
                Wheels::turn90(Wheels::Side::LEFT);
                finder.isObjectInFront(timeOut_, Wheels::Side::RIGHT);
                state_ = States::FIND_OBJECT;
                break;

            case States::UP :
                led.setColor(Led::Color::GREEN);
                _delay_ms(2000);
                led.setColor(Led::Color::OFF);
                state_ = States::FIND_OBJECT;
                break;

            case States::FIND_OBJECT :
                debug::send("Find object from position: \n"); // à enlever
                finder.sendLastPosition();                    // à enlever
                led.setColor(Led::Color::OFF);
                InterruptButton::clear();
                finder.finder(timeOut_);
                debug::send("New position: \n"); // à enlever
                finder.sendLastPosition();       // à enlever

                if (finder.isObjectFound()) {
                    state_ = States::FOUND_OBJECT;
                }
                else {
                    state_ = States::FOUND_NOTHING;
                }
                break;

            case States::FOUND_OBJECT :
                finder.alertParked();
                state_ = States::WAIT_NEXT_DETECTION;
                break;

            case States::WAIT_NEXT_DETECTION :
                interrupts::startCatching();

                led.setAmberForMs(constants::DELAY_LED_AMBER_2HZ_MS);
                _delay_ms(constants::DELAY_LED_AMBER_2HZ_MS);
                break;

            case States::FOUND_NOTHING :
                MapManager::save(map);
                finder.alertFoundNothing();
                while (true) {
                    led.setColor(Led::Color::RED); // fonction pour flash ??
                    _delay_ms(constants::DELAY_LED_AMBER_2HZ_MS);
                    led.setColor(Led::Color::OFF);
                    _delay_ms(constants::DELAY_LED_AMBER_2HZ_MS);
                }
                return 0;
                break;
        }
    }
}