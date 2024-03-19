#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define EMPTY_STACK -1

#define FLOOR_HEIGHT 25
#define PEG_WIDTH 10

#define NUMBER_PEGS 3
#define NUMBER_DISC 4
#define BASE_DISC_HEIGHT 15
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

int get_peg_height(){
	return BASE_DISC_HEIGHT * (NUMBER_DISC + 2);
}

void init_pegs(Peg* pegs)
{
    for (int i = 0; i < NUMBER_PEGS; i++) {
        pegs[i].top = EMPTY_STACK;
    }
}

void init_disc(Peg* pegs)
{
    for (int i = NUMBER_DISC; i > 0; i--) {
        Disc new_disc;
        new_disc.x_left = get_peg_x(1) - BASE_DISC_WIDTH * (i + 1);
        new_disc.x_right = get_peg_x(1) + PEG_WIDTH + BASE_DISC_WIDTH * (i + 1);
        new_disc.y_bot = gfx_screenHeight() - FLOOR_HEIGHT - BASE_DISC_HEIGHT * (NUMBER_DISC - i);
        new_disc.y_top = gfx_screenHeight() - FLOOR_HEIGHT - BASE_DISC_HEIGHT * (NUMBER_DISC - i + 1);
        new_disc.size = i;
        push(&pegs[0], new_disc);

        printf("Disc %d - X: %d, Y: %d\n", new_disc.size, new_disc.x_left, new_disc.y_bot);
    }
}

void draw_disc(Peg *pegs) {
    for (int i = 0; i < NUMBER_PEGS; i++) {
        Peg current_peg = pegs[i];
        for (int j = 0; j <= current_peg.top; j++) {
            Disc current_disc = current_peg.discs[j];
            gfx_filledRect(current_disc.x_left, current_disc.y_bot, 
                           current_disc.x_right, current_disc.y_top, BLUE);
        }
    }
}

bool legal_move(Peg *pegs, int current, int destination){
	Disc top_current = peek(&pegs[current]);
    Disc top_destination = peek(&pegs[destination]);

	if (top_current.size < top_destination.size) {	
	return true;
	}
	return false;
}

void move_disc(Peg *pegs, int current, int destination){
	if (!legal_move(pegs, current, destination)){
		return;
	}
	Disc disc_to_move = pop(&pegs[current]);
	push(&pegs[destination], disc_to_move);
}


void draw_pegs(){
	for (int i = 0; i < NUMBER_PEGS; i++){
		
		gfx_filledRect(get_peg_x(i + 1),
               gfx_screenHeight() - FLOOR_HEIGHT - get_peg_height(),
               get_peg_x(i + 1) + PEG_WIDTH,
               gfx_screenHeight() - FLOOR_HEIGHT,
               GREEN);
	}
}

void draw_screen(){
	gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
						BLACK);
}

void draw_floor(){
	gfx_filledRect(0, gfx_screenHeight() - FLOOR_HEIGHT,
			gfx_screenWidth() -1,
			gfx_screenHeight() - 1,
			YELLOW);
}

void draw_on_screen(Peg *pegs){
	
	draw_screen();

	draw_floor();

	draw_pegs();
	draw_disc(pegs);

	gfx_updateScreen();

}

int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	Peg pegs[NUMBER_PEGS];


	init_pegs(pegs);
	init_disc(pegs);

	peek(&pegs[0]);

	
	while(1){
		
		draw_on_screen(pegs);

		SDL_Delay(10);

		if(gfx_pollkey() == SDLK_ESCAPE){
			break;
		}
	}

	return 0;
}
