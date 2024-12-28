ls build/linux > /dev/null 2>&1 || mkdir -p build/linux
cc main.c -Wall -Wextra -Wpedantic -std=c99 -lGL -lglut -lm -o build/linux/breakout
