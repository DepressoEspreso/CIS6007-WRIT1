//
// Array Visualisation for CIS6007 WRIT1
//

#include <iostream>
#include <string>
#include <random>
#include "gl_stuff.h"
#include "Array2D.h"
#include <tbb/tbb.h>
#include <chrono>
#include <thread>


using namespace std;
using namespace tbb;


#pragma region Function Prototypes

void initPattern(Array2D<int>& array); // Initialise grid with "alive" and "dead" cell patterns
void update();
// Main update function - apply Game of Life rules here

#pragma endregion


//
// Example array
//
Array2D<int>						myArray = Array2D<int>(64, 64);


// Starting point for our program...
int main(int argc, char* argv[])
{
	// Initialise OpenGL window...
	gl_init(argc, argv, update, "CIS6007 WRIT1 - Game of Life Demo", true);

	// Initialise our Game of Life starting pattern...
	initPattern(myArray);

	// Start the main loop - this calls our update function (below) to update our game of life generations.
	// Note: We never return from glutMainLoop until we quit the application
	glutMainLoop();

    return 0;
}


// Initialise grid with "alive" and "dead" cell patterns
void initPattern(Array2D<int>& array) {

	// 1. First clear the array (set all cells to "dead" ie. 0)
	parallel_for(
		0,
		array.w * array.h,
		[&](int i) {

			int x = i % array.w;
			int y = i / array.h;
			array(x, y) = 0;
		}
	);

	// Setup some cells to be "alive" - this is the initial state
	// NOTE: *** REPLACE THIS WITH YOUR OWN CODE TO SETUP THE LIVE CELL PATTERNS
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    parallel_for(
        0,
        array.w * array.h,
        [&](int i) {

            int x = i % array.w;
    int y = i / array.h;
    array(x, y) = dis(gen);
        }
    );

}

// Main update function - apply Game of Life rules here
int countLiveCells(Array2D<int>& array) {
	int liveCells = 0;
	parallel_for(
		0,
		array.w * array.h,
		[&](int i) {
			int x = i % array.w;
	int y = i / array.h;
	if (array(x, y) == 1) {
		liveCells++;
	}
		}
	);
	return liveCells;
}
// Variables for time averaging
double totalTime = 0;
int genCount = 0;

// Note: This is called by FreeGLUT's main event loop - YOU DON'T NEED TO CALL THIS - IT'S CALLED FOR YOU!!!
void update(void) {

    Array2D<int> newArray = Array2D<int>(64, 64);
    // Initialize the newArray to all dead cells
    

    //
    // Sequential or parallel code to update Game of Life
   
    // Sequential 
    
    //for (int x = 0; x < myArray.h; x++) {
        //for (int y = 0; y < myArray.w; y++) {

    //Parallel
    parallel_for(
        0,
        myArray.w * myArray.h,
        [&](int i) {
            int x = i % myArray.w;
    int y = i / myArray.h;
    


    int count = 0; // Count of live neighbors

    // Check the status of each neighbor
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue; // Skip the current cell
            }
            int neighbor_x = x + i;
            int neighbor_y = y + j;

            // Check if the neighbor is out of bounds
            if (neighbor_x < 0 || neighbor_x >= myArray.h || neighbor_y < 0 || neighbor_y >= myArray.w) {
                continue;
            }

            // Check if the neighbor is alive
            if (myArray(neighbor_x, neighbor_y) == 1) {
                count++;
            }
        }
    }

    // Apply the Game of Life rules
    if (myArray(x, y) == 1 && (count < 2 || count > 3)) {
        newArray(x, y) = 0; // Cell dies due to under- or overpopulation
    }
    else if (myArray(x, y) == 0 && count == 3) {
        newArray(x, y) = 1; // Cell becomes alive due to reproduction
    }
    else {
        newArray(x, y) = myArray(x, y); // Cell remains the same
    }
        }
      );
   // }
    
    

    int numTrials = 10; // or any other desired value
    double totalTime = 0;

    // Update myArray with the new state
    // add timer here
    for (int trial = 0; trial < numTrials; trial++) {
        auto start = std::chrono::high_resolution_clock::now();


    parallel_for(
        0,
        myArray.w * myArray.h,
        [&](int i) {
            int x = i % myArray.w;
    int y = i / myArray.h;
    myArray(x, y) = newArray(x, y);
        }
    );
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    totalTime += duration;
    }

    double avgTime = totalTime / numTrials;
    std::cout << "Average time for " << numTrials << " generations: " << avgTime << " microseconds" << std::endl;

    //this_thread::sleep_for(chrono::milliseconds(90));
    renderLatestGeneration(myArray);
}




