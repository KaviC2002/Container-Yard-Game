#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NUM_ROWS 8
#define NUM_COLS 8

#define VACANT 0
#define BOUNDARY -1
#define ENTRY -2
#define EXIT -3

/***********************************************************/
/***********************************************************/
/******* ADD THE REQUIRED FUNCTIONS BELOW THIS POINT *******/
/***********************************************************/
/***********************************************************/

// Your function definitions should go here...
void InitialiseFloor(int floor[NUM_ROWS][NUM_COLS], char entryboundary, int index);
void PrintFloor(int floor[NUM_ROWS][NUM_COLS]);
double FloorAreaAvailable(int floor[NUM_ROWS][NUM_COLS], double length, double width);
void AddContainer(int floor[NUM_ROWS][NUM_COLS], int position, int size, int direction);
int LocateContainer(int floor[NUM_ROWS][NUM_COLS], char move, int* rowStart, int* colStart, int* rowEnd, int* colEnd);
int MoveContainer(int floor[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1, int isBlocked);

// Initialise Floor function: 
// Purpose: To assign a values to each position in the 8x8 2D array. Assign a -1 for the boundry, a 0 for the empty spaces, a -2 for the entry and -3 for the exit.
//Inputs: floor - 8x8 2D array 
//        entryboundry - a character which indicates the boundry that the entry will be
//        index - a number indicating how many rows down or collums across the entry will be located
//Output: N/A
//Author: Kavi Chand
void InitialiseFloor(int floor[NUM_ROWS][NUM_COLS], char entryboundary, int index)
{
	int i, j;
	//initialise all positions as -1 
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++){
			floor[i][j] = -1;
		}
	}	
	//initialise all positions except the border as 0
	for (i = 1; i < NUM_ROWS-1; i++) {
		for (j = 1; j < NUM_COLS-1; j++){
			floor[i][j] = 0;
		}
	}	
	// check where the entry and exit should be located	
	if (entryboundary == 'T'){
		floor[0][index] = -2;
		floor[7][index] = -3;
	} else if (entryboundary == 'B') {
		floor[7][index] = -2;
		floor[0][index] = -3;
	} else if (entryboundary == 'R') {
		floor[index][7] = -2;
		floor[index][0] = -3;
	} else if (entryboundary == 'L') {
		floor[index][0] = -2;
		floor[index][7] = -3;
	} 
}

// Print Floor Function:
// Purpose: Prints the 2D 8x8 array floor. Prints the boundry as an '@' symbol, the vacant positions as a ' ', the entry as a 'U' and 'X' for the exit
// Inputs: Floor 2D 8x8 array
// Output; N/A
//Author: Kavi Chand
void PrintFloor(int floor[NUM_ROWS][NUM_COLS])
{
	char letter;
	int i, j;
	// Nested for loop with conditional statements to check each value is in each position and assign the relevant symbol or character to be printed
	for (i = 0; i < NUM_ROWS; i++){
		for (j = 0; j < NUM_COLS; j++){
			if (floor[i][j] == -1){
				printf("@");
			} else if (floor[i][j] == 0){
				printf(" ");
			} else if (floor[i][j] == -2){
				printf("U");
			} else if (floor[i][j] == -3){
				printf("X");
			} else {
				letter = floor[i][j];
				printf("%c", letter);
			}
		}
		printf("\n");
	}
}

// Floor Area Available function:
// Purpose: To determine the amount of vacant spots and return the amount of Area in sq ft is available
// Inputs: floor - 2D 8x8 array 
//         length - length of each cell 
//         width - width of each cell
// Output: free - the available area
double FloorAreaAvailable(int floor[NUM_ROWS][NUM_COLS], double length, double width)
{
	//Initialise the varibles as double 
	double free = 0;
	double cellArea;
	//calculate the area per cell 
	cellArea = (length * width);
	//determine the amount of vacant cells 
	for (int i = 1; i < NUM_ROWS-1; i++) {
		for (int j = 1; j < NUM_COLS-1; j++){
			if (floor[i][j] == 0){
				free += 1;
			}
		}
	}	
	//calculate avalible area
	free = (cellArea * free);
	return free;
}

// Add Container funnction
// Purpose: Adding containers of different sizes and directions (horizontal or verticle) at differetn positions on the floor
// Inputs: floor - 2D 8x8 array 
// 		   position - a integer indicating the row and col of the container
//		   size - the length of the container
// 		   direction - a boolean value indicating horizontal or verticle
// Output: N/A
// Author: Kavi Chand 
void AddContainer(int floor[NUM_ROWS][NUM_COLS], int position, int size, int direction){
	// Initialise varibles 
	int i, j, row, col;
	int index = 0;
	int length = 0;
	char letter = 65; 
	//Find (row, col) of postion
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++){
			if(index == position){
				row = i;
				col = j;
			}
		 	index++;
		}
	}	
	// check the last letter to be used and update to next letter
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++){
			if (floor[i][j] >= letter){
				letter = ((floor[i][j]) + 1);
			}
		}
	}
	// check if the container is being placed on vacant cells
	while (length < size){
		if (direction){
			if (floor[row][col] != 0) {
				return;
			} else {
				row++;
			}
		} else {
			if (floor[row][col] != 0) {
				return;
			} else {
				col++;
			}
		}
		length++;
	}
	//place container
	for (i = 0; i < size; i++){
		if(direction){
			row--;
			floor[row][col] = letter;
		} else {
			col--;
			floor[row][col] = letter;
		}
	}	
	return;
}

// Locate Container function
//Purpose: Locate a specified container and determine if the container is stuck or can move 
// Inputs: Floor - 2D 8x8 array
//		   rowStart, colStart - the co-ordinates of the start of the container	
//		   rowEnd, colNed - the co-ordinates of the End of the container	
// 		   move - the letter represnting a container 
// Output: isBlocking - a boolean value representing if the container is stuck(0) or can move(1)
// Author: Kavi Chand 
int LocateContainer(int floor[NUM_ROWS][NUM_COLS], char move, int *rowStart, int *colStart, int *rowEnd, int *colEnd)
{
	int i, j;
	int blocking;	
	//determine end of the container
	for (i = 1; i < NUM_ROWS-1; i++) {
		for (j = 1; j < NUM_COLS-1; j++){
			if (floor[i][j] == move) {
				*rowEnd = i;
				*colEnd = j;
			}
		}
	}	
	// determine start of the container
	for (i = 6; i > 0; i--) {
		for (j = 6; j > 0; j--){
			if (floor[i][j] == move) {
				*rowStart = i;
				*colStart = j;
			}
		}
	}	
	//check if the container is stuck or can move
	if (*rowStart == *rowEnd){ //horizontal containers
		if ((floor[*rowStart][*colStart-1] == 0) || (floor[*rowEnd][*colEnd+1] == 0)){
			blocking = 1;
		} else {
			blocking = 0;
		}
	}
	if (*colStart == *colEnd){ //verticle containers
		if ((floor[*rowStart-1][*colStart] == 0) || (floor[*rowEnd+1][*colEnd] == 0)){
			blocking = 1;
		} else {
			blocking = 0;
		}
	}
	return blocking;
}

// Move Container Function
// Purpose: to move a container to a new location on the floor and return a value indicating what is infront of the container
// Inputs: floor - 2D 8x8 array 
//         r0, c0 - the coordinates of the start of the container
// 		   r1, c1 - the coordinates of the end of the container
// Outputs: result - a value that indicates whether the container is next to the entry or exit.
// Author: Kavi Chand 
int MoveContainer(int floor[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1, int isBlocked)
{
	//initilise values
	int i;
	int result;
	int countleft = 0;
	int countright = 0;
	int countup = 0;
	int countdown = 0;
	int size = 0;
	char letter = floor[r0][c0];
	//figure out size of container
	if(r0==r1){ //horizontal
		for (i=c0; i != c1+1; i++){
			size++;
		}
	}
	if(c0==c1){ // verticle
		for (i=r0; i != r1+1; i++){
			size++;
		}
	}
	//blocked containers 
	if (isBlocked == 0){
		if (r0 == r1){ //horizontal containers
			if ((floor[r0][c0-1] == -2) || (floor[r1][c1+1] == -2)){
				result = 1;
			} else if ((floor[r0][c0-1] == -3) || (floor[r1][c1+1] == -3)) {
				result = 2;
			} else {
				result = -1;
			}
		}
		if (c0 == c1){ //verticle containers
			if ((floor[r0-1][c0] == -2) || (floor[r1+1][c1] == -2)){
				result = 1;
			} else if ((floor[r0-1][c0] == -3) || (floor[r1+1][c1] == -3)) {
				result = 2;
			} else {
				result = -1;
			}
		}
	}
	//check how far the container can move 
	if (isBlocked){
		if (r0 == r1){ //horizontal containers
			for (i = c0-1; floor[r0][i]==0; i--){ //left
				countleft++;
			}
			for (i = c1+1; floor[r1][i]==0; i++){ //right
				countright++;
			}
		}
		if (c0 == c1){ //verticle containers
			for (i = r0-1; floor[i][c0]==0; i--){ //up
				countup++;
			}
			for (i = r1+1; floor[i][c1]==0; i++){ //down
				countdown++;
			}
		}
	}
	//move the containers and determine result value
	if (isBlocked){
		if(r0==r1){ //horizontal
			if (countleft > 0){ //left
				for (i = 0; i < size; i++){
					//move container to the left
			 		floor[r0][c0+i] = 0;
					floor[r0][c0-countleft+i] = letter;
				}
				//check value next to container
				if (floor[r0][c0-countleft-1] == -2){
					result = 1;
				} else if (floor[r0][c0-countleft-1] == -3){
					result = 2;
				} else{
					result = 0;
				}
			} else if (countright > 0){ //right
				for (i = 0; i < size; i++){
					//move container to the right
					floor[r1][c1-i] = 0;
					floor[r1][c1+countright-i] = letter;
				}
				//check value next to container
				if (floor[r1][c1+countright+1] == -2){
					result = 1;
				} else if (floor[r1][c1+countright+1] == -3){
					result = 2;
				} else{
					result = 0;
				}
			}
		}
		if(c0==c1){ //verticle 
			if (countup > 0){ //up
				for (i = 0; i < size; i++){
					//move container up
					floor[r0+i][c0] = 0;
					floor[r0-countup+i][c0] = letter;
				}
				//check value next to container
				if (floor[r0-countup-1][c0] == -2){
					result = 1;
				} else if (floor[r0-countup-1][c0] == -3){
					result = 2;
				} else{
					result = 0;
				}
			} else if ( countdown > 0){ //down
				for (i = 0; i < size; i++){
					//move container down
					floor[r1-i][c1] = 0;
					floor[r1+countdown-i][c0] = letter;
				}
				//check value next to container
				if (floor[r1+countdown+1][c1] == -2){
					result = 1;
				} else if (floor[r1+countdown+1][c1] == -3){
					result = 2;
				} else{
					result = 0;
				}
			}
		}
    }
	//return result
	return result;
} 


/***********************************************************/
/***********************************************************/
/********* DO NOT MODIFY ANY CODE BELOW THIS POINT *********/
/***********************************************************/
/***********************************************************/

/* Function to obtain capital letter as input */
char GetMove(void)
{
	char move;
	printf("\nMove container: ");
	scanf("%c", &move);
	// Ignore non-capital letter inputs
	while ((move < 'A') || (move > 'Z')) {
		scanf("%c", &move);
	}
	return move;
}

/* The main Container Yard simulation */
int main(void)
{
	int gameOver = 0;
	int isBlocked = 0;
	int floor[NUM_ROWS][NUM_COLS];
	int rowStart, colStart, rowEnd, colEnd;
	char input;

	/* Print banner */
	printf("............**********************************............\n");
	printf("............* CONTAINER YARD GAME SIMULATION *............\n");
	printf("............**********************************............\n");
	
	/* Initialise the yard floor grid and add containers */
	InitialiseFloor(floor, 'R', 3);
	AddContainer(floor, 28, 2, 0);
	AddContainer(floor, 11, 3, 1);
	AddContainer(floor, 41, 2, 1);
	AddContainer(floor, 42, 2, 1);
	AddContainer(floor, 42, 2, 1);
	AddContainer(floor, 34, 2, 0);
	AddContainer(floor, 36, 3, 1);
	AddContainer(floor, 37, 2, 1);
	AddContainer(floor, 53, 2, 0);
	AddContainer(floor, 30, 3, 1);

	/* Print status */
	printf("ENGGEN131 2021 - C Project\nContainer Yard!  The containers are rushing in!\n");
	printf("In fact, %.2f sq ft of the yard floor is available for containers!\n\n", FloorAreaAvailable(floor, 20.5, 10.3));

	/* Main simulation loop */
	while (gameOver != 2) {
		PrintFloor(floor);
		input = GetMove();
		isBlocked = LocateContainer(floor, input, &rowStart, &colStart, &rowEnd, &colEnd);
		gameOver = MoveContainer(floor, rowStart, colStart, rowEnd, colEnd, isBlocked);
	}

	/* A container is ready to exit - the simulation is over */
	PrintFloor(floor);
	printf("\nCongratulations, you've succeeded!");

	return 0;
}
