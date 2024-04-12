#include "primlib.h"
#include "pieces.inl"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define SCREEN_HEIGHT gfx_screenHeight()
#define SCREEN_WIDTH gfx_screenWidth()

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define PIECE_SIZE 4
#define ROTATIONS 4
#define COLORS 5
#define SHAPES 7

#define SQUARE_SIZE (SCREEN_HEIGHT / (BOARD_HEIGHT + 5))
#define BOARD_X ((SCREEN_WIDTH - SQUARE_SIZE * BOARD_WIDTH) / 2)
#define BOARD_Y ((SCREEN_HEIGHT - SQUARE_SIZE * BOARD_HEIGHT) / 2)
#define START_X (BOARD_WIDTH / 2 - 1)
#define ROTATION_AXIS 2
#define DUMPED_PIECE 3
#define COLOR_DUMPED 7
#define EMPTY_SPACE 0
#define DELAY 150

int board[BOARD_HEIGHT][BOARD_WIDTH] = {{0}};

typedef struct
{
	int fields[4][4];
	int number_active_fields[2];
	int active_fields[4][2];
	int shape;
	int rotation;
	int color;
	int x;
	int y;
	bool is_dump;
	int rot_axis_x;
	int rot_axis_y;
} Piece;

typedef struct
{
	int shape_index;
	int color_index;
	int rotation_index;
} Information;

Information array[2];

void draw_screen();
void draw_grid();
void draw_square(int x_pos, int y_pos, int color);
Piece init_piece(int shape, int rotation, int color);
void random_piece();
void add_on_board(Piece piece);
int find_left(Piece *piece);
int find_right(Piece *piece);
int find_top(Piece piece);
int find_bottom_y(Piece piece);
void draw_piece(Piece piece);
void draw_dumped_pieces();
bool check_collision_bottom(Piece *piece);
void remove_spaces(Piece *piece);
void move_Y(Piece *piece);
void change_fields_into_dump(Piece *piece);
void draw_next(Piece *piece);
void check_and_add_new_piece(Piece *piece);
void input(Piece *piece);
bool check_collision_right(Piece *piece);
bool check_collision_left(Piece *piece);
void move_x(Piece *piece, int change_x);
void find_rot_axis(Piece *piece);
bool can_rotate(Piece *piece, int delta_x, int delta_y);
void rotate(Piece *piece);
void draw_all(Piece *piece);
void game_loop(Piece *piece);
void fast_fall(Piece *piece);
void check_and_clear_rows();
bool check_lose();
void draw_end_screen();
void next_piece(Piece *piece);

int main(int argc, char *argv[])
{
	if (gfx_init())
	{
		exit(3);
	}
	srand(time(NULL));

	array[0].shape_index = rand() % SHAPES;
	array[0].rotation_index = rand() % ROTATIONS;
	array[0].color_index = rand() % COLORS + 1;

	Piece current_piece;

	next_piece(&current_piece);

	game_loop(&current_piece);

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
	gfx_filledRect(BOARD_X + x_pos * SQUARE_SIZE, BOARD_Y + y_pos * SQUARE_SIZE, BOARD_X + (x_pos + 1) * SQUARE_SIZE, BOARD_Y + (y_pos + 1) * SQUARE_SIZE, color);
}

Piece init_piece(int shape, int rotation, int color)
{
	Piece piece;
	piece.shape = shape;
	piece.rotation = rotation;
	piece.color = color;
	piece.is_dump = false;

	for (int row = 0; row < PIECE_SIZE; row++)
	{
		for (int col = 0; col < PIECE_SIZE; col++)
		{
			piece.fields[row][col] = pieces[shape][rotation][row][col];
		}
	}

	piece.x = BOARD_WIDTH / 2 - 1;
	piece.y = 0;

	find_rot_axis(&piece);

	add_on_board(piece);

	return piece;
}

void random_piece()
{

	array[1].shape_index = rand() % SHAPES;
	array[1].rotation_index = rand() % ROTATIONS;
	array[1].color_index = rand() % COLORS + 1;
}

void add_on_board(Piece piece)
{
	for (int row = 0; row < PIECE_SIZE; row++)
	{
		for (int col = 0; col < PIECE_SIZE; col++)
		{
			if (piece.fields[row][col])
			{
				board[piece.y + row][piece.x + col] = piece.fields[row][col];
			}
		}
	}
}

int find_left(Piece *piece)
{
	return piece->x;
}

int find_right(Piece *piece)
{
	int temp = -1;
	for (int row = 0; row < PIECE_SIZE; row++)
	{
		for (int col = PIECE_SIZE - 1; col >= 0; col--)
		{
			if (piece->fields[row][col])
			{
				if (col > temp)
				{
					temp = col;
				}
				break;
			}
		}
	}
	return temp + piece->x;
}

int find_top(Piece piece)
{
	for (int row = 0; row < PIECE_SIZE; row++)
	{
		for (int col = 0; col < PIECE_SIZE; col++)
		{
			if (piece.fields[row][col])
			{
				return row + piece.y;
			}
		}
	}
	return 0;
}

int find_bottom_y(Piece piece)
{
	for (int row = PIECE_SIZE - 1; row >= 0; row--)
	{
		for (int col = 0; col < PIECE_SIZE; col++)
		{
			if (piece.fields[row][col])
			{
				return row + piece.y;
			}
		}
	}
	return 0;
}

void draw_piece(Piece piece)
{
	for (int row = 0; row < PIECE_SIZE; row++)
	{
		for (int col = 0; col < PIECE_SIZE; col++)
		{
			if (piece.fields[row][col] == 1)
			{
				draw_square(piece.x + col, piece.y + row, piece.color);
			}
			else if (piece.fields[row][col] == 2)
			{
				draw_square(piece.x + col, piece.y + row, YELLOW);
			}
		}
	}
}

void draw_dumped_pieces()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (board[y][x] == DUMPED_PIECE)
			{
				draw_square(x, y, COLOR_DUMPED);
			}
		}
	}
}

bool check_collision_bottom(Piece *piece)
{
	if (find_bottom_y(*piece) == BOARD_HEIGHT - 1)
	{
		return true;
	}
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		for (int j = 0; j < PIECE_SIZE; j++)
		{
			if (piece->fields[i][j] && board[piece->y + i + 1][piece->x + j] == DUMPED_PIECE)
			{
				return true;
			}
		}
	}

	return false;
}

void remove_spaces(Piece *piece)
{
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		for (int j = 0; j < PIECE_SIZE; j++)
		{
			if (piece->fields[i][j])
			{
				board[piece->y + i][piece->x + j] = EMPTY_SPACE;
			}
		}
	}
}

void move_Y(Piece *piece)
{
	bool can_dump = check_collision_bottom(piece);
	if (!can_dump)
	{
		remove_spaces(piece);
		piece->y++;
	}

	add_on_board(*piece);

	if (can_dump)
	{
		for (int i = 0; i < PIECE_SIZE; i++)
		{
			for (int j = 0; j < PIECE_SIZE; j++)
			{
				piece->is_dump = true;
				change_fields_into_dump(piece);
			}
			break;
		}
	}
}

void change_fields_into_dump(Piece *piece)
{
	if (piece->is_dump)
	{
		for (int i = 0; i < PIECE_SIZE; i++)
		{
			for (int j = 0; j < PIECE_SIZE; j++)
			{
				if (piece->fields[i][j])
				{
					piece->fields[i][j] = DUMPED_PIECE;
				}
			}
		}
	}
}

void draw_next(Piece *piece)
{
	Piece to_draw;
	to_draw.color = array->color_index;
	to_draw.rotation = array->rotation_index;
	to_draw.shape = array->shape_index;

	for (int row = 0; row < PIECE_SIZE; row++)
	{
		for (int col = 0; col < PIECE_SIZE; col++)
		{

			to_draw.fields[row][col] = pieces[array[1].shape_index][array[1].rotation_index][row][col];

			if (to_draw.fields[row][col] == 1)
			{
				draw_square(BOARD_WIDTH + 5 + col, 5 + row, array[1].color_index);
			}
			else if (to_draw.fields[row][col] == 2)
			{
				draw_square(BOARD_WIDTH + 5 + col, 5 + row, YELLOW);
			}
		}
	}
}

void check_and_add_new_piece(Piece *piece)
{
	if (piece->is_dump)
	{
		add_on_board(*piece);
		next_piece(piece);
	}
}

void input(Piece *piece)
{
	int key = gfx_pollkey();

	switch (key)
	{
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
	case SDLK_DOWN:
		fast_fall(piece);
		break;
	}
}

bool check_collision_right(Piece *piece)
{
	if (find_right(piece) >= BOARD_WIDTH - 1)
	{
		return true;
	}

	for (int i = 0; i < PIECE_SIZE; i++)
	{
		for (int j = 0; j < PIECE_SIZE; j++)
		{
			if (piece->fields[i][j] && board[piece->y + i][piece->x + j + 1] == DUMPED_PIECE)
			{
				return true;
			}
		}
	}
	return false;
}

bool check_collision_left(Piece *piece)
{
	if (piece->x <= 0)

	{
		return true;
	}

	for (int i = 0; i < PIECE_SIZE; i++)
	{
		for (int j = 0; j < PIECE_SIZE; j++)
		{
			if (piece->fields[i][j] && board[piece->y + i][piece->x + j - 1] == DUMPED_PIECE)
			{
				return true;
			}
		}
	}
	return false;
}

void move_x(Piece *piece, int change_x)
{

	if (check_collision_right(piece) && change_x == 1)
	{
		return;
	}
	if (check_collision_left(piece) && change_x == -1)
	{
		return;
	}

	remove_spaces(piece);

	piece->x += change_x;

	add_on_board(*piece);
}

void find_rot_axis(Piece *piece)
{
	for (int i = 0; i < PIECE_SIZE; i++)
	{
		for (int j = 0; j < PIECE_SIZE; j++)
		{
			if (piece->fields[i][j] == ROTATION_AXIS)
			{
				piece->rot_axis_y = i + piece->y;
				piece->rot_axis_x = j + piece->x;
			}
		}
	}
}

bool can_rotate(Piece *piece, int delta_x, int delta_y)
{
	Piece rotated = *piece;
	rotated.rotation = (rotated.rotation + 1) % ROTATIONS;

	rotated.x += delta_x;
	rotated.y += delta_y;

	if (rotated.x < 0 || rotated.x >= BOARD_WIDTH || rotated.y >= BOARD_HEIGHT || rotated.y < 0)
		return false;

	for (int i = 0; i < PIECE_SIZE; i++)
	{
		for (int j = 0; j < PIECE_SIZE; j++)
		{
			if (rotated.fields[i][j] && board[rotated.y + i][rotated.x + j] == DUMPED_PIECE)
			{
				return false;
			}
		}
	}

	if (find_right(&rotated) >= BOARD_WIDTH || find_left(&rotated) < 0)
		return false;

	return true;
}

void rotate(Piece *piece)
{
	Piece rotated = init_piece(piece->shape, (piece->rotation + 1) % ROTATIONS, piece->color);

	rotated.x = piece->x;
	rotated.y = piece->y;

	int diff_x = piece->rot_axis_x - rotated.rot_axis_x;
	int diff_y = piece->rot_axis_y - rotated.rot_axis_y;

	rotated.x -= -diff_x;
	rotated.y -= -diff_y;

	for (int i = 0; i < PIECE_SIZE; i++)
	{
		if (find_left(&rotated) <= 0)
		{
			rotated.x = 0;
			diff_x = 0;
		}
		if (find_right(&rotated) >= BOARD_WIDTH - 1)
		{
			rotated.x -= 1;
			diff_x = 0;
		}
		if (find_top(rotated) <= 0)
		{
			rotated.y += 1;
		}
		if (find_bottom_y(rotated) >= BOARD_HEIGHT - 1)
		{
			rotated.y -= 1;
		}
	}

	if (can_rotate(&rotated, diff_x, diff_y))
	{
		*piece = rotated;
	}

	add_on_board(*piece);
}

void draw_all(Piece *piece)
{
	draw_screen();
	draw_grid();
	draw_piece(*piece);
	draw_dumped_pieces();
	draw_next(piece);
	gfx_updateScreen();
}

void game_loop(Piece *piece)
{
	int counter = 0;
	while (true)
	{
		draw_all(piece);
		input(piece);

		if (counter++ >= DELAY)
		{
			counter = 0;
			move_Y(piece);
		}

		if (check_lose(piece))
		{
			draw_end_screen();
			exit(1);
		}

		check_and_clear_rows();

		check_and_add_new_piece(piece);
	}
}

void fast_fall(Piece *piece)
{
	while (!check_collision_bottom(piece))
	{
		move_Y(piece);
	}
	move_Y(piece);
}

void check_and_clear_rows()
{
	for (int i = BOARD_HEIGHT - 1; i >= 0; i--)
	{
		bool is_full = true;
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			if (board[i][j] != DUMPED_PIECE)
			{
				is_full = false;
				break;
			}
		}
		if (is_full)
		{
			for (int k = i; k > 0; k--)
			{
				for (int j = 0; j < BOARD_WIDTH; j++)
				{
					board[k][j] = board[k - 1][j];
				}
				for (int j = 0; j < BOARD_WIDTH; j++)
				{
					board[0][j] = EMPTY_SPACE;
				}
			}
		}
	}
}

bool check_lose()
{
	for (int i = BOARD_WIDTH / 2 - 2; i < BOARD_WIDTH / 2 + 2; i++)
	{

		if (board[0][i] == DUMPED_PIECE)
		{
			return true;
		}
	}
	return false;
}

void draw_end_screen()
{
	draw_screen();

	char *message = "GAME OVER";

	int lenght_message = strlen(message);

	gfx_textout(SCREEN_WIDTH / 2 - lenght_message, SCREEN_HEIGHT / 2, message, WHITE);
	gfx_updateScreen();
	SDL_Delay(5000);
}

void next_piece(Piece *piece)
{
	*piece = init_piece(array[0].shape_index, array[0].rotation_index, array[0].color_index);
	random_piece(array);
	array[0] = array[1];
}
