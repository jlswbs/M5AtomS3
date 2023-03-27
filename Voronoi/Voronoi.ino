// Voronoi distribution (cell noise) //

#include "M5AtomS3.h"
M5Display Lcd;

#define WIDTH   130
#define HEIGHT  130
#define SCR     (WIDTH*HEIGHT)
#define NUMS    64
#define PARTIC  8

float distance(int x1, int y1, int x2, int y2) { return sqrtf(powf(x2 - x1, 2.0f) + powf(y2 - y1, 2.0f)); }

  uint16_t col[SCR];
  float mindist;
  int x[PARTIC];
  int y[PARTIC];
  int dx[PARTIC];
  int dy[PARTIC];
  uint16_t color[NUMS];

void rndrule(){
  
  memset(col, 0, 2*SCR);

  for (int i=0; i<NUMS; i++) color[i] = esp_random();
  
  for (int i=0; i<PARTIC; i++) {
    
    x[i] = esp_random()%WIDTH;
    y[i] = esp_random()%HEIGHT;
    dx[i] = esp_random()%8;
    dy[i] = esp_random()%8;

  } 

}

void setup(void){

  srand(time(NULL));

  M5.begin();
  
  rndrule();

}

void loop(void){

  if (M5.Btn.wasReleased()) rndrule();
  
  for (int j=0; j<HEIGHT; j++) {
    
    for (int i=0; i<WIDTH; i++) {
      
      mindist = WIDTH;
      
      for (int p=0; p<PARTIC; p++) {
        if (distance(x[p], y[p], i, j) < mindist) mindist = distance(x[p], y[p], i, j);
        if (distance(x[p]+WIDTH, y[p], i, j) < mindist) mindist = distance(x[p]+WIDTH, y[p], i, j);
        if (distance(x[p]-WIDTH, y[p], i, j) < mindist) mindist = distance(x[p]-WIDTH, y[p], i, j);       
      }
      
      uint8_t coll = mindist / 2.0f;
      col[i+j*WIDTH] = color[coll%NUMS];

    }
  }
  
  for (int p=0; p<PARTIC; p++) {
    x[p] += dx[p];
    y[p] += dy[p];
    x[p] = x[p] % WIDTH;
    y[p] = y[p] % HEIGHT;
  }

  Lcd.drawBitmap(0, 0, WIDTH, HEIGHT, col);
  M5.update();

}