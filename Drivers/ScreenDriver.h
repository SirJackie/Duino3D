/*
 * Screen Driver for Duino3D
 * Support Screen Mode: SSD1306
 * If you use the diffrent screen, please write this driver by yourself.
 */

#include <U8g2lib.h>
#include <U8x8lib.h>

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 16, /* data=*/ 17);

void LcdInit(void){
    u8g2.begin();
}

void LcdBeginFrame(){
    u8g2.firstPage();
}

void LcdEndFrame(){
    u8g2.nextPage();
}

void LcdFill(int x, int y, int width, int height){
    u8g2.drawBox(x, y, width, height);
}

void LcdDrawLine(int x1, int y1, int x2, int y2){
    u8g2.drawLine(x1, y1, x2, y2);
}