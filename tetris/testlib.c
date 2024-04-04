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
#define START_X (BOARD_WIDTH / 2 - 1)

int board[BOARD_HEIGHT][BOARD_WIDTH] = {{0}};

// TO DO: LOSOWANIE

typedef struct
{	
	int fields[4][4];
	int shape;
	int rotation;
	int color;
	int x;
	int y;
}Piece;

void draw_screen();
void draw_grid();
void draw_square(int x_pos, int y_pos, int color);
Piece init_piece(int shape, int rotation, int color);
void draw_piece(Piece piece);
void print_board();
void add_on_board(Piece piece);
int find_left(Piece piece);
int find_top(Piece piece);
void check_left();
void check_right();
void check_collisions();
void next_piece();

int main(int argc, char *argv[])
{
	if (gfx_init())
	{
		exit(3);
	}
	srand(time(NULL));

	print_board();
	Piece p;
	p = init_piece(1, 1, 3);
	print_board();

	while (true)
	{
		draw_screen();
		draw_piece(p);
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

Piece init_piece(int shape, int rotation, int color)
{
	Piece piece;
	piece.shape = shape;
	piece.rotation = rotation;
	piece.color = color;

	for (int row = 0; row < PIECE_SIZE; row++){
		for (int col = 0; col < PIECE_SIZE; col++){
			piece.fields[row][col] = pieces[shape][rotation][row][col];
		}
	}

	piece.x = rand() % BOARD_WIDTH - find_left(piece);
	piece.y = -find_top(piece);
	add_on_board(piece);

	return piece;
}

void add_on_board(Piece piece)
{
	for (int row = 0; row < PIECE_SIZE; row++){
		for (int col = 0; col < PIECE_SIZE; col++){
			if (piece.fields[row][col]){
			board[piece.y + row][piece.x + col] = piece.fields[row][col];
			}
		}
	}

}

int find_left(Piece piece)
{

	// to do: optymalizacja, dodac break
	int temp = PIECE_SIZE;
	for (int row = 0; row < PIECE_SIZE; row++){
		for (int col = 0; col < PIECE_SIZE; col++){
			if (piece.fields[row][col] && (col < temp)){
				temp = col;
				break;
			}
		}
	}
	return temp;
}

int find_top(Piece piece)
{
	int temp = PIECE_SIZE;
	for (int row = 0; row < PIECE_SIZE; row++){
		for (int col = 0; col < PIECE_SIZE; col++){
			if (piece.fields[row][col] && (row < temp)){
				temp = row;
				break;
			}
		}
	}
	return temp;
}

void draw_piece(Piece piece) {
    for (int row = 0; row < PIECE_SIZE; row++) {
        for (int col = 0; col < PIECE_SIZE; col++) {
            if (piece.fields[row][col]) {
                draw_square(piece.x + col, piece.y + row, piece.color);
            }
        }
    }
}


void print_board() {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            printf("%d ", board[y][x]);
        }
        printf("\n");
    }
}