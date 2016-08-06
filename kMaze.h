/********************************************************************
 	filename	: 	F:\MyProjects\c++\KMaze\kMaze.h
	file path	:	F:\MyProjects\c++\KMaze
	file base	:	kMaze
	file ext	:	header
	author		:	Kamran H. Khan - kamran_h_khan@yahoo.com
	
	Working		:	ASCII A-maze-ment
	This program re-output the maze with solution path filled, if input is 
	guaranteed to be a well-formed maze and to have a unique solution 
	path from the bottom left grid cell to the top right grid cell.
	
*********************************************************************/

#if !defined(AFX_KMAZE_H__FE439F3C_C7DE_4938_9867_032559AB6429__INCLUDED_)
#define AFX_KMAZE_H__FE439F3C_C7DE_4938_9867_032559AB6429__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

//Resuable class k maze.
class CKMaze
{
public:
	//Default constructor
 	CKMaze();
	//Default destructor
	~CKMaze();
	//Show the error information on screen
 	void ErrorScreen(char *pszComment);
	//Get the maximum height and width of the input maze from file.
	void GetDimensions(int &nHeight, int &nWidth, char* cFileName);
	//Get data from file into array
 	void FillBufferFromFile(char cFileName[],char pCharBuffer[], int mazeWidth, int mazeHeight);
	//Display the filled/unfilled path in the array
	void ShowMaze(char pCharBuffer[], int mazeWidth, int mazeHeight);
	//Get the solved path from array the save it into the file.
	void FillFileFromBuffer(char cFileName[], char pCharBuffer[], int mazeWidth, int mazeHeight);
	//Search a solveable path in the maze.
	int	 SearchPath(char pCharBuffer[], int nCurrentPos, int mazeWidth, int mazeHeight);
	//Initializes and starts searching the maze.
	void StartMazing(void);

public:
	//Total maze width
	int mazeWidth;
	//Total maze height
	int mazeHeight;

	//Character pointer which will be used to point an array of height x width
	char* pCharBuffer;
	
	//File name, complete twelve character, FILENAME.TXT = 8+1+3=12
	char cFileName[12];

	//A flag to check whether its been solved or not?
 	BOOL bSolved;


};

#endif // !defined(AFX_KMAZE_H__FE439F3C_C7DE_4938_9867_032559AB6429__INCLUDED_)

