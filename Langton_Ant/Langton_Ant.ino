// Langton's ant cellular automata //

#include "M5AtomS3.h"
M5Display Lcd;

#define WIDTH   130
#define HEIGHT  130
#define SCR     (WIDTH*HEIGHT)
#define ITER    60

  uint16_t col[SCR];
  bool state[SCR];
  int antLoc[2];
  int antDirection;

void rndrule(){
  
  memset(col, 0, 2*SCR);
  memset(state, 0, SCR);  
  antDirection = 1 + esp_random() % 4;
  antLoc[0] = esp_random() % WIDTH;
  antLoc[1] = esp_random() % HEIGHT;
         
}

void turnLeft(){

  if (antDirection > 1) antDirection--;
  else antDirection = 4;

}

void turnRight(){

  if (antDirection < 4) antDirection++;
  else antDirection = 1;

}

void moveForward(){

  if (antDirection == 1) antLoc[0]--;
  if (antDirection == 2) antLoc[1]++;
  if (antDirection == 3) antLoc[0]++;
  if (antDirection == 4) antLoc[1]--;

  if (antLoc[0] < 0) antLoc[0] = WIDTH-1;
  if (antLoc[0] > WIDTH-1) antLoc[0] = 0;
  if (antLoc[1] < 0) antLoc[1] = HEIGHT-1;
  if (antLoc[1] > HEIGHT-1) antLoc[1] = 0;

}

void updateScene(){

  moveForward();

  if (state[antLoc[0]+antLoc[1]*WIDTH] == 0){
    state[antLoc[0]+antLoc[1]*WIDTH] = 1;
    turnRight();
  } else {
    state[antLoc[0]+antLoc[1]*WIDTH] = 0;
    turnLeft();
  }

}

void setup(void){

  srand(time(NULL));

  M5.begin();
  
  rndrule();

}

void loop(void){

  if (M5.Btn.wasReleased()) rndrule();
  
  for(int i = 0; i < ITER; i++) updateScene();

  for (int i = 0; i < SCR; i++) {
 
    if(state[i] == 1) col[i] = TFT_WHITE;
    else col[i] = TFT_BLACK;

  }

  Lcd.drawBitmap(0, 0, WIDTH, HEIGHT, col);
  M5.update();

}