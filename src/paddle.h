#ifndef PADDLE_H
#define PADDLE_H

extern float PADDLE_SPEED;
#define PADDLE_WIDTH 0.25f
#define PADDLE_HEIGHT 0.05f
extern float paddle_x;
extern float paddle_y;

void resetPaddle();

void drawPaddle();

#endif
