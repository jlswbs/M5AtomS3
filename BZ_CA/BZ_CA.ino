// Belousov-Zhabotinsky reaction cellular automata //

#include "M5AtomS3.h"
M5Display Lcd;

#define WIDTH   130
#define HEIGHT  130
#define SCR     (WIDTH*HEIGHT)

  uint16_t col[SCR];
  uint8_t cells[SCR];
  uint8_t nextcells[SCR];
  int dir[2][4] = {{0, 2, 0, -2},{-2, 0, 2, 0}};
  uint16_t image;

void rndrule(){
  
  memset(col, 0, 2*SCR);
  memset(cells, 0, 2*SCR);
  memset(nextcells, 0, 2*SCR);

  cells[(esp_random()%WIDTH)+(esp_random()%HEIGHT)*WIDTH] = 1;

}

void setup(void){

  srand(time(NULL));

  M5.begin();
  
  rndrule();

}

void loop(void){

  if (M5.Btn.wasReleased()) rndrule();
  
  for(int y = 0; y < HEIGHT; y=y+2){
    
    for(int x = 0; x < WIDTH; x=x+2){
      
      if(cells[x+y*WIDTH] == 0) image = TFT_BLACK;
      else if(cells[x+y*WIDTH] == 1) image = TFT_RED;
      else image = TFT_WHITE;
      col[x+y*WIDTH] = image;
      
    }
  }

  for(int y = 0; y < HEIGHT; y=y+2){
    
    for(int x = 0; x < WIDTH; x=x+2){
      
      if(cells[x+y*WIDTH] == 1) nextcells[x+y*WIDTH] = 2;
      else if(cells[x+y*WIDTH] == 2) nextcells[x+y*WIDTH] = 0;
      else {
        
        nextcells[x+y*WIDTH] = 0;
        
        for(int k = 0; k < 4; k++){
          int dx = x + dir[0][k];
          int dy = y + dir[1][k];
          if(0 <= dx && dx < WIDTH && 0 <= dy && dy < HEIGHT && cells[dx+dy*WIDTH] == 1) nextcells[x+y*WIDTH] = 1;
        }
      
      }
    }
  }

  memcpy(cells, nextcells, SCR);
  cells[(esp_random()%WIDTH)+(esp_random()%HEIGHT)*WIDTH] = 1;

  Lcd.drawBitmap(0, 0, WIDTH, HEIGHT, col);
  M5.update();

}