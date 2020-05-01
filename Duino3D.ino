
/* External Libraries Including */
#include <U8g2lib.h>
#include <avr/pgmspace.h>
#include "Buttons.h"
#include "MemoryFree.h"


#define cosd(x) cos(x*0.017453293)
#define sind(x) sin(x*0.017453293)
#define tand(x) tan(x*0.017453293)
#define acosd(x) acos(x*0.017453293)
#define asind(x) asin(x*0.017453293)
#define atand(x) atan(x*0.017453293)

#define V4DLIST_LEN  108
#define V4DLIST_MAX  300
#define V2DLIST_MAX  32

const int V4DList[108] PROGMEM = {
  1,1,1, 4,1,1, 1,4,1, //front
  1,4,1, 4,4,1, 4,1,1, //front
  1,4,4, 1,1,4, 1,1,1, //left
  1,4,4, 1,4,1, 1,1,1, //left
  4,4,4, 4,1,4, 4,1,1, //right
  4,4,4, 4,4,1, 4,1,1, //right
  1,1,4, 4,1,4, 1,4,4, //back
  1,4,4, 4,4,4, 4,1,4, //back
  1,4,1, 1,4,4, 4,4,4, //up
  1,4,1, 4,4,1, 4,4,4, //up
  1,1,1, 1,1,4, 4,1,4, //down
  1,1,1, 4,1,1, 4,1,4  //down
};


U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 16, /* data=*/ 17);

/*
** Calculation Structs
*/

struct Matrix4X4{
  int m00;
  int m01;
  int m02;
  int m03;

  int m10;
  int m11;
  int m12;
  int m13;

  int m20;
  int m21;
  int m22;
  int m23;

  int m30;
  int m31;
  int m32;
  int m33;
};

void initM44(struct Matrix4X4* m44,
             int a00, int a01, int a02, int a03,
             int a10, int a11, int a12, int a13,
             int a20, int a21, int a22, int a23,
             int a30, int a31, int a32, int a33)
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
  int m00;
  int m01;
  int m02;
  int m03;
};

void initM41(struct Matrix4X1* m41,
             int a00, int a01, int a02, int a03)
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
  int    x;
  int    y;
  int    z;
  int    angleX;
  int    angleY;
  int    angleZ;
  int    screenWidth;
  int    screenHeight;
  struct Matrix4X4 rotationMatrix;
};


void initCamera(struct Camera* cam, int positionX, int positionY, int positionZ, int angleX, int angleY, int angleZ, int screenWidth, int screenHeight){
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
  int zoom  = 1;
  int zfix  = 0.01;
  initM41(&tmpCameraPositionMatrix,
          v4dx - camera->x, v4dy - camera->y, v4dz - camera->z, 1);
  M41timesM44(&tmpCameraPositionMatrix, &(camera->rotationMatrix), &final);
  int vecx  = final.m00;
  int vecy  = final.m01;
  int vecz  = final.m02;

  int x2d = (final.m00) / (final.m02*zfix);
  int y2d = (final.m01) / (final.m02*zfix);

  *v2dx = (camera->screenWidth  / 2) + (x2d * zoom);
  *v2dy = (camera->screenHeight / 2) + (y2d * zoom);

}

struct Camera cam1;
char V2DList[V2DLIST_MAX];
int V2DList_next = 0;

bool V4DListAvailable(int* V4DList_next)
{
  if((*(V4DList_next) + 9) < V4DLIST_MAX)
  {
    return true;
  }
  else{
    Serial.println("V4DList Overflowed!!!");
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(0,15,"V4DList Overflowed!!!");
    } while ( u8g2.nextPage() );
    delay(500);
    return false;
  }
}

//void pushMesh4D(char** V4DList, int* V4DList_next, char v1x, char v1y, char v1z, char v2x, char v2y, char v2z, char v3x, char v3y, char v3z)
//{
//  if(!V4DListAvailable(V4DList_next)){
//    return;
//  }
//  (*V4DList)[*V4DList_next] = v1x;
//  (*V4DList_next)++;
//  (*V4DList)[*V4DList_next] = v1y;
//  (*V4DList_next)++;
//  (*V4DList)[*V4DList_next] = v1z;
//  (*V4DList_next)++;
//
//  (*V4DList)[*V4DList_next] = v2x;
//  (*V4DList_next)++;
//  (*V4DList)[*V4DList_next] = v2y;
//  (*V4DList_next)++;
//  (*V4DList)[*V4DList_next] = v2z;
//  (*V4DList_next)++;
//
//  (*V4DList)[*V4DList_next] = v3x;
//  (*V4DList_next)++;
//  (*V4DList)[*V4DList_next] = v3y;
//  (*V4DList_next)++;
//  (*V4DList)[*V4DList_next] = v3z;
//  (*V4DList_next)++;
//}

void setup() {
  Serial.begin(9600);
  
  //Initialize the SSD1306 screen
  u8g2.begin();

  //initialize the camera
  initCamera(&cam1, 0, 0, 0, 0, 0, 0, 128, 64);

  String tmp = "";
  tmp += V4DLIST_MAX;
  
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,15,tmp.c_str());
  } while ( u8g2.nextPage() );
  delay(500);

  for(int i = 0; i < V4DLIST_LEN; i++){
    Serial.println(i);
    Serial.println(pgm_read_word(V4DList + i));
  }

  delay(500);
}

//bool firstFrame = true;
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  short XState = getJoystickXState();
//  short YState = getJoystickYState();
//  if(XState == 0 && YState == 0 && firstFrame == false){
//    return;
//  }
//  if(XState == -1){
//    cam1->x -= 1;
//  }
//  else if(XState == 1){
//    cam1->x += 1;
//  }
//  if(YState == 1){
//    cam1->z -= 1;
//  }
//  else if(YState == -1){
//    cam1->z += 1;
//  }
//
//  cam1->refreshRotationMatrix();
//  Mesh2D** meshlist2d = new Mesh2D* [listlen];
//  for(int i = 0; i < listlen; i++){
//    meshlist2d[i] = Mesh4D2Mesh2D(cam1, meshlist[i]);
//  }
//  
//  //Draw Meshes
//  u8g2.firstPage();
//  do {
//    for(int i = 0; i < listlen; i++){
//      CanvasDrawMesh2D(meshlist2d[i]);
//    }
//  } while ( u8g2.nextPage() );
//
//  for(int i = 0; i < listlen; i++){
//    delete meshlist2d[i];
//  }
//  delete meshlist2d;
//  firstFrame = false;
//}

//void setup(){
//  Serial.begin(9600);
//  struct Matrix4X4 m44_1, m44_2, m44_result;
//  initM44(&m44_1,
//          1, 0, 0, 0,
//          0, 1, 0, 0,
//          0, 0, 1, 0,
//          0, 0, 0, 1);
//  initM44(&m44_2,
//          1, 0, 0, 0,
//          0, 1, 0, 0,
//          0, 0, 1, 0,
//          0, 0, 0, 1);
//  M44timesM44(&m44_1, &m44_2, &m44_result);
//  showM44(&m44_result);
//}

void loop(){
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,15,"Loop");
  } while ( u8g2.nextPage() );
}
