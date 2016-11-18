//
// 	snake.cpp
// 	 purpose: recreating the snake game by using the skills we have 
// 		  learned so far, particularly by using a  2-D array and a
// 		  struct. in short, the snake game is a game that has a 
//		  snake that can be moved around by the user by pressing the
//		  r, l, and f keys. there are 2 snacks on the board that
//		  the snake wants to eat to gain more energy. every time 
//		  the snake moves, the user gains 1 point in score and 
//	       	  loses 1 unit in energy. if the snake runs out of energy
//		  or runs into its own body, the game is over. 	  
//     	      by: Annie Chen
// last modified: Oct. 27, 2015

#include <iostream>
#include "termfuncs.h"

using namespace std;

const int ROWS = 23;
const int COLS = 63;
const int INITIALENERGY= 20;
const int OENERGY = 15;
const int XENERGY =  25;
struct Snake {
	char direction[4];
	int r;
	int c;
	int energy;
	int score;
};

void setup_board(char board[ROWS][COLS]);
void setup_topbottom(char board[ROWS][COLS]);
void setup_sides(char board[ROWS][COLS]);
void setup_spaces(char board[ROWS][COLS]);
Snake print_board(char board[ROWS][COLS], Snake snake);
Snake rotate_head(char board[ROWS][COLS], Snake snake);
int rotate_left(int direction);
int rotate_right(int direction);
Snake move_forward(Snake snake, int direction);
void hash(char board[ROWS][COLS], Snake snake);
void update_board(char board[ROWS][COLS], Snake snake, int direction);
Snake wrap(Snake snake);
int play_game(char board[ROWS][COLS]);
Snake make_snake(char board[ROWS][COLS], Snake snake);
Snake check_food(char board[ROWS][COLS], Snake snake);
Snake print_food_o(char board[ROWS][COLS], Snake snake);
Snake print_food_X(char board[ROWS][COLS], Snake snake);
void report(int score);

int main()
{
	char board[ROWS][COLS];
	int score;

	setup_board(board);
	score = play_game(board);
  	report(score);
	return 0;
}

// setup_board
// purpose: to call other functions that all together draws the board
// arguments: a 2-D array board
// returns: nothing, because it is a void function
void setup_board(char board[ROWS][COLS])
{
	setup_topbottom(board);
	setup_sides(board);
	setup_spaces(board);
}		
		
// setup_bottom
// purpose: dash and corner the top and bottom of the board (rows 0 & 22)
// arguments: a 2-D array board
// returns: nothing, because it is a void function
void setup_topbottom(char board[ROWS][COLS])        
{
	board[0][0] = '+';	// the corners
	board[0][62] = '+';
	board[22][0] = '+';
	board[22][62] = '+';		

	for (int c = 1; c < COLS - 1; c++) {
  		board[0][c] = '-';	// the top and bottom borders
		board[22][c] = '-';
	}
}

// setup_sides
// purpose: put lines in the sides of the board (columns 0 & 62)
// arguments: a 2-D array board
// returns: nothing, because it is a void function
void setup_sides(char board[ROWS][COLS])
{
	for (int r = 1; r < ROWS - 1; r++) {
		board[r][0] = '|';	// the left and right borders
		board[r][62] = '|';		      
	}
}

// setup_spaces
// purpose: put spaces in the rest of the board, which are the places in 
// 	    the array that has no board border
// arguments: a 2-D array board
// returns: nothing, because it is a void function
void setup_spaces(char board[ROWS][COLS])
{
	for (int r = 1; r < ROWS - 1; r++) {
		for (int c = 1; c < COLS - 1; c++) {
			board[r][c] = ' ';
  		}
	}
}

// play_game
// purpose: calls other functions to change the snake struct based on 
// the input and then updates the 2-D array and prints the 2-D array board
// arguments: a 2-D array board
// returns: the integer that has the final score, which is
//	    copied from tne member score of the snake struct
int play_game(char board[ROWS][COLS])
{     
	int score;
	Snake snake;
	snake.score = 0;	// initialize score at 0
	snake.energy = INITIALENERGY;// initialize the energy at 20 because
        			// everytime food is printed
				// the energy will go up by 40 units
	snake = make_snake(board, snake); 
    	score = snake.score;	

return score;
}

// make_snake
// purpose: setting up the snake struct member direction with the 4 snake 
//	    head directions, and inputting the first location of the head, 
//  	    which is the middle of the board
// arguments: the 2-D array board and a copy of the snake struct
// returns: a new copy of the snake struct
Snake make_snake(char board[ROWS][COLS], Snake snake)
{
	int direction = 0;
	

      	snake.direction[0] = '^';	// the 4 characters that the
	snake.direction[1] = '<';	// snake head can be
	snake.direction[2] = 'v';
	snake.direction[3] = '>';
	
	snake.r = ROWS/2;		// allowing the snake head to
	snake.c = COLS/2;		// start in the middle of the board

	screen_clear();
	update_board(board, snake, direction);
	
	snake = check_food(board, snake);
	print_board(board, snake);
	snake = rotate_head(board, snake);

return snake;
}

// rotate_head
// purpose: to rotate the snake head direction based on the input
// arguments: the 2-D array of the board and a copy of the
// 	      snake struct
// returns: a new copy of the snake struct
Snake rotate_head(char board[ROWS][COLS], Snake snake)
{	char input;
	int direction = 0;
	bool notbody = true;	// to check if snake is going to hit body
	while (snake.energy > 0 and notbody)
	{
		input = getachar();
		if (input == 'l') {
			direction = rotate_left(direction);

		}	      
		else if (input == 'r') {
			direction = rotate_right(direction);		

		}
		else if (input == 'f') {
			snake.score++; 	// +1 to  score when move foward
		       	snake.energy--;	// -1 to energy when move forward
			hash(board, snake);	       	       
			snake = move_forward(snake, direction);
			if (board[snake.r][snake.c] == '#')
				notbody = false;
		}
	update_board(board, snake, direction);			    
	screen_clear();
	snake = check_food(board, snake);
	snake = print_board(board, snake);
	} 
	cout << endl;
return snake;
}

// rotate_left
// purpose: rotates the head direction to the left by adding one to the
//	    direction which corresponds to the array of(head)direction chars
// arguments: the integer that keeps track of the position in the member
//	      array direction in the snake struct. 
// returns: the same integer that keeps track of the position but the 
//	    integer is updated
int rotate_left(int direction)
{	
	direction = direction + 1; 
	if (direction  == 4)  	// if position is at the end of the array
	     	direction = 0;	// the position goes back to the first
				// element
return direction;
}

// rotate_right
// purpose: rotates the head direction to the right by adding one to the 
//	    direction which corresponds to the array of(head) direction char
// arguments: the integer that keeps track of the position in the member
//	      array direction in the snake struct. 
// returns: the same integer that keeps track of the position but the 
//	    integer is updated
int rotate_right(int direction) 
{
	direction = direction - 1;
	if (direction == -1) 	// if position is at the beginning of the
		direction = 3;	// array, position goes to the end
return direction;
}

// move_forward
// purpose: moves the snake forward by going up the row/column or 
//          down row/column by editing the location of the snake which is 
//	    done by editing the row and column within the snake struct
// arguments: a copy of the snake struct and the integer that tells us
//	      the position within the member array direction in snake struct
// returns: a new copy of the snake struct
Snake move_forward(Snake snake, int direction)
{
	if (direction == 0) {
		snake.r--;	// decreases row by one because going down 
	}

	if (direction == 1) {
		snake.c--;	// decrease column by one to go left
	}
	
	if (direction == 2) {
		snake.r++;	// increase row by one to go up
	}

	if (direction == 3) {
		snake.c++;	// increase column by one to go right
	}

	snake = wrap(snake);
return snake;
}

// hash
// purpose: puts a # in the board array at the location the snake used to 
// 	    be at
// arguments: the 2-D array board and a copy of the snake struct
// returns: nothing, because it is a void function
void hash(char board[ROWS][COLS], Snake snake)
{
	board[snake.r][snake.c] = '#'; // body of the snake

}

// wrap
// purpose: sends the snake to the other side of the board.
// it does this by sending the snake to row 21 when it hits row 0,
// and vice versa.this function sends the snake to column 0 when it
// hits column 61 and vice versa.
// argumnents: a copy of the snake struct
// returns: a new copy of the snake struct
Snake wrap(Snake snake)
{
	if (snake.r == 0) {
		snake.r = ROWS-2;
	}

	if (snake.r == ROWS-1) {
		snake.r = 1;
	}

	if (snake.c == 0) {
		snake.c = COLS-2;
	}
	
	if (snake.c == COLS-1) {
		snake.c = 1;
	}
return snake;
}

// update_board
// purpose: puts in snake onto the board by updating the 2D array
// 	    with the struct
// arguments: the 2-D array board, a copy of the snake struct, and an int
//	      that gives us the direction of the snake head 
// returns: nothing, because it is a void function
void update_board(char board[ROWS][COLS], Snake snake, int direction)
{
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			if (snake.r == r && snake.c == c) {
				board[r][c] = snake.direction[direction];
			}
		}
	}
}

// check_food
// purpose: to check to see if there is a "o" or "X" food on the board
// 	    already. if there isn't, meaning that if the food is eaten,
//	    then the print food function is called to print a new food
// arguments: the 2-D array board and a copy of the snake struct
// return: a new copy of snake struct 
Snake check_food(char board[ROWS][COLS], Snake snake)
{
	bool foodo = false; 
	bool foodx = false;
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			if (board[r][c] == 'o') {
				foodo = true;	// that means a "o" food is 
			}			// already there
		}
	}
	if (!foodo) {
		snake = print_food_o(board, snake);
	}
	foodo = false;
	
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			if (board[r][c] == 'X') {
				foodx = true;
			}
		}
	}
	if (!foodx) {
		snake = print_food_X(board, snake);
	}
	foodx = false;

return snake;	
}

// print_food_o
// purpose: to put the 'o' food into the 2-D array and add to the
// 	    energy unit everytime the food is reprinted, which is 
//	    when the food is eaten
// arguments: copy of the 2-D array board and copy of the snake struct
// return: a new copy of snake struct
Snake print_food_o(char board[ROWS][COLS], Snake snake)
{
      	int rdmrow, rdmcol;
	rdmrow = random_int(1, ROWS-2);
	rdmcol = random_int(1, COLS-2);

  	if (board[rdmrow][rdmcol] == ' ') {	// to check if the body is
		board[rdmrow][rdmcol] = 'o';	// already there
	    	snake.energy += OENERGY;
	}
	else { rdmrow = random_int(1, 21);	// if body is at that point
	       rdmcol = random_int(1, 61);	// find another random
	       board[rdmrow][rdmcol] = 'o'; 	// point
	       snake.energy += OENERGY;
	}


return snake;
}

// print_food_X
// purpose: to put the 'X' food into the 2-D array and add to the energy
//	    unit everytime the food is reprinted, which is when the food
//	    is eaten
// arguments: a copy of the 2-D array board and a copy of the snake struct
// return: a new copy of snake struct
Snake print_food_X(char board[ROWS][COLS], Snake snake)
{
      	int rdmrow, rdmcol;
	rdmrow = random_int(1, ROWS-2);
	rdmcol = random_int(1, COLS-2);

  	if (board[rdmrow][rdmcol] == ' ') {
		board[rdmrow][rdmcol] = 'X';
		snake.energy += XENERGY;
	}
	else { rdmrow = random_int(1, 21);
	       rdmcol = random_int(1, 61);
	       board[rdmrow][rdmcol] = 'X';
	       snake.energy += XENERGY;
	}

return snake;
}

// print_board
// purpose: prints the 2-D array board and also states the energy
// units and the score
// arguments: a copy of the 2-D array board and a copy of the snake struct
// return: a new copy of snake struct
Snake print_board (char board[ROWS][COLS], Snake snake)
{

	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			cout << board[r][c];
		}
		cout << endl;
	}

	cout << "EU: ";
	snake = check_food(board, snake);
	cout << snake.energy;
	cout << "   Score: ";
	cout << snake.score;
        
return snake;

}

// report
// purpose: to print out game over and print out the final score
// arguments: an integer that holds the score value
// returns: nothing, because it is a void function

void report(int score)
{
	cout << "Game Over.   Final Score: " << score << endl;
}
