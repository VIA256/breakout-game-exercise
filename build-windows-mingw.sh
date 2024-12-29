CC="i686-w64-mingw32-gcc"
CFLAGS="-Wall -Wextra -Wpedantic -std=c99"
LDFLAGS="-mwindows glut32.lib -lopengl32 -lm"
BDIR="build/windows"
SDIR="src"

ls $BDIR > /dev/null 2>&1 || mkdir -p $BDIR &&
	$CC $SDIR/game.c $CFLAGS -c -o $BDIR/game.o &&
	$CC $SDIR/ball.c $CFLAGS -c -o $BDIR/ball.o &&
	$CC $SDIR/paddle.c $CFLAGS -c -o $BDIR/paddle.o &&
	$CC $SDIR/bricks.c $CFLAGS -c -o $BDIR/bricks.o &&
	$CC $BDIR/*.o $SDIR/main.c $CFLAGS $LDFLAGS -o $BDIR/breakout
