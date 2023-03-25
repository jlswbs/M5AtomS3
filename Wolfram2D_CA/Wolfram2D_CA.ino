// Wolfram 2D cellular automata //

#include "M5AtomS3.h"
M5Display Lcd;

#define WIDTH   130
#define HEIGHT  130
#define SCR     (WIDTH*HEIGHT)

  uint16_t col[SCR];
  bool state[SCR];
  bool newstate[SCR];
  bool rules[10];

void rndrule(){
  
  memset(col, 0, 2*SCR);

  for(int i = 0; i < 10; i++) rules[i] = esp_random()%2;

  memset(newstate, 0, SCR);
  memset(state, 0, SCR);
  
  state[(WIDTH/2)+(HEIGHT/2)*WIDTH] = 1;
  state[(WIDTH/2)+((HEIGHT/2)-1)*WIDTH] = 1;
  state[((WIDTH/2)-1)+((HEIGHT/2)-1)*WIDTH] = 1;
  state[((WIDTH/2)-1)+(HEIGHT/2)*WIDTH] = 1;
         
}

uint8_t neighbors(int i) {

  uint16_t x = i % WIDTH;
  uint16_t y = i / WIDTH;
  uint8_t result = 0;

  if(y > 0 && state[x+(y-1)*WIDTH] == 1) result = result + 1;
  if(x > 0 && state[(x-1)+y*WIDTH] == 1) result = result + 1;
  if(x < WIDTH-1 && state[(x+1)+y*WIDTH] == 1) result = result + 1;
  if(y < HEIGHT-1 && state[x+(y+1)*WIDTH] == 1) result = result + 1;
  
  return result;
 
}

void setup(void){

  srand(time(NULL));

  M5.begin();
  
  rndrule();

}

void loop(void){

  if (M5.Btn.wasReleased()) rndrule();
  
  for(int i = 0; i < SCR; i++){
    
    uint8_t totalNeighbors = neighbors(i);
            
    if(state[i] == 0 && totalNeighbors == 0)      {newstate[i] = rules[0]; col[i] = TFT_WHITE;}
    else if(state[i] == 1 && totalNeighbors == 0) {newstate[i] = rules[1]; col[i] = TFT_RED;}
    else if(state[i] == 0 && totalNeighbors == 1) {newstate[i] = rules[2]; col[i] = TFT_GREEN;}
    else if(state[i] == 1 && totalNeighbors == 1) {newstate[i] = rules[3]; col[i] = TFT_BLUE;}
    else if(state[i] == 0 && totalNeighbors == 2) {newstate[i] = rules[4]; col[i] = TFT_YELLOW;}
    else if(state[i] == 1 && totalNeighbors == 2) {newstate[i] = rules[5]; col[i] = TFT_BLUE;}
    else if(state[i] == 0 && totalNeighbors == 3) {newstate[i] = rules[6]; col[i] = TFT_MAGENTA;}
    else if(state[i] == 1 && totalNeighbors == 3) {newstate[i] = rules[7]; col[i] = TFT_CYAN;}
    else if(state[i] == 0 && totalNeighbors == 4) {newstate[i] = rules[8]; col[i] = TFT_RED;}
    else if(state[i] == 1 && totalNeighbors == 4) {newstate[i] = rules[9]; col[i] = TFT_BLACK;}
      
  }
 
  memcpy(state, newstate, SCR);

  Lcd.drawBitmap(0, 0, WIDTH, HEIGHT, col);
  M5.update();

}