#include <iostream>
#include "sensors.h"

int main() {
    using namespace std;

    ToggleSensor s(3);
    cout << s.getState() << endl;
    s.setState(2);
    cout << s.getState() << endl;
    return 0;
}
