#include "ball.h"
#include "paddle.h"
#include "game.h"

float LAUNCH_VX = 0.0125f;
float LAUNCH_VY = 0.0125f;
float ball_x;
float ball_y;
float ball_vx;
float ball_vy;

void newBall(){
  ball_vx = ball_vy = 0.0f;
  ball_x = paddle_x;
  ball_y = paddle_y + PADDLE_HEIGHT/2.0f + BALL_R;
  idling = 1;
}

void drawBall(){
  int points = 10;
  float spacing = 1.0f/(float)points;

  glBegin(GL_TRIANGLE_FAN);
  glColor4d(0.0, 0.0, 1.0, 1.0);

  for(float i = 0.0f; i <= 2.0f; i += spacing){
    glVertex2f(ball_x + BALL_R * cos(PI * i), ball_y + BALL_R * sin(PI * i));
  }

  glEnd();
}
