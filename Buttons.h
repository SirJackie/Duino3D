#ifndef BUTTONS_H_
#define BUTTONS_H_

/* Physical Buttons */

#define BTN_UP             6
#define BTN_LEFT           7
#define BTN_RIGHT          8
#define BTN_DOWN           9

#define BTN_A             10
#define BTN_B             11

/* Virtual Buttons */
#define BTN_NONE           0

#define BTN_LEFTUP        13
#define BTN_RIGHTUP       14
#define BTN_LEFTDOWN      16
#define BTN_RIGHTDOWN     17

//About BTN_A
#define BTN_APRESS        30

#define BTN_ROT_UP        36
#define BTN_ROT_LEFT      37
#define BTN_ROT_RIGHT     38
#define BTN_ROT_DOWN      39
#define BTN_ROT_LEFTUP    43
#define BTN_ROT_RIGHTUP   44
#define BTN_ROT_LEFTDOWN  46
#define BTN_ROT_RIGHTDOWN 47

//About BTN_B
#define BTN_BPRESS        60

#define BTN_ADV_UP        66
#define BTN_ADV_LEFT      67
#define BTN_ADV_RIGHT     68
#define BTN_ADV_DOWN      69
#define BTN_ADV_LEFTUP    73
#define BTN_ADV_RIGHTUP   74
#define BTN_ADV_LEFTDOWN  76
#define BTN_ADV_RIGHTDOWN 77

void initButtons(){
  pinMode(BTN_UP,    INPUT_PULLUP);
  pinMode(BTN_DOWN,  INPUT_PULLUP);
  pinMode(BTN_LEFT,  INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_A,     INPUT_PULLUP);
  pinMode(BTN_B,     INPUT_PULLUP);
}

char getButtonState(){
  
  char state = BTN_NONE;

  if(digitalRead(BTN_UP)    == 0) //BTN_UP    has been pressed
  {
    state += BTN_UP;
  }
  if(digitalRead(BTN_LEFT)  == 0) //BTN_LEFT  has been pressed
  {
    state += BTN_LEFT;
  }
  if(digitalRead(BTN_RIGHT) == 0) //BTN_RIGHT has been pressed
  {
    state += BTN_RIGHT;
  }
  if(digitalRead(BTN_DOWN)  == 0) //BTN_DOWN  has been pressed
  {
    state += BTN_DOWN;
  }

  if(digitalRead(BTN_A)     == 0) //BTN_A has been pressed
  {
    state += 30;
  }
  if(digitalRead(BTN_B)     == 0) //BTN_B has been pressed
  {
    state += 60;
  }
  return state;
}

#endif
