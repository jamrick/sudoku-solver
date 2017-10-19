// Jonathon Hamrick
// Main entrypoint of program
// 7/31/17

/*INCLUDES*/
#include <Windows.h>
#include "GameTimer.h"
#include "Sudoku.h"

/*PREPROCESSOR CONSTANTS*/
#define BENCHMARKING false

int main()
{
	SudokuSolver sudokuSolver;
	sudokuSolver.LoadSudokuLevel("SudokuTest.txt");
	sudokuSolver.PrintSudokuLevel();

#if BENCHMARKING	// benchmarking code, strickly timing the length of time to find the solution
	jhTimer jt;
	jt.StartTime();
	//sudokuSolver.LoadSudoku("SudokuTest.txt");
	while (!sudokuSolver.CompleteSudoku())
	{
		sudokuSolver.SolveSudoku();
	}
	DWORD solved = jt.ElapsedTime();
	system("cls");
	sudokuSolver.PrintSudoku();
	cout << solved / 1000.0f << " secs solved\n";
#else				// showing the solution as it happens with a short sleep between prints

#endif

	system("Pause");
	return 0;
}