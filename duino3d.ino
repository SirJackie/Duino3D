#include "Duino9341.h"
#define cosd(x) cos(x*0.017453293)
#define sind(x) sin(x*0.017453293)
#define tand(x) tan(x*0.017453293)
#define acosd(x) acos(x*0.017453293)
#define asind(x) asin(x*0.017453293)
#define atand(x) atan(x*0.017453293)
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
  Matrix4X4(float a00, float a01, float a02, float a03,
            float a10, float a11, float a12, float a13,
            float a20, float a21, float a22, float a23,
            float a30, float a31, float a32, float a33){
    this->m00 = a00;
    this->m01 = a01;
    this->m02 = a02;
    this->m03 = a03;
  
    this->m10 = a10;
    this->m11 = a11;
    this->m12 = a12;
    this->m13 = a13;
  
    this->m20 = a20;
    this->m21 = a21;
    this->m22 = a22;
    this->m23 = a23;
  
    this->m30 = a30;
    this->m31 = a31;
    this->m32 = a32;
    this->m33 = a33;
  }
};

struct Matrix4X1{
  float m00;
  float m01;
  float m02;
  float m03;
  Matrix4X1(float a00, float a01, float a02, float a03){
    this->m00 = a00;
    this->m01 = a01;
    this->m02 = a02;
    this->m03 = a03;
  }
};

/*
** Calculation Functions
*/

Matrix4X4* Matrix4X4timesMatrix4X4(Matrix4X4* a, Matrix4X4* b){
  float m00 = (a->m00 * b->m00) + (a->m01*b->m10) + (a->m02*b->m20) + (a->m03*b->m30);
  float m01 = (a->m00 * b->m01) + (a->m01*b->m11) + (a->m02*b->m21) + (a->m03*b->m31);
  float m02 = (a->m00 * b->m02) + (a->m01*b->m12) + (a->m02*b->m22) + (a->m03*b->m32);
  float m03 = (a->m00 * b->m03) + (a->m01*b->m13) + (a->m02*b->m23) + (a->m03*b->m33);
           
  float m10 = (a->m10 * b->m00) + (a->m11*b->m10) + (a->m12*b->m20) + (a->m13*b->m30);
  float m11 = (a->m10 * b->m01) + (a->m11*b->m11) + (a->m12*b->m21) + (a->m13*b->m31);
  float m12 = (a->m10 * b->m02) + (a->m11*b->m12) + (a->m12*b->m22) + (a->m13*b->m32);
  float m13 = (a->m10 * b->m03) + (a->m11*b->m13) + (a->m12*b->m23) + (a->m13*b->m33);
           
  float m20 = (a->m20 * b->m00) + (a->m21*b->m10) + (a->m22*b->m20) + (a->m23*b->m30);
  float m21 = (a->m20 * b->m01) + (a->m21*b->m11) + (a->m22*b->m21) + (a->m23*b->m31);
  float m22 = (a->m20 * b->m02) + (a->m21*b->m12) + (a->m22*b->m22) + (a->m23*b->m32);
  float m23 = (a->m20 * b->m03) + (a->m21*b->m13) + (a->m22*b->m23) + (a->m23*b->m33);
           
  float m30 = (a->m30 * b->m00) + (a->m31*b->m10) + (a->m32*b->m20) + (a->m33*b->m30);
  float m31 = (a->m30 * b->m01) + (a->m31*b->m11) + (a->m32*b->m21) + (a->m33*b->m31);
  float m32 = (a->m30 * b->m02) + (a->m31*b->m12) + (a->m32*b->m22) + (a->m33*b->m32);
  float m33 = (a->m30 * b->m03) + (a->m31*b->m13) + (a->m32*b->m23) + (a->m33*b->m33);

  return new Matrix4X4(m00, m01, m02, m03,
                       m10, m11, m12, m13,
                       m20, m21, m22, m23,
                       m30, m31, m32, m33);
}

void showMatrix4X4(Matrix4X4* m){
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

Matrix4X1* Matrix4X1timesMatrix4X4(Matrix4X1* a, Matrix4X4* b){
  float m00 = (a->m00 * b->m00) + (a->m01*b->m10) + (a->m02*b->m20) + (a->m03*b->m30);
  float m01 = (a->m00 * b->m01) + (a->m01*b->m11) + (a->m02*b->m21) + (a->m03*b->m31);
  float m02 = (a->m00 * b->m02) + (a->m01*b->m12) + (a->m02*b->m22) + (a->m03*b->m32);
  float m03 = (a->m00 * b->m03) + (a->m01*b->m13) + (a->m02*b->m23) + (a->m03*b->m33);

  return new Matrix4X1(m00, m01, m02, m03);
}

void showMatrix4X1(Matrix4X1* m){
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

struct Camera{
  float x;
  float y;
  float z;
  float angleX;
  float angleY;
  float angleZ;
  float screenWidth;
  float screenHeight;
  Matrix4X4* rotationMatrix;
  
  Camera(float positionX, float positionY, float positionZ, float angleX, float angleY, float angleZ, float screenWidth, float screenHeight){
    this->x              = positionX;
    this->y              = positionY;
    this->z              = positionZ;
    this->angleX         = angleX;
    this->angleY         = angleY;
    this->angleZ         = angleZ;
    this->screenWidth    = screenWidth;
    this->screenHeight   = screenHeight;
  }
  void refreshRotationMatrix(){
      Matrix4X4* ZRotationMatrix = new Matrix4X4(cosd(this->angleZ),      sind(this->angleZ),     0,                        0,
                                                 -1*sind(this->angleZ),   cosd(this->angleZ),     0,                        0,
                                                 0,                        0,                       1,                        0,
                                                 0,                        0,                       0,                        1);
  
      Matrix4X4* YRotationMatrix = new Matrix4X4(cosd(this->angleY),      0,                       -1*sind(this->angleY),   0,
                                                 0,                        1,                       0,                        0,
                                                 sind(this->angleY),      0,                       cosd(this->angleY),      0,
                                                 0,                        0,                       0,                        1);
  
      Matrix4X4* XRotationMatrix = new Matrix4X4(1,                        0,                       0,                        0,
                                                 0,                        cosd(this->angleX),     sind(this->angleX),      0,
                                                 0,                        -1*sind(this->angleX),  cosd(this->angleX),      0,
                                                 0,                        0,                       0,                        1);
      
      delete this->rotationMatrix;
      Matrix4X4* ZYMatrix  = Matrix4X4timesMatrix4X4(ZRotationMatrix, YRotationMatrix);
      Matrix4X4* ZYXMatrix = Matrix4X4timesMatrix4X4(ZYMatrix, XRotationMatrix);
      this->rotationMatrix = ZYXMatrix;
      delete ZRotationMatrix;
      delete YRotationMatrix;
      delete XRotationMatrix;
      delete ZYMatrix;
    }
};


struct Vector2D{
  int x;
  int y;
  Vector2D(int x, int y){
    this->x = x;
    this->y = y;
  }
};

struct Mesh2D{
  Vector2D* vec1;
  Vector2D* vec2;
  Vector2D* vec3;
  Mesh2D(Vector2D* vec1, Vector2D* vec2, Vector2D* vec3){
    this->vec1 = vec1;
    this->vec2 = vec2;
    this->vec3 = vec3;
  }
};

//function World2D(){
//  this.meshlist = [];
//}

struct Vector4D{
  float x;
  float y;
  float z;
  float w = 1;
  Vector4D(float x,float y,float z){
    this->x = x;
    this->y = y;
    this->z = z;
  }
};

struct Mesh4D{
  Vector4D* vec1;
  Vector4D* vec2;
  Vector4D* vec3;
  Mesh4D(Vector4D* vec1, Vector4D* vec2, Vector4D* vec3){
    this->vec1 = vec1;
    this->vec2 = vec2;
    this->vec3 = vec3;
  }
};

//function Object4D(meshlist){
//  this->meshlist = meshlist;
//}

//function World4D(){
//  this->meshlist = [];
//  this->PlaceObject4D = function(newObject,x,y,z){
//    for(var i = 0; i < newObject.meshlist.length; i++){
//      this.meshlist.push(new Mesh4D(new Vector4D(newObject.meshlist[i].vec1.x + x,
//                             newObject.meshlist[i].vec1.y + y,
//                             newObject.meshlist[i].vec1.z + z,),
//                      new Vector4D(newObject.meshlist[i].vec2.x + x,
//                             newObject.meshlist[i].vec2.y + y,
//                             newObject.meshlist[i].vec2.z + z,),
//                      new Vector4D(newObject.meshlist[i].vec3.x + x,
//                             newObject.meshlist[i].vec3.y + y,
//                             newObject.meshlist[i].vec3.z + z,)));
//    }
//    this.meshlist.push();
//  }
//}

/*
** Canvas Functions
*/

void CanvasDrawVector2D(struct Vector2D* vector){
  LcdFill(vector->x-3, //Start X Position
          vector->y-3, //Start Y Position
          6,          //Width
          6,          //Height
          RGB(0,0,0));
}

void CanvasDrawMesh2D(struct Mesh2D* mesh){
  if(mesh->vec1->x == -1 || mesh->vec2->x == -1 || mesh->vec3->x == -1){
    return; //don't show this vector
  }
  CanvasDrawVector2D(mesh->vec1);
  CanvasDrawVector2D(mesh->vec2);
  CanvasDrawVector2D(mesh->vec3);

  LcdDrawLine(mesh->vec1->x, mesh->vec1->y, mesh->vec2->x, mesh->vec2->y, RGB(0,0,0));
  LcdDrawLine(mesh->vec2->x, mesh->vec2->y, mesh->vec3->x, mesh->vec3->y, RGB(0,0,0));
  LcdDrawLine(mesh->vec3->x, mesh->vec3->y, mesh->vec1->x, mesh->vec1->y, RGB(0,0,0));
}

//function CanvasDrawWorld2D(ctx, world, color){
//  for(var i = 0; i < world.meshlist.length; i++){
//    CanvasDrawMesh2D(ctx, world.meshlist[i], color);
//  }
//}
//
///*
//** Projection Functions
//*/
//
//function Vector4D2Vector2D(camera,vector){
//  var zoom  = 200;
//  var zfix  = 0.35;
//  var final = Matrix4X1timesMatrix4X4(new Matrix4X1(vector.x - camera.x, vector.y - camera.y, vector.z - camera.z, 1), camera.rotationMatrix);
//  var vecx  = final.m00;
//  var vecy  = final.m01;
//  var vecz  = final.m02;
//
//  if(vecz <= 0){
//    return new Vector2D(-1,-1); //don't show this vector
//  } else {
//    var x2d     = (vecx) / (vecz*zfix);
//    var y2d     = (vecy) / (vecz*zfix);
//
//    var xScreen = (camera.screenWidth  / 2) + (x2d * zoom);
//    var yScreen = (camera.screenHeight / 2) + (y2d * zoom);
//    return new Vector2D(xScreen, yScreen);
//  }
//}
//
//function Mesh4D2Mesh2D(camera,mesh4d){
//  return new Mesh2D(Vector4D2Vector2D(camera, mesh4d.vec1),
//            Vector4D2Vector2D(camera, mesh4d.vec2),
//            Vector4D2Vector2D(camera, mesh4d.vec3));
//}
//
//function World4D2World2D(camera,world4d){
//  var world2d = new World2D();
//  for(var i = 0; i < world4d.meshlist.length; i++){
//    world2d.meshlist.push(Mesh4D2Mesh2D(camera, world4d.meshlist[i]));
//  }
//  return world2d;
//}
//
//window.onload = function(){
//  cam1 = new Camera(0, 0, -10, 0, 0, 0, document.body.clientWidth, document.body.clientHeight);//global variable,with no "var"
//  var ctx  = CanvasInit("canvas1", cam1.screenWidth, cam1.screenHeight);
//  var obj1 = new Object4D([new Mesh4D(new Vector4D(1,1,1), new Vector4D(4,1,1), new Vector4D(1,4,1)), //front
//               new Mesh4D(new Vector4D(1,4,1), new Vector4D(4,4,1), new Vector4D(4,1,1)), //front
//               new Mesh4D(new Vector4D(1,4,4), new Vector4D(1,1,4), new Vector4D(1,1,1)), //left
//               new Mesh4D(new Vector4D(1,4,4), new Vector4D(1,4,1), new Vector4D(1,1,1)), //left
//               new Mesh4D(new Vector4D(4,4,4), new Vector4D(4,1,4), new Vector4D(4,1,1)), //right
//               new Mesh4D(new Vector4D(4,4,4), new Vector4D(4,4,1), new Vector4D(4,1,1)), //right
//               new Mesh4D(new Vector4D(1,1,4), new Vector4D(4,1,4), new Vector4D(1,4,4)), //back
//               new Mesh4D(new Vector4D(1,4,4), new Vector4D(4,4,4), new Vector4D(4,1,4)), //back
//               new Mesh4D(new Vector4D(1,4,1), new Vector4D(1,4,4), new Vector4D(4,4,4)), //up
//               new Mesh4D(new Vector4D(1,4,1), new Vector4D(4,4,1), new Vector4D(4,4,4)), //up
//               new Mesh4D(new Vector4D(1,1,1), new Vector4D(1,1,4), new Vector4D(4,1,4)), //down
//               new Mesh4D(new Vector4D(1,1,1), new Vector4D(4,1,1), new Vector4D(4,1,4)), //down
//               ]);
//  var world4d1 = new World4D();
//  var area = 70;
//  world4d1.PlaceObject4D(obj1, -2.5, -2.5, -2.5);
//  for(var i = 0; i < 8; i++){
//    world4d1.PlaceObject4D(obj1, Math.random()*area   , Math.random()*area/2   , Math.random()*area);
//    world4d1.PlaceObject4D(obj1, Math.random()*area*-1, Math.random()*area/2   , Math.random()*area);
//    world4d1.PlaceObject4D(obj1, Math.random()*area   , Math.random()*area/2*-1, Math.random()*area);
//    world4d1.PlaceObject4D(obj1, Math.random()*area*-1, Math.random()*area/2*-1, Math.random()*area);
//  }
//
//  requestAnimationFrame(function main(){
//    cam1.refreshRotationMatrix();
//    var world2d1 = World4D2World2D(cam1,world4d1);
//    ctx.clearRect(0,0,cam1.screenWidth,cam1.screenHeight);
//    CanvasDrawWorld2D(ctx,world2d1,"rgba(0,144,255,0.5)");
//    requestAnimationFrame(main);
//  });
//}
Camera* cam1;
void setup() {
  // put your setup code here, to run once:
  LcdInit();
  LcdFill(0,0,239,319,RGB(255,255,255));
  Serial.begin(9600);
  Mesh2D* mesh = new Mesh2D(new Vector2D(10,10), new Vector2D(110,10), new Vector2D(10,110));
  CanvasDrawMesh2D(mesh);
  cam1 = new Camera(0,0,0,30,0,0,240,320);
}

void loop() {
  // put your main code here, to run repeatedly:
  cam1->refreshRotationMatrix();
  showMatrix4X4(cam1->rotationMatrix);
}
