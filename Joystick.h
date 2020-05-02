#ifndef JOYSTICK_H_
#define JOYSTICK_H_

/* Joystick State */

#define JOY_NONE           0

#define JOY_UP             6
#define JOY_LEFT           7
#define JOY_RIGHT          8
#define JOY_DOWN           9

#define JOY_LEFTUP        13
#define JOY_RIGHTUP       14
#define JOY_LEFTDOWN      16
#define JOY_RIGHTDOWN     17


struct Joystick{
  int  JoystickX, JoystickY, JoystickBTN, Joystick;
  bool JoystickXInverse, JoystickYInverse;
};

void initJoystick(struct Joystick* joystick, int JoystickX, int JoystickY, int JoystickBTN, bool JoystickXInverse, bool JoystickYInverse){
  
  joystick->JoystickX        = JoystickX;
  joystick->JoystickY        = JoystickY;
  joystick->JoystickBTN      = JoystickBTN;
  joystick->JoystickXInverse = JoystickXInverse;
  joystick->JoystickYInverse = JoystickYInverse;
  
  pinMode(joystick->JoystickX,   INPUT);
  pinMode(joystick->JoystickY,   INPUT);
  pinMode(joystick->JoystickBTN, INPUT);
}

char getJoystickState(struct Joystick* joystick){
  char state = 0;

  int JoystickXState = analogRead(joystick->JoystickX);
  int JoystickYState = analogRead(joystick->JoystickY);

  if(joystick->JoystickXInverse == false){
    if(JoystickXState <= 150){
      state += JOY_LEFT;
    }
    else if(JoystickXState >= 874){
      state += JOY_RIGHT;
    }
  }
  else{
    if(JoystickXState >= 874){
      state += JOY_LEFT;
    }
    else if(JoystickXState <= 150){
      state += JOY_RIGHT;
    }
  }

  if(joystick->JoystickYInverse == false){
    if(JoystickYState <= 150){
      state += JOY_UP;
    }
    else if(JoystickYState >= 874){
      state += JOY_DOWN;
    }
  }
  else{
    if(JoystickYState >= 874){
      state += JOY_UP;
    }
    else if(JoystickYState <= 150){
      state += JOY_DOWN;
    }
  }

  return state;
}

#endif
