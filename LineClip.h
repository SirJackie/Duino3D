#ifndef LINECLIP_H_
#define LINECLIP_H_

#include <U8g2lib.h>

#define LEFT_EDGE   1
#define RIGHT_EDGE  2
#define BOTTOM_EDGE 4
#define TOP_EDGE    8

struct Rectangle
{
    int xmin,xmax,ymin,ymax;
};

void initRectangle(struct Rectangle* rect, int xmin, int ymin, int xmax, int ymax){
  rect->xmin = xmin;
  rect->ymin = ymin;
  rect->xmax = xmax;
  rect->ymax = ymax;
}

int CompCode(int x, int y, struct Rectangle* rect)
{
    int code = 0x00;
    if (y < rect->ymin)   code = code | 4;
    if (y > rect->ymax)   code = code | 8;
    if (x > rect->xmax)   code = code | 2;
    if (x < rect->xmin)   code = code | 1;
    return code;
}


int LineClip(void (*DrawLineCallback)(int* x0, int* y0, int* x1, int* y1), struct Rectangle* rect, int x0, int y0, int x1, int y1)
/* Cohen-Sutherland Line Clip Algorithm */
{
    int accept = 0, done = 0;
    int x, y;

    int code0, code1, codeout;
    code0 = CompCode(x0, y0, rect);
    code1 = CompCode(x1, y1, rect);

    do {
        if (!(code0 | code1))
        {
            accept = 1;
            done = 1;
        }

        else if(code0 & code1)            //当线段全部都在边界外时
        {
            done = 1;
            x1 = 0; y1 = 0; x0 = 0; y0 = 0;
            return accept;
        }

        else
        {
            if (code0 != 0) {
                codeout = code0;
            }

            else {
                codeout = code1;
            }

            if (codeout & LEFT_EDGE)                        //线段与左边界相交
            {
                y = y0 + (y1 - y0) * (rect->xmin - x0) / (x1 - x0);
                x = (float)rect->xmin;
            }

            else if(codeout & RIGHT_EDGE)                  //线段与右边界相交
            {
                y = y0 + (y1 - y0) * (rect->xmax - x0) / (x1 - x0);
                x = (float)rect->xmax;
            }

            else if(codeout & BOTTOM_EDGE)                  //线段与下边界相交
            {
                x = x0 + (x1 - x0) * (rect->ymin - y0) / (y1 - y0);
                y = (float)rect->ymin;
            }

            else if(codeout & TOP_EDGE)                    //线段与上边界相交
            {
                x = x0 + (x1 - x0) * (rect->ymax - y0) / (y1 - y0);
                y = (float)rect->ymax;
            }



            if (codeout == code0)
            {
                x0 = x; y0 = y;
                code0 = CompCode(x0, y0, rect);
            }
            else
            {
                x1 = x; y1 = y;
                code1 = CompCode(x1, y1, rect);
            }

        }

    } while (!done);

    if (accept)
    {
        (*DrawLineCallback)(&x0, &y0, &x1, &y1);
    }
    return accept;
}

#endif
