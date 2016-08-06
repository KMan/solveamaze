// kMaze.cpp : Defines the entry point for the console application.
//
/********************************************************************
	author	:	Kamran H. Khan - kamran_h_khan@yahoo.com
	web		:	http://geocities.com/sharpsucker/index.htm

	purpose	:	A prerequisite for finding an opportunity in ITA.
	
	Working	:	ASCII A-maze-ment
	This program re-output the maze with solution path filled, if input is 
	guaranteed to be a well-formed maze and to have a unique solution 
	path from the bottom left grid cell to the top right grid cell.
	
*********************************************************************/

#include "stdafx.h"
#include "kMaze.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
**Function	: Main.
**Purpose	: Main entry point.
**Arguments	: number of arguments, arguments list.
**Return	: Nothing.
*/
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	CKMaze m_k;
	if(argc<2)
	{
 		m_k.ErrorScreen("\nError: No input file specified on commandline \
		\nUsage: kmazes.exe <inputN.txt>\nN=Number \
		\nNOTE: The output will be generated as <outputN.txt>\n");
 		return -1;
	}
	strcpy(m_k.cFileName, argv[1]);
 	m_k.StartMazing();
	return 0;
}

/*
**Function	: Default constructor.
**Purpose	: Initializations of local variables.
**Arguments	: None.
**Return	: None.
*/
CKMaze::CKMaze()
{
  	pCharBuffer = NULL;
	bSolved = FALSE;
	mazeWidth = 0;
	mazeHeight = 0;
}

/*
**Function	: Default destructor.
**Purpose	: Garbage collection and cleaning.
**Arguments	: None.
**Return	: None.
*/
CKMaze::~CKMaze()
{
 	delete[] pCharBuffer;
	delete pCharBuffer;
	pCharBuffer= NULL;
	free(cFileName);
}
	
/*
**Function	: Start Mazing.
**Purpose	: Starts the path search.
**Arguments	: Nothing.
**Return	: Nothing.
*/
void CKMaze::StartMazing(void)
{
  	GetDimensions(mazeHeight, mazeWidth, cFileName);

	char* pCharBuffer = new char[mazeWidth * mazeHeight];
	FillBufferFromFile(cFileName, pCharBuffer, mazeWidth, mazeHeight);
	
 	int POSITION_Y= mazeHeight -2  ; //this will always start from totalHeight - 1

	pCharBuffer[ POSITION_X + POSITION_Y * mazeWidth] = MARK_TRACK;
	pCharBuffer[ (POSITION_X+1) + POSITION_Y * mazeWidth] = MARK_TRACK;

   	int nCurrentPos = POSITION_X + POSITION_Y * mazeWidth ;
 	SearchPath(pCharBuffer, nCurrentPos, mazeWidth, mazeHeight);

 	if(!bSolved)
		cout<<"Un-MazeAble: incorrect maze format found... not done!\nPlease select the maze having correct format."<<endl;
	else
		FillFileFromBuffer(cFileName, pCharBuffer, mazeWidth, mazeHeight);
} 

/*
**Function	: Get Dimensions.
**Purpose	: Retrives the Height and Width of the input maze.
**Arguments	: CharacterBuffer, Reference to MazeWidth, Reference to MazeHeight.
**Return	: Nothing.
*/
void CKMaze::GetDimensions(int &nHeight, int &nWidth, char* cFileName)
{
	FILE* pFile;
 	char ch;
	int h=0;
	int w=0;
	if( (pFile = fopen(cFileName, "r")) == NULL)
	{
		ErrorScreen("Invalid file name" );
		return;
	}
	BOOL bWidth=TRUE;
	while(!feof(pFile))
	{
		ch = fgetc(pFile);
 		if(ch=='T' || ch=='A')
			continue;
		else if(ch=='\n') 
			++h, bWidth =FALSE;
		else if(bWidth)
			++w;
	}
 	nHeight=h;
	nWidth=w;
}

/*
**Function	: Fill Buffer From File.
**Purpose	: Gets the maze's walls information from file and saves it 
**			  in a dynamic buffer.
**Arguments	: File name, CharacterBuffer, MazeWidth, MazeHeight.
**Return	: Nothing.
*/
void CKMaze::FillBufferFromFile(char cFileName[], char pCharBuffer[], int mazeWidth, int mazeHeight)
{
	char ch=NULL;
	FILE* fpInput=NULL;
	if( (fpInput = fopen(cFileName, "r")) == NULL)
	{
		ErrorScreen("Invalid file name" );
		return;
	}
	int xPos=0, yPos=0;
	while(!feof(fpInput))
	{
		ch = fgetc(fpInput);
		if(ch=='T' || ch=='A')
			continue;
		else if(ch=='\n')
			xPos=0, yPos++; 
		else
		{
			if(yPos<mazeHeight)
			{
				if(xPos<mazeWidth)
				{
					pCharBuffer[xPos + yPos * mazeWidth]= ch;
  					xPos++;
				}
				else
				{
					xPos=0;
					yPos++;
				}
			}
		}
	}
}

/*
**Function	: Search Path
**Purpose	: Main Search Algorithm, Up, down, Right, left.
**Arguments	: Buffer, Current Position, Maze Width, Maze Height.
**Return	: Nothing.
*/
int CKMaze::SearchPath(char pCharBuffer[], int nCurrentPos, int mazeWidth, int mazeHeight)
{
	static int NSEW[4]= {-mazeWidth,mazeWidth,1,-1}; //North South East West.
	int nNextPos=0;
 	for (int i=0; i<4 ; i++) 
	{	
		if (!bSolved) 
		{
			//up or down is only at 0 or 1
 			if(i<2) 
			{
				// skip a cell while up/down
 				nNextPos = ( nCurrentPos + (NSEW[i] * 2) );
 				if ((pCharBuffer[nCurrentPos + NSEW[i] ] == ' ') && (pCharBuffer[nNextPos] == ' ')) 
				{
 					
					pCharBuffer[nNextPos    ] = MARK_TRACK;
					pCharBuffer[nNextPos + 1] = MARK_TRACK;
					//End has been reached if the upper-right corner
					//contains I 
 					if(pCharBuffer[( (nNextPos +2) - mazeWidth) ] == 'I')
 						bSolved = TRUE;
					//Search Again.
   					SearchPath(pCharBuffer, nNextPos, mazeWidth, mazeHeight);
 					if(!bSolved)
					{
						//if not solved(all sides are walls) than 
						//apply spaces to the paths having dead-end.
						pCharBuffer[nNextPos  ] = ' ';
						pCharBuffer[nNextPos+1] = ' ';
					} 
				}
 			}
			//left or right are on 2 and 3.
			else 
			{
				 //skip two cells while left/right
 				nNextPos = (nCurrentPos + (NSEW[i] * 3) );
				if ((pCharBuffer[nCurrentPos + NSEW[i] * 3 ] == ' ') && (pCharBuffer[nCurrentPos + NSEW[i] * 2] == ' ') && (pCharBuffer[nCurrentPos + NSEW[i] ] != '|')) 
				{
					pCharBuffer[nNextPos	] = MARK_TRACK;
					pCharBuffer[nNextPos + 1] = MARK_TRACK;
 					if(pCharBuffer[( (nNextPos +2) - mazeWidth) ] == 'I')
 						bSolved = TRUE;
  					SearchPath(pCharBuffer, nNextPos, mazeWidth, mazeHeight);
 					if(!bSolved)
					{
						pCharBuffer[nNextPos  ] = ' ';
						pCharBuffer[nNextPos+1] = ' ';
					} 
				}
			}
		}
 
	}
	return FALSE;
}
 
/*
**Function	: Fill File From Buffer.
**Purpose	: Gets the solved path from the array and saves it
**			  into the file having same number but renamed as
**			  ouput. E.g.: outputN.txt where n=number.
**Arguments	: File Name, Character Buffer, mazeHeight, mazeWidth.
**Return	: Nothing.
*/
void CKMaze::FillFileFromBuffer(char cFileName[], char pCharBuffer[], int mazeWidth, int mazeHeight)
{
	char ch[2]= { cFileName[5], cFileName[6]};
	FILE* fpOutput=NULL;
	char filename[13] = "outputX.txt";

	if(ch[1] != '.')
	{
		strcpy(filename,"outputXX.txt");
		filename[6] = ch[0];
		filename[7] = ch[1];
	}
	else
		filename[6] = ch[0];

	if( (fpOutput= fopen(filename, "w")) == NULL)
	{
		ErrorScreen("Invalid file name" );
	}
	else
	{
		for(int y = 0; y < mazeHeight; y++)				
		{
			for(int x = 0; x < mazeWidth; x++)			
 				fprintf(fpOutput, "%c", pCharBuffer[x + y * mazeWidth]);
 			if(!y)
				fprintf(fpOutput, "TA");
			fprintf(fpOutput, "\n");
		}
	
		fclose(fpOutput);
		cout<<"Please checkout the file "<<filename<<" in the current directory."<<endl;
	}

	return;
}

/*
**Function	: Show Maze, only if one may want to view it on screen.
**Purpose	: Use '/d' to Display the Array containing the 
**			  filled/unfilled region.
**Arguments	: CharacterBuffer, MazeWidth, MazeHeight.
**Return	: Nothing.
*/
void CKMaze::ShowMaze(char pCharBuffer[], int mazeWidth, int mazeHeight)
{
 	for(int y = 0; y < mazeHeight; y++)				
	{
		for(int x = 0; x < mazeWidth; x++)			
		{												
			cout<<pCharBuffer[x + y * mazeWidth]<<endl;
		}
		cout<<endl;
	}	
}
 
/*
**Function	: Error Screen.
**Purpose	: Display the Error information and the reason that 
**			  caused the error.
**Arguments	: Comments.
**Return	: Nothing.
*/
void CKMaze::ErrorScreen(char *pszComment)
{
   LPVOID lpvMessageBuffer;
   CHAR szPrintBuffer[512];
   DWORD nCharsWritten;

   FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpvMessageBuffer, 0, NULL);

   wsprintf(szPrintBuffer,
     "ERROR:\n\tName\t= %s.\n\tError Code  = %d.\n\tMessage\t= %s.\n",
            pszComment, GetLastError(), (char *)lpvMessageBuffer);

   WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),szPrintBuffer,
                 lstrlen(szPrintBuffer),&nCharsWritten,NULL);

   LocalFree(lpvMessageBuffer);
   ExitProcess(GetLastError());
}