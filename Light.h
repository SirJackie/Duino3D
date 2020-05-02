#ifndef LIGHT_LIGHT_H_
#define LIGHT_LIGHT_H_

struct RGBLight{
  int  i, rPin, gPin, bPin;
  bool addingState;
};

void initRGB(struct RGBLight* rgblight, int rPin, int gPin, int bPin){
  rgblight->rPin        = rPin;
  rgblight->gPin        = gPin;
  rgblight->bPin        = bPin;
  rgblight->i           = 0;
  rgblight->addingState = true;
  
  pinMode(rgblight->rPin, OUTPUT);
  pinMode(rgblight->gPin, OUTPUT);
  pinMode(rgblight->bPin, OUTPUT);
}

void updateRGB(struct RGBLight* rgblight){

  //Update the state of light
  analogWrite(rgblight->rPin, rgblight->i        );
  analogWrite(rgblight->gPin, 30-rgblight->i     );
  analogWrite(rgblight->bPin, abs(15-rgblight->i));

  //Calculate next state
  if(rgblight->i >= 30 && rgblight->addingState == true){
    rgblight->addingState = false;
  }
  else if(rgblight->i <= 0 && rgblight->addingState == false){
    rgblight->addingState = true;
  }
  else if(rgblight->addingState == true){
    rgblight->i += 1;
  }
  else if(rgblight->addingState == false){
    rgblight->i -= 1;
  }
}

#endif
