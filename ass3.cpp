/****************************************************************************************************************************************
Filename: ass3.c++
Version: 3.0
Author: Felipe de Paula Souza Filho
Student Number: 040843714
Course: Numerical CST8233
Lab Section: 300
Assignment number : 3
Due date: April 14 2019
Submition date: April 13 2019
Professor name: Andrew Tyler
Purpose: to solve a differential equation for acceleration under gravity, with controllable wind drag and with an additional elastic
restoring term B so as to simulate falling off the top of a 200 metres high tower with a 10 metre length bungee cord tied to your ankles.
****************************************************************************************************************************************/
// ass3.c

#include <iostream>
using namespace std;
#include <algorithm>
#include <conio.h>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <string>
#include <vector>
#include <stdlib.h> // for using the function sleep
#include <Windows.h>
#include <thread> // std::this_thread::sleep_for
#include <chrono> 

#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC	// need this to get the line identification
//NB must be in debug build
	//forumulas
#define GRAVITY 9.8 // Gravity
//#define c 0.0 // initially
#define BUNGEE_CORD_MAX 10 // 10 metres bungee cord
#define k 0.06 
#define DRAG 0.02

// Forward declarations
void simulatorBungeeJump();

/****************************************************************************************************************************************
Function name: main
Purpose: Display main message and run function to perform a differential equation for acceleration under gravity
In parameters: None
Out parameters: void
Version: 1.0
Author: Felipe de Paula Souza Filho
****************************************************************************************************************************************/
int main(void)
{
	int response;
	bool RUNNING = true;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	while (RUNNING)
	{
		cout << "        Bungee Jump\nMENU\n1. Run Simulator\n2: Quit\n" << endl;
		cin >> response;
		
		switch (response)
		{
		case 1:simulatorBungeeJump(); break;
		case 2:exit(1);
		default:cout << "Please enter a valid option\n\n" << endl;
			break;
		}
		getchar();
	}
	return 0;
}// end of main

/****************************************************************************************************************************************
Function name: simulatorBungeeJump
Purpose: To perform all the necessary math operations to reach the ground (height < 1.0 metres) at low speed (<1.0 metres/sec) so you can
safely slip off the bungee and walk away. for that would need to adjust the Wing drag from the keyboard 'w'increases it and decreases' it.
In parameters: None
Out parameters: void
Version: 2.7
Author: Felipe de Paula Souza Filho
****************************************************************************************************************************************/
void simulatorBungeeJump() {

	// second menu arguments
	char response_two = 'a'; // holds user's input 
	bool RUNNING = true;
	int counter = 0;
	int bungeeExtension = 0;
	double c = 0; // wind drag value
	double currentHeight = 200; // 200 metres
	double oldHeight = 200;
	double forceBungee = 0; // elastic restoring force B of the bungee cord pulling you up
	double acceleration = 0.0; // dtdv
	double currentV = 0.0; // current velocity
	double oldV = 0.0; // old velocity
	double oldTime, newTime, step;
	double start = oldTime = newTime = GetTickCount();
	int min = 0, sec = 0, milli = 0;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	while (RUNNING)
	{
		newTime = GetTickCount(); // Get milliseconds
		DWORD currentTime = newTime - start;
		step = (newTime - oldTime) / 1000;
		counter = currentTime % 1000;

		if (currentTime >= 1000) { // 1 second

			// checking to current time is begger the 20 milliseconds everytime it loops
			if (currentTime < 0.02) {
				newTime = GetTickCount();
				step = (newTime - oldTime) / 1000;
			}

			sec++; // increment seconds
			start = GetTickCount();
		}

		if (sec >= 60) {
			min++;
			sec = 0;
		}

		// Second display menu
		cout << "Press q to quit; w to increase drag; e to decrease drag" << endl;
		cout << "minute = " << min;
		cout << "; second = " << sec;
		cout << "; millisec = " << counter << endl;

		// if user presses e or w
		if (response_two == 'e') {
			cout << "\te was pressed - decrease wind drag" << endl;
		}
		else if (response_two == 'w') {
			cout << "\tw was pressed - increase wind drag" << endl;
		}
		else {
			cout << "starting bungee jump :" << endl;
			c = 0;
		}

		// Differential Equations 
		//dV/dT - delta v over delta t
		acceleration = GRAVITY - ((0.02 * c) * (oldV + (0.004 * oldV * oldV * oldV))) - forceBungee; // calculating the acceleration for falling under gravity according to the changes over time
		currentV = oldV + (acceleration * (step)); // calculating current velocity using Heuler method
		currentHeight = oldHeight - (currentV * (step)); // calucultng the current height 

		/* Getting users input detecting which key was pressed */
		if (_kbhit()) {
			response_two = _getch();
			switch (response_two)
			{
			case 'w':
				if (c >= 0 && c < 20) { // Increase must be between 0 and 20 
					c++;
				}
				break;
			case 'e':
				if (c > 0 && c <= 20) { // decrease must be between 1 and 20 
					c--;
				}
				break;
			case 'q':exit(1);
			default:printf("Please enter a valid option\n");
				break;
			}
		}

		oldV = currentV; // getting the old velocity
		oldHeight = currentHeight; // getting old height 
		oldTime = newTime; // getting oldtime 

		bungeeExtension = 190 - currentHeight; // make the bunggee extension value to change according to the current height minus the total height 

		if (currentHeight <= 190) { // or currentHeight > 10 - if the current height is less then the 10 metre lengh bungee cord. 
			forceBungee = (k * bungeeExtension); // Once it is fully extended (>10m) it pulls you up with a force that gets larger the more it is extended.
			cout << "\tBungee: EXTENDED \n";
		}
		else {
			cout << "\tBungee: NOT EXTENDED \n";
		}

		// Printing results 
		printf("\tWind drag = %.3f \n", c);
		printf("\tspeed of descent = %.3fm/s\n", currentV);
		printf("\theight above ground = %.3fmetres\n", currentHeight);
		//cout << "\tCurrent time:  " << currentTime<< endl;
		//cout << setprecision(3) << fixed <<"\tWind drag = " << c << endl;
		//cout << "\tspeed of descent = " << currentV << " m/s \n\theight above ground = " << currentHeight << " metres" << endl;

		//currentHeight--; // // decreamenting height above ground from 200 to 0
		if (currentHeight <= 0) {
			cout << "You crashed! \nTime Taken (min:sec:millisec) = " << min << ":" << sec << ":" << counter << "\n" << endl;
			break;
		}

		if (currentHeight > 0) { // clear screen 
			system("CLS");
		}
	} // end of loop
} // end of function
