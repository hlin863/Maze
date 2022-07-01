//Purpy Pupple's amazing maze generator. 
//Released under the CC-BY-SA 3.0 License and the GFDL
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define UP 0     //-y
#define DOWN 1   //+y
#define LEFT 2   //-x
#define RIGHT 3  //+x
#define OUTFILE "MAZE"
#define WHITE fprintf(outfile, "%c%c%c", 255,255,255)
#define BLACK fprintf(outfile, "%c%c%c", 0,0,0)
#define RED   fprintf(outfile, "%c%c%c", 0,0,255)

#define nodeadend//generate a maze without any dead ends! (consequently, many solutions to maze)
//#define prim    //enable this to generate mazes using prim's algorithm.
#define backtrack//enable this to generate mazes using depth-first search. Don't enable both.
//#define movie   //this option spams bitmaps to illustrate each step of generation.

long numin=1;     //Number of cells in the maze.
#define xsize 10
#define ysize 10

void initialize();
void generate();
void savebmp(int xspecial, int yspecial);
void displaymaze();
int *get_start_point();
int *get_destination();
void traverseMaze(int x, int y, int dx, int dy);
bool within_bounds(int x, int y);

struct cell{
    char type; // determines maze type.
	bool in;  //Is this cell in the maze?
	bool up;  //Does the wall above this cell exist?
	bool left;//Does the wall to the left of this cell exist?
	int prevx, prevy; //The coordinates of the previous cell, used for backtracking.
};

int start_x;
int start_y;

struct cell MAZE[xsize][ysize];

int main(){
	srand((unsigned int)time(NULL)); //seed random number generator with system time
	initialize();      //initialize the maze
	generate();        //generate the maze

	for(int i=1;i<10;i++){
		numin++;
		savebmp(0,0);      //output the bitmap
	}

	savebmp(0,0);

    displaymaze();

    int *start = get_start_point();

    start_x = start[0];
    start_y = start[1];

    int *dest = get_destination();

    traverseMaze(start[0], start[1], dest[0], dest[1]);

    printf("SUCCESSFULLY TRAVERSED MAZE!\n");

	return 0;
}

int *get_destination(){
    int* destination = malloc(2 * sizeof(int));

    bool within_bounds = false;

    while (!within_bounds) {
        destination[0] = rand() % xsize;
        destination[1] = rand() % ysize;

        if (MAZE[destination[0]][destination[1]].type == ' ') {
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

        if (MAZE[start[0]][start[1]].type == ' ') {
            within_bounds = true;
        }
    }

    return start;
}

bool within_bounds(int x, int y){
	return (x >= 0 && x < xsize && y >= 0 && y < ysize && MAZE[x][y].type == ' ');
}

void traverseMaze(int x, int y, int dx, int dy){

    if (x == start_x && y == start_y) {
        MAZE[x][y].type = 'S';
    } else {
        MAZE[x][y].type = '-';
    }
    
    if (x == dx && y == dy){
        MAZE[x][y].type = 'D';
        displaymaze();
    } else {
        if (within_bounds(x + 1, y)){
            traverseMaze(x + 1, y, dx, dy);
        }

        if (within_bounds(x - 1, y)){
            traverseMaze(x - 1, y, dx, dy);
        }

        if (within_bounds(x, y + 1)){
            traverseMaze(x, y + 1, dx, dy);
        }

        if (within_bounds(x, y - 1)){
            traverseMaze(x, y - 1, dx, dy);
        }
    }
}

void initialize(){
	//Initialize the maze!
    printf("This is an initalise process.\n");
	for(int x=0;x<xsize;x++){
		for(int y=0;y<ysize;y++){
            // MAZE[x][y].type = '*';
			//The maze cells on the edges of the maze are "in" to provide padding. Otherwise, all maze cells are not in.
			MAZE[x][y].in   = (x==0||x==xsize-1||y==0||y==ysize-1)?1:0;
			//All maze cells have all walls existing by default, except the perimeter cells.
			MAZE[x][y].up   = (x==0||x==xsize-1||y==0)?0:1;
			MAZE[x][y].left = (x==0||y==0||y==ysize-1)?0:1;
		}
	}
	return;
}

void displaymaze(){
    for(int x=0;x<xsize;x++){
		for(int y=0;y<ysize;y++){
            printf("%c", MAZE[x][y].type);
		}
        printf("\n");
	}
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
    printf("Saving bmp...\n");
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
					if(MAZE[x/2+1][y/2+1].in){ 
                        // MAZE[x/2+1][y/2+1].type = ' ';
                        WHITE; 
                    }
                    else { 
                        // MAZE[x/2+1][y/2+1].type = '*';
                        BLACK;
                    }
				}
			}else if(x%2 == 0 && y%2 == 0){
                // MAZE[x][y].type = ' ';
				BLACK;
			}else if(x%2 == 0 && y%2 == 1){
				if(MAZE[x/2+1][y/2+1].left){ 
                    // MAZE[x/2+1][y/2+1].type = '*';
                    BLACK; 
                } else {
                    // MAZE[x/2+1][y/2+1].type = ' ';
                    WHITE;
                }
			}else if(x%2 == 1 && y%2 == 0){
				if(MAZE[x/2+1][y/2+1].up){
                    MAZE[x/2+1][y/2+1].type = '*';
                    BLACK; 
                } else {
                    MAZE[x/2+1][y/2+1].type = ' ';
                    WHITE;
                }
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