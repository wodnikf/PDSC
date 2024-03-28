#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define EMPTY_STACK -1

#define FLOOR_HEIGHT 25
#define PEG_WIDTH 10

#define NUMBER_PEGS 3
#define NUMBER_DISC 3
#define MAX_DISC_HEIGHT 40
#define BASE_DISC_WIDTH 5

typedef struct 
{
	int x_left;
	int x_right;
	int y_top;
	int y_bot;
	int size;
} Disc;

typedef struct
{
	Disc discs[NUMBER_DISC];
	int top;
} Peg;

bool is_empty(Peg* pegs);
bool is_full(Peg* pegs, int peg_index);
void push(Peg* pegs, Disc disc);
Disc pop(Peg* pegs);
Disc peek(Peg* pegs);
int get_peg_x(int index);
int get_disc_height();
int get_peg_height();
void init_pegs(Peg* pegs);
double get_distance_between_poles();
double get_disc_width(int disc_size);
void init_disc(Peg* pegs);
void draw_disc(Peg *pegs);
int get_animation_height();
void draw_pegs();
int input();
void draw_screen();
void draw_floor();
void draw_common_elements(Peg *pegs);
void draw_on_screen(Peg *pegs);
int sign(int expression);
void animation(Peg *pegs, Disc *disc);
void movement_up(Disc *disc, Peg *pegs);
void movement_left_or_right(Disc *disc, Peg *pegs, int source, int destination);
void movement_down(Disc *disc, Peg *pegs, int destination);
void animation_movement(Peg *pegs, Disc *disc, int source, int destination);
bool legal_move(Peg *pegs, int source, int destination);
void move_disc(Peg *pegs, int source, int destination);
bool check_win(Peg *pegs);
void draw_win_screen();

int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	Peg pegs[NUMBER_PEGS];
	init_pegs(pegs);
	init_disc(pegs);

	while(1){
		draw_on_screen(pegs);

		int source = input();
		int destination = input();

        move_disc(pegs, source, destination);

		SDL_Delay(10);

        if (check_win(pegs)){
            draw_win_screen();
        }
	}
	return 0;
}

bool is_empty(Peg* pegs) {
    if (pegs->top == EMPTY_STACK) {
        return true;
    }
    return false;
}

bool is_full(Peg* pegs, int peg_index) {
    if (pegs[peg_index].top == NUMBER_DISC - 1) {
        return true;
    }
    return false;
}


void push(Peg* pegs, Disc disc)
{
    if (pegs->top == NUMBER_DISC - 1) {
        printf("The stack is full.\n");
        exit(1);
    }

    pegs->top++;
    pegs->discs[pegs->top] = disc;
}

Disc pop(Peg* pegs)
{
    if (pegs->top == EMPTY_STACK) {
        printf("The stack is empty.\n");
        exit(1);
    }

    Disc popped_disc = pegs->discs[pegs->top];
    pegs->top--;

    return popped_disc;
}

Disc peek(Peg* pegs)
{
    if (pegs->top == EMPTY_STACK) {
        printf("The stack is empty.\n");
        exit(1);
    }

    return pegs->discs[pegs->top];
}

int get_peg_x(int index){
	int x = gfx_screenWidth() / (NUMBER_PEGS + 1) * index;
	return x;
}

int get_disc_height()
{
    int height = (gfx_screenHeight() - 100 - FLOOR_HEIGHT) / NUMBER_DISC;
    if (height > MAX_DISC_HEIGHT){
        return MAX_DISC_HEIGHT;
    }
    return height;
}

int get_peg_height(){
	return get_disc_height() * (NUMBER_DISC + 2);
}

void init_pegs(Peg* pegs)
{
    for (int i = 0; i < NUMBER_PEGS; i++) {
        pegs[i].top = EMPTY_STACK;
    }
}

double get_distance_between_poles()
{
    double distance = get_peg_x(0) + PEG_WIDTH + get_peg_x(1);
    return distance;
}

double get_disc_width(int disc_size)
{
    double distance_between_poles = get_distance_between_poles();
    double total_space_for_disc = (distance_between_poles / 2) - 10;
    double disc_width = total_space_for_disc / (NUMBER_DISC * 2 - 1);
    return disc_width;
}

void init_disc(Peg* pegs)
{
    for (int i = NUMBER_DISC; i > 0; i--) {
        Disc new_disc;
        new_disc.x_left = get_peg_x(1) - get_disc_width(i) * (i + 1);
        new_disc.x_right = get_peg_x(1) + PEG_WIDTH + get_disc_width(i) * (i+1);
        new_disc.y_bot = gfx_screenHeight() - FLOOR_HEIGHT - get_disc_height() * (NUMBER_DISC - i);
        new_disc.y_top = gfx_screenHeight() - FLOOR_HEIGHT - get_disc_height() * (NUMBER_DISC - i + 1);
        new_disc.size = i;
        push(&pegs[0], new_disc);
    }
}

void draw_disc(Peg *pegs)
{
    for (int i = 0; i < NUMBER_PEGS; i++) {
        Peg current_peg = pegs[i];
        for (int j = 0; j <= current_peg.top; j++) {
            Disc current_disc = current_peg.discs[j];
            gfx_filledRect(current_disc.x_left, current_disc.y_bot, 
                           current_disc.x_right, current_disc.y_top, BLUE);
        }
    }
}

int get_animation_height()
{
    return gfx_screenHeight() - FLOOR_HEIGHT - get_peg_height() - (3 * get_disc_height());
}

void draw_pegs()
{
	for (int i = 0; i < NUMBER_PEGS; i++){
		
		gfx_filledRect(get_peg_x(i + 1),
               gfx_screenHeight() - FLOOR_HEIGHT - get_peg_height(),
               get_peg_x(i + 1) + PEG_WIDTH,
               gfx_screenHeight() - FLOOR_HEIGHT,
               GREEN);
	}
}

int input()
{
    int key = gfx_getkey();
    int key_value = -1;
    switch (key) {
        case SDLK_ESCAPE:
            exit(0);
            break;
        case SDLK_0:
            if(NUMBER_PEGS == 10){
                key_value = 9;
            }
            break;
        default:
            if (key >= '1' && key <= '1' + NUMBER_PEGS - 1) {
                key_value = key - '1';
			}
            break;
    }
    return key_value;
}

void draw_screen()
{
	gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
						BLACK);
}

void draw_floor()
{
	gfx_filledRect(0, gfx_screenHeight() - FLOOR_HEIGHT,
			gfx_screenWidth() -1,
			gfx_screenHeight() - 1,
			YELLOW);
}

void draw_common_elements(Peg *pegs)
{
    draw_screen();
    draw_floor();
    draw_pegs();
    draw_disc(pegs);
}

void draw_on_screen(Peg *pegs)
{
    draw_common_elements(pegs);
	gfx_updateScreen();
}

int sign(int expression)
{
    if (expression > 0){
        return 1;
    }
    return -1;
}

void animation(Peg *pegs, Disc *disc)
{
    draw_common_elements(pegs);

    gfx_filledRect(disc->x_left, disc->y_top, disc->x_right, disc->y_bot, BLUE);

    gfx_updateScreen();

    SDL_Delay(1);
}

void movement_up(Disc *disc, Peg *pegs)
{
     while(disc->y_bot != get_animation_height()){
        disc->y_bot--;
        disc->y_top--;
        animation(pegs, disc);
    }
}

void movement_left_or_right(Disc *disc, Peg *pegs, int source, int destination)
{
    int goal_x = get_peg_x(destination + 1) - get_disc_width(disc->size) * (disc->size + 1);
    while (disc->x_left != goal_x){
        disc->x_left += sign(destination - source);
        disc->x_right += sign(destination - source);
        animation(pegs, disc);
    }
}

void movement_down(Disc *disc, Peg *pegs, int destination)
{
    int goal_height = gfx_screenHeight() - FLOOR_HEIGHT - (pegs[destination].top + 1) * get_disc_height();

    while((int)disc->y_bot != goal_height){
        disc->y_bot++;
        disc->y_top++;
        animation(pegs, disc);
    }
}


void animation_movement(Peg *pegs, Disc *disc, int source, int destination)
{
    movement_up(disc, pegs);

    movement_left_or_right(disc, pegs, source, destination);
    
    movement_down(disc, pegs, destination);
}

bool legal_move(Peg *pegs, int source, int destination)
{
    if (source == -1 || destination == -1){
        return false;
    }

    if (is_empty(&pegs[source])){
        return false;
    }
    if (is_empty(&pegs[destination])){
        return true;
    }

    int size_source = peek(&pegs[source]).size;
    int size_destination = peek(&pegs[destination]).size;

    if (size_source < size_destination){
        return true;
    }
    return false;
}

void move_disc(Peg *pegs, int source, int destination) {
    if (legal_move(pegs, source, destination)) {
        Disc disc_to_move = pop(&pegs[source]);
        animation_movement(pegs, &disc_to_move, source, destination);
        push(&pegs[destination], disc_to_move);
    }
}

bool check_win(Peg *pegs){
    if (is_full(pegs, NUMBER_PEGS -1)){
        return true;
    }
    return false;
}

void draw_win_screen()
{
    draw_screen();
    gfx_textout(gfx_screenWidth() / 2 - 100, gfx_screenHeight() / 2, "Congratulations! YOU WON!", RED);
    gfx_updateScreen();
    SDL_Delay(5000);
    exit(0);
}