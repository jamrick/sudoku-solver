// Jonathon Hamrick
// Main entrypoint of program
// 7/31/17

/*INCLUDES*/
#include <Windows.h>
#include <iostream>
#include "GameTimer.h"
#include "Sudoku.h"

/*PREPROCESSOR CONSTANTS*/
#define BENCHMARKING false

int main()
{
	SudokuSolver sudokuSolver;
	sudokuSolver.LoadSudokuLevel("SudokuTest.txt");

	

#if BENCHMARKING	// benchmarking code, strickly timing the length of time to find the solution
	GameTimer jt;
	jt.StartTime();
	sudokuSolver.Solve();
	DWORD solved = jt.ElapsedTime();
	system("cls");
	sudokuSolver.Print();
	std::cout << solved / 1000.0f << " secs solved\n";
#else				// showing the solution as it happens with a short sleep between prints
	while (!sudokuSolver.CompleteSudoku())
	{
		system("cls");
		sudokuSolver.Update();
		sudokuSolver.Print();
		Sleep(250);
	}
#endif

	system("Pause");
	return 0;
}