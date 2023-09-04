// Worms cellular automata //

#include "M5AtomS3.h"
M5Display Lcd;

#define WIDTH   130
#define HEIGHT  130
#define SCR     (WIDTH*HEIGHT)
#define ITER    30
#define NUMANTS 4

  uint16_t col[SCR];
  uint16_t coll[NUMANTS];
  int x[NUMANTS];
  int y[NUMANTS];
  int antsdir[NUMANTS];

void rndrule(){
  
  memset(col, 0, 2*SCR);

  for(int i = 0; i < NUMANTS; i++){
  
    x[i] = esp_random() % WIDTH;
    y[i] = esp_random() % HEIGHT;
    antsdir[i] = esp_random() % 8;
    coll[i] = esp_random();
    
  }
         
}

void setup(void){

  srand(time(NULL));

  M5.begin();
  
  rndrule();

}

void loop(void){

  if (M5.Btn.wasReleased()) rndrule();
  
  for(int k = 0; k < ITER; k++){
  
    for(int i = 0; i < NUMANTS; i++){
    
      if (col[x[i]+WIDTH*y[i]] > TFT_BLACK){ antsdir[i] = antsdir[i] - 1; col[x[i]+WIDTH*y[i]] = TFT_BLACK; }
      else { antsdir[i] = antsdir[i] + 1; col[x[i]+WIDTH*y[i]] = coll[i]; }

      if (antsdir[i] > 7) antsdir[i] = 0;   
      if (antsdir[i] < 0) antsdir[i] = 7;
    
      switch(antsdir[i]){
        case 0: y[i]--; break;
        case 1: y[i]--; x[i]++; break;
        case 2: x[i]++; break;
        case 3: x[i]++; y[i]++; break;
        case 4: y[i]++; break;
        case 5: y[i]++; x[i]--; break;
        case 6: x[i]--; break;
        case 7: x[i]--; y[i]--; break;
      }
    
      if (x[i] > WIDTH-1) x[i] = 0;
      if (x[i] < 0) x[i] = WIDTH-1;
      if (y[i] > HEIGHT-1) y[i] = 0;
      if (y[i] < 0) y[i] = HEIGHT-1;
    
    }
    
  }

  Lcd.drawBitmap(0, 0, WIDTH, HEIGHT, col);
  M5.update();

}