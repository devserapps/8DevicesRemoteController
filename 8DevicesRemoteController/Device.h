class Device {
private:
  const uint8_t id;
  const String name;
  bool state;
  const uint8_t pin;

public:
  Device(uint8_t idParam, String nameParam, bool stateParam, uint8_t pinParam) 
    :id(idParam), name(nameParam), state(stateParam), pin(pinParam) {}

  void setState(bool state) {
    this->state = state;
  }

  uint8_t getID() {
    return id;
  }

  String getName() {
    return name;
  }

  bool getState() {
    return state;
  }

  uint8_t getPin() {
    return pin;
  }
};