#ifndef BRICKS_H
#define BRICKS_H

#define BR_ROWS 10
#define BR_COLS 10
#define BR_WIDTH 1.0f / BR_COLS
#define BR_HEIGHT 0.5f / BR_ROWS

extern float bricks_x[BR_ROWS*BR_COLS];
extern float bricks_y[BR_ROWS*BR_COLS];
extern int bricks_active[BR_ROWS*BR_COLS];
extern int bricks_active_count;

void drawBricks();
void spreadBricks();

#endif
