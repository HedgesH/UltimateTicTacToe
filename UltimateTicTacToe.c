//ULTIMATE TIC TAC TOE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//GLOBAL VARIABLES
char board[18][18];
int regions[3][3]= {{1,2,3},{4,5,6},{7,8,9}};
int regtocoord[9][2] = {{0,0},{1,0},{2,0},{0,1},{1,1},{2,1},{0,2},{1,2},{2,2}};

//ENUMS
enum player { X, O, N };
typedef enum player player;

enum validity { Unchecked, BadFormat, BadLetter, BadDigit, BadCell,WrongRegion, OK };
typedef enum validity validity;

//The game structure
struct game{
	char grid[3][3][9];
	int moves;
	player next;
	int currentRegion;
	char winBoard[3][3];
};

typedef struct game game;

//returns 2 coordinates

struct coordinates{
	int x;
	int y;
};

typedef struct coordinates coordinates;

////LOGIC OF GAME

//new game

void newGame(game *g, player first){

	for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
        	for (int k = 0; k < 9; ++k)
        	{
        		g -> grid[i][j][k] =  N;
        	}
        	g -> winBoard[i][j] = N;


            
        }
    }

    g->currentRegion = 5;

    g -> moves = 0;

    g -> next = first;
}

//convert coordinate to row and col

int row(char *coord) {
    return *(coord+1) - '1';
}

int col(char *coord) {
    return *(coord) - 'a';
}

//valid region chcker

int validReg(game *g, int r, int c){
	int reg = g->currentRegion;
	if(reg == 1 && r<3 && c<3) return 1;
	if(reg == 2 && r<3 && (c<6 && c>2)) return 1;
	if(reg == 3 && r<3 && (c<9 && c>5)) return 1;
	if(reg == 4 && (r<6 && r>2) && c<3) return 1;
	if(reg == 5 && (r<6 && r>2) && (c<6 && c>2)) return 1;
	if(reg == 6 && (r<6 && r>2) && (c<9 && c>5)) return 1;
	if(reg == 7 && (r<9 && r>5) && c<3) return 1;
	if(reg == 8 && (r<9 && r>5) && (c<6 && c>2)) return 1;
	if(reg == 9 && (r<9 && r>5) && (c<9 && c>5)) return 1;
	return 0;
}

//validity checker for input coordinates

validity valid(game *g, char *coord){

	if (strlen(coord) != 2) return BadFormat;
    if(*coord < 'a' || *coord > 'i') return BadLetter;
    if(*(coord+1) < '1' || *(coord+1) > '9') return BadDigit;
    if( g -> grid[row(coord)%3][col(coord)%3][g->currentRegion - 1] != N) return BadCell;
    if ( validReg(g,row(coord),col(coord)) == 0) return WrongRegion;
    return OK;

}

//converts validity enum to a string

char* errorString(validity v){
	char *p = malloc(30*sizeof(char));
	if(v == 0) p = "Unchecked";
	if(v == 1) p = "Bad Format";
	if(v == 2) p = "Bad Letter";
	if(v == 3) p = "Bad Digit";
	if(v == 4) p = "Bad Cell";
	if(v == 5) p = "Wrong Region";
	if(v == 6) p = "Ok";

	return p;

}

//turns player to string

char* playerString(player p){
	char *q = malloc(sizeof(char));
	if (p == X) *q = 'X';
	else *q = 'O';
	return q;
	
}



//next region checker

int nextRegion(int row, int col){
	return regions[row % 3][col % 3];
}


//line checker for 3 in a row

player line(player one, player two, player three){
	if ( one == two && one == three) return one;
    return N;

}

//3x3 board win checker

void win3x3(game *g){
	int reg = g-> currentRegion;
	player winner = N;
	for (int i = 0; i < 3; ++i)
    {
    	player a = line( g -> grid[i][0][reg-1] , g -> grid[i][1][reg-1] , g -> grid[i][2][reg-1] );
        player b = line( g -> grid[0][i][reg-1] , g -> grid[1][i][reg-1] , g -> grid[2][i][reg-1] );
        if (a != N ) winner = a;
        if (b != N ) winner = b;
    }
    player a = line( g -> grid[0][0][reg-1] , g -> grid[1][1][reg-1] , g -> grid[2][2][reg-1] );
    player b = line( g -> grid[0][2][reg-1] , g -> grid[1][1][reg-1] , g -> grid[2][0][reg-1] );
    if (a != N) winner = a;
    if (b != N) winner = b;
    
    if (winner != N && (g -> winBoard[regtocoord[(g->currentRegion)-1][0]][regtocoord[(g->currentRegion)-1][1]] == N))
    {
		g -> winBoard[regtocoord[(g->currentRegion)-1][0]][regtocoord[(g->currentRegion)-1][1]] = winner;
		printf("board %d won \n", g -> currentRegion );
	}
}

//9x9 board win checker

player win9x9(game *g){
	for (int i = 0; i < 3; ++i)
	{
	    player a = line( g -> winBoard[i][0] , g -> winBoard[i][1] , g -> winBoard[i][2] );
	    player b = line( g -> winBoard[0][i] , g -> winBoard[1][i] , g -> winBoard[2][i] );
	    if (a != N ) return a;
		if (b != N) return b;
	}
	player a = line( g -> winBoard[0][0] , g -> winBoard[1][1] , g -> winBoard[2][2] );
	player b = line( g -> winBoard[0][2] , g -> winBoard[1][1] , g -> winBoard[2][0] );
	if (a != N) return a;
	if (b != N) return b;
	return N;
}

//move player

void move(game *g, int row, int col){
	int r = row%3;
	int c = col%3;
	player play = g -> next;
	int reg = g -> currentRegion;
    g -> grid[r][c][reg-1] = play;
    if (g->next == X) g->next = O;
    else g->next = X;
    g->moves++;
    
}

//special rules here

//when the player has nowhere they can go :-/, chooses the easiest free

void completelyStuck(game *g){
	int reg = g->currentRegion - 1;
	int check = 0;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if(g->grid[i][j][reg] == N){
				check++;
			}
		}
	}

	if( check == 0 ){
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 9; ++k)
				{
					if(g->grid[i][j][k] == N){
						
						g->currentRegion = k + 1;
					}
				}
			}
		}
	}

}
 
////OUTPUT AND GRAPHICS


//board generator

void boardGuiCreate(){

	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int i = 6*x + 1; i < 6*x + 6; ++i)
			{
				for (int j = 6*y + 1; j< 6*y + 6; ++j)
				{
					if( (i % 6 == 2 || i % 6 == 4)  ) board[i][j] = '_';
					else if (j % 6 == 2 || j % 6 == 4) board[i][j] = '|';
					//input actual values
					else board[i][j] = ' ';
					
				}
				
			}
		}
	}
	for (int i = 0; i < 18; ++i)
	{
		board[6][i] = ' ';
		board[12][i] = ' ';
		board[i][6] = ' ';
		board[i][12] = ' ';
		board[0][i] = ' ';
		board[i][0] = ' ';

	}

}

//change letters and numbers displayed on GUI

void boardChange(game *g){
	int reg = g->currentRegion;
	if(reg == 1){
		board[0][1] = 'a';
		board[0][3] = 'b';
		board[0][5] = 'c';
		board[1][0] = '1';
		board[3][0] = '2';
		board[5][0] = '3';

	}
	if(reg == 2){
		board[0][7] = 'd';
		board[0][9] = 'e';
		board[0][11] = 'f';
		board[1][0] = '1';
		board[3][0] = '2';
		board[5][0] = '3';
		
	}
	if(reg == 3){
		board[0][13] = 'g';
		board[0][15] = 'h';
		board[0][17] = 'i';
		board[1][0] = '1';
		board[3][0] = '2';
		board[5][0] = '3';
		
	}
	if(reg == 4){
		board[0][1] = 'a';
		board[0][3] = 'b';
		board[0][5] = 'c';
		board[7][0] = '4';
		board[9][0] = '5';
		board[11][0] = '6';
		
	}
	if(reg == 5){
		board[0][7] = 'd';
		board[0][9] = 'e';
		board[0][11] = 'f';
		board[7][0] = '4';
		board[9][0] = '5';
		board[11][0] = '6';
		
	}
	if(reg == 6){
		board[0][13] = 'g';
		board[0][15] = 'h';
		board[0][17] = 'i';
		board[7][0] = '4';
		board[9][0] = '5';
		board[11][0] = '6';
		
	}
	if(reg == 7){
		board[0][1] = 'a';
		board[0][3] = 'b';
		board[0][5] = 'c';
		board[13][0] = '7';
		board[15][0] = '8';
		board[17][0] = '9';
		
	}
	if(reg == 8){
		board[0][7] = 'd';
		board[0][9] = 'e';
		board[0][11] = 'f';
		board[13][0] = '7';
		board[15][0] = '8';
		board[17][0] = '9';
		
	}
	if(reg == 9){
		board[0][13] = 'g';
		board[0][15] = 'h';
		board[0][17] = 'i';
		board[13][0] = '7';
		board[15][0] = '8';
		board[17][0] = '9';
		
	}

}

//empty border of the display board

void boardReset(){

	for (int i = 0; i < 18; ++i)
	{
		board[0][i] = ' ';
		board[i][0] = ' ';
	}

}

//print board

void printBoard(){
	for(int i =0 ; i <18;i++){

		if (i== 6 || i == 12) printf("\n");
		
		for (int j = 0; j < 18; ++j)
		{
			if (j == 6 || j == 12 || j == 0)
			{
				printf("   %c   ", board[i][j] );
			}
			else printf(" %c ", board[i][j] );
		}

		if (i== 5 || i == 11 || i == 0) printf("\n");
		
		printf("\n");
	}

}

//update board on the screen

void update(int x, int y, game *g){
	int tempx = 2*x + 1;
	int tempy = 2*y + 1;
	char insert;
	if (g->next == O) insert = 'O';
	else insert = 'X';
	board[tempx][tempy] = insert;

}

//print logic board of region you are in

void printBoard2(game *g){
	printf("current region for printboard2 %d\n",g->currentRegion );
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			printf(" %d ", g->grid[i][j][(g->currentRegion) - 1] );		
		}
		printf("\n");
	}

}

//prints the winboard, which is the whole 3x3 boards that have been won

void printWBoard2(game *g){
	printf("current region for printboard2 %d\n",g->currentRegion );
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			printf(" %d ", g->winBoard[i][j] );		
		}
		printf("\n");
	}

}


//INPUTS

char* input(game *g){
	char *coord = malloc(10 * sizeof(char));
	printf("Choose a coordinate please\n");
	scanf("%s",coord);
  	return coord;
}

//AI 

validity validAI(game *f, int r, int c){

	
    if( f->grid[r][c][f->currentRegion - 1] != N) return BadCell;
    return OK;

}

player LinePair(player one, player two, player three, int variant){
	if(variant == 1){
		if ( one == two || two == three) return two;
	}
	else{
		if ( one == two && one == three) return one;
	}
	
    return N;

}

player AIWin3x3(game *f, int var){
	int reg = f->currentRegion;
	for (int i = 0; i < 3; ++i)
    {
    	player a = LinePair( f->grid[i][0][reg-1] , f->grid[i][1][reg-1] , f->grid[i][2][reg-1], var );
        player b = LinePair( f->grid[0][i][reg-1] , f->grid[1][i][reg-1] ,  f->grid[2][i][reg-1],var );
        if (a != N ) return a;
        if (b != N ) return b;
    }
    player a = LinePair(f->grid[0][0][reg-1] ,f->grid[1][1][reg-1] , f->grid[2][2][reg-1],var );
    player b = LinePair( f->grid[0][2][reg-1] ,f->grid[1][1][reg-1] ,f->grid[2][0][reg-1],var );
    if (a != N) return a;
    if (b != N) return b;
    return N;
    
}

void gameCopy(game *f, game *g){

	for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
        	for (int k = 0; k < 9; ++k)
        	{
        		f -> grid[i][j][k] = g -> grid[i][j][k]  ;
        	}

        	f -> winBoard[i][j] = g -> winBoard[i][j];


            
        }
    }

    f->currentRegion = g->currentRegion;

    f -> moves = g->next;

    f -> next = g->next;


}

int DepthSearch(game *g, int row, int col, int depth, player p){
	game *f = malloc(sizeof(game));
	gameCopy(f,g);
	int reg = f->currentRegion;
	f->grid[row][col][reg-1] = f->next;
	f->next = 1-(g->next);
	f->currentRegion = nextRegion(row,col);
	int ans = 0;
	int wins = 0;
	if (depth != 0)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (validAI(f,i,j) != BadCell)
				{
					
					ans += DepthSearch(f,i,j,depth-1,p);
					
				}
				
			}
		}
		free(f);
		return ans;
	}
	else{

		for (int i = 1; i < 10; ++i)
		{
			f->currentRegion = i;
			
			player temp = AIWin3x3(f,0);
			player temp2 = AIWin3x3(f,1);
			player temp3 = win9x9(f);
			if(temp3 == p) wins = wins + 5000;
			if(temp3 == 1-p) wins = wins - 5000;
			if (temp == p) wins = wins + 3;
			if (temp == 1-p) wins = wins - 3;
			if(temp == N){
				if (temp2 == p) wins++;
				if (temp2 == 1-p) wins--;
			}
			
			
		}
		free(f);
		return wins;
		



	}

}


////GAMEPLAY LOOP

//actually play game

void play0(game *g,coordinates *c, int depth){
	
	int max = DepthSearch(g,0,0,depth,g->next);
	int row = -100;
	int col;
	int tempr;
	int tempc;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{

			if (validAI(g,i,j) != BadCell)
			{
				tempr = i;
				tempc = j;

				int temp = DepthSearch(g,i,j,depth,g->next);
				//printf("temp %d\n",temp);
				if(temp > max){
				max = temp;
				row = i;
				col = j;
				}
					
					
			}
			
		}
	}

	if (row == -100)
	{
		row = tempr;
		col = tempc;
	}

	int reg = g->currentRegion;
	c->y = row;
	c->x = col + 3*((reg-1)%3);
	if (reg == 4 || reg == 5 || reg == 6)
	{
		c->y += 3;
	}
	if (reg == 7 || reg == 8 || reg == 9)
	{
		c->y  += 6;
	}

	

}

void playX(game *g,coordinates *c){

 
	boardChange(g);
	printBoard();
	boardReset();

	//input coordinate
	printf("It is player X's turn\n");
	char *coord = input(g);

	//turn coord into points
	c->y = row(coord);
	c->x = col(coord);


	if (valid(g,coord) != OK)
	{
		printf("incorrect coordinate you fool: %s\n", errorString(valid(g,coord))  );
		c->x = -50;
	}

}

void play(const char *n){
	int depth = (*n) -'0';
	game *g = malloc(sizeof(game));
	coordinates *c = malloc(sizeof(coordinates));
	newGame(g,X);
	boardGuiCreate();

	while(win9x9(g) == N){

		completelyStuck(g);
		if (g->next == X) playX(g,c);
		else play0(g,c,depth);

		if (c->x == -50) continue;


		update(c->y ,c->x,g);
		move(g,c->y,c->x);
		win3x3(g);
		


		g->currentRegion = nextRegion(c->y,c->x);
		
		printf("\n\n---------------------------------------------------------------------\n\n");

	}

	printBoard();
	printf("YOU HAVE WON PLAYER %s, on depth %d SO COOL!!\n", playerString(1- (g->next)),depth);

}

//TESTS

//sets up special inputs to test the win3x3 function with
void setup(game *g, char *grid) {
    int xs = 0, os = 0, moves = 0;
    for (int r=0; r<3; r++) {
        for (int c=0; c<3; c++) {
            char ch = grid[r*4 + c];
            if (ch == 'X') { g->grid[r][c][0] = X; xs++; moves++; }
            else if (ch == 'O') { g->grid[r][c][0] = O; os++; moves++; }
            else g->grid[r][c][0] = N;
        }
    }
    g->moves = moves;
    if (xs == os || xs == os - 1) g->next = X;
    else if (xs == os + 1) g->next = O;
    else printf("Setup problem\n");
}

//test special win3x3 function

player testWin3x3(game *g){
	int reg = g-> currentRegion;
	for (int i = 0; i < 3; ++i)
    {
    	player a = line( g -> grid[i][0][reg-1] , g -> grid[i][1][reg-1] , g -> grid[i][2][reg-1] );
        player b = line( g -> grid[0][i][reg-1] , g -> grid[1][i][reg-1] , g -> grid[2][i][reg-1] );
        if (a != N ) return a;
        if (b != N ) return b;
    }
    player a = line( g -> grid[0][0][reg-1] , g -> grid[1][1][reg-1] , g -> grid[2][2][reg-1] );
    player b = line( g -> grid[0][2][reg-1] , g -> grid[1][1][reg-1] , g -> grid[2][0][reg-1] );
    if (a != N) return a;
    if (b != N) return b;
    return N;
    
}

//test row and col

void testRowCol() {
	
    assert(row("a3") == 2);
    assert(row("b2") == 1);
    
    assert(col("b1") == 1);
    assert(col("b2") == 1);
    
}

void testMove(game *g) {
    newGame(g, X);
    move(g, row("b2"), col("b2"));
    assert(g->grid[1][1][4] == X);
    assert(g->next == O);
    assert(g->moves == 1);
    move(g, row("a3"), col("a3"));
    assert(g->grid[2][0][4] == O);
    assert(g->next == X);
    assert(g->moves == 2);
}

void testLine() {
    assert(line(X, X, X) == X);
    assert(line(O, O, O) == O);
    assert(line(X, O, O) == N);
    assert(line(O, X, O) == N);
    assert(line(O, O, X) == N);
    assert(line(N, N, N) == N);
    assert(line(X, N, N) == N);
}

void test3x3Win(game *g) {
	g->currentRegion = 1;
    setup(g, "XXX -O- -O-");
    assert(testWin3x3(g) == X);
    setup(g, "-O- XXX -O-");
    assert(testWin3x3(g) == X);
    setup(g, "-O- -O- XXX");
    assert(testWin3x3(g) == X);
    setup(g, "O-- OX- O-X");
    assert(testWin3x3(g) == O);
    setup(g, "-O- XO- -OX");
    assert(testWin3x3(g) == O);
    setup(g, "--O X-O -XO");
    assert(testWin3x3(g) == O);
    setup(g, "X-O -XO --X");
    assert(testWin3x3(g) == X);
    setup(g, "X-O -OX O--");
    assert(testWin3x3(g) == O);
}

void testregion(){

	assert(nextRegion(8,8) == 9);
	assert(nextRegion(5,7) == 8);
}

void testPlay(){

	for (int i = 1; i < 7; ++i)
	{
		game *g = malloc(sizeof(game));
		coordinates *c = malloc(sizeof(coordinates));
		newGame(g,X);
		while(win9x9(g) == N){
			completelyStuck(g);
			play0(g,c,i);
			if (c->x == -50) continue;
			move(g,c->y,c->x);
			win3x3(g);
			g->currentRegion = nextRegion(c->y,c->x);
		}	
		printf("game depth: %d complete\n",i);

	}
	
}

void test(){
	game *g = malloc(sizeof(game));
	g->currentRegion = 1;

	testRowCol();
	testMove(g);
	testLine();
	test3x3Win(g);
	testregion();
	testPlay();
	printf("all tests passed mate\n");

}

int main(int argc, char const *argv[])
{
	
	if (argc == 2) play(*(argv + 1));
	else test();
	return 0;
}
