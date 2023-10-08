// Brian's brain cellular automata //

#include "M5AtomS3.h"
M5Display Lcd;

#define WIDTH       130
#define HEIGHT      130
#define SCR         (WIDTH*HEIGHT)
#define DENSITY     6
#define READY       0
#define REFRACTORY  1
#define FIRING      2

  uint16_t col[SCR];
  uint8_t world[SCR];
  uint8_t temp[SCR];

uint8_t weighted_randint(int true_weight){
  
    int choice = esp_random() % 10;
    if (choice > true_weight) return 1;
    else return 0;
  
}

void rndrule(){
  
  memset(col, 0, 2*SCR);

  for (int i = 0; i < SCR; i++){
    int r = weighted_randint(DENSITY);
    if (r == 1) world[i] = FIRING;
    else world[i] = READY;
  }
         
}

uint8_t count_neighbours(uint8_t world[SCR], int x_pos, int y_pos){
  
    int x, y, cx, cy, cell;
    int count = 0;

    for (y = -1; y < 2; y++) {
        for (x = -1; x < 2; x++) {
            cx = x_pos + x;
            cy = y_pos + y;
            if ( (0 <= cx && cx < WIDTH) && (0 <= cy && cy < HEIGHT)) {
                cell = world[(x_pos + x) + (y_pos + y) * WIDTH];
                if (cell == FIRING) count ++;
            }
        }
    }
  return count;
}


void apply_rules(uint8_t world[SCR]){

  memcpy(temp, world, 4*SCR);

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++){
      int cell = temp[x+y*WIDTH];          
      if (cell == READY) {
        int neighbours = count_neighbours(temp, x, y);
        if (neighbours == 2) world[x+y*WIDTH] = FIRING; }
      else if (cell == FIRING) world[x+y*WIDTH] = REFRACTORY;
      else world[x+y*WIDTH] = READY;
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
  
  apply_rules(world);

  for (int i = 0; i < SCR; i++) {
    if (world[i] == FIRING) col[i] = TFT_BLUE;    
    else if (world[i] == REFRACTORY) col[i] = TFT_WHITE;
    else col[i] = TFT_BLACK; 
  }

  Lcd.drawBitmap(0, 0, WIDTH, HEIGHT, col);
  M5.update();

}