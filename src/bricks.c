#include "bricks.h"
#include "game.h"

float bricks_x[BR_ROWS*BR_COLS];
float bricks_y[BR_ROWS*BR_COLS];
int bricks_active[BR_ROWS*BR_COLS];
int bricks_active_count;

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
