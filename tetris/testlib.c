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
#define ROTATIONS 4
#define COLORS 6
#define SHAPES 7

#define SQUARE_SIZE SCREEN_HEIGHT / (BOARD_HEIGHT + 5)
#define BOARD_X ((SCREEN_WIDTH - SQUARE_SIZE * BOARD_WIDTH) / 2)
#define BOARD_Y ((SCREEN_HEIGHT - SQUARE_SIZE * BOARD_HEIGHT) / 2)
#define START_X (BOARD_WIDTH / 2 - 1)
#define DUMPED_PIECE 3
#define COLOR_DUMPED 7
#define EMPTY_SPACE 0

int board[BOARD_HEIGHT][BOARD_WIDTH] = {{0}};

typedef struct
{	
	int fields[4][4];
	int shape;
	int rotation;
	int color;
	int x;
	int y;
	bool is_dump;
}Piece;

void draw_screen();
void draw_grid();
void draw_square(int x_pos, int y_pos, int color);
Piece init_piece(int shape, int rotation, int color);
void random_piece();
void draw_piece(Piece piece);
void draw_dumped_pieces(); 
void print_board();
void add_on_board(Piece piece);
int find_left(Piece piece);
int find_top(Piece piece);
int find_right(Piece *piece);
int find_bottom_y(Piece piece);
void move_Y(Piece *piece);
void input();
void move_x(Piece *piece, int change_x);
bool check_collision_bottom(Piece *piece);
void change_fields_into_dump(Piece *piece);
void check_and_add_new_piece(Piece *piece);
void rotate(Piece *piece);

int main(int argc, char *argv[])
{

	if (gfx_init())
	{
		exit(3);
	}
	srand(time(NULL));

	Piece current_piece;
	random_piece(&current_piece);
	// current_piece = init_piece(3, 3, 2);
	printf("%d \n", find_right(&current_piece));

	while (true)
	{
		draw_screen();
		
		draw_piece(current_piece);
		draw_grid();
		
		print_board();
		input(&current_piece);
		move_Y(&current_piece);
		draw_dumped_pieces();
		check_and_add_new_piece(&current_piece);
		SDL_Delay(500);
		
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
	piece.is_dump = false;

	for (int row = 0; row < PIECE_SIZE; row++){
		for (int col = 0; col < PIECE_SIZE; col++){
			piece.fields[row][col] = pieces[shape][rotation][row][col];
		}
	}

	// piece.x = rand() % BOARD_WIDTH - find_left(piece);
	piece.x = BOARD_WIDTH / 2 - 1;
	piece.y = -find_top(piece);
	
	add_on_board(piece);

	return piece;
}


void random_piece(Piece *piece)
{
	int piece_index = rand() % SHAPES;
	int rotation_index = rand() % ROTATIONS;
	int color_index = rand() % COLORS + 1;
	
	*piece = init_piece(piece_index, rotation_index, color_index);
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
int find_bottom_y(Piece piece){
	for (int row = PIECE_SIZE-1; row >= 0; row--) {
        for (int col = 0; col < PIECE_SIZE; col++) {
            if (piece.fields[row][col]) {
                return row + piece.y;
            }
        }
	}
	return 0;
}

int find_right(Piece *piece)
{
	for (int row = 0; row < PIECE_SIZE; row++){
		for (int col = PIECE_SIZE -1; col >= 0; col--){
			if (piece->fields[row][col]){
				return col + piece->x;
			}
		}
	}
	return 0;
}

void draw_piece(Piece piece) {
    for (int row = 0; row < PIECE_SIZE; row++) {
        for (int col = 0; col < PIECE_SIZE; col++) {
            if (piece.fields[row][col] == 1 || piece.fields[row][col] == 2) {
                draw_square(piece.x + col, piece.y + row, piece.color);
            }
			else if(piece.fields[row][col] == 3){
			draw_square(piece.x + col, piece.y + row, COLOR_DUMPED);
			}
        }
    }
}

void draw_dumped_pieces() {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board[y][x] == DUMPED_PIECE) {
                draw_square(x, y, COLOR_DUMPED);
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
	printf("\n");
}

bool check_collision_bottom(Piece *piece)
{
    // Check if piece exceeds the left or right board boundary
    if (piece->x < 0 || find_right(piece) >= BOARD_WIDTH) {
        return true;
    }

    // Check if the bottom row of the piece reaches the bottom of the board
    if (find_bottom_y(*piece) == BOARD_HEIGHT - 1) {
        return true;
    }

    // Check for collision with dumped pieces on the board
    for (int i = PIECE_SIZE - 1; i >= 0; i--) {
        for (int j = 0; j < PIECE_SIZE; j++) {
            if (piece->fields[i][j]) {
                int new_x = piece->x + j;
                int new_y = find_bottom_y(*piece) + i; // Next row below the piece
                if (board[new_y][new_x] == DUMPED_PIECE) {
                    return true;
                }
            }
        }
    }
    return false;
}


void move_Y(Piece *piece) {
	bool can_dump = check_collision_bottom(piece);
    if (!can_dump) {
		for (int i = 0; i < PIECE_SIZE; i++){
			for (int j = 0; j < PIECE_SIZE; j++){
				if(piece->fields[i][j]){
					board[piece->y + i][piece->x + j] = EMPTY_SPACE;
				}
			}
		}
		piece->y++;
	}

	add_on_board(*piece);

	if (can_dump){
		for (int i = 0; i < PIECE_SIZE; i++){
			for (int j = 0; j < PIECE_SIZE; j++){
				piece->is_dump = true;
				change_fields_into_dump(piece);
			}
			break;
		}
	}
}

void change_fields_into_dump(Piece *piece){
	if (piece->is_dump){
		for (int i = 0; i < PIECE_SIZE; i++){
			for (int j = 0; j < PIECE_SIZE; j++){
				if (piece->fields[i][j]){
					piece->fields[i][j] = DUMPED_PIECE;
				}
			}
		}
	}
}

void check_and_add_new_piece(Piece *piece) {
    if (piece->is_dump) {
        add_on_board(*piece);
        random_piece(piece);
    }
}

void input(Piece *piece)
{
	int key = gfx_pollkey();

	switch(key){
		case SDLK_ESCAPE:
			exit(4);
			break;
		case SDLK_SPACE:
			rotate(piece);
			break;
		case SDLK_LEFT:
			move_x(piece, -1);
			break;
		case SDLK_RIGHT:
			move_x(piece, 1);
			break;
	}

}

void move_x(Piece *piece, int change_x)
{
    // Temporarily remove the piece from the board
	if (!check_collision_bottom(piece)){
    	for (int i = 0; i < PIECE_SIZE; i++){
        	for (int j = 0; j < PIECE_SIZE; j++){
            	if (piece->fields[i][j]){
                	board[piece->y + i][piece->x + j] = EMPTY_SPACE;
        	}
		}
	}

    // Update the piece position
    piece->x += change_x;

    // Add the piece back to the board
    add_on_board(*piece);
	}
}
void rotate(Piece *piece)
{
	int new_rot = (piece->rotation + 1) % ROTATIONS;
	Piece rotated = init_piece(piece->shape, new_rot, piece->color);

	*piece = rotated;

	add_on_board(*piece);
}