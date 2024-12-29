#include "game.h"
#include "ball.h"
#include "paddle.h"
#include "bricks.h"

#define BGCOL 0.125, 0.125, 0.125
#define BORDERCOL 0.0625, 0.0625, 0.0625
#define WWIDTH 600
#define WHEIGHT 600
#define WTITLE "EPIC BREAKOUT :3"

int window;

static int quit;

#define LVLUP_MUL 1.25

int idling;

static int unreleased_key;

#define VEC_D(xa, ya, xb, yb) ( sqrt(pow(xa-xb, 2) + pow(ya-yb, 2)) )

void drawBorder(){
  glBegin(GL_LINES);
  glColor4d(BORDERCOL, 1.0);
  
  glVertex2f(0.999f, 0.999f);
  glVertex2f(-0.999f, 0.999f);
  
  glVertex2f(-0.999f, 0.999f);
  glVertex2f(-0.999f, -0.999f);
  
  glVertex2f(-0.999f, -0.999f);
  glVertex2f(0.999f, -0.999f);
  
  glVertex2f(0.999f, -0.999f);
  glVertex2f(0.999f, 0.999f);
  glEnd();
}

void next_level(){
  spreadBricks();
  resetPaddle();
  newBall();
  LAUNCH_VX *= LVLUP_MUL;
  LAUNCH_VY *= LVLUP_MUL;
  PADDLE_SPEED *= LVLUP_MUL;
}

void input(unsigned char ascii, int x, int y){
  switch(ascii){
  case 0x1b:
    quit = 1;
    break;
  case ' ':
    if(idling){
      idling = 0;
      ball_vx = LAUNCH_VX;
      ball_vy = LAUNCH_VY;
    }
    break;
  }
}

void special_input(int key, int x, int y){
  unreleased_key = key;
}

void special_input_up(int key, int x, int y){
  unreleased_key = 0;
}

void loop();

void init(){
  int argc = 0;
  char* argv = "";
  glutInit(&argc, &argv);
  glutInitWindowSize(WWIDTH, WHEIGHT);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  
  window = glutCreateWindow(WTITLE);
  glutShowWindow();
  
  unreleased_key = 0;
  glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
  
  glViewport(-1, -1, WWIDTH, WHEIGHT);

  spreadBricks();
  resetPaddle();
  newBall();
  
  quit = 0;
  
  glutKeyboardFunc(input);
  glutSpecialFunc(special_input);
  glutSpecialUpFunc(special_input_up);
  glutDisplayFunc(loop);
  glutMainLoop();
}

void cleanup(){
  glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
  glutDestroyWindow(window);
  exit(0);
}

void loop(){
  if(quit) cleanup();

  /* == INPUT == */
  switch(unreleased_key){
  case GLUT_KEY_LEFT:
    paddle_x -= PADDLE_SPEED;
    LAUNCH_VX = -fabsf(LAUNCH_VX);
    break;
  case GLUT_KEY_RIGHT:
    paddle_x += PADDLE_SPEED;
    LAUNCH_VX = fabsf(LAUNCH_VX);
    break;  
  }

  
  /* == PROCESSING (PHYSICS, GENERAL) == */
  if(paddle_x + PADDLE_WIDTH/2.0f > 1.0f) paddle_x -= PADDLE_SPEED;
  else if(paddle_x - PADDLE_WIDTH/2.0f < -1.0f) paddle_x += PADDLE_SPEED;
  
  if(idling) ball_x = paddle_x;
  ball_x += ball_vx;
  ball_y += ball_vy;
  
  if(ball_x + BALL_R > 1.0f || ball_x - BALL_R < -1.0f){
    ball_vx *= -1.0f;
    ball_x += ball_vx;
  }
  if(ball_y + BALL_R > 1.0f ||
     (ball_x - BALL_R <= paddle_x + PADDLE_WIDTH/2.0f &&
      ball_x + BALL_R >= paddle_x - PADDLE_WIDTH/2.0f &&
      ball_y - BALL_R <= paddle_y + PADDLE_HEIGHT/2.0f)){
    ball_vy *= -1.0f;
    ball_y += ball_vy;
  }
  if(ball_y - BALL_R < -1.0f){
    newBall();
  }
  
  if(ball_y + BALL_R >= bricks_y[0] - BR_HEIGHT){
    int brid;
    float prevd = 69.0f;
    float newd;
    
    for(int i=0; i<BR_ROWS*BR_COLS; ++i){
      newd = VEC_D(ball_x, ball_y, bricks_x[i], bricks_y[i]);
      if(newd <= prevd){
	brid = i;
	prevd = newd;
      }
    }
    
    if(bricks_active[brid]){
      float brx = bricks_x[brid];
      float bry = bricks_y[brid];
      if(ball_x - BALL_R <= brx + BR_WIDTH &&
	 ball_x + BALL_R >= brx - BR_WIDTH &&
	 ball_y - BALL_R <= bry + BR_HEIGHT &&
	 ball_y + BALL_R >= bry - BR_HEIGHT)
	{
	  float nx, ny;
	  nx = ball_x - brx;
	  ny = ball_y - bry;
	  nx /= fabs(nx);
	  ny /= fabs(ny);
	  ball_vx *= -nx;
	  ball_vy *= ny;
	  
	  bricks_active[brid] = 0;
	  --bricks_active_count;
	  if(bricks_active_count <= 0) next_level();
	}
    }
  }
  
  /* == RENDERING == */
  
  glClearColor(BGCOL, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  
  drawBorder();
  drawPaddle();
  drawBall();
  drawBricks();

  glutSwapBuffers();
  glutPostRedisplay();
}
