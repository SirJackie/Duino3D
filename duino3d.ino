#include "Duino9341.h"

/*
** Calculation Structs
*/

function Matrix4X4(a00, a01, a02, a03, a10, a11, a12, a13, a20, a21, a22, a23, a30, a31, a32, a33){
  this.m00 = a00;
  this.m01 = a01;
  this.m02 = a02;
  this.m03 = a03;

  this.m10 = a10;
  this.m11 = a11;
  this.m12 = a12;
  this.m13 = a13;

  this.m20 = a20;
  this.m21 = a21;
  this.m22 = a22;
  this.m23 = a23;

  this.m30 = a30;
  this.m31 = a31;
  this.m32 = a32;
  this.m33 = a33;
}

function Matrix4X1(a00, a01, a02, a03){
  this.m00 = a00;
  this.m01 = a01;
  this.m02 = a02;
  this.m03 = a03;
}

/*
** Calculation Functions
*/

function Matrix4X4timesMatrix4X4(a, b){
  var m00 = (a.m00 * b.m00) + (a.m01*b.m10) + (a.m02*b.m20) + (a.m03*b.m30);
  var m01 = (a.m00 * b.m01) + (a.m01*b.m11) + (a.m02*b.m21) + (a.m03*b.m31);
  var m02 = (a.m00 * b.m02) + (a.m01*b.m12) + (a.m02*b.m22) + (a.m03*b.m32);
  var m03 = (a.m00 * b.m03) + (a.m01*b.m13) + (a.m02*b.m23) + (a.m03*b.m33);
           
  var m10 = (a.m10 * b.m00) + (a.m11*b.m10) + (a.m12*b.m20) + (a.m13*b.m30);
  var m11 = (a.m10 * b.m01) + (a.m11*b.m11) + (a.m12*b.m21) + (a.m13*b.m31);
  var m12 = (a.m10 * b.m02) + (a.m11*b.m12) + (a.m12*b.m22) + (a.m13*b.m32);
  var m13 = (a.m10 * b.m03) + (a.m11*b.m13) + (a.m12*b.m23) + (a.m13*b.m33);
           
  var m20 = (a.m20 * b.m00) + (a.m21*b.m10) + (a.m22*b.m20) + (a.m23*b.m30);
  var m21 = (a.m20 * b.m01) + (a.m21*b.m11) + (a.m22*b.m21) + (a.m23*b.m31);
  var m22 = (a.m20 * b.m02) + (a.m21*b.m12) + (a.m22*b.m22) + (a.m23*b.m32);
  var m23 = (a.m20 * b.m03) + (a.m21*b.m13) + (a.m22*b.m23) + (a.m23*b.m33);
           
  var m30 = (a.m30 * b.m00) + (a.m31*b.m10) + (a.m32*b.m20) + (a.m33*b.m30);
  var m31 = (a.m30 * b.m01) + (a.m31*b.m11) + (a.m32*b.m21) + (a.m33*b.m31);
  var m32 = (a.m30 * b.m02) + (a.m31*b.m12) + (a.m32*b.m22) + (a.m33*b.m32);
  var m33 = (a.m30 * b.m03) + (a.m31*b.m13) + (a.m32*b.m23) + (a.m33*b.m33);

  return new Matrix4X4(m00, m01, m02, m03,
               m10, m11, m12, m13,
             m20, m21, m22, m23,
             m30, m31, m32, m33);
}

function Matrix4X1timesMatrix4X4(a, b){
  var m00 = (a.m00 * b.m00) + (a.m01*b.m10) + (a.m02*b.m20) + (a.m03*b.m30);
  var m01 = (a.m00 * b.m01) + (a.m01*b.m11) + (a.m02*b.m21) + (a.m03*b.m31);
  var m02 = (a.m00 * b.m02) + (a.m01*b.m12) + (a.m02*b.m22) + (a.m03*b.m32);
  var m03 = (a.m00 * b.m03) + (a.m01*b.m13) + (a.m02*b.m23) + (a.m03*b.m33);

  return new Matrix4X1(m00, m01, m02, m03);
}

function cosd(x){
  return Math.cos(x * 0.017453293);
}

function sind(x){
  return Math.sin(x * 0.017453293);
}

function tand(x){
  return Math.tan(x * 0.017453293);
}

function acosd(x){
  return Math.acos(x * 0.017453293);
}

function asind(x){
  return Math.asin(x * 0.017453293);
}

function atand(x){
  return Math.atan(x * 0.017453293);
}

/*
** Structs
*/

function Camera(positionX, positionY, positionZ, angleX, angleY, angleZ, screenWidth, screenHeight){
  this.x              = positionX;
  this.y              = positionY;
  this.z              = positionZ;
  this.angleX         = angleX;
  this.angleY         = angleY;
  this.angleZ         = angleZ;
  this.screenWidth    = screenWidth;
  this.screenHeight   = screenHeight;
  this.rotationMatrix = null;

  this.refreshRotationMatrix = function(){
    var ZRotationMatrix = new Matrix4X4(cosd(this.angleZ),      sind(this.angleZ),     0,                        0,
                      -1*sind(this.angleZ),   cosd(this.angleZ),     0,                        0,
                      0,                        0,                       1,                        0,
                      0,                        0,                       0,                        1);

    var YRotationMatrix = new Matrix4X4(cosd(this.angleY),      0,                       -1*sind(this.angleY),   0,
                      0,                        1,                       0,                        0,
                      sind(this.angleY),      0,                       cosd(this.angleY),      0,
                      0,                        0,                       0,                        1);

    var XRotationMatrix = new Matrix4X4(1,                        0,                       0,                        0,
                      0,                        cosd(this.angleX),     sind(this.angleX),      0,
                      0,                        -1*sind(this.angleX),  cosd(this.angleX),      0,
                      0,                        0,                       0,                        1);
    
    this.rotationMatrix = Matrix4X4timesMatrix4X4(Matrix4X4timesMatrix4X4(ZRotationMatrix, YRotationMatrix), XRotationMatrix);
  }
  
}

function Vector2D(x,y){
  this.x = x;
  this.y = y;
}

function Mesh2D(vec1,vec2,vec3){
  this.vec1 = vec1;
  this.vec2 = vec2;
  this.vec3 = vec3;
}

function World2D(){
  this.meshlist = [];
}

function Vector4D(x,y,z){
  this.x = x;
  this.y = y;
  this.z = z;
  this.w = 1;
}

function Mesh4D(vec1,vec2,vec3){
  this.vec1 = vec1;
  this.vec2 = vec2;
  this.vec3 = vec3;
}

function Object4D(meshlist){
  this.meshlist = meshlist;
}

function World4D(){
  this.meshlist = [];
  this.PlaceObject4D = function(newObject,x,y,z){
    for(var i = 0; i < newObject.meshlist.length; i++){
      this.meshlist.push(new Mesh4D(new Vector4D(newObject.meshlist[i].vec1.x + x,
                             newObject.meshlist[i].vec1.y + y,
                             newObject.meshlist[i].vec1.z + z,),
                      new Vector4D(newObject.meshlist[i].vec2.x + x,
                             newObject.meshlist[i].vec2.y + y,
                             newObject.meshlist[i].vec2.z + z,),
                      new Vector4D(newObject.meshlist[i].vec3.x + x,
                             newObject.meshlist[i].vec3.y + y,
                             newObject.meshlist[i].vec3.z + z,)));
    }
    this.meshlist.push();
  }
}

/*
** Canvas Functions
*/

function CanvasInit(canvasID, width, height){
  var canvas    = document.getElementById(canvasID);
  var ctx       = canvas.getContext("2d");
  canvas.width  = width;
  canvas.height = height;
  return ctx;
}

function CanvasDrawVector2D(ctx, vector){
  ctx.fillStyle   = "#000000";
  ctx.fillRect(vector.x-3, //Start X Position
         vector.y-3, //Start Y Position
         6,          //Width
         6);         //Height
}

function CanvasDrawMesh2D(ctx, mesh, color){
  if(mesh.vec1.x == -1 || mesh.vec2.x == -1 || mesh.vec3.x == -1){
    return; //don't show this vector
  }
  CanvasDrawVector2D(ctx, mesh.vec1);
  CanvasDrawVector2D(ctx, mesh.vec2);
  CanvasDrawVector2D(ctx, mesh.vec3);

  ctx.fillStyle   = "rgba(0,144,255,0.5)";
  ctx.strokeStyle = "#000000";
  ctx.beginPath();
  ctx.moveTo(mesh.vec1.x, mesh.vec1.y);
  ctx.lineTo(mesh.vec2.x, mesh.vec2.y);
  ctx.lineTo(mesh.vec3.x, mesh.vec3.y);
  ctx.lineTo(mesh.vec1.x, mesh.vec1.y);
  ctx.fill();
  ctx.stroke();
  ctx.closePath();
}

function CanvasDrawWorld2D(ctx, world, color){
  for(var i = 0; i < world.meshlist.length; i++){
    CanvasDrawMesh2D(ctx, world.meshlist[i], color);
  }
}

/*
** Projection Functions
*/

function Vector4D2Vector2D(camera,vector){
  var zoom  = 200;
  var zfix  = 0.35;
  var final = Matrix4X1timesMatrix4X4(new Matrix4X1(vector.x - camera.x, vector.y - camera.y, vector.z - camera.z, 1), camera.rotationMatrix);
  var vecx  = final.m00;
  var vecy  = final.m01;
  var vecz  = final.m02;

  if(vecz <= 0){
    return new Vector2D(-1,-1); //don't show this vector
  } else {
    var x2d     = (vecx) / (vecz*zfix);
    var y2d     = (vecy) / (vecz*zfix);

    var xScreen = (camera.screenWidth  / 2) + (x2d * zoom);
    var yScreen = (camera.screenHeight / 2) + (y2d * zoom);
    return new Vector2D(xScreen, yScreen);
  }
}

function Mesh4D2Mesh2D(camera,mesh4d){
  return new Mesh2D(Vector4D2Vector2D(camera, mesh4d.vec1),
            Vector4D2Vector2D(camera, mesh4d.vec2),
            Vector4D2Vector2D(camera, mesh4d.vec3));
}

function World4D2World2D(camera,world4d){
  var world2d = new World2D();
  for(var i = 0; i < world4d.meshlist.length; i++){
    world2d.meshlist.push(Mesh4D2Mesh2D(camera, world4d.meshlist[i]));
  }
  return world2d;
}

window.onload = function(){
  cam1 = new Camera(0, 0, -10, 0, 0, 0, document.body.clientWidth, document.body.clientHeight);//global variable,with no "var"
  var ctx  = CanvasInit("canvas1", cam1.screenWidth, cam1.screenHeight);
  var obj1 = new Object4D([new Mesh4D(new Vector4D(1,1,1), new Vector4D(4,1,1), new Vector4D(1,4,1)), //front
               new Mesh4D(new Vector4D(1,4,1), new Vector4D(4,4,1), new Vector4D(4,1,1)), //front
               new Mesh4D(new Vector4D(1,4,4), new Vector4D(1,1,4), new Vector4D(1,1,1)), //left
               new Mesh4D(new Vector4D(1,4,4), new Vector4D(1,4,1), new Vector4D(1,1,1)), //left
               new Mesh4D(new Vector4D(4,4,4), new Vector4D(4,1,4), new Vector4D(4,1,1)), //right
               new Mesh4D(new Vector4D(4,4,4), new Vector4D(4,4,1), new Vector4D(4,1,1)), //right
               new Mesh4D(new Vector4D(1,1,4), new Vector4D(4,1,4), new Vector4D(1,4,4)), //back
               new Mesh4D(new Vector4D(1,4,4), new Vector4D(4,4,4), new Vector4D(4,1,4)), //back
               new Mesh4D(new Vector4D(1,4,1), new Vector4D(1,4,4), new Vector4D(4,4,4)), //up
               new Mesh4D(new Vector4D(1,4,1), new Vector4D(4,4,1), new Vector4D(4,4,4)), //up
               new Mesh4D(new Vector4D(1,1,1), new Vector4D(1,1,4), new Vector4D(4,1,4)), //down
               new Mesh4D(new Vector4D(1,1,1), new Vector4D(4,1,1), new Vector4D(4,1,4)), //down
               ]);
  var world4d1 = new World4D();
  var area = 70;
  world4d1.PlaceObject4D(obj1, -2.5, -2.5, -2.5);
  for(var i = 0; i < 8; i++){
    world4d1.PlaceObject4D(obj1, Math.random()*area   , Math.random()*area/2   , Math.random()*area);
    world4d1.PlaceObject4D(obj1, Math.random()*area*-1, Math.random()*area/2   , Math.random()*area);
    world4d1.PlaceObject4D(obj1, Math.random()*area   , Math.random()*area/2*-1, Math.random()*area);
    world4d1.PlaceObject4D(obj1, Math.random()*area*-1, Math.random()*area/2*-1, Math.random()*area);
  }

  requestAnimationFrame(function main(){
    cam1.refreshRotationMatrix();
    var world2d1 = World4D2World2D(cam1,world4d1);
    ctx.clearRect(0,0,cam1.screenWidth,cam1.screenHeight);
    CanvasDrawWorld2D(ctx,world2d1,"rgba(0,144,255,0.5)");
    requestAnimationFrame(main);
  });
}

void setup() {
  // put your setup code here, to run once:
  LcdInit();
  LcdFill(0,0,239,319,RGB(255,255,255));
}

void loop() {
  // put your main code here, to run repeatedly:

}
