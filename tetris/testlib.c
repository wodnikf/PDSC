#include "primlib.h"
#include "pieces.inl"
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define SCREEN_HEIGHT gfx_screenHeight()
#define SCREEN_WIDTH gfx_screenWidth()

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define PIECE_SIZE 4
#define SHAPE 7

#define SQUARE_SIZE SCREEN_HEIGHT / (BOARD_HEIGHT + 4)
#define BOARD_X ((SCREEN_WIDTH - SQUARE_SIZE * BOARD_WIDTH) / 2)
#define BOARD_Y ((SCREEN_HEIGHT - SQUARE_SIZE * BOARD_HEIGHT) / 2)


int board[BOARD_WIDTH][BOARD_HEIGHT];

typedef struct
{	
	int x;
	int y;
	int shape;
	int rotation;
	int color;
}Tetromino;

void draw_screen();
void draw_grid();
void draw_square(int x_pos, int y_pos, int color);
Tetromino init_piece(int shape, int rotation, int color);
void check_collisions();
void next_piece();

int main(int argc, char *argv[])
{
	if (gfx_init())
	{
		exit(3);
	}

	while (true)
	{
		draw_screen();
		draw_grid();
		gfx_updateScreen();

		if (gfx_pollkey() == SDLK_ESCAPE){
			break;
        }
	}
	
	return 0;
}

void draw_screen()
{
	gfx_filledRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
}

void draw_grid()
{
	for (int i = 0; i < BOARD_HEIGHT + 1; i++)
	{
		gfx_line(BOARD_X, BOARD_Y + SQUARE_SIZE * i, BOARD_X + SQUARE_SIZE * BOARD_WIDTH, BOARD_Y + SQUARE_SIZE * i, WHITE);
	}
	for (int i = 0; i < BOARD_WIDTH + 1; i++)
	{
		gfx_line(BOARD_X + SQUARE_SIZE * i, BOARD_Y, BOARD_X + SQUARE_SIZE * i, BOARD_Y + SQUARE_SIZE * BOARD_HEIGHT, WHITE);

	}
}

void draw_square(int x_pos, int y_pos, int color)
{
	gfx_filledRect(BOARD_X + x_pos * SQUARE_SIZE, BOARD_Y + y_pos * SQUARE_SIZE, BOARD_X + (x_pos + 1) * SQUARE_SIZE,  BOARD_Y + (y_pos + 1) * SQUARE_SIZE, color);
}

Tetromino init_piece(int shape, int rotation, int color)
{
	Tetromino piece;
	piece.shape = shape;
	piece.rotation = rotation;
	piece.color = color;

	for (int i = 0; i < PIECE_SIZE; i++){
		for (int j = 0; j < PIECE_SIZE; j++){
			if(pieces[shape][rotation][i][j]){
				piece.x = 1;
				piece.y = 1;
			}
		}
	}
	return piece;
}