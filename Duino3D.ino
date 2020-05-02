/* External Libraries Including */
#include <U8g2lib.h>
#include <avr/pgmspace.h>
#include "Light.h"
#include "Joystick.h"
#include "LineClip.h"
#include "MemoryFree.h"

#define pgm_read_float_near(address_short) __LPM_float((uint16_t)(address_short))

#define cosd(x) cos(x*0.017453293)
#define sind(x) sin(x*0.017453293)
#define tand(x) tan(x*0.017453293)
#define acosd(x) acos(x*0.017453293)
#define asind(x) asin(x*0.017453293)
#define atand(x) atan(x*0.017453293)

#define V4DLIST_LEN  36
#define V4DLIST_MAX  300
#define V2DLIST_MAX  32

/* Joystick Pins */

#define JOY_X_A           A3
#define JOY_Y_A           A2
#define JOY_BTN_A         11

#define JOY_X_B           A1
#define JOY_Y_B           A0
#define JOY_BTN_B         10

const float V4DList[V4DLIST_LEN * 3] PROGMEM = {
  1.0, 1.0, 1.0,  4.0, 1.0, 1.0,  1.0, 4.0, 1.0,  //front
  1.0, 4.0, 1.0,  4.0, 4.0, 1.0,  4.0, 1.0, 1.0,  //front
  1.0, 4.0, 4.0,  1.0, 1.0, 4.0,  1.0, 1.0, 1.0,  //left
  1.0, 4.0, 4.0,  1.0, 4.0, 1.0,  1.0, 1.0, 1.0,  //left
  4.0, 4.0, 4.0,  4.0, 1.0, 4.0,  4.0, 1.0, 1.0,  //right
  4.0, 4.0, 4.0,  4.0, 4.0, 1.0,  4.0, 1.0, 1.0,  //right
  1.0, 1.0, 4.0,  4.0, 1.0, 4.0,  1.0, 4.0, 4.0,  //back
  1.0, 4.0, 4.0,  4.0, 4.0, 4.0,  4.0, 1.0, 4.0,  //back
  1.0, 4.0, 1.0,  1.0, 4.0, 4.0,  4.0, 4.0, 4.0,  //up
  1.0, 4.0, 1.0,  4.0, 4.0, 1.0,  4.0, 4.0, 4.0,  //up
  1.0, 1.0, 1.0,  1.0, 1.0, 4.0,  4.0, 1.0, 4.0,  //down
  1.0, 1.0, 1.0,  4.0, 1.0, 1.0,  4.0, 1.0, 4.0  //down
};

/*
** Calculation Structs
*/

struct Matrix4X4{
  float m00;
  float m01;
  float m02;
  float m03;

  float m10;
  float m11;
  float m12;
  float m13;

  float m20;
  float m21;
  float m22;
  float m23;

  float m30;
  float m31;
  float m32;
  float m33;
};

void initM44(struct Matrix4X4* m44,
             float a00, float a01, float a02, float a03,
             float a10, float a11, float a12, float a13,
             float a20, float a21, float a22, float a23,
             float a30, float a31, float a32, float a33)
{
  m44->m00 = a00;
  m44->m01 = a01;
  m44->m02 = a02;
  m44->m03 = a03;
  
  m44->m10 = a10;
  m44->m11 = a11;
  m44->m12 = a12;
  m44->m13 = a13;
  
  m44->m20 = a20;
  m44->m21 = a21;
  m44->m22 = a22;
  m44->m23 = a23;
  
  m44->m30 = a30;
  m44->m31 = a31;
  m44->m32 = a32;
  m44->m33 = a33;
}

struct Matrix4X1{
  float m00;
  float m01;
  float m02;
  float m03;
};

void initM41(struct Matrix4X1* m41,
             float a00, float a01, float a02, float a03)
{
  m41->m00 = a00;
  m41->m01 = a01;
  m41->m02 = a02;
  m41->m03 = a03;
}

/*
** Calculation Functions
*/

void M44timesM44(struct Matrix4X4* a, struct Matrix4X4* b, struct Matrix4X4* result){
  result->m00 = (a->m00 * b->m00) + (a->m01*b->m10) + (a->m02*b->m20) + (a->m03*b->m30);
  result->m01 = (a->m00 * b->m01) + (a->m01*b->m11) + (a->m02*b->m21) + (a->m03*b->m31);
  result->m02 = (a->m00 * b->m02) + (a->m01*b->m12) + (a->m02*b->m22) + (a->m03*b->m32);
  result->m03 = (a->m00 * b->m03) + (a->m01*b->m13) + (a->m02*b->m23) + (a->m03*b->m33);
           
  result->m10 = (a->m10 * b->m00) + (a->m11*b->m10) + (a->m12*b->m20) + (a->m13*b->m30);
  result->m11 = (a->m10 * b->m01) + (a->m11*b->m11) + (a->m12*b->m21) + (a->m13*b->m31);
  result->m12 = (a->m10 * b->m02) + (a->m11*b->m12) + (a->m12*b->m22) + (a->m13*b->m32);
  result->m13 = (a->m10 * b->m03) + (a->m11*b->m13) + (a->m12*b->m23) + (a->m13*b->m33);
           
  result->m20 = (a->m20 * b->m00) + (a->m21*b->m10) + (a->m22*b->m20) + (a->m23*b->m30);
  result->m21 = (a->m20 * b->m01) + (a->m21*b->m11) + (a->m22*b->m21) + (a->m23*b->m31);
  result->m22 = (a->m20 * b->m02) + (a->m21*b->m12) + (a->m22*b->m22) + (a->m23*b->m32);
  result->m23 = (a->m20 * b->m03) + (a->m21*b->m13) + (a->m22*b->m23) + (a->m23*b->m33);
           
  result->m30 = (a->m30 * b->m00) + (a->m31*b->m10) + (a->m32*b->m20) + (a->m33*b->m30);
  result->m31 = (a->m30 * b->m01) + (a->m31*b->m11) + (a->m32*b->m21) + (a->m33*b->m31);
  result->m32 = (a->m30 * b->m02) + (a->m31*b->m12) + (a->m32*b->m22) + (a->m33*b->m32);
  result->m33 = (a->m30 * b->m03) + (a->m31*b->m13) + (a->m32*b->m23) + (a->m33*b->m33);
}

void M41timesM44(Matrix4X1* a, Matrix4X4* b, Matrix4X1* result){
  result->m00 = (a->m00 * b->m00) + (a->m01*b->m10) + (a->m02*b->m20) + (a->m03*b->m30);
  result->m01 = (a->m00 * b->m01) + (a->m01*b->m11) + (a->m02*b->m21) + (a->m03*b->m31);
  result->m02 = (a->m00 * b->m02) + (a->m01*b->m12) + (a->m02*b->m22) + (a->m03*b->m32);
  result->m03 = (a->m00 * b->m03) + (a->m01*b->m13) + (a->m02*b->m23) + (a->m03*b->m33);
}

void showM44(Matrix4X4* m){
  Serial.println("[");
  Serial.print(m->m00);
  Serial.print(",");
  Serial.print(m->m01);
  Serial.print(",");
  Serial.print(m->m02);
  Serial.print(",");
  Serial.print(m->m03);
  Serial.print(",\n");

  Serial.print(m->m10);
  Serial.print(",");
  Serial.print(m->m11);
  Serial.print(",");
  Serial.print(m->m12);
  Serial.print(",");
  Serial.print(m->m13);
  Serial.print(",\n");

  Serial.print(m->m20);
  Serial.print(",");
  Serial.print(m->m21);
  Serial.print(",");
  Serial.print(m->m22);
  Serial.print(",");
  Serial.print(m->m23);
  Serial.print(",\n");

  Serial.print(m->m30);
  Serial.print(",");
  Serial.print(m->m31);
  Serial.print(",");
  Serial.print(m->m32);
  Serial.print(",");
  Serial.print(m->m33);
  Serial.print(",\n");
  Serial.println("]");
  Serial.print("\n");
}

void showM41(Matrix4X1* m){
  Serial.print("[");
  Serial.print(m->m00);
  Serial.print(",");
  Serial.print(m->m01);
  Serial.print(",");
  Serial.print(m->m02);
  Serial.print(",");
  Serial.print(m->m03);
  Serial.print("]\n");
}

/*
** Structs
*/

struct Matrix4X4 ZRotationMatrix, YRotationMatrix, XRotationMatrix, ZYRotationMatrix;

struct Camera{
  float    x;
  float    y;
  float    z;
  float    angleX;
  float    angleY;
  float    angleZ;
  int    screenWidth;
  int    screenHeight;
  struct Matrix4X4 rotationMatrix;
};


void initCamera(struct Camera* cam, float positionX, float positionY, float positionZ, float angleX, float angleY, float angleZ, int screenWidth, int screenHeight){
  cam->x              = positionX;
  cam->y              = positionY;
  cam->z              = positionZ;
  cam->angleX         = angleX;
  cam->angleY         = angleY;
  cam->angleZ         = angleZ;
  cam->screenWidth    = screenWidth;
  cam->screenHeight   = screenHeight;
}

void refreshRotationMatrix(struct Camera* cam){
  initM44(&ZRotationMatrix,
          cosd(cam->angleZ),       sind(cam->angleZ),     0,                        0,
          -1*sind(cam->angleZ),    cosd(cam->angleZ),     0,                        0,
          0,                       0,                     1,                        0,
          0,                       0,                     0,                        1);
  
  initM44(&YRotationMatrix,
          cosd(cam->angleY),       0,                     -1*sind(cam->angleY),     0,
          0,                       1,                     0,                        0,
          sind(cam->angleY),       0,                     cosd(cam->angleY),        0,
          0,                       0,                     0,                        1);
  
  initM44(&XRotationMatrix,
          1,                       0,                     0,                        0,
          0,                       cosd(cam->angleX),     sind(cam->angleX),        0,
          0,                       -1*sind(cam->angleX),  cosd(cam->angleX),        0,
          0,                       0,                     0,                        1);

  M44timesM44(&ZRotationMatrix,  &YRotationMatrix, &ZYRotationMatrix);
  M44timesM44(&ZYRotationMatrix, &XRotationMatrix, &(cam->rotationMatrix));
}

/*
** Projection Functions
*/

struct Matrix4X1 tmpCameraPositionMatrix, final;

void Vector4D2Vector2D(struct Camera* camera, int v4dx, int v4dy, int v4dz, int* v2dx, int* v2dy){
  float zoom  = 0.15;
  float zfix  = 0.003;
  initM41(&tmpCameraPositionMatrix,
          v4dx - camera->x, v4dy - camera->y, v4dz - camera->z, 1);
  
  M41timesM44(&tmpCameraPositionMatrix, &(camera->rotationMatrix), &final);

  float x2d = (float) ((float)(final.m00)) / ((float)final.m02*(float)zfix);
  float y2d = (float) ((float)(final.m01)) / ((float)final.m02*(float)zfix);

  *v2dx = (int)(   ((float)camera->screenWidth  / (float)2) + ((float)x2d * (float)zoom)   );
  *v2dy = (int)(   ((float)camera->screenHeight / (float)2) + ((float)y2d * (float)zoom)   );
  Serial.println(*v2dx);
}



/* Structure about hardware */
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 16, /* data=*/ 17);
inline void DrawLineCallback(int* x0, int* y0, int* x1, int* y1){
  u8g2.drawLine(*x0, *y0, *x1, *y1);
}
struct RGBLight rgblight;
struct Joystick joystickA, joystickB;
Rectangle rect;

/* Structure about 3D processing */
struct Camera cam1;
unsigned int V2DList[V4DLIST_LEN * 2];
int  V2DList_next = 0;


void setup() {
  //Initialize
  Serial.begin(9600);
  u8g2.begin();
  initRGB(&rgblight, 11, 10, 9);
  initJoystick(&joystickA, JOY_X_A, JOY_Y_A, JOY_BTN_A, true, false);
  initJoystick(&joystickB, JOY_X_B, JOY_Y_B, JOY_BTN_B, true, false);
  initRectangle(&rect, 0, 0, 128, 64);
  initCamera(&cam1, 2, 2, -1, 0, 0, 0, 128, 64);
}


char joytmp;
void loop(){
  
  joytmp = getJoystickState(&joystickA);
  if(joytmp == JOY_UP){
    cam1.z += 0.4;
  }
  else if(joytmp == JOY_DOWN){
    cam1.z -= 0.4;
  }
  else if(joytmp == JOY_LEFT){
    cam1.x -= 0.4;
  }
  else if(joytmp == JOY_RIGHT){
    cam1.x += 0.4;
  }
  else if(joytmp == JOY_LEFTUP){
    cam1.x -= 0.4;
    cam1.z += 0.4;
  }
  else if(joytmp == JOY_RIGHTUP){
    cam1.x += 0.4;
    cam1.z += 0.4;
  }
  else if(joytmp == JOY_LEFTDOWN){
    cam1.x -= 0.4;
    cam1.z -= 0.4;
  }
  else if(joytmp == JOY_RIGHTDOWN){
    cam1.x += 0.4;
    cam1.z -= 0.4;
  }
  

  joytmp = getJoystickState(&joystickB);
  if(joytmp == JOY_UP){
    cam1.angleX -= 4;
  }
  else if(joytmp == JOY_DOWN){
    cam1.angleX += 4;
  }
  else if(joytmp == JOY_LEFT){
    cam1.angleY += 4;
  }
  else if(joytmp == JOY_RIGHT){
    cam1.angleY -= 4;
  }
  else if(joytmp == JOY_LEFTUP){
    cam1.angleY += 4;
    cam1.angleX -= 4;
  }
  else if(joytmp == JOY_RIGHTUP){
    cam1.angleY -= 4;
    cam1.angleX -= 4;
  }
  else if(joytmp == JOY_LEFTDOWN){
    cam1.angleY += 4;
    cam1.angleX += 4;
  }
  else if(joytmp == JOY_RIGHTDOWN){
    cam1.angleY -= 4;
    cam1.angleX += 4;
  }
  

  refreshRotationMatrix(&cam1);
  for(int i = 0; i < V4DLIST_LEN; i++){
    Vector4D2Vector2D(&cam1,
                      pgm_read_float_near(V4DList + (i*3+0)), pgm_read_float_near(V4DList + (i*3+1)), pgm_read_float_near(V4DList + (i*3+2)),
                      &(V2DList[i*2+0]), &(V2DList[i*2+1])
                     );
  }
  u8g2.firstPage();
  do {
    for(int i = 0; i < V4DLIST_LEN / 3; i++){
      LineClip(DrawLineCallback, &rect, (int)(V2DList[(i*3+0)*2+0]), (int)(V2DList[(i*3+0)*2+1]), (int)(V2DList[(i*3+1)*2+0]), (int)(V2DList[(i*3+1)*2+1]));
      LineClip(DrawLineCallback, &rect, (int)(V2DList[(i*3+1)*2+0]), (int)(V2DList[(i*3+1)*2+1]), (int)(V2DList[(i*3+2)*2+0]), (int)(V2DList[(i*3+2)*2+1]));
      LineClip(DrawLineCallback, &rect, (int)(V2DList[(i*3+2)*2+0]), (int)(V2DList[(i*3+2)*2+1]), (int)(V2DList[(i*3+0)*2+0]), (int)(V2DList[(i*3+0)*2+1]));

      LineClip(DrawLineCallback, &rect, (int)(V2DList[(i*3+0)*2+0]), (int)(V2DList[(i*3+0)*2+1]) + 1, (int)(V2DList[(i*3+1)*2+0]), (int)(V2DList[(i*3+1)*2+1]) + 1);
      LineClip(DrawLineCallback, &rect, (int)(V2DList[(i*3+1)*2+0]), (int)(V2DList[(i*3+1)*2+1]) + 1, (int)(V2DList[(i*3+2)*2+0]), (int)(V2DList[(i*3+2)*2+1]) + 1);
      LineClip(DrawLineCallback, &rect, (int)(V2DList[(i*3+2)*2+0]), (int)(V2DList[(i*3+2)*2+1]) + 1, (int)(V2DList[(i*3+0)*2+0]), (int)(V2DList[(i*3+0)*2+1]) + 1);
      
//      if(i == 2){
//        Serial.println((int)&(V2DList[(i*3+0)*2+0]));
//      }
    }
  } while ( u8g2.nextPage() );

  updateRGB(&rgblight);
  updateRGB(&rgblight);
  updateRGB(&rgblight);
}
