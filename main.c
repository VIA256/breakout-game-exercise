#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include <stdio.h>
#include <math.h>
#define PI 3.141592653589793

#define BGCOL 0.125, 0.125, 0.125
#define BORDERCOL 0.0625, 0.0625, 0.0625
#define WWIDTH 600
#define WHEIGHT 600
#define WTITLE "EPIC BREAKOUT :3"

static Display* display;
static Window window;
static XVisualInfo* visual;
static GLXContext gl_context;

static int quit;

#define BR_ROWS 5
#define BR_COLS 10
#define BR_WIDTH 1.0f / BR_COLS
#define BR_HEIGHT 0.5f / BR_ROWS

static float bricks_x[BR_ROWS*BR_COLS];
static float bricks_y[BR_ROWS*BR_COLS];

#define BALL_R 0.05f
#define LAUNCH_VX 0.0125
#define LAUNCH_VY 0.0125
static float ball_x;
static float ball_y;
static float ball_vx;
static float ball_vy;

#define PADDLE_SPEED 0.01f
#define PADDLE_WIDTH 0.25f
#define PADDLE_HEIGHT 0.05f
static float paddle_x;
static float paddle_y;

static KeySym unreleased_key;

static int idling;

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
    }
  }
}

void init(){
  display = XOpenDisplay(NULL);
  window = XCreateSimpleWindow(display,
				      XDefaultRootWindow(display),
				      0, 0,
				      WWIDTH, WHEIGHT,
				      0,
				      0x00000000,
				      0x00000000);
  
  XStoreName(display, window, WTITLE);
  XSelectInput(display, window, KeyPressMask | KeyReleaseMask | StructureNotifyMask);
  XMapWindow(display, window);

  unreleased_key = XK_VoidSymbol;
  XAutoRepeatOff(display);
  
  visual = glXChooseVisual(display, 0, (int[]){
      GLX_RGBA,
      GLX_DEPTH_SIZE, 24,
      GLX_DOUBLEBUFFER,
      None
    });
  printf("Visual ID %lx\n", visual->visualid);
  
  gl_context = glXCreateContext(display, visual, 0, True);
  glXMakeCurrent(display, window, gl_context);

  glViewport(-1, -1, WWIDTH, WHEIGHT);

  spreadBricks();
  resetPaddle();
  newBall();
}

void cleanup(){
  glXDestroyContext(display, gl_context);
  XAutoRepeatOn(display);
  XCloseDisplay(display);
}

void loop(){
  quit = 0;
  KeySym ks;
  while(!quit){
    /* == INPUT == */
    
    while(XPending(display) > 0){
      XEvent event = {0};
      XNextEvent(display, &event);
      switch(event.type){
      case KeyPress:
	ks = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
	switch(ks){
	case XK_Escape:
	  quit = 1;
	  break;
	case XK_space:
	  if(idling){
	    idling = 0;
	    ball_vx = LAUNCH_VX;
	    ball_vy = LAUNCH_VY;
	  }
	  break;
	default:
	  unreleased_key = ks;
	  break;
	}
	break;
      case KeyRelease:
	ks = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
	if(ks == unreleased_key)
	  unreleased_key = XK_VoidSymbol;
	break;
      }
    }
    
    switch(unreleased_key){
    case XK_Left:
      paddle_x -= PADDLE_SPEED;
      break;
    case XK_Right:
      paddle_x += PADDLE_SPEED;
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
    
    /* == RENDERING == */
    
    glClearColor(BGCOL, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawBorder();
    drawPaddle();
    drawBall();
    drawBricks();

    glXSwapBuffers(display, window);
  }
}

int main(void){
  init();
  loop();
  cleanup();
  
  return 0;
}
