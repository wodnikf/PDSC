#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>


#define EMPTY_STACK -1

#define FLOOR_HEIGHT 25
#define BASE_PEG_HEIGHT 1000
#define PEG_WIDTH 10

#define NUMBER_PEGS 3
#define NUMBER_DISK 40
#define DISK_HEIGHT 15


typedef struct 
{
	int x_left;
	int x_right;
	int y_top;
	int y_bot;
	int index;
}Disk;


// stack
typedef struct
{
	Disk disks[NUMBER_DISK];
	int top;
}Peg;

void push(Peg *pegs, Disk index){

	if (pegs->top == NUMBER_DISK -1){
		return;
	}

	pegs->top++;
	pegs->disks[pegs->top] = index;
}

Disk pop(Peg *pegs){
	Disk popped_disk;

	if (pegs->top == EMPTY_STACK){
		popped_disk.index = EMPTY_STACK;
		return popped_disk;
	}

	popped_disk = pegs->disks[pegs->top];
	pegs->top--;

	return popped_disk;
}

Disk peek(Peg *pegs) {
    Disk peeked_disk;

    if (pegs->top == EMPTY_STACK) {
        printf("The stack is empty.\n");
        peeked_disk.index = EMPTY_STACK;
        return peeked_disk;
    }

    peeked_disk = pegs->disks[pegs->top];
    printf("Top disk index: %d\n", peeked_disk.index);
    return peeked_disk;
}



// stack


int get_peg_x(int index){
	int x = gfx_screenWidth() / (NUMBER_PEGS + 1) * index;
	return x;
}

int get_peg_height(){
	int height = BASE_PEG_HEIGHT / NUMBER_PEGS;
	return height;
}


void init_pegs(Peg *pegs){
	for (int i = 0; i < NUMBER_PEGS; i++){
		Peg new_peg;
		new_peg.top = EMPTY_STACK;
		pegs[i] = new_peg;
	}
}

void init_disk(Peg *pegs){
    for(int i = NUMBER_DISK; i > 0; i--){
        Disk new_disk;
        new_disk.x_left = get_peg_x(1) - 5 * (i+1);
        new_disk.x_right = get_peg_x(1) + PEG_WIDTH + 5 * (i+1);
        new_disk.y_bot = gfx_screenHeight() - FLOOR_HEIGHT - DISK_HEIGHT * (NUMBER_DISK - i);
        new_disk.y_top = gfx_screenHeight() - FLOOR_HEIGHT - DISK_HEIGHT * (NUMBER_DISK - i + 1);
        new_disk.index = i;
        push(&pegs[0], new_disk);

		printf("Disk %d - X: %d, Y: %d\n", new_disk.index, new_disk.x_left, new_disk.y_bot);
    }
}



void draw_disk(Peg *pegs) {
    for (int i = 0; i < NUMBER_PEGS; i++) {
        Peg current_peg = pegs[i];
        for (int j = 0; j <= current_peg.top; j++) {
            Disk current_disk = current_peg.disks[j];
            gfx_filledRect(current_disk.x_left, current_disk.y_bot, 
                           current_disk.x_right, current_disk.y_top, BLUE);
        }
    }
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


int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	Peg pegs[NUMBER_PEGS];


	init_pegs(pegs);
	init_disk(pegs);

	peek(&pegs[0]);

	
	while(1){
		gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
						BLACK);

		gfx_filledRect(0, gfx_screenHeight() - FLOOR_HEIGHT, gfx_screenWidth() -1, gfx_screenHeight() - 1, YELLOW);
		
		gfx_line(0, 540, gfx_screenWidth()-1, 540, RED);
		gfx_line(0, 675, gfx_screenWidth()-1, 675, MAGENTA);

		draw_pegs();
		draw_disk(pegs);

		gfx_updateScreen();
		SDL_Delay(10);

		if(gfx_pollkey() == SDLK_ESCAPE){
			break;
		}
	}

	return 0;
}
