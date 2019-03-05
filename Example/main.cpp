/*

    Example to use the new functions

*/


#include <TFT_HX8357_Due.h> // Hardware-specific library

TFT_HX8357_Due_Button buttons[20];
TFT_HX8357_Due tft = TFT_HX8357_Due();       // Invoke custom library

#define TS_MINX 150
#define TS_MINY 140
#define TS_MAXX 820
#define TS_MAXY 920

#define MINPRESSURE 10
#define MAXPRESSURE 1000

int flag = 0;

//  ******************** Screen ********************
  
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BLACK);


// create buttons
  buttons[0].initButton(&tft, 60, 285, 90, 40, ILI9341_WHITE, ILI9341_BLUE, ILI9341_WHITE, "button", 2);
  buttons[0].drawButton();
  
  int main()
  {
    while (flag == 0)
    {
      digitalWrite(13, HIGH);
      TSPoint p;
      p.x = readTouchX();
      p.y = readTouchY();
      p.z = pressure();
      digitalWrite(13, LOW);

      if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {

        int aux = p.y;
        p.y = map(p.x, TS_MINX, TS_MAXX, tft.height(), 0);
        p.x = map(aux, TS_MAXY, TS_MINY, tft.width(), 0);

      }
      p.x = tft.width() - p.x;
      // go thru all the buttons, checking if they were pressed

      if (buttons[0].contains(p.x, p.y))
      {
        buttons[0].press(true);  // tell the button it is pressed
      }
      else
      {
        buttons[0].press(false);  // tell the button it is NOT pressed
      }


      // now we can ask the buttons if their state has changed
      if (buttons[0].justPressed())
      {
        buttons[0].drawButton(true);  // draw invert!
        delay(200);
        flag = 1;
      }

    }
  }


//  ******************** Position and pressure in the screen ********************


int readTouchX(void) {

  int samples[2];
  int i;

  pinMode(_yp, INPUT);
  pinMode(_ym, INPUT);
  digitalWrite(_yp, LOW);
  digitalWrite(_ym, LOW);

  pinMode(_xp, OUTPUT);
  digitalWrite(_xp, HIGH);
  pinMode(_xm, OUTPUT);
  digitalWrite(_xm, LOW);

  for (i = 0; i < 2; i++) {
    samples[i] = analogRead(_yp);
  }

  pinMode(_yp, OUTPUT);
  pinMode(_ym, OUTPUT);

  return (1023 - samples[2 / 2]);
}

int readTouchY(void) {

  int samples[2];
  int i;

  pinMode(_xp, INPUT);
  pinMode(_xm, INPUT);
  digitalWrite(_xp, LOW);
  digitalWrite(_xm, LOW);

  pinMode(_yp, OUTPUT);
  digitalWrite(_yp, HIGH);
  pinMode(_ym, OUTPUT);
  digitalWrite(_ym, LOW);

  for (i = 0; i < 2; i++) {
    samples[i] = analogRead(_xm);
  }

  pinMode(_xp, OUTPUT);
  pinMode(_xm, OUTPUT);

  return (1023 - samples[2 / 2]);
}

uint16_t pressure(void) {
  // Set X+ to ground
  pinMode(_xp, OUTPUT);
  digitalWrite(_xp, LOW);

  // Set Y- to VCC
  pinMode(_ym, OUTPUT);
  digitalWrite(_ym, HIGH);

  // Hi-Z X- and Y+
  digitalWrite(_xm, LOW);
  pinMode(_xm, INPUT);
  digitalWrite(_yp, LOW);
  pinMode(_yp, INPUT);

  int z1 = analogRead(_xm);
  int z2 = analogRead(_yp);

  pinMode(_xm, OUTPUT);
  pinMode(_yp, OUTPUT);

  if (rxplate != 0) {
    // now read the x
    float rtouch;
    rtouch = z2;
    rtouch /= z1;
    rtouch -= 1;
    rtouch *= readTouchX();
    rtouch *= rxplate;
    rtouch /= 1024;

    return rtouch;
  } else {
    return (1023 - (z2 - z1));
  }
}
