#ifndef Hexapod_h
#define Hexapod_h
//#include "Arduino.h"
/* Author: Joyce Chiu
* This code goes together with hexapod.cpp and defines a hexapod class
*/
class Hexapod{
    public:
      Hexapod(int x);
      String getCommandToMoveServo(int servo, int impulse);
      void sendCommand(String command);
      String moveLeg(String leg,int alpha, int betta, int gamma,bool exec);
      void defaultPositionOne();
      void moveOneLegForward(String leg);
      void moveTripod(String side);
      String controlTail(String servo,int degr, bool exec);
      void walkForward(int stepNumber);
      void moveToSide(String stage,String side);
      void walkSideways(int stepNumber);
      String controlHead(String dir,int degr, bool exec);
      int degreesToImpulse(int x);
      int relativeToServo(int x);
    private:
    
  };
  #endif
