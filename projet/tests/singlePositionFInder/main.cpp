#include <avr/io.h>
#include <util/delay.h>

#include <lib/button.hpp>
#include <lib/debug.hpp>
#include <lib/interruptButton.hpp>
#include <lib/interruptTimer.hpp>
#include <lib/interrupts.hpp>
#include <lib/led.hpp>
#include <lib/objectFinder.hpp>
#include <lib/sound.hpp>
#include <lib/wheels.hpp>

#include <app/misc/map/mapManager.hpp>

const uint16_t DELAY_BEFORE_SEARCHING_MS = 2000;
const uint8_t DELAY_LED_AMBER_MS = 20;
const io::Position SENSOR = PA0;

enum class States
{
    SET_DIRECTION,
    RIGHT,
    FROM_RIGH_UP,
    UP,
    FIND_OBJECT,
    FOUND_NOTHING,
    FOUND_OBJECT,
    WAIT_NEXT_DETECTION

};
volatile States state = States::SET_DIRECTION;
volatile bool timeOut = false;

ISR(InterruptTimer_vect)
{
    // debug::send("timerIsr\n");
    if (OCR1A < TCNT1)
        timeOut = true;
}

ISR(InterruptButton_vect)
{
    // debug::send("intIsr\n");
    InterruptButton::waitForDebounce();
    state = States::FIND_OBJECT;
    InterruptButton::clear();
}

int main()
{
    interrupts::stopCatching();

    debug::initialize();
    Wheels::initialize();
    Sound::initialize();
    Communication::initialize();

    InterruptButton::initialize(InterruptButton::Mode::ANY);

    Led led = Led(&DDRB, &PORTB, PB0, PB1);
    Button whiteButton(&DDRC, &PINC, PC2);
    Button interruptButton(&DDRD, &PIND, PD2);
    InterruptTimer::initialize(InterruptTimer::Mode::NORMAL, 2.0);

    Map map;
    IrSensor irSensor(SENSOR);
    ObjectFinder finder(led, irSensor);
    // finder.initialize();
    while (true) {
        switch (state) {
            case States::SET_DIRECTION :
                led.setAmberForMs(DELAY_LED_AMBER_MS);
                // setAmberForMs bloquantte

                if (!whiteButton.isPressed())
                    state = States::RIGHT;
                else if (interruptButton.isPressed())
                    state = States::UP;

                break;

            case States::RIGHT :
                led.setColor(Led::Color::RED);
                _delay_ms(2000);
                led.setColor(Led::Color::OFF);
                state = States::FROM_RIGH_UP;
                break;

            case States::FROM_RIGH_UP :
                Wheels::turn(Wheels::Side::LEFT); // replace turn with rotate?
                finder.isObjectInFront(timeOut, Wheels::Side::RIGHT);
                state = States::FIND_OBJECT;
                break;

            case States::UP :
                led.setColor(Led::Color::GREEN);
                _delay_ms(2000);
                led.setColor(Led::Color::OFF);
                state = States::FIND_OBJECT;
                break;

            case States::FIND_OBJECT :
                // finder.initialize();
                debug::send("Find object from position: \n");
                finder.sendLastPosition();
                led.setColor(Led::Color::OFF);
                InterruptButton::clear();
                // interrupts::startCatching();
                // InterruptTimer::reset();
                finder.finder(timeOut);
                debug::send("New position: \n");
                finder.sendLastPosition();
                // InterruptTimer::stop();
                // interrupts::stopCatching();

                if (finder.isObjectFound()) {
                    debug::send("ObjectFound-> Now Parking\n");
                    state = States::FOUND_OBJECT;
                }
                else {
                    state = States::FOUND_NOTHING;
                }
                break;

            case States::FOUND_OBJECT :
                finder.alertParked();
                Point detectedPosition;
                detectedPosition = finder.getLastPosition();
                if (detectedPosition.x <= 7 && detectedPosition.y <= 3)
                    map[detectedPosition.x][detectedPosition.y].set();
                state = States::WAIT_NEXT_DETECTION;
                break;

            case States::WAIT_NEXT_DETECTION :
                interrupts::startCatching();
                led.setAmberForMs(250);
                _delay_ms(250);
                break;

            case States::FOUND_NOTHING :
                MapManager::save(map);
                // InterruptButton::clear();
                finder.alertFoundNothing();
                debug::send("\n\nMAP RESULT\n\n");
                debug::send(map[0].get());
                debug::send(map[1].get());
                debug::send(map[2].get());
                debug::send(map[3].get());
                debug::send(map[4].get());
                debug::send(map[5].get());
                debug::send(map[6].get());
                debug::send(map[7].get());
                while (true) {
                    led.setColor(Led::Color::RED);
                    _delay_ms(250);
                    led.setColor(Led::Color::OFF);
                    _delay_ms(250);
                }
                return 0;
                break;
        }
    }
}
