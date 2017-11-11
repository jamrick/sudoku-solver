// Jonathon Hamrick
// Implementation of sudoku solver
// 7/31/17

#include "Sudoku.h"
#include <fstream>
#include <iostream>

using namespace std;

// cell struct method definitions
SudokuSolver::Cell::Cell()
{
	nVal = 0;
	for (int i = 1; i <= 9; ++i)
	{
		nvPossible.Append(i);
	}
}
void SudokuSolver::Cell::CompleteCell(int n)
{
	nVal = n;
	if (nVal != 0)
		nvPossible.Clear();
}
void SudokuSolver::Cell::ResetCell()
{
	nVal = 0;
	nvPossible.Clear();
	for (int i = 1; i <= 9; ++i)
	{
		nvPossible.Append(i);
	}
}
bool SudokuSolver::Cell::RemovePossible(int n)
{
	for (int i = 0; i < nvPossible.Size(); ++i)
	{
		if (nvPossible[i] == n)
		{
			nvPossible.Remove(i);
			if (nvPossible.Size() == 1)
			{
				CompleteCell(nvPossible[0]);
			}
			return true;
		}
	}
	return false;
}


// sudokusolver class method definitions
SudokuSolver::SudokuSolver()
{
	ResetCells();
}

void SudokuSolver::LoadSudokuLevel(const char* fileName)
{
	if (fileName != NULL)
	{
		ifstream ifs(fileName);
		if (ifs.is_open())
		{
			ResetCells();
			int r = 0, c = 0;
			for (;;)
			{
				char in = ifs.get();
				if (ifs.eof())
					break;
				if (in == '\n')
					continue;

				SudokuLevel[r][c++].CompleteCell((int)(in - '0'));

				if (c == 9)
				{
					r++;
					c = 0;
				}
			}
			ifs.close();
		}

		//// level is loaded, take out exisitng values
		//for (int startRow = 1; startRow < 9; startRow += 3)
		//{
		//	for (int startCol = 1; startCol < 9; startCol += 3)
		//	{
		//		RemoveExisting(startRow, startCol);
		//	}
		//}
	}
}
void SudokuSolver::ResetCells()
{
	for (int r = 0; r < 9; r++)
		for (int c = 0; c < 9; c++)
			SudokuLevel[r][c].ResetCell();
}
void SudokuSolver::Print() const
{
	for (int r = 0; r < 9; r++)
	{
		for (int c = 0; c < 9; c++)
		{
			int val = SudokuLevel[r][c].nVal;
			if (val == 0)
			{
				cout << '{';
				for (int i = 0; i < SudokuLevel[r][c].nvPossible.Size(); ++i)
					cout << SudokuLevel[r][c].nvPossible[i] << ',';
				cout << '}';
			}
			else
			{
				cout << val;
			}
			cout << ' ';
		}
		cout << '\n';
	}
}
bool SudokuSolver::CompleteSudoku() const
{
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			if (SudokuLevel[i][j].nVal == 0)
				return false;
		}
	return true;
}
void SudokuSolver::RemoveExisting(int row, int col)
{
	int adjRow1 = row - 1, adjRow2 = row + 1;
	int adjCol1 = col - 1, adjCol2 = col + 1;

	for (int r = adjRow1; r <= adjRow2; ++r)
	{
		for (int c = adjCol1; c <= adjCol2; ++c)
		{
			// remove the val from the other possible numbers 
			for (int i = 0; i < 9; ++i)
			{
				// row
				SudokuLevel[r][i].RemovePossible(SudokuLevel[r][c].nVal);
				// col
				SudokuLevel[i][c].RemovePossible(SudokuLevel[r][c].nVal);
			}

			for (int r2 = adjRow1; r2 <= adjRow2; ++r2)
			{
				for (int c2 = adjCol1; c2 <= adjCol2; ++c2)
				{
					if (c == c2 && r == r2)
						continue;
					SudokuLevel[r][c].RemovePossible(SudokuLevel[r2][c2].nVal);
				}
			}
		}
	}
}

// solution algorithm putting everything together to solve the sudoku board.
void SudokuSolver::Solve()
{
	while (!CompleteSudoku())
	{
		Update();
	}
}

void SudokuSolver::Update()
{
	// search the grid and remove any possible values that have already been solved
	for (int startRow = 1; startRow < 9; startRow += 3)
	{
		for (int startCol = 1; startCol < 9; startCol += 3)
		{
			RemoveExisting(startRow, startCol);
			CheckPossibleRow(startRow, startCol);
			CheckPossibleCol(startRow, startCol);
		}
	}
}

void SudokuSolver::CheckPossible(int row, int col)
{
	int adjRow1 = row - 1, adjRow2 = row + 1;
	int adjCol1 = col - 1, adjCol2 = col + 1;

	//bool found = true;
	for (int r = adjRow1; r <= adjRow2; ++r)
	{
		for (int c = adjCol1; c <= adjCol2; ++c)
		{
			//// remove the val from the other possible numbers 
			//for (int pos = 0; pos < SudokuLevel[r][c].nvPossible.Size(); ++pos)
			//{
			//	
			//	//bool skip = false;
			//	for (int i = 0; i < 9/* && found == false*/; ++i)
			//	{
			//		if (i == c)
			//			continue;

			//		
			//		//// row
			//		for (int rows = 0; rows < SudokuLevel[r][i].nvPossible.Size() && found == false; ++rows)
			//		{
			//			found = false;
			//			//if (i == c)
			//			//{
			//			//	//skip = true;
			//			//	found = true;
			//			//	break;
			//			//}
			//			found = SudokuLevel[r][c].nvPossible[pos] == SudokuLevel[r][i].nvPossible[rows]/* || SudokuLevel[r][c].nvPossible[pos] == SudokuLevel[r][i].nVal*/;
			//		}
			//		if (found == false/* && skip == false*/)
			//		{
			//			SudokuLevel[r][c].CompleteCell(SudokuLevel[r][c].nvPossible[pos]);
			//			break;
			//		}
			//		else
			//			continue;
			//		//else if (skip == true)
			//		//{
			//		//	skip = false;
			//		//	//continue;
			//		//}
			//	}

				//for (int r2 = adjRow1; r2 <= adjRow2 && found == false; ++r2)
				//{
				//	for (int c2 = adjCol1; c2 <= adjCol2 && found == false; ++c2)
				//	{
				//		if (c == c2 && r == r2)
				//			continue;
				//		for (int box = 0; box < SudokuLevel[r2][c2].nvPossible.Size() && found == false; ++box)
				//		{
				//			found = SudokuLevel[r][c].nvPossible[pos] == SudokuLevel[r2][c2].nvPossible[box];
				//		}
				//	}
				//}

				//if (found == false)
				//{
				//	SudokuLevel[r][c].CompleteCell(SudokuLevel[r][c].nvPossible[pos]);
				//	return;
				//}
		}
	}
}

void SudokuSolver::CheckPossibleRow(int row, int col)
{
	int adjRow1 = row - 1, adjRow2 = row + 1;
	//int adjCol1 = col - 1, adjCol2 = col + 1;
	for (int r = adjRow1; r <= adjRow2; ++r)
	{
		//for (int c = adjCol1; c <= adjCol2; ++c)
		//{
			
			for (int c = 0; c < 9; ++c)
			{
				for (int i = 0; i < SudokuLevel[r][c].nvPossible.Size(); ++i)
				{
					bool found = false;
					for (int c2 = 0; c2 < 9; ++c2)
					{
						if (c == c2)
						{
							continue;
						}
						for (int i2 = 0; i2 < SudokuLevel[r][c2].nvPossible.Size() && found == false; ++i2)
						{
							found = SudokuLevel[r][c].nvPossible[i] == SudokuLevel[r][c2].nvPossible[i2];
						}
						if (found == true)
						{
							break;
						}
					}
					if (found == false)
					{
						SudokuLevel[r][c].CompleteCell(SudokuLevel[r][c].nvPossible[i]);
						//c = -1;
						break;
					}
				}
			}
		//}
	}
}
void SudokuSolver::CheckPossibleCol(int row, int col)
{
	//int adjRow1 = row - 1, adjRow2 = row + 1;
	int adjCol1 = col - 1, adjCol2 = col + 1;
	//for (int r = adjRow1; r <= adjRow2; ++r)
	{
		for (int c = adjCol1; c <= adjCol2; ++c)
		{

		for (int r = 0; r < 9; ++r)
		{
			for (int i = 0; i < SudokuLevel[r][c].nvPossible.Size(); ++i)
			{
				bool found = false;
				for (int r2 = 0; r2 < 9; ++r2)
				{
					if (c == r2)
					{
						continue;
					}
					for (int i2 = 0; i2 < SudokuLevel[r2][c].nvPossible.Size() && found == false; ++i2)
					{
						found = SudokuLevel[r][c].nvPossible[i] == SudokuLevel[r2][c].nvPossible[i2];
					}
					if (found == true)
					{
						break;
					}
				}
				if (found == false)
				{
					SudokuLevel[r][c].CompleteCell(SudokuLevel[r][c].nvPossible[i]);
					//c = -1;
					break;
				}
			}
		}
		}
	}
}