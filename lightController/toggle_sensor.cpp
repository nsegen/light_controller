#include "arduino/mock.h"
#include "sensors.h"

ToggleSensor::ToggleSensor(unsigned short pin) : SensorBase::SensorBase()
{
  this -> sensorPin = pin;
  attachInterrupt(pin, this -> setState)
}

ToggleSensor::~ToggleSensor()
= default;