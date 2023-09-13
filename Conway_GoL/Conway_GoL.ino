// Conway's Game of Life cellular automata //

#include "M5AtomS3.h"
M5Display Lcd;

#define WIDTH   130
#define HEIGHT  130
#define SCR     (WIDTH*HEIGHT)
#define crNum(x,y,z)  ((x)+(y))%z

  uint16_t col[SCR];
  bool newCells[SCR];
  bool cells[SCR];

void rndrule(){
  
  memset(col, 0, 2*SCR);
  memset(cells, 0, SCR);
  memset(newCells, 0, SCR);

  for(int k = 0; k < SCR; k++) cells[k] = esp_random()%2;
         
}

void next(){

  memcpy(newCells, cells, SCR);

  for(int y = 0; y < HEIGHT; y++){

    for(int x = 0; x < WIDTH; x++){
    
      int surrounding = 0;
     
      bool isAlive = false;

      cells[x+y*WIDTH] ? isAlive = true : isAlive = false;
      
      for(int j = -1; j < 2; j++) {
        for(int i = -1; i < 2; i++) surrounding += cells[crNum(x,i,WIDTH)+crNum(y,j,HEIGHT)*WIDTH];
      }

      surrounding -= cells[x+y*WIDTH];
      if((surrounding < 2 || surrounding > 3) && isAlive) newCells[x+y*WIDTH] = 0;
      if(surrounding == 3 && !isAlive) newCells[x+y*WIDTH] = 1;
      
    }

  }

  memcpy(cells, newCells, SCR);

}

void setup(void){

  srand(time(NULL));

  M5.begin();
  
  rndrule();

}

void loop(void){

  if (M5.Btn.wasReleased()) rndrule();
  
  for(int i = 0; i < SCR; i++) cells[i] ? col[i] = TFT_WHITE : col[i] = TFT_BLACK;

  next();

  Lcd.drawBitmap(0, 0, WIDTH, HEIGHT, col);
  M5.update();

}