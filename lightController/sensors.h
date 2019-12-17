#ifndef sensors_h
#define sensors_h

class SensorBase
{
  protected:
    unsigned int state;
  public:
    SensorBase();
    unsigned int getState();
    void setState(unsigned int);
    virtual bool updateState();
};

class ToggleSensor : public SensorBase
{
  private:
    unsigned short sensorPin;
  public:
    ToggleSensor(unsigned short);
    ~ToggleSensor();
};

#endif
