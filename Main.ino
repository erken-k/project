#include <Hexapod.h>
//Declare a Hexapod with pin IO number 7
Hexapod hex(7);
void setup() {
  Serial.begin(115200); // begin communication using 115.2K baud rate
      while (!Serial) {
        ;// wait while connecting
      }

}

void loop() {
    
   // Put Hexapod in  Default Position One
    hex.defaultPositionOne();
    Wait 10 seconds to demonstrate  porition 1
    delay(10000);
    //Hexapod walks forward 10 steps
    hex.walkForward(10);
    //hexapod walks sideways 10 steps.
    hex.walkSideways(10);
}
