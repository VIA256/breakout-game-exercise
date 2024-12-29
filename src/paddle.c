#include "paddle.h"
#include "game.h"

float PADDLE_SPEED = 0.015f;

float paddle_x;
float paddle_y;

void resetPaddle(){
  paddle_x = 0.0f;
  paddle_y = PADDLE_HEIGHT - 1.0f;
}

void drawPaddle(){
  glBegin(GL_QUADS);
  glColor4d(0.0, 1.0, 0.0, 1.0);
  glVertex2f(paddle_x + PADDLE_WIDTH/2.0f, paddle_y + PADDLE_HEIGHT/2.0f);
  glVertex2f(paddle_x - PADDLE_WIDTH/2.0f, paddle_y + PADDLE_HEIGHT/2.0f);
  glVertex2f(paddle_x - PADDLE_WIDTH/2.0f, paddle_y - PADDLE_HEIGHT/2.0f);
  glVertex2f(paddle_x + PADDLE_WIDTH/2.0f, paddle_y - PADDLE_HEIGHT/2.0f);
  glEnd();
}
