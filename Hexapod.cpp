#include "Arduino.h"
#include "Hexapod.h"
/* Author: Erken Kambarov
*This class represents Hexapod and allows user to generate and send commands to the servos on the
* LynxMotion SSC-32 board from botBoarduino to make the Hexapod robot move by updating
* the positions and writing their positions to the servos. It also has moving forward and sideways implemented in it, as well as the tools
to implement other movements
*/
Hexapod::Hexapod(int x){
  ;
}
/*
String getCommandToMoveServo()
input: pin number of servo to be moved, impulse to be send to this pin
processing: combine passed information into a passable command
output: returns rsulted command
*/
String Hexapod::getCommandToMoveServo(int servo, int impulse){
  String x=String(servo);
  String y=String(impulse);
  String command="#"+x+"p"+y;
  return command;
  
}
/*
void sendCommand()
input: string representation of the command
processing: sends passed command to ssc32
*/
void Hexapod::sendCommand(String command){
  Serial.println(command);
}
/*
String moveLeg()
input: String representation of leg (lfl,lml,lrl,rfl,rml,rrl),alpha-number of degrees to move top servo, betta=number of degrees to move middle servo, 
gamma-number of degrees to move bottom servo. bool that represents whether we want the command to be executed.
processing: Adjusts degree numbers based on mechanical offsets in the robots to make moveements similar. 
decides which servos id's to move based on the leg name
translates all degrees from relative to servo to absolute, translates degrees to impulse
creates command to move the leg
returns command to move the leg if bool is true also executes command
*/
String Hexapod::moveLeg(String leg,int alpha, int betta, int gamma,bool exec){
   int x,y,z;
   betta=betta-40;
   // part of code for adjustement of posture.  positive is up/forward negative is down/backward
   alpha=alpha;
   betta=betta;
   gamma=gamma;
   
  if (leg=="lfl"){
    x=24;
    y=25;
    z=26;
    alpha=alpha-10;
    alpha=-alpha;
    betta=-betta+10;
    gamma+=10;
   
  }
  else if (leg=="lml"){
    x=20;
    y=21;
    z=22;
    alpha=alpha-10;
    alpha=-alpha;
   betta=-betta-35;
   
  }
  else if (leg=="lrl"){
    x=16;
    y=17;
    z=18;
    alpha=alpha+10;
    alpha=-alpha;
    betta=-betta+10;
    gamma+=10;
    
   
  }
  else if (leg=="rfl"){
    x=8 ;
    y=9;
    z=10;
    alpha=alpha-10;
    gamma=gamma-10;
    gamma=-gamma;
  }
  else if (leg=="rml"){
    x=4;
    y=5;
    z=6;
    alpha=alpha-10;
      betta=betta-5;
      gamma=-gamma;
  } else {
    x=0;
    y=1;
    z=2;
    gamma=-gamma;
  }
  alpha=degreesToImpulse(alpha);
   betta=degreesToImpulse(betta);
   gamma=degreesToImpulse(gamma);
    String command=getCommandToMoveServo(x,alpha);
    command=command+getCommandToMoveServo(y,betta);
    command=command+getCommandToMoveServo(z,gamma);
    if (exec==true){
      sendCommand(command);
    }
    return command;
}
/*
int degreesToImpulse()
input: int representing degrees
processing: calls relativeToServo()
transfers that value to impulse
output: impulse number*/
int Hexapod::degreesToImpulse(int x){
  x=relativeToServo(x); 
  int d=x*11.115+500;
  return d;
}
/*
int relativeToServo()
Input: number of degrees
processing :transfers degrees from relative to posture to relative to servo
output returns number of degrees relatie to servo
*/
int Hexapod::relativeToServo(int x){
  return x+90;
}
/*
void defaultPositionOne()
input: none
processing: move each leg, tail and head to the default posture by calling moveLeg() mcontrolTail() controlHead()
output: none*/

void Hexapod::defaultPositionOne(){
  String com=moveLeg("lfl",0,0,0,false);
  com+=moveLeg("lml",0,0,0,false);
  com+=moveLeg("lrl",0,0,0,false);
  com+=moveLeg("rfl",0,0,0,false);
  com+=moveLeg("rml",0,0,0,false);
  com+=moveLeg("rrl",0,0,0,false);
  com+=controlTail("hor",0,false);
  com+=controlTail("ver",20,false);
  com+=controlHead("hor",0,false);
  com+=controlHead("ver",0,false);
  com+="T2000";
  sendCommand(com);

  
}
/*void moveOneLegForward()
input: leg  id
processing: moves leg up, forward, down and than backward imitating a step()
output: none
*/
void Hexapod::moveOneLegForward(String leg){
    moveLeg(leg,0,40,0,false);
    delay(500);
    moveLeg(leg,-20,40,0,false);
    delay(500);
    moveLeg(leg,-20,0,0,false);
    delay(500);
    moveLeg(leg,-0,0,0,false);
    
    
}
/*voidMoveTripod
input:String representing the main side of tripod l or r. if l, legs involved are lfl,lrl,rml otherwise rfl,rrl,lml
processing: determines legs  in tripod, moves all three legs at the same time upward, forward down to the ground. these steps are performed 
one after another
Than slowly brings leg backwards, this action is spread over longer period of time, so that it starts when opposite tripod is performing actions above
it reducea the friction and lets move faster
output none*/
void Hexapod::moveTripod(String side){
  
  String s[3];
  if (side=="r"){
    s[0]="rfl";
    s[1]="rrl";
    s[2]="lml";
  } else {
    s[0]="lfl";
    s[1]="lrl";
    s[2]="rml";
  }
  String com="";
  for(int i=0;i<3;i++){
      com+=moveLeg(s[i],0,30,0,false);
    }
    
    com+="T300";
    sendCommand(com);
    if(side=="l"){
      com=controlTail("hor",-25,false);
      com+=controlTail("ver",20,false);
    } else {
      com=controlTail("hor",25,false);
      com+=controlTail("ver",20,false);
    }
    com+="T700";
    sendCommand(com);
    delay(300);
    com="";
     for(int i=0;i<3;i++){
      com+=moveLeg(s[i],25,30,10,false);
    }
    com+="T300";
    sendCommand(com);
    delay(300);
    com="";
     for(int i=0;i<3;i++){
      com+=moveLeg(s[i],25,0,-15,false);
    }
    com+="T300";
    sendCommand(com);
    delay(300);
    
    com="";
     for(int i=0;i<3;i++){
      com+=moveLeg(s[i],0,0,0,false);
     }
     com+="T1000"; 
    sendCommand(com);
 
}
/*
String controlTail()
input: direction user desires to control, number of degrees to move it, whether to execute the command
processing: based on the direction decides which servos to move
translates degrees from relative to psoture to relative to servo,than to impulse, than builds command if exec is true, sends command
output: command generated to move servo*/
String Hexapod::controlTail(String servo,int degr, bool exec){
  int x;
  if (servo=="hor"){
    x=30;
  } else {
    x=31;
  }
  degr=degreesToImpulse(degr);
  String command=getCommandToMoveServo(x,degr);
  if (exec){
    sendCommand(command);
  }
  return command;
}
/*
void walkForward()
input: number of steps to walk forward
processing: one full step is considered left tripod movement+right tripod movement. Moves left tripod, than right desired nummber of times
at the end puts hexapod to the default position
output: none */
void Hexapod::walkForward(int stepNumber){
  for(int i=0;i<stepNumber;i++){
    moveTripod("l");
    
    moveTripod("r");
  }
  defaultPositionOne();
}
/*voidMoveSide()
input:String representing the main  tripod a  or b to be moved. if a, legs involved are lfl,lrl,rml otherwise rfl,rrl,lml also gets direction to be moved to
processing: determines legs  in tripod, moves all three legs at the same time upward, outward down in the direction of the side to the ground. these steps are performed 
one after another
Than slowly brings leg inwardss, this action is spread over longer period of time, so that it starts when opposite tripod is performing actions above
it reducea the friction and lets move faster
output none*/
void Hexapod::moveToSide(String stage,String side){
  
  String s[3];
  if (stage=="a"){
    s[0]="rfl";
    s[1]="rrl";
    s[2]="lml";
  } else {
    s[0]="lfl";
    s[1]="lrl";
    s[2]="rml";
  } 

  
  String com="";
  for(int i=0;i<3;i++){
      com+=moveLeg(s[i],0,15,0,false);
    }
    com+="T400";
    sendCommand(com);
    delay(400);
    com="";
     
   for(int i=0;i<3;i++){
    if (s[i][0]==side[0]){
      com+=moveLeg(s[i],0,0,35,false);
    } else{
      com+=moveLeg(s[i],0,15,-35,false);
    }
    }
    com+="T400";
    sendCommand(com);
    delay(400);
    
    com="";
     for(int i=0;i<3;i++){
      com+=moveLeg(s[i],0,0,0,false);
     }
     com+="T700"; 
    sendCommand(com);
    
    
}
/*
void walkForward()
input: number of steps to walk forward
processing: one full step is considered left tripod movement+right tripod movement. Moves left tripod, than right desired nummber of times
at the end puts hexapod to the default position
output: none */


void Hexapod::walkSideways(int stepNumber){
   for(int i=0;i<stepNumber;i++){
    moveToSide("a","l");
    moveToSide("b","l");
  }
  
}
/*
String ControlHead()
input: direction to move head(up/down or sideways),number of degrees and whether to execute command
processing: determines servos based on direction, does translation for degrees, generates command to move servos, executes if bool is true, retursns command
Output:*/
String Hexapod::controlHead(String dir,int degr, bool exec){
  String com="";
  int x;
  if (dir=="hor"){
    x=12;
  } else {
    x=14;
  }
  com+=getCommandToMoveServo(x,degreesToImpulse(0));
  if (exec){
    com+="T100";
    sendCommand(com);
  }
  return com;
  
}
