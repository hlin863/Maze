//Purpy Pupple's amazing maze generator. 
//Released under the CC-BY-SA 3.0 License and the GFDL
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#define UP 0     //-y
#define DOWN 1   //+y
#define LEFT 2   //-x
#define RIGHT 3  //+x
#define OUTFILE "MAZE"
#define WHITE fprintf(outfile, "%c%c%c", 255,255,255)
#define BLACK fprintf(outfile, "%c%c%c", 0,0,0)
#define RED   fprintf(outfile, "%c%c%c", 0,0,255)
#define BLUE  fprintf(outfile, "%c%c%c", 0,255,0)

#define nodeadend//generate a maze without any dead ends! (consequently, many solutions to maze)
//#define prim    //enable this to generate mazes using prim's algorithm.
#define backtrack//enable this to generate mazes using depth-first search. Don't enable both.
//#define movie   //this option spams bitmaps to illustrate each step of generation.
#define prim //enable this to generate mazes using prim's algorithm.

long numin=1;     //Number of cells in the maze.
#define xsize 5
#define ysize 5

void initialize();
void generate();
void savebmp(int xspecial, int yspecial);

struct cell{
	bool in;  //Is this cell in the maze?
	bool up;  //Does the wall above this cell exist?
	bool left;//Does the wall to the left of this cell exist?
	bool visited; //Has this cell been visited?
	int prevx, prevy; //The coordinates of the previous cell, used for backtracking.
};

struct cell MAZE[xsize][ysize];

// void traverse();

int *get_start_point();

int *get_destination();

void traverse(int x1, int y1, int x2, int y2);

bool within_bounds(int x, int y);

void display_maze();

int main(){
	srand((unsigned int)time(NULL)); //seed random number generator with system time
	initialize();      //initialize the maze
	generate();        //generate the maze

    /*
	for(int i=1;i<10;i++){
		numin++;
		savebmp(0,0);      //output the bitmap
	} */

	savebmp(0,0);

    int *start = get_start_point();

    printf("start: (%d %d)\n", start[0], start[1]);

    int *destination = get_destination();

	printf("destination: (%d %d)\n", destination[0], destination[1]);

    traverse(start[0], start[1], destination[0], destination[1]);

    // printf("done\n");

	return 0;
}

void display_maze(){
	for (int i = 0; i < xsize; i++){
		for (int j = 0; j < ysize; j++){
			// printf("%d", MAZE[i][j].in);
			// if (MAZE[i][j].in){
			// 	if (MAZE[i][j].visited){
			// 		printf("*");
			// 	} else {
			// 		printf(" ");
			// 	}
			// } else {
			// 	printf("#");
			// }
		}
		printf("\n");
	}
}

bool within_bounds(int x, int y){
	return (x >= 0 && x < xsize && y >= 0 && y < ysize && MAZE[x][y].in && !MAZE[x][y].visited);
}

void traverse(int x1, int y1, int x2, int y2){
    if (x1 == x2 && y1 == y2){
        printf("Done!\n");
        return;
    } else {
        if (!within_bounds(x1, y1)){
            return;
        } else {
            sleep(3);
			// display_maze();
			printf("(%d %d)\n", x1, y1);
			printf("%d\n", MAZE[x1][y1].in);
			MAZE[x1][y1].visited = true;
			for (int i = 0; i < 4; i++){
				printf("I: %d\n", i);
				switch(i){
					case UP:
						// printf("UP\n");
						traverse(x1, y1 + 1, x2, y2);
						break;
					case LEFT:
						// printf("LEFT\n");
						traverse(x1 - 1, 1, x2, y2);
						break;
					case DOWN:
						// printf("DOWN\n");
						traverse(x1, y1 - 1, x2, y2);
						break;
					case RIGHT:
						// printf("RIGHT\n");
						traverse(x1 + 1, y1, x2, y2);
						break;
				}
				sleep(3);
			}

        }
    }

}

int *get_destination(){
    int* destination = malloc(2 * sizeof(int));

    bool within_bounds = false;

    while (!within_bounds) {
        destination[0] = rand() % xsize;
        destination[1] = rand() % ysize;

        if (MAZE[destination[0]][destination[1]].in) {
            within_bounds = true;
        }
    }

    return destination;
}

int *get_start_point(){
    int* start = malloc(2 * sizeof(int));

    bool within_bounds = false;

    while (!within_bounds) {
        start[0] = rand() % xsize;
        start[1] = rand() % ysize;

        if (MAZE[start[0]][start[1]].in) {
            within_bounds = true;
        }
    }

    return start;
}

void initialize(){
	//Initialize the maze!
	for(int x=0;x<xsize;x++){
		for(int y=0;y<ysize;y++){
			//The maze cells on the edges of the maze are "in" to provide padding. Otherwise, all maze cells are not in.
			MAZE[x][y].in   = (x==0||x==xsize-1||y==0||y==ysize-1)?1:0;
			//All maze cells have all walls existing by default, except the perimeter cells.
			MAZE[x][y].up   = (x==0||x==xsize-1||y==0)?0:1;
			MAZE[x][y].left = (x==0||y==0||y==ysize-1)?0:1;
			MAZE[x][y].visited = false;
		}
	}
	return;
}

void generate(){
	int xcur=1, ycur=1;//start growing from the corner. It could theoretically start growing from anywhere, doesn't matter.
	MAZE[xcur][ycur].in = 1;
	int whichway;
	bool success;
	do{
#ifdef movie
		savebmp(xcur,ycur);
#endif
#ifdef nodeadend
		if( MAZE[xcur][ycur-1].in&&MAZE[xcur][ycur+1].in&&
			   MAZE[xcur-1][ycur].in&&MAZE[xcur+1][ycur].in ){
				   //If at a dead end, randomly destroy a wall to make it not a dead end!
			do{
				success=0;
				whichway=rand()%4;
				switch(whichway){
				case UP:
					if(MAZE[xcur][ycur].up&&ycur!=1){
						success=1;
						MAZE[xcur][ycur].up=0;
					}
					break;
				case DOWN:
					if(MAZE[xcur][ycur+1].up&&ycur!=ysize-2){
						success=1;
						MAZE[xcur][ycur+1].up=0;
					}
					break;
				case LEFT:
					if(MAZE[xcur][ycur].left&&xcur!=1){
						success=1;
						MAZE[xcur][ycur].left=0;
					}
					break;
				case RIGHT:
					if(MAZE[xcur+1][ycur].left&&xcur!=xsize-2){
						success=1;
						MAZE[xcur+1][ycur].left=0;
					}
					break;
				}
			}while(!success);
		}
#endif
#ifdef backtrack
		while( MAZE[xcur][ycur-1].in&&MAZE[xcur][ycur+1].in&&
			   MAZE[xcur-1][ycur].in&&MAZE[xcur+1][ycur].in ){
				   //If all the neighbourhood cells are in, backtrack.
				int xcur2=MAZE[xcur][ycur].prevx;
				ycur=MAZE[xcur][ycur].prevy;
				xcur=xcur2;
		}
#endif
#ifdef prim
		do{
			//randomly find a cell that's in the maze
			xcur=rand()%(xsize-2)+1;
			ycur=rand()%(ysize-2)+1;
		}while(!MAZE[xcur][ycur].in ||
			MAZE[xcur][ycur-1].in&&MAZE[xcur][ycur+1].in&&
			MAZE[xcur-1][ycur].in&&MAZE[xcur+1][ycur].in);
#endif
		do{
			//Randomly grow the maze if possible.
			success=0;
			whichway=rand()%4;
			switch(whichway){
			case UP:
				if(!MAZE[xcur][ycur-1].in){
					success=1;
					MAZE[xcur][ycur].up=0;
					MAZE[xcur][ycur-1].prevx=xcur;
					MAZE[xcur][ycur-1].prevy=ycur;
					ycur--;
				}
				break;
			case DOWN:
				if(!MAZE[xcur][ycur+1].in){
					success=1;
					MAZE[xcur][ycur+1].up=0;
					MAZE[xcur][ycur+1].prevx=xcur;
					MAZE[xcur][ycur+1].prevy=ycur;
					ycur++;
				}
				break;
			case LEFT:
				if(!MAZE[xcur-1][ycur].in){
					success=1;
					MAZE[xcur][ycur].left=0;
					MAZE[xcur-1][ycur].prevx=xcur;
					MAZE[xcur-1][ycur].prevy=ycur;
					xcur--;
				}
				break;
			case RIGHT:
				if(!MAZE[xcur+1][ycur].in){
					success=1;
					MAZE[xcur+1][ycur].left=0;
					MAZE[xcur+1][ycur].prevx=xcur;
					MAZE[xcur+1][ycur].prevy=ycur;
					xcur++;
				}
				break;
			}
		}while(!success);
		MAZE[xcur][ycur].in=1;
		numin++; //Every iteration of this loop, one maze cell is added to the maze.
	}while(numin<(xsize-2)*(ysize-2));
#ifdef movie
	savebmp(xcur,ycur);
#endif
	return;
}

void savebmp(int xspecial, int yspecial){
	//save a bitmap file! the xspecial, yspecial pixel is coloured red.
	FILE * outfile;
	int extrabytes, paddedsize;
	int x, y, n;
	int width=(xsize-1)*2-1;
	int height=(ysize-1)*2-1;

	extrabytes = (4 - ((width * 3) % 4))%4; 

	char filename[200];
	
	sprintf(filename, "%s_%dx%d_n%d.bmp", OUTFILE, xsize, ysize, numin);
	paddedsize = ((width * 3) + extrabytes) * height;

	unsigned int headers[13] = {paddedsize + 54, 0, 54, 40, width, height, 0, 0, paddedsize, 0, 0, 0, 0};

	outfile = fopen(filename, "wb");
	fprintf(outfile, "BM");

	for (n = 0; n <= 5; n++){
	   fprintf(outfile, "%c", headers[n] & 0x000000FF);
	   fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
	   fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
	   fprintf(outfile, "%c", (headers[n] & (unsigned int) 0xFF000000) >> 24);
	}

	fprintf(outfile, "%c", 1);fprintf(outfile, "%c", 0);
	fprintf(outfile, "%c", 24);fprintf(outfile, "%c", 0);

	for (n = 7; n <= 12; n++){
	   fprintf(outfile, "%c", headers[n] & 0x000000FF);
	   fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
	   fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
	   fprintf(outfile, "%c", (headers[n] & (unsigned int) 0xFF000000) >> 24);
	}

	//Actual writing of data begins here:
	for(y = 0; y <= height - 1; y++){
		for(x = 0; x <= width - 1; x++){
			if(x%2 == 1 && y%2 == 1){
				if(x/2+1 == xspecial && y/2+1 == yspecial) RED;
				else{
					if(MAZE[x/2+1][y/2+1].in) WHITE; else BLACK;
				}
			}else if(x%2 == 0 && y%2 == 0){
				BLACK;
			}else if(x%2 == 0 && y%2 == 1){
				if(MAZE[x/2+1][y/2+1].left) BLACK; else WHITE;
			}else if(x%2 == 1 && y%2 == 0){
				if(MAZE[x/2+1][y/2+1].up) BLACK; else WHITE;
			}
		}
		if (extrabytes){     // See above - BMP lines must be of lengths divisible by 4.
			for (n = 1; n <= extrabytes; n++){
				fprintf(outfile, "%c", 0);
			}
		}
	}
	printf("file printed: %s\n", filename); 
	fclose(outfile);
	return;
}