#include <GL/gl.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.141592653589793

#define BGCOL 0.125, 0.125, 0.125
#define BORDERCOL 0.0625, 0.0625, 0.0625
#define WWIDTH 600
#define WHEIGHT 600
#define WTITLE "EPIC BREAKOUT :3"

int window;

static int quit;

#define BR_ROWS 10
#define BR_COLS 10
#define BR_WIDTH 1.0f / BR_COLS
#define BR_HEIGHT 0.5f / BR_ROWS

static float bricks_x[BR_ROWS*BR_COLS];
static float bricks_y[BR_ROWS*BR_COLS];
static int bricks_active[BR_ROWS*BR_COLS];
static int bricks_active_count;

#define BALL_R 0.025f
#define LVLUP_MUL 1.25
static float LAUNCH_VX = 0.0125f;
static float LAUNCH_VY = 0.0125f;
static float ball_x;
static float ball_y;
static float ball_vx;
static float ball_vy;

static float PADDLE_SPEED = 0.015f;
#define PADDLE_WIDTH 0.25f
#define PADDLE_HEIGHT 0.05f
static float paddle_x;
static float paddle_y;

static int unreleased_key;

static int idling;

#define VEC_D(xa, ya, xb, yb) ( sqrt(pow(xa-xb, 2) + pow(ya-yb, 2)) )

void newBall(){
  ball_vx = ball_vy = 0.0f;
  ball_x = paddle_x;
  ball_y = paddle_y + PADDLE_HEIGHT/2.0f + BALL_R;
  idling = 1;
}

void resetPaddle(){
  paddle_x = 0.0f;
  paddle_y = PADDLE_HEIGHT - 1.0f;
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

void drawPaddle(){
  glBegin(GL_QUADS);
  glColor4d(0.0, 1.0, 0.0, 1.0);
  glVertex2f(paddle_x + PADDLE_WIDTH/2.0f, paddle_y + PADDLE_HEIGHT/2.0f);
  glVertex2f(paddle_x - PADDLE_WIDTH/2.0f, paddle_y + PADDLE_HEIGHT/2.0f);
  glVertex2f(paddle_x - PADDLE_WIDTH/2.0f, paddle_y - PADDLE_HEIGHT/2.0f);
  glVertex2f(paddle_x + PADDLE_WIDTH/2.0f, paddle_y - PADDLE_HEIGHT/2.0f);
  glEnd();
}

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

void drawBricks(){
  glBegin(GL_QUADS);
  glColor4d(1.0, 0.0, 0.0, 1.0);
  
  for(int i=0; i<BR_ROWS*BR_COLS; ++i){
    if(!bricks_active[i]) continue;
    
    glVertex2f(bricks_x[i] + BR_WIDTH/2.0f, bricks_y[i] + BR_HEIGHT/2.0f);
    glVertex2f(bricks_x[i] - BR_WIDTH/2.0f, bricks_y[i] + BR_HEIGHT/2.0f);
    glVertex2f(bricks_x[i] - BR_WIDTH/2.0f, bricks_y[i] - BR_HEIGHT/2.0f);
    glVertex2f(bricks_x[i] + BR_WIDTH/2.0f, bricks_y[i] - BR_HEIGHT/2.0f);
  }
  
  glEnd();
}

void spreadBricks(){
  for(int i=0; i<BR_ROWS; ++i){
    for(int j=0; j<BR_COLS; ++j){
      bricks_x[BR_COLS*i+j] = (float)(2*j + 2) / (float)(BR_COLS+1) - 1.0f;
      bricks_y[BR_COLS*i+j] = (float)(i+1) / (float)(BR_ROWS+1);
      bricks_active[BR_COLS*i+j] = 1;
      bricks_active_count = BR_COLS*BR_ROWS;
    }
  }
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

int main(void){
  init();
  cleanup();
  
  return 0;
}
