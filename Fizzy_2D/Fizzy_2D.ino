// Fizzy 2D cellular automata //

#include "M5AtomS3.h"
M5Display Lcd;

#define WIDTH   130
#define HEIGHT  130
#define SCR     (WIDTH*HEIGHT)
#define NUMS    16

float randomf(float minf, float maxf) {return minf + (esp_random()%(1UL << 31))*(maxf - minf) / (1UL << 31);}

  uint16_t col[SCR];
  float CellVal[SCR];
  uint8_t Calm = 233;
  int CellIndex = 0;
  uint16_t color[NUMS];

void rndrule(){
  
  memset(col, 0, 2*SCR);
  
  CellIndex = 0;
  Calm = 16 + esp_random()%233;

  for (int i = 0; i < SCR; i++) CellVal[i] = randomf(0.0f, 128.0f);
  for (int i = 0; i < NUMS; i++) color[i] = esp_random();    

}

void setup(void){

  srand(time(NULL));

  M5.begin();
  
  rndrule();

}

void loop(void){

  if (M5.Btn.wasReleased()) rndrule();
  
  for (int i = 0; i < WIDTH; i++) {
    
    for (int j = 0; j < HEIGHT; j++) {

      CellIndex = (CellIndex+1)%SCR;

      uint8_t coll = (uint8_t)round(CellVal[CellIndex])%NUMS;
      col[i+j*WIDTH] = color[coll];  

      int below      = (CellIndex+1)%SCR;
      int above      = (CellIndex+SCR-1)%SCR;
      int left       = (CellIndex+SCR-HEIGHT)%SCR;
      int right      = (CellIndex+HEIGHT)%SCR;
      int aboveright = ((CellIndex-1) + HEIGHT + SCR)%SCR;
      int aboveleft  = ((CellIndex-1) - HEIGHT + SCR)%SCR;
      int belowright = ((CellIndex+1) + HEIGHT + SCR)%SCR;
      int belowleft  = ((CellIndex+1) - HEIGHT + SCR)%SCR;

      float NeighbourMix = powf((CellVal[left]*CellVal[right]*CellVal[above]*CellVal[below]*CellVal[belowleft]*CellVal[belowright]*CellVal[aboveleft]*CellVal[aboveright]),0.125f);
      CellVal[CellIndex] = fmod((sqrtf(CellVal[CellIndex]*NeighbourMix))+0.5f, Calm);

    }

  }

  Lcd.drawBitmap(0, 0, WIDTH, HEIGHT, col);
  M5.update();

}