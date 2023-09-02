// Super Langton's ant cellular automata //

#include "M5AtomS3.h"
M5Display Lcd;

#define WIDTH   130
#define HEIGHT  130
#define SCR     (WIDTH*HEIGHT)
#define ITER    240
#define LENGHT  32

  uint16_t col[SCR];
  uint8_t state[SCR];
  uint16_t antX = WIDTH / 2;
  uint16_t antY = HEIGHT / 2;
  uint8_t direction;
  uint8_t stateCount;
  bool type[LENGHT];
  uint16_t stateCols[LENGHT];

void rndrule(){
  
  memset(col, 0, 2*SCR);
  memset(state, 0, 2*SCR);

  antX = WIDTH / 2;
  antY = HEIGHT / 2;
  stateCount = 2 + esp_random() % (LENGHT-2);
  direction = esp_random() % 4;
  for(int i = 0; i < stateCount; i++) type[i] = esp_random() % 2;
  for(int i = 0; i < stateCount; i++) stateCols[i] = esp_random();
         
}

void turn(int angle){
  
  if(angle == 0){
    if(direction == 0){
      direction = 3;
    } else {
      direction--;
    }
  } else {
    if(direction == 3){
      direction = 0;
    } else {
      direction++;
    }
  }
}

void move(){
  
  if(antY == 0 && direction == 0){
    antY = HEIGHT-1;
  } else {
    if(direction == 0 ){
      antY--;
    }
  }
  if(antX == WIDTH-1 && direction == 1){
    antX = 0;
  } else {
    if(direction == 1){
      antX++;
    }
  }
  if(antY == HEIGHT-1 && direction == 2){
   antY = 0; 
  } else {
    if(direction == 2){
      antY++;
    }
  }
  if(antX == 0 && direction == 3){
    antX = WIDTH-1;
  } else {
    if(direction == 3){
      antX--;
    }
  }
}

void setup(void){

  srand(time(NULL));

  M5.begin();
  
  rndrule();

}

void loop(void){

  if (M5.Btn.wasReleased()) rndrule();
  
  for(int i = 0; i < ITER; i++) {

    move();
    turn(type[(state[antX+antY*WIDTH]%stateCount)]);
    state[antX+antY*WIDTH]++;
    col[antX+antY*WIDTH] = stateCols[(state[antX+antY*WIDTH]%stateCount)];

  }

  Lcd.drawBitmap(0, 0, WIDTH, HEIGHT, col);
  M5.update();

}