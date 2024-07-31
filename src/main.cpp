#include <Arduino.h>

#include "at8236.h"
#include "event_center.h"
#include "listener.h"

constexpr uint8_t first_pin{1};
constexpr uint8_t second_pin{2};

constexpr uint8_t start_pin{3};
constexpr uint8_t stop_pin{4};
constexpr uint8_t reverse_pin{5};

simia::AT8236 pump{first_pin, second_pin};
simia::Listener listener{115200};

void start(std::shared_ptr<void> param)
{
    pump.start();
}

void stop(std::shared_ptr<void> param)
{
    pump.stop();
}

void reverse(std::shared_ptr<void> param)
{
    pump.reverse();
}

void set_speed(std::shared_ptr<void> param)
{
    auto msg = std::static_pointer_cast<uint8_t>(param);
    auto speed = static_cast<float>(*msg) / 100;
    pump.set_speed(speed);
}

void start_interrupt()
{
    pump.start();
}

void stop_interrupt()
{
    pump.stop();
}

void reverse_interrupt()
{
    pump.reverse();
}

void setup()
{
    simia::EventCenter::instance().regsiter(simia::CMD::Start, start);
    simia::EventCenter::instance().regsiter(simia::CMD::Stop, stop);
    simia::EventCenter::instance().regsiter(simia::CMD::Reverse, reverse);
    simia::EventCenter::instance().regsiter(simia::CMD::SetSpeed, set_speed);

    pinMode(start_pin, INPUT_PULLUP);
    pinMode(stop_pin, INPUT_PULLUP);
    pinMode(reverse_pin, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(start_pin), start_interrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(stop_pin), stop_interrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(reverse_pin), reverse_interrupt, FALLING);
}

void loop()
{
    listener.listen();
}