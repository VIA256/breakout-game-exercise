CC="cc"
CFLAGS="-Wall -Wextra -Wpedantic -std=c99"
LDFLAGS="-lGL -lglut -lm"
BDIR="build/linux"
SDIR="src"

ls $BDIR > /dev/null 2>&1 || mkdir -p $BDIR &&
	$CC $SDIR/game.c $CFLAGS -c -o $BDIR/game.o &&
	$CC $SDIR/ball.c $CFLAGS -c -o $BDIR/ball.o &&
	$CC $SDIR/paddle.c $CFLAGS -c -o $BDIR/paddle.o &&
	$CC $SDIR/bricks.c $CFLAGS -c -o $BDIR/bricks.o &&
	$CC $BDIR/*.o $SDIR/main.c $CFLAGS $LDFLAGS -o $BDIR/breakout
