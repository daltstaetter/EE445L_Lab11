//ball movement

#include <stdio.h>
#include "driverlib/gpio.h"
#include "Output.h"
#include "PLL.h"
#include "lm3s1968.h"
#include "sysctl.h"
#include "DAC.h"
#include "Ball.h"

#define TRUE = 1;
#define FALSE = 0;


char xReverse = 0, yReverse = 0; //values for reversing direciton of x or y


//clears the balls old position in the 2d array Map
void clearBall(int ballPosX, int ballPosY, char Map[][6]) {
	Map[ballPosX][ballPosY] = 0x00;
	Map[ballPosX - 1][ballPosY] = 0x00;
	Map[ballPosX - 2][ballPosY] = 0x00;
	Map[ballPosX - 3][ballPosY] = 0x00;
}

//puts ball in 2d array called Map
void drawBall(int ballPosX, int ballPosY, int top, char Map[][6], int* level) {
	if(top == 1) { //prints ball at top
		Map[ballPosX][ballPosY] = 0xF0;
		Map[ballPosX - 1][ballPosY] = 0xF0;
		Map[ballPosX - 2][ballPosY] = 0xF0;
		Map[ballPosX - 3][ballPosY] = 0xF0;
		Nokia5110_DrawMap(Map, *level);
	}
	else { //prints ball at bottom
		Map[ballPosX][ballPosY] = 0x0F;
		Map[ballPosX - 1][ballPosY] = 0x0F;
		Map[ballPosX - 2][ballPosY] = 0x0F;
		Map[ballPosX - 3][ballPosY] = 0x0F;
		Nokia5110_DrawMap(Map, *level);
	}
}

//used to detect the walls in the x direction and signals ball to reverse direction
void DetectWallX(int ballPosX, int ballPosY, int top, int* level) {
	
	//check course 1
	if(*level == 0) {
		if((ballPosX >= 79)  || (ballPosX < 8)) {
					xReverse = xReverse ^ 1;
		}		
	}
	
	//check course 2
	else if(*level == 1) { //top y wall
		if((ballPosY >= 2) && (top ==1)) { //lower x limit
			if((ballPosX >= 79) || (ballPosX < 8)) {
				xReverse = xReverse ^ 1;
			}
		}
		if(ballPosY <= 2) { //top x limit
			if(ballPosX > 32) {
				xReverse = xReverse ^ 1;
			}
		}
		if(ballPosX < 8) {
			xReverse = xReverse ^ 1;
		}
	}	
}

//detects walls in the y direction for each course and tells ball to reverse direction
void DetectWallY(int ballPosX, int ballPosY, int top, int* level) {
	
	//check course 1
	if(*level == 0) {
		if((ballPosY <= 0)  && (top	== 1)) {
			yReverse = yReverse ^ 1;
		}
		if((ballPosY >= 5) && (top == 0)) {
			yReverse = yReverse ^ 1;
		}
	}
	
	//check course 2
	else if(*level == 1) { //top y wall
		if((ballPosY >= 5) &&(top == 0)) {
			yReverse = yReverse ^ 1;
		}
		if((ballPosY <= 3) && (top == 0) && (ballPosX >= 36)) {
			yReverse = yReverse ^ 1;
		}
		if((ballPosY <= 0) && (top == 1)) { //bottom y wall
			yReverse = yReverse ^ 1;
		}
	}	
}

//detects hole for each course using the balls position
int DetectHole(int ballPosX, int ballPosY, int* level) {
	//check course 1
	if(*level == 0) {
		if((ballPosX < 20) && (ballPosX > 7) && (ballPosY <= 2) && (ballPosY >= 1)) {
			return 1;
		}
	}
	
	//check course 2
	else if(*level == 1) {
		if((ballPosX < 33) && (ballPosX > 19) && (ballPosY < 2) && (ballPosY > 0)) {
			return 1;
		}
	}
	else {
		return 0;
	}
	return 0;
}

int MoveBall(int x, int y, int* posX, int* posY, int* level, char Map[][6]) {
		int ballPosX, ballPosY;
		int top = 1;
		ballPosX = *posX;
		ballPosY = *posY;
	
		x = 7 /*xVector*/; //some calculation for distance ball will travel in x direction
		y = 4 /*yVector*/; //some calculation for distance ball will travel in y direction
	
		
		
		while((x > 0) || (y > 0)) {
			if (x > 0) {
				DetectWallX(ballPosX, ballPosY, top, level); //detects if there is a wall in x direction
			}
			if (y > 0) {
				DetectWallY(ballPosX, ballPosY, top, level);
			}
			
			//hits hole
			if(DetectHole(ballPosX, ballPosY, level)) {
				*level = *level + 1;
				Nokia5110_Clear();
				return 1;
			}

			//clear ball's old position
			clearBall(ballPosX, ballPosY, Map);

			//change ball position
			//this will move in an unnatural way but it will go to the correct position
			if((xReverse == 1) && (x > 0)) {
				ballPosX = ballPosX + 4;
			}
			else if(x > 0) {
				ballPosX = ballPosX - 4;
			}
			if((yReverse == 1) && (y > 0)) {
				if(top == 1) {
					ballPosY = ballPosY + 1;
					top = top ^ 1;
				}
				else {
					top = top ^ 1;
				}
			}
			else if(y > 0) {
				if(top == 0) {
					ballPosY = ballPosY - 1;
					top = top ^ 1;
				}
				else {
					top = top ^ 1;
				}
			}
			
			//decrement vector unit values
			x--;
			y--;
			
			//print new ball position
			drawBall(ballPosX, ballPosY, top, Map, level);
			Delay(5000000);
			//update ball position values
			*posX = ballPosX;
			*posY = ballPosY;
			 
		
		}
		
		return 0; //never reached hole
		
		
}
