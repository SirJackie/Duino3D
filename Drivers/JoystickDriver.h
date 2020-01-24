/*
 * Joystick Driver for Duino3D
 * Support Joystick Mode: 5-Pin Risistor Joystick
 * If you use the diffrent joystick module, please write this driver by yourself.
 */

#define Joystick_X_Pin A1
#define Joystick_Y_Pin A0
#define Joystick_X_Reverse 0
#define Joystick_Y_Reverse 1
int Joystick_Global_X_Tmp;
int Joystick_Global_Y_Tmp;

short getJoystickXState(){
  Joystick_Global_X_Tmp = analogRead(Joystick_X_Pin);
  if(Joystick_X_Reverse){
    if(Joystick_Global_X_Tmp < 400){
      return 1; //right
    }
    if(Joystick_Global_X_Tmp > 700){
      return -1; //left
    }
    return 0; //middle
  }
  else{
    if(Joystick_Global_X_Tmp < 400){
      return -1; //left
    }
    if(Joystick_Global_X_Tmp > 700){
      return 1; //right
    }
    return 0; //middle
  }
}

short getJoystickYState(){
  Joystick_Global_Y_Tmp = analogRead(Joystick_Y_Pin);
  if(Joystick_Y_Reverse){
    if(Joystick_Global_Y_Tmp < 400){
      return 1; //right
    }
    if(Joystick_Global_Y_Tmp > 700){
      return -1; //left
    }
    return 0; //middle
  }
  else{
    if(Joystick_Global_Y_Tmp < 400){
      return -1; //left
    }
    if(Joystick_Global_Y_Tmp > 700){
      return 1; //right
    }
    return 0; //middle
  }
}
