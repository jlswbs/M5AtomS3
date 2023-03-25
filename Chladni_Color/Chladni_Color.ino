// Chladni patterns - color //

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
  float a, b, c, d;
  uint16_t color[NUMS];

void rndrule(){
  
  memset(col, 0, 2*SCR);

  a = randomf(0.005f, 0.05f);
  b = randomf(0.005f, 0.05f);
  c = randomf(0.005f, 0.05f);
  d = randomf(1.0f, 12.0f);
  
  for (int i = 0; i < NUMS; i++) color[i] = esp_random();

  memset(p, 0, 4*SCR);
  memset(v, 0, 4*SCR);
         
}

void setup(void){

  srand(time(NULL));

  M5.begin();
  
  rndrule();

}

void loop(void){

  if (M5.Btn.wasReleased()) rndrule();
  
  int frame = millis() / 32;

  v[(WIDTH/2)+(HEIGHT/2)*WIDTH] = 0;
  p[(WIDTH/2)+(HEIGHT/2)*WIDTH] = (sinf(frame * a) + sinf(frame * b) + sinf(frame * c)) * d;
  
  for (int y = 1; y < HEIGHT-1; y++) {
    for (int x = 1; x < WIDTH-1; x++) {
      v[x+y*WIDTH] += (p[(x-1)+y*WIDTH] + p[(x+1)+y*WIDTH] + p[x+(y-1)*WIDTH] + p[x+(y+1)*WIDTH]) * 0.25f - p[x+y*WIDTH];
    }
  }
  
  for(int y = 0; y < HEIGHT; y++){
    for(int x = 0; x < WIDTH; x++){
      p[x+y*WIDTH] += v[x+y*WIDTH];
      uint8_t coll = NUMS * fabs(constrain(p[x+y*WIDTH], -1.0f, 1.0f));
      col[x+y*WIDTH] = color[coll%NUMS];
    }
  }

  Lcd.drawBitmap(0, 0, WIDTH, HEIGHT, col);
  M5.update();

}