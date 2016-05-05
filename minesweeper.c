/*
    Minesweeper game (Initial)
    for 6 x 6 
*/
#include "../../sdk/dexsdk.h"
#include "../../sdk/time.h"
#define SIZE 6

// Global Variables
//      State of the Board            Tiles Activated
    int boardState[SIZE][SIZE], boardActivated[SIZE][SIZE];

int checkNearbyMines(int x, int y){
    if(boardState[x][y] == 100) return 1;
    else if(boardState[x-1][y] == 100 && x>0) return 1;
    else if(boardState[x+1][y] == 100 && x<SIZE-1) return 2;
    else if(boardState[x][y-1] == 100 && y>0) return 3;
    else if(boardState[x][y+1] == 100 && y<SIZE-1) return 4;
    else return -1;   
}

void otherNeighbors(int x, int y,int mx, int my){

}

//activates all the neighboring tiles that does not contain mine
int checkNeighbors(int x, int y){
   
}

void printBoard(){
    int i,j;   
    for(i = 0; i < SIZE ; i++){
        for(j = 0; j < SIZE ; j++){
            if(boardActivated[i][j] == 1){ 
                if(boardState[i][j]!=100)printf("%d ", boardState[i][j]);
                else printf("* ");
            
            }
            else printf("- ");
        }
        printf("\n"); 
    }
}



//initializes the state of the board
// contains (SIZE^2)/3 mines
void initializeBoard(){
     int i,j,mines,rx,ry,count;
     
     mines = (SIZE * SIZE) * 0.3;
     
     srand(time(NULL));
     for(i = 0 ; i < mines; i++){
        rx = rand()%SIZE;
        ry = rand()%SIZE;
        while(boardState[rx][ry]==1){
            rx = rand()%SIZE;
            ry = rand()%SIZE;
        }
        boardState[rx][ry] = 100;
     }
     
      
        for(i = 0 ; i < SIZE ; i++){
            for(j = 0; j < SIZE; j++){
                count = 0;
                if(boardState[i][j]) continue;
                if(boardState[i-1][j] == 100 && i>0) count ++;
                if(boardState[i+1][j] == 100 && i<SIZE-1) count ++ ;
                if(boardState[i][j-1] == 100 && j>0) count ++;
                if(boardState[i][j+1] == 100 && j<SIZE-1) count ++;
                if(boardState[i-1][j-1] == 100 && i>0 && j>0 ) count ++;
                if(boardState[i-1][j+1] == 100 && i>0 && j<SIZE-1) count ++;
                if(boardState[i+1][j+1] == 100 && i<SIZE-1 && j<SIZE-1) count ++;
                if(boardState[i+1][j-1] == 100 && i<SIZE-1 && j>0) count ++;
                boardState[i][j] = count;
            }
        
        }
     
     for(i = 0; i < SIZE ; i ++){
            for(j = 0; j < SIZE ; j ++){
                if(boardState[i][j] == 100) printf("* ");
                else printf("%d ",boardState[i][j]);
                
            }    
            printf("\n");
        }

}

//checks if the player meets the Goal
int isGoal(int boardState[SIZE][SIZE], int boardActivated[SIZE][SIZE]){
    int i,j;
    
    for(i = 0; i < SIZE ; i ++ ) {
        for(j = 0; j < SIZE ; j ++ ) {
            if(boardState[i][j] == 0 && boardActivated[i][j] != 1) return -1;
        }  
       
    }
    printf("SUCCESS!");
    return 1;

}

main() {
    // 0 if no mine, 1 if mine
    int gameOver = 0, winGame = 0;
    int i, j;
    char x[1],y[1];
    int row,col;
   
    for(i = 0; i < SIZE ; i ++){
        for(j = 0; j < SIZE ; j ++){
            boardState[i][j] = 0;
            boardActivated[i][j] = 0;
        }    
    }
    
    initializeBoard();

   /**********************************/
    
    
    while(1){
        printf("Choose index of the tile [row]<space>[col]:");
        fgets(x,2,stdin);
        row = x[0] - '0';
        getchar();
        fgets(y,2,stdin); 
        col = y[0] - '0';
        getchar();
        
        boardActivated[row][col] = 1;
        printBoard();
            
        if(boardState[row][col] == 100){ 
            gameOver = 1;
            break;
        }
        
       // else checkNeighbors(x,y);
        
        if (isGoal(boardState, boardActivated)== 100){ 
            winGame = 1;       
            break;
        }
        
             
    }
    
    if(winGame){
        printf("You win the game!\n");
    }
    
    else if(gameOver){
        printf("BOOOM! \n");
    }
    
}
