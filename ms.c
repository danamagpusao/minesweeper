/*
    Minesweeper game
    No Highscores yet  
*/
#include "../../sdk/dexsdk.h"
#include "../../sdk/time.h"

#define START '1'
#define INSTRUCTIONS '2'
#define HIGHSCORE '3'
#define EXIT_GAME '0'


#define UP_KEY 'w'
#define DOWN_KEY 's'
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define ACTIVE_KEY 'k'
#define FLAG_KEY 'l'
#define EXIT_TO_MENU 'e'

#define BOX_SIZE 20

#define YELLOW 54
#define PALE_YELLOW 62
#define ROYAL_BLUE 1
#define DARK_BLUE 8
#define GRAY 56
#define WHITE 63


// Global Variables
//      State of the Board            Tiles Activated
    int boardState[10][10], boardActivated[10][10];
    int SIZE=0;
   // int ** boardState, ** boardActivated;
    int width, height;
    int tile_dimension;

void erase(int x, int y, int w, int h){ //basically covers an area with a black rectangle 
   int i,j;
   for (i=y;i<=(y+h);i++)
      for (j=x;j<=(x+w);j++)
         write_pixel(j,i,100);
}

void drawBox(int x, int y, int w, int h,int color){ //basically covers an area with a black rectangle 
   int i,j;
   for (i=y;i<=(y+h);i++)
      for (j=x;j<=(x+w);j++)
         write_pixel(j,i,color);
}

void drawTile(int x, int y, int color){ 
   int i,j;
   for (i=y;i<=(y+tile_dimension);i++)
      for (j=x;j<=(x+tile_dimension);j++)
         write_pixel(j,i,color);
}

void  drawHint(int i,int j){
     char str[20];
     sprintf(str,"%d",boardState[i][j]);
     if(boardState[i][j]!=100)  write_text(str,getX(j)+3,getY(i)+3,BLACK,0);
     else write_text("*",getX(j)+3,getY(i)+5,BLACK,0);
}

void drawBoard(){   
    int i, j;
    int x,y;
    tile_dimension = 125/SIZE;
    for(i = 0; i < SIZE; i++) {
        for(j = 0; j < SIZE; j++){
            drawTile(getX(j),getY(i),WHITE);            
        }   
    } 
}

int getX(int j){
     return 150 +(j*tile_dimension)+(j*5);    
}

int getY(int i){
    return 20 +(i*tile_dimension)+(i*5); 
}

int checkNearbyMines(int i, int j){
    if(boardState[i][j] == 0){
        drawTile(getX(j), getY(i), GREEN);
        drawHint(i,j);
        if(i>0){
          boardActivated[i-1][j] = 1;
          drawTile(getX(j), getY(i-1), GREEN);
          drawHint(i-1,j);      
        } 
        
        if(i<SIZE-1){
          boardActivated[i+1][j] = 1;
          drawTile(getX(j), getY(i+1), GREEN); 
          drawHint(i+1,j);              
        } 
            
        if(j>0){
          boardActivated[i][j-1] = 1;
          drawTile(getX(j-1), getY(i), GREEN); 
          drawHint(i,j-1);          
        }
        if( j<SIZE-1){
          boardActivated[i][j+1] = 1;
          drawTile(getX(j+1), getY(i), GREEN);
          drawHint(i,j+1);       
        } 
        
        if( j<SIZE-1 && i<SIZE-1){
          boardActivated[i+1][j+1] = 1;
          drawTile(getX(j+1), getY(i+1), GREEN);
          drawHint(i+1,j+1);        
        } 
         if(j<SIZE-1 && i>0){
          boardActivated[i-1][j+1] = 1;
          drawTile(getX(j+1), getY(i-1), GREEN);
          drawHint(i-1,j+1);      
        } 
        
        if(j>0 && i<SIZE-1){
          boardActivated[i+1][j-1] = 1;
          drawTile(getX(j-1), getY(i+1), GREEN);
          drawHint(i+1,j-1);         
        } 
        if(i>0 && j>0){
          boardActivated[i-1][j-1] = 1;
          drawTile(getX(j-1), getY(i-1), GREEN);
          drawHint(i-1,j-1);    
        } 
    }
}


//activates all the neighboring tiles that does not contain mine
int checkNeighbors(int i, int j){ 
    int ni=i,nj=j,pi=i,pj=j;
    while(ni>=0 || nj>=0 || pi<SIZE || pj<SIZE){
        ni--;
        nj--;
        pi++;
        pj++;
        
        if(boardState[ni][j] == 0 && ni >= 0){
            boardActivated[ni][j] = 1;
            checkNearbyMines(ni,j);
        }
        else ni = -1;
        if(boardState[pi][j] == 0 && pi<SIZE){
            boardActivated[pi][j] = 1;
            checkNearbyMines(pi,j);
        }
        else pi = SIZE;
        
        if(boardState[i][pj] == 0 && pj<SIZE){
            boardActivated[i][pj] = 1;
            checkNearbyMines(i,pj);
        }
        else pj = SIZE;
        
        if(boardState[i][nj] == 0 && nj>=0){
            boardActivated[i][nj] = 1;
            checkNearbyMines(i,nj);
        }
        else nj = -1;
       
    }   
  
}


//initializes the state of the board
// contains (SIZE^2)*0.3 mines
void initializeBoard(int row, int col){
     int i,j,mines,rx,ry,count;
     char str[10];
    
     srand(time(NULL));
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
     boardState[row][col] = 0;
      
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
        
        
     

}


int isGoal(){
    int i,j;    
    for(i = 0; i < SIZE ; i ++ ) {
        for(j = 0; j < SIZE ; j ++ ) {
            if(boardState[i][j] < 100 && boardActivated[i][j] != 1) return -1;
        }      
    }    
    return 1;

}

int main() {
    int gameOver = 0, winGame = 0;
    int i, j,first;
    int row=0,col=0,prow=0,pcol=0;
    char keypress = START;
    char str[100];

	set_graphics(VGA_320X200X256); 

	do{
        SIZE=0;
	    erase(1,1,400,220);

      	write_text("MINESWEEPER",120,40,YELLOW,1); //title

	    //menu options
	    write_text("1 - Start",93,100,WHITE,0); 
        write_text("2 - Instructions",93,120,WHITE,0);
        write_text("3 - High Scores",93,140,WHITE,0);
	    write_text("0 - Quit",93,160,WHITE,0);
	
	    keypress=(char)getch();
	    erase(1,1,400,220); //erase menu

	    if(keypress==START){
	    do{

		    if(keypress == START){
                do{
                    erase(1,1,400,220);

                    write_text("SELECT LEVEL:",30,40,WHITE,1); //title

                    //level options
                    write_text("1 - 6x6 Board",120,100,WHITE,0); 
                    write_text("2 - 8x8 Board",120,120,WHITE,0);
                    write_text("3 - 10x10 Board",120,140,WHITE,0);
                    if(keypress=(char)getch()){
                        if(keypress == '1') SIZE = 6;
                        else if(keypress == '2') SIZE = 8;
                        else if(keypress == '3') SIZE = 10;
                    }                  
                }while(SIZE == 0);
                erase(1,1,400,220); //erase menu
                gameOver = 0;
                winGame =0;
                row=0;
                col=0;
                prow=0;
                pcol=0;
			    erase(25,125,40,30);
                first=-1;
                for(i = 0; i < SIZE ; i ++){
                    for(j = 0; j < SIZE ; j ++){
                        boardState[i][j] = 0;
                        boardActivated[i][j] = 0;
                    }    
                }

                drawBoard();
                write_text("w - up",25,90,WHITE,0);
                write_text("s - down",25,100,WHITE,0);
                write_text("a - left",25,110,WHITE,0);
                write_text("d - right",25,120,WHITE,0);
                write_text("k - activate",25,130,WHITE,0);
                write_text("l - flag",25,140,WHITE,0);
                write_text("e - exit",25,150,WHITE,0);
               
		    }		

	        	do{
                    
	        		if (keypress=(char)getch()){
                        // to ensure the keyboard will not work if the player wins or loses
                        if(winGame != 0 || gameOver != 0) continue;
                        prow = row;
                        pcol = col;
	        		    if(keypress == UP_KEY){		       
	        		        if(row>0) row --;
	        		    }
	        		    else if(keypress == DOWN_KEY){
	        		        if(row<SIZE-1) row ++;
	        		        
	        		    }
	        		    else if(keypress == RIGHT_KEY){            
	        		        if(col<SIZE-1) col ++;
	        		       
	        		    }
	        		    else if(keypress == LEFT_KEY){
	        		        if(col>0) col --;			    
	        		    }
	        		    else if(keypress == ACTIVE_KEY){
                            boardActivated[row][col] = 1;
                            if(first == -1){ 
                                initializeBoard(row,col);
                                first = 1;
                            }
                            drawHint(row,col);
                            drawTile(getX(col),getY(row),GREEN);
                            checkNeighbors(row,col);
	        		    }
	        		    else if(keypress == FLAG_KEY){
                           //toggle flag 
                           if(boardActivated[row][col] != 1){
                               if(boardActivated[row][col] != 2){
                                    boardActivated[row][col] = 2;
                                    drawTile(getX(col),getY(row),BLUE);
                               
                               }
                               else{
                                   boardActivated[row][col] = 0;
                                   drawTile(getX(col),getY(row),WHITE);
                                   
                               }	 
                           } 
	        		    }
                        
                        if ( prow != row || pcol != col ) {    
                            
                            if(boardActivated[prow][pcol] == 1) {
                                drawTile(getX(pcol),getY(prow),GREEN);
                                drawHint(prow,pcol);
                            }
                            else if(boardActivated[prow][pcol] == 2)
                                drawTile(getX(pcol),getY(prow),BLUE);
                            else  
                                drawTile(getX(pcol),getY(prow),WHITE);                        
                        } 
                        
	    		        
                        drawTile(getX(col),getY(row),RED);
                      //  if( boardActivated[row][col] == 1){
                            drawHint(row,col);
                      //  }
                        
                        
                        if(boardState[row][col] == 100 && boardActivated[row][col] == 1){  
                            gameOver = 1;  
                            drawBox(90,70,150,50,GRAY);
                            write_text("Game Over!",124,90,BLACK,0);
                        }
                        
                        if (isGoal(boardState, boardActivated) == 1){ 
                            winGame = 1;  
                            drawBox(90,70,150,50,YELLOW);
                            write_text("You Win!",126,90,BLACK,0);
                                
                        }
                                                 
	        		}		   		
	        	} while (keypress != EXIT_TO_MENU);
            }while(keypress != EXIT_TO_MENU);	
        }
        
        else if(keypress == INSTRUCTIONS){
            do{
               if(keypress == INSTRUCTIONS){
                   write_text("Instructions",105,20,YELLOW,1);
                   //write instructions here
                   write_text("- Open(k) all the tiles of the ",10,50,WHITE,0);
                   write_text("  board which do not contain a",10,60,WHITE,0);
                   write_text("  bomb.",10,70,WHITE,0);
                   write_text("- The player will lose once a",10,80,WHITE,0);
                   write_text("  tile with a bomb is opened.",10,90,WHITE,0);
                   write_text("- A non-bomb tile contains number ",10,100,WHITE,0);
                   write_text("  of bombs in the 8 neighboring",10,110,WHITE,0);
                   write_text("  tile.",10,120,WHITE,0);
                   write_text("- Put a flag(l) on a tile that ",10,130,WHITE,0);
                   write_text("  contains a bomb",10,140,WHITE,0);
                   write_text("- Use w(up),s(down),a(left),",10,150,WHITE,0);
                   write_text("  d(right) keys for navigation.",10,160,WHITE,0);
                   write_text("  e - exit to menu ",10,180,WHITE,0);
                   write_text("  2 - next ",180,180,WHITE,0);
                   
                   do{
                    
	        		if (keypress=(char)getch()){
                        if(keypress == '2'){
                            erase(1,1,400,220);
                            write_text("Instructions",105,20,YELLOW,1);
                            write_text("Tile Colors: ",10,50,WHITE,0);
                            write_text("  - Activated Tile ",10,70,GREEN,0);
                            write_text("  - Flagged Tile",10,80,BLUE,0);
                            write_text("  - Selected Tile",10,90,RED,0);
                            write_text("  - Default Tile.",10,100,WHITE,0);  
                            write_text("  e - exit to menu ",10,180,WHITE,0);
                            write_text("  1 - prev ",180,180,WHITE,0);          
                      }
                      else if(keypress== '1'){
                           erase(1,1,400,220);
                            write_text("Instructions",105,20,YELLOW,1);
                            write_text("- Open(k) all the tiles of the ",10,50,WHITE,0);
                            write_text("  board which do not contain a",10,60,WHITE,0);
                            write_text("  bomb.",10,70,WHITE,0);
                            write_text("- The player will lose once a",10,80,WHITE,0);
                            write_text("  tile with a bomb is opened.",10,90,WHITE,0);
                            write_text("- A non-bomb tile contains number ",10,100,WHITE,0);
                            write_text("  of bombs in the 8 neighboring",10,110,WHITE,0);
                            write_text("  tile.",10,120,WHITE,0);
                            write_text("- Put a flag(l) on a tile that ",10,130,WHITE,0);
                            write_text("  contains a bomb",10,140,WHITE,0);
                            write_text("- Use w(up),s(down),a(left),",10,150,WHITE,0);
                            write_text("  d(right) keys for navigation.",10,160,WHITE,0);
                            write_text("  e - exit to menu ",10,180,WHITE,0);
                            write_text("  2 - next ",180,180,WHITE,0);           
                      }
                    }
                   }while(keypress != EXIT_TO_MENU);
                   
              }	     
            }while(keypress != EXIT_TO_MENU);        
        }
        
       else if(keypress == HIGHSCORE){
            do{
              if(keypress == HIGHSCORE){
                   write_text("High Scores!",120,40,WHITE,1);
                   //time needed
		        keypress=(char)getch();
              }	     
            }while(keypress != EXIT_TO_MENU);        
        }
    }while(keypress != EXIT_GAME);
    
    set_graphics(VGA_TEXT80X25X16);
	clrscr();
}
