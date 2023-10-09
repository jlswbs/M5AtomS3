// Star-Wars cellular automata //

#include "M5AtomS3.h"
M5Display Lcd;

#define WIDTH   130
#define HEIGHT  130
#define SCR     (WIDTH*HEIGHT)
#define ALIVE   3
#define DEATH_1 2
#define DEATH_2 1
#define DEAD    0

  uint16_t col[SCR];
  uint8_t current[SCR];
  uint8_t next[SCR];
  uint8_t alive_counts[SCR];
  uint8_t temp[SCR];

void rndrule(){
  
  memset(col, 0, 2*SCR);
  memset(next, 0, sizeof(next));
  memset(alive_counts, 0, sizeof(alive_counts));  
  memset(temp, 0, sizeof(temp));

  for (int i = 0; i < SCR; i++) current[i] = (esp_random()%100) < 20 ? ALIVE : DEAD;
         
}

void step(){

  for (int y = 0; y < HEIGHT; y++) {
  
    for (int x = 0; x < WIDTH; x++) {  
    
      int count = 0;
      int next_val;
    
      int mx = WIDTH-1;
      int my = HEIGHT-1;
    
      int self = current[x+y*WIDTH];
    
      for (int nx = x-1; nx <= x+1; nx++) {
  
        for (int ny = y-1; ny <= y+1; ny++) {
    
          if (nx == x && ny == y) continue;     
          if (current[(wrap(nx, mx))+(wrap(ny, my))*WIDTH] == ALIVE) count++;
      
        }   
      }  

    int neighbors = count;
    
    if (self == ALIVE) next_val = ((neighbors == 3) || (neighbors == 4) || (neighbors == 5)) ? ALIVE : DEATH_1;
  
    else if (self == DEAD) next_val = (neighbors == 2) ? ALIVE : DEAD;
  
    else next_val = self-1;
   
    next[x+y*WIDTH] = next_val;
  
    if (next_val == ALIVE) alive_counts[x+y*WIDTH] = min(alive_counts[x+y*WIDTH]+1, 100);
    else if (next_val == DEAD) alive_counts[x+y*WIDTH] = 0;
    
    }
  }
  
  memcpy(temp, current, sizeof(temp));
  memcpy(current, next, sizeof(current));
  memcpy(next, temp, sizeof(next));

}
  
int wrap(int v, int m){

    if (v < 0) return v + m;
    else if (v >= m) return v - m;
    else return v;

}

void draw_type(int min_alive, int max_alive, uint16_t color){

  uint16_t coll;
       
  for (int i = 0; i < SCR; i++) {
   
    int self = current[i];
    if (self == DEAD) continue;
    int alive = alive_counts[i];
    if (alive < min_alive || alive > max_alive) continue;
    if (self == ALIVE) coll = color;
    else if (self == DEATH_1) coll = color>>1;
    else if (self == DEATH_2) coll = TFT_BLACK;

    col[i] = coll;

  }

}

void setup(void){

  srand(time(NULL));

  M5.begin();
  
  rndrule();

}

void loop(void){

  if (M5.Btn.wasReleased()) rndrule();
  
  step();

  draw_type(50, 100, TFT_RED);
  draw_type(2, 49, TFT_BLUE);
  draw_type(0, 1, TFT_WHITE);

  Lcd.drawBitmap(0, 0, WIDTH, HEIGHT, col);
  M5.update();

}