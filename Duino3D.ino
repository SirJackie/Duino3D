
/* External Libraries Including */
#include <U8g2lib.h>
#include "Buttons.h"
#include "MemoryFree.h"


#define cosd(x) cos(x*0.017453293)
#define sind(x) sin(x*0.017453293)
#define tand(x) tan(x*0.017453293)
#define acosd(x) acos(x*0.017453293)
#define asind(x) asin(x*0.017453293)
#define atand(x) atan(x*0.017453293)


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
//
///*
//** Structs
//*/
//
//struct Camera{
//  int x;
//  int y;
//  int z;
//  int angleX;
//  int angleY;
//  int angleZ;
//  int screenWidth;
//  int screenHeight;
//  Matrix4X4* rotationMatrix;
//  
//  Camera(int positionX, int positionY, int positionZ, int angleX, int angleY, int angleZ, int screenWidth, int screenHeight){
//    this->x              = positionX;
//    this->y              = positionY;
//    this->z              = positionZ;
//    this->angleX         = angleX;
//    this->angleY         = angleY;
//    this->angleZ         = angleZ;
//    this->screenWidth    = screenWidth;
//    this->screenHeight   = screenHeight;
//  }
//  void refreshRotationMatrix(){
//    Matrix4X4* ZRotationMatrix = new Matrix4X4(cosd(this->angleZ),      sind(this->angleZ),     0,                        0,
//                                               -1*sind(this->angleZ),   cosd(this->angleZ),     0,                        0,
//                                               0,                        0,                       1,                        0,
//                                               0,                        0,                       0,                        1);
//    
//    Matrix4X4* YRotationMatrix = new Matrix4X4(cosd(this->angleY),      0,                       -1*sind(this->angleY),   0,
//                                               0,                        1,                       0,                        0,
//                                               sind(this->angleY),      0,                       cosd(this->angleY),      0,
//                                               0,                        0,                       0,                        1);
//    
//    Matrix4X4* XRotationMatrix = new Matrix4X4(1,                        0,                       0,                        0,
//                                               0,                        cosd(this->angleX),     sind(this->angleX),      0,
//                                               0,                        -1*sind(this->angleX),  cosd(this->angleX),      0,
//                                               0,                        0,                       0,                        1);
//    
//    delete this->rotationMatrix;
//    Matrix4X4* ZYMatrix  = Matrix4X4timesMatrix4X4(ZRotationMatrix, YRotationMatrix);
//    Matrix4X4* ZYXMatrix = Matrix4X4timesMatrix4X4(ZYMatrix, XRotationMatrix);
//    this->rotationMatrix = ZYXMatrix;
//    delete ZRotationMatrix;
//    delete YRotationMatrix;
//    delete XRotationMatrix;
//    delete ZYMatrix;
//  }
//};
//
//
//
//struct Vector2D{
//  int x;
//  int y;
//  Vector2D(int x, int y){
//    this->x = x;
//    this->y = y;
//  }
//};
//
//struct Mesh2D{
//  Vector2D* vec1;
//  Vector2D* vec2;
//  Vector2D* vec3;
//  Mesh2D(Vector2D* vec1, Vector2D* vec2, Vector2D* vec3){
//    this->vec1 = vec1;
//    this->vec2 = vec2;
//    this->vec3 = vec3;
//  }
//  ~Mesh2D(){
//    delete this->vec1;
//    delete this->vec2;
//    delete this->vec3;
//  }
//};
//
//struct Vector4D{
//  int x;
//  int y;
//  int z;
//  int w = 1;
//  Vector4D(int x,int y,int z){
//    this->x = x;
//    this->y = y;
//    this->z = z;
//  }
//};
//
//struct Mesh4D{
//  Vector4D* vec1;
//  Vector4D* vec2;
//  Vector4D* vec3;
//  Mesh4D(Vector4D* vec1, Vector4D* vec2, Vector4D* vec3){
//    this->vec1 = vec1;
//    this->vec2 = vec2;
//    this->vec3 = vec3;
//  }
//  ~Mesh4D(){
//    delete this->vec1;
//    delete this->vec2;
//    delete this->vec3;
//  }
//};
//
///*
//** Canvas Functions
//*/
//
//void CanvasDrawVector2D(struct Vector2D* vector){
//  LcdFill(vector->x-1, //Start X Position
//          vector->y-1, //Start Y Position
//          2,          //Width
//          2);         //Height
//}
//
//void CanvasDrawMesh2D(struct Mesh2D* mesh){
//  if(mesh->vec1->x == -1 || mesh->vec2->x == -1 || mesh->vec3->x == -1){
//    return; //don't show this vector
//  }
//  CanvasDrawVector2D(mesh->vec1);
//  CanvasDrawVector2D(mesh->vec2);
//  CanvasDrawVector2D(mesh->vec3);
//
//  LcdDrawLine(mesh->vec1->x, mesh->vec1->y, mesh->vec2->x, mesh->vec2->y);
//  LcdDrawLine(mesh->vec2->x, mesh->vec2->y, mesh->vec3->x, mesh->vec3->y);
//  LcdDrawLine(mesh->vec3->x, mesh->vec3->y, mesh->vec1->x, mesh->vec1->y);
//}
//
///*
//** Projection Functions
//*/
//
//Vector2D* Vector4D2Vector2D(struct Camera* camera,struct Vector4D* vector){
//  int zoom  = 1;
//  int zfix  = 0.01;
//  Matrix4X1* tmpCameraPositionMatrix = new Matrix4X1(vector->x - camera->x, vector->y - camera->y, vector->z - camera->z, 1);
//  Matrix4X1* final = Matrix4X1timesMatrix4X4(tmpCameraPositionMatrix, camera->rotationMatrix);
//  delete tmpCameraPositionMatrix;
//  int vecx  = final->m00;
//  int vecy  = final->m01;
//  int vecz  = final->m02;
//  delete final;
//
//  if(vecz <= 0){
//    return new Vector2D(-1,-1); //don't show this vector
//  } else {
//    int x2d = (vecx) / (vecz*zfix);
//    int y2d = (vecy) / (vecz*zfix);
//
//    int xScreen = (camera->screenWidth  / 2) + (x2d * zoom);
//    int yScreen = (camera->screenHeight / 2) + (y2d * zoom);
//    return new Vector2D(xScreen, yScreen);
//  }
//}
//
//Mesh2D* Mesh4D2Mesh2D(struct Camera* camera,struct Mesh4D* mesh4d){
//  return new Mesh2D(Vector4D2Vector2D(camera, mesh4d->vec1),
//                    Vector4D2Vector2D(camera, mesh4d->vec2),
//                    Vector4D2Vector2D(camera, mesh4d->vec3));
//}
//
//Camera* cam1;
//int listlen = 8;
//Mesh4D** meshlist = new Mesh4D* [listlen];
//
//void setup() {
//  // put your setup code here, to run once:
//  LcdInit();
//  cam1 = new Camera(0,0,0,0,0,0,128,64);
//  u8g2.firstPage();
//  do {
//    u8g2.setFont(u8g2_font_ncenB14_tr);
//    u8g2.drawStr(0,15,"Setup");
//  } while ( u8g2.nextPage() );
//  meshlist[0] = new Mesh4D(new Vector4D(1,1,1), new Vector4D(4,1,1), new Vector4D(1,4,1)); //front
//  meshlist[1] = new Mesh4D(new Vector4D(1,4,1), new Vector4D(4,4,1), new Vector4D(4,1,1)); //front
//  meshlist[2] = new Mesh4D(new Vector4D(1,4,4), new Vector4D(1,1,4), new Vector4D(1,1,1)); //left
//  meshlist[3] = new Mesh4D(new Vector4D(1,4,4), new Vector4D(1,4,1), new Vector4D(1,1,1)); //left
//  meshlist[4] = new Mesh4D(new Vector4D(4,4,4), new Vector4D(4,1,4), new Vector4D(4,1,1)); //right
//  meshlist[5] = new Mesh4D(new Vector4D(4,4,4), new Vector4D(4,4,1), new Vector4D(4,1,1)); //right
//  meshlist[6] = new Mesh4D(new Vector4D(1,1,4), new Vector4D(4,1,4), new Vector4D(1,4,4)); //back
//  meshlist[7] = new Mesh4D(new Vector4D(1,4,4), new Vector4D(4,4,4), new Vector4D(4,1,4)); //back
////  meshlist[8] = new Mesh4D(new Vector4D(1,4,1), new Vector4D(1,4,4), new Vector4D(4,4,4)); //up
////  meshlist[9] = new Mesh4D(new Vector4D(1,4,1), new Vector4D(4,4,1), new Vector4D(4,4,4)); //up
////  meshlist[10] = new Mesh4D(new Vector4D(1,1,1), new Vector4D(1,1,4), new Vector4D(4,1,4)); //down
////  meshlist[11] = new Mesh4D(new Vector4D(1,1,1), new Vector4D(4,1,1), new Vector4D(4,1,4)); //down
//  delay(500);
//}
//
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

void setup(){
  Serial.begin(9600);
  struct Matrix4X4 m44_1, m44_2, m44_result;
  initM44(&m44_1,
          1, 0, 0, 0,
          0, 1, 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1);
  initM44(&m44_2,
          1, 0, 0, 0,
          0, 1, 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1);
  M44timesM44(&m44_1, &m44_2, &m44_result);
  showM44(&m44_result);
}

void loop(){
  ;
}
