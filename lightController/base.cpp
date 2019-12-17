#include "sensors.h"

SensorBase::SensorBase()
{
  this -> state = 0;
}

unsigned int SensorBase::getState()
{
  return this -> state;
};

void SensorBase::setState(unsigned int value)
{
  this -> state = value;
};

bool SensorBase::updateState() { return false; }
