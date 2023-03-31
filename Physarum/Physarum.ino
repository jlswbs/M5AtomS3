// Physarum growth //

#include "M5AtomS3.h"
M5Display Lcd;

#define WIDTH   130
#define HEIGHT  130
#define SCR     (WIDTH*HEIGHT)
#define ITER    10000
#define NUM     6

  uint16_t col[SCR];
  uint16_t grid[SCR];
  uint16_t coll[NUM];

void rndrule(){
  
  memset(col, 0, 2*SCR);
  
  for (int i = 0; i < NUM; i++) coll[i] = esp_random();

  for (int y = 0; y < HEIGHT; y++){  
    for (int x = 0; x < WIDTH; x++){
      
      if(x == 0 || x == 1 || x == WIDTH-2 || x == WIDTH-1 || y == 0 || y == 1 || y == HEIGHT-2 || y == HEIGHT-1) grid[x+y*WIDTH] = 1;
      else grid[x+y*WIDTH] = 0;

    }
  }
  
  for (int i = 1; i < NUM; i++){
    
    int x = 2 * (5 + esp_random()%((WIDTH/2)-10));
    int y = 2 * (5 + esp_random()%((HEIGHT/2)-10));
    if(grid[x+y*WIDTH] == 0) grid[x+y*WIDTH] = 1000+(i*100);

  }

}

void nextstep(){
  
  int x = 2 * (1 + esp_random()%((WIDTH/2)-2));
  int y = 2 * (1 + esp_random()%((HEIGHT/2)-2));
  int t, q;
    
  if(grid[x+y*WIDTH] >= 100 && grid[x+y*WIDTH] < 1000){
      
    q = grid[x+y*WIDTH]/100;
    int p = grid[x+y*WIDTH] - (q*100);
      
    if(p < 30){
        
      t = 1 + esp_random()%5;
      if(t == 1 && grid[(x+2)+y*WIDTH] == 0){ grid[(x+2)+y*WIDTH] = q*100; grid[(x+1)+y*WIDTH] = q*100; } 
      if(t == 2 && grid[x+(y+2)*WIDTH] == 0){ grid[x+(y+2)*WIDTH] = q*100; grid[x+(y+1)*WIDTH] = q*100; } 
      if(t == 3 && grid[(x-2)+y*WIDTH] == 0){ grid[(x-2)+y*WIDTH] = q*100; grid[(x-1)+y*WIDTH] = q*100; } 
      if(t == 4 && grid[x+(y-2)*WIDTH] == 0){ grid[x+(y-2)*WIDTH] = q*100; grid[x+(y-1)*WIDTH] = q*100; } 
      grid[x+y*WIDTH] = grid[x+y*WIDTH] + 1;
        
    } else {
        
      t = 0;
      if(grid[(x+1)+y*WIDTH] > 1) t = t + 1;
      if(grid[x+(y+1)*WIDTH] > 1) t = t + 1;
      if(grid[(x-1)+y*WIDTH] > 1) t = t + 1;
      if(grid[x+(y-1)*WIDTH] > 1) t = t + 1;
      if(t <= 1){
        grid[x+y*WIDTH] = 9100;
        grid[(x+1)+y*WIDTH] = 0;
        grid[x+(y+1)*WIDTH] = 0;
        grid[(x-1)+y*WIDTH] = 0;
        grid[x+(y-1)*WIDTH] = 0; 
      }
    }     
  }
 
  if(grid[x+y*WIDTH] >= 1000 && grid[x+y*WIDTH] < 2000){
      
    q = (grid[x+y*WIDTH]/100)-10;
    if(grid[(x+2)+y*WIDTH] == 0){ grid[(x+2)+y*WIDTH] = q*100; grid[(x+1)+y*WIDTH] = q*100; }
    if(grid[x+(y+2)*WIDTH] == 0){ grid[x+(y+2)*WIDTH] = q*100; grid[x+(y+1)*WIDTH] = q*100; }
    if(grid[(x-2)+y*WIDTH] == 0){ grid[(x-2)+y*WIDTH] = q*100; grid[(x-1)+y*WIDTH] = q*100; }
    if(grid[x+(y-2)*WIDTH] == 0){ grid[x+(y-2)*WIDTH] = q*100; grid[x+(y-1)*WIDTH] = q*100; }
    
  }

  if(grid[x+y*WIDTH] >= 9000){
      
    grid[x+y*WIDTH] = grid[x+y*WIDTH] - 1;
    if(grid[x+y*WIDTH] < 9000) grid[x+y*WIDTH] = 0;
    
  }

}

void setup(void){

  srand(time(NULL));

  M5.begin();
  
  rndrule();

}

void loop(void){

  if (M5.Btn.wasReleased()) rndrule();
  
  for(int k = 0; k < ITER; k++) nextstep();

  for(int i = 0; i < SCR; i++){

    if(grid[i] >= 100 && grid[i] < 1000) col[i] = coll[(grid[i])%NUM];
    else col[i] = TFT_BLACK;      

  }
  
  Lcd.drawBitmap(0, 0, WIDTH, HEIGHT, col);
  M5.update();

}