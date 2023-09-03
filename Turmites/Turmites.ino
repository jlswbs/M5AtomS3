// Turmites cellular automata //

#include "M5AtomS3.h"
M5Display Lcd;

#define WIDTH   130
#define HEIGHT  130
#define SCR     (WIDTH*HEIGHT)
#define ITER    60

  uint16_t col[SCR];
  uint8_t world[SCR];
  int posx, posy;
  int oldposx, oldposy;
  int state;
  int dir;
  int last_filled;
  int current_col;
  int next_col[4][4];
  int next_state[4][4];
  int directions[4][4];

void rndrule(){
  
  memset(col, 0, 2*SCR);
  memset(world, 0, 2*SCR);  

  state = esp_random() % 4;
  dir = 0;
  posx = WIDTH / 2;
  posy = HEIGHT / 2;
  
  for(int j=0; j<4; j++){   
    for(int i=0; i<4; i++){         
      next_col[i][j] = esp_random() % 4;
      next_state[i][j] = esp_random() % 4;
      directions[i][j] = esp_random() % 8;
    }   
  }

  world[posx+posy*WIDTH] = esp_random() % 4;
         
}

void move_turmite(){
  
  int cols = world[posx+posy*WIDTH];
  
  oldposx = posx;
  oldposy = posy;
  current_col = next_col[cols][state];
  world[posx+posy*WIDTH] = next_col[cols][state];
  state = next_state[cols][state];    

  dir = (dir + directions[cols][state]) % 8;

  switch(dir){
    case 0: posy--; break;
    case 1: posy--; posx++; break;
    case 2: posx++; break;
    case 3: posx++; posy++; break;
    case 4: posy++; break;
    case 5: posy++; posx--; break;
    case 6: posx--; break;
    case 7: posx--; posy--; break;
  }

  if(posy < 0) posy = HEIGHT-1;
  if(posy >= HEIGHT) posy = 0;
  if(posx < 0) posx = WIDTH-1;
  if(posx >= WIDTH) posx=0;
  
}

void setup(void){

  srand(time(NULL));

  M5.begin();
  
  rndrule();

}

void loop(void){

  if (M5.Btn.wasReleased()) rndrule();
  
  for (int i = 0; i < ITER; i++) {

    move_turmite();

    switch(current_col){
      case 0: col[oldposx + oldposy * WIDTH] = TFT_RED; break;
      case 1: col[oldposx + oldposy * WIDTH] = TFT_GREEN; break;
      case 2: col[oldposx + oldposy * WIDTH] = TFT_BLUE; break;
      case 3: col[oldposx + oldposy * WIDTH] = TFT_WHITE; break;
    }
    
  }

  Lcd.drawBitmap(0, 0, WIDTH, HEIGHT, col);
  M5.update();

}