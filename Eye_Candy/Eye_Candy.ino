// Eye candy patterns //

#include "M5AtomS3.h"
M5Display Lcd;

#define WIDTH   130
#define HEIGHT  130
#define SCR     (WIDTH*HEIGHT)
#define NUMS    16

float randomf(float minf, float maxf) {return minf + (esp_random()%(1UL << 31))*(maxf - minf) / (1UL << 31);}

  uint16_t col[SCR];
  float p[SCR];
  float v[SCR];
  float a[SCR];
  uint16_t color[NUMS];

void rndrule(){
  
  memset(col, 0, 2*SCR);
  memset(p, 0, 4*SCR);
  memset(v, 0, 4*SCR);
  memset(a, 0, 4*SCR);

  for (int i = 0; i < NUMS; i++){

    v[(1+esp_random()%(WIDTH-2))+(1+esp_random()%(HEIGHT-2))*WIDTH] = randomf(0.0f, TWO_PI);
    color[i] = esp_random();    

  }
         
}

void setup(void){

  srand(time(NULL));

  M5.begin();
  
  rndrule();

}

void loop(void){

  if (M5.Btn.wasReleased()) rndrule();
  
  for (int y = 1; y < HEIGHT-1; y++) {
    for (int x = 1; x < WIDTH-1; x++) {
      a[x+y*WIDTH] = (v[(x-1)+y*WIDTH] + v[(x+1)+y*WIDTH] + v[x+(y-1)*WIDTH] + v[x+(y+1)*WIDTH]) * 0.25f - v[x+y*WIDTH];
    }
  }
  
  for (int i = 0; i < SCR; i++){
    v[i] += a[i];
    p[i] += v[i];
    uint8_t coll = NUMS * sinf(p[i]);
    col[i] = color[coll%NUMS];
  }

  Lcd.drawBitmap(0, 0, WIDTH, HEIGHT, col);
  M5.update();

}