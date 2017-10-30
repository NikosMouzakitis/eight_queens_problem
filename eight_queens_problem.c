#include <stdio.h>
#include <stdlib.h>
#define MEM_SIZE 8

// 	Using backtrace algorithm to solve the eight queens problem.
// 	author: Mouzakitis Nikolaos
// 	Crete 2017
//	---
//	Use of static memory for the save_state memory here but 
//	we could implement it dynamically with the malloc functions.
//	By using backtrace algorithm we do not need more than size 8 anyway.

struct save_state
{
	int board[8][8];
	int already_placed[8][8];
};

int step;

struct save_state memory[MEM_SIZE];

void board_display(int board[][8])
{
	int i,j;

	printf("----------------------------------------------------------\n");

	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			printf("%d\t",board[i][j]);
		}
		putchar('\n');
		putchar('\n');
	}
	printf("----------------------------------------------------------\n");
}

void check_update(int board[][8],int *state)
{

	int i,j;
	int cnt=0;

	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
			if(board[i][j] == 1)
				cnt++;
	if(cnt == 8)
		*state = 0;
	return;
}

void update_valid(int is_valid[][8],int row,int col)
{
	int i;

	for(i = 0; i < 8; i++)
		is_valid[row][i] = is_valid[i][col] = 0;

	//	diagonal expressions.
	
	for( i = 0; ( (row+i) < 8 ) && ( (col+i ) < 8 ) ; i++ )
		is_valid[row+i][col+i] = 0;

	for( i = 0; ( (row-i) >= 0 ) && ( (col-i ) >= 0) ; i++ )
		is_valid[row-i][col-i] = 0;

	for( i = 0; ( (row+i) < 8 ) && ( (col-i ) >= 0 ) ; i++ )
		is_valid[row+i][col-i] = 0;

	for( i = 0; ( (row-i) >= 0 ) && ( (col+i ) < 8 ) ; i++ )
		is_valid[row-i][col+i] = 0;
}

void pr_mem(void)	//	debugging purpose function.
{
	int i,j;

	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			printf("%d ",memory[step].already_placed[i][j]);
		}
		putchar('\n');
	}
}

void update_whole_valid(int board[][8],int is_valid[][8])
{
	int i,j;

	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
			is_valid[i][j] = 1;
	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
			if(board[i][j] == 1)
				update_valid(is_valid,i,j);
}

void save(int board[][8],int i,int j)
{
	int k,l;

	// saving the new board.
	for(k = 0; k < 8; k++)
		for(l = 0; l < 8; l++)
			 memory[step].board[k][l] = board[k][l];

	memory[step].already_placed[i][j] = 1;
	//pr_mem();
	step++; // increasing global variable step.
}

int dead_end_check(int is_valid[][8])
{
	int i,j;

	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
			if(is_valid[i][j]== 1)
				return 0;
	return 1;	// means we have reached on a dead end situation.Need to load previous state of memory.
}
void debug(void)
{
	int i,j;

	printf("------debug-----\n");

	for(i = 0; i < 8; i++)
		for( j = 0; j < 8; j++)
			if(memory[step].already_placed[i][j] == 1)
				printf("%d %d\n",i,j);
	printf("-----------------\n");

}

void load(int board[][8])
{
	int i,j;

	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
			memory[step].already_placed[i][j] = 0;	// essential to happen here.!
	step--;
	
	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
			board[i][j] = memory[step].board[i][j];
}

void solve(int board[][8],int is_valid[][8])
{
	int i,j;

	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			if( (board[i][j] == 0) && (is_valid[i][j] == 1) && (memory[step].already_placed[i][j] == 0 ) )
			{
				save(board,i,j); /*		will save the state before we continue
				       	             		and place a queen in the board. */
				board[i][j] = 1;
				update_valid(is_valid,i,j);

				return;
			}
		}
	}

	load(board);
	update_whole_valid(board,is_valid);
}

int main(int argc,char *argv[],char *envp[])
{
	int board[8][8],is_valid[8][8];
	int i,j,run_state = 1,round = 0;

	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			is_valid[i][j] = 1;
			board[i][j] = 0;
		}
	}
	
	while(run_state)
	{
		solve(board,is_valid); 				   //		The solving function.
		check_update(board,&run_state);			   //		whether the machine stops execution or not.
		if( dead_end_check(is_valid) && (run_state == 1) ) // 		Essential to check the run state,otherwise
		{						   // 		on completion, it will take us one state behind.
			load(board);
			update_whole_valid(board,is_valid);
		}
	}

	printf("________________Solution display_________________\n");	
	board_display(board);

	return 0;
}