#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;

const double root2 = sqrt(2);
const double cos30 = sqrt(3) / 2;

const uint16_t projectileColor = 0b1111100000011111;

const uint16_t BACKGROUND = 0x1F;

const uint16_t sprite[] PROGMEM = {
  BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND,
  BACKGROUND, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND
};

double x = 0;
double y = 0;
int width = 10;
int height = 10;
int time = 0;

int z = 0;

void setup() {

  Serial.begin(9600);  //AUTOMATICALLY GENERATED to handle the serial monitor

  delay(2000);

  carrier.noCase();
  carrier.begin();

  carrier.display.fillScreen(BACKGROUND);
}

void loop() {

  //canvas.setTextSize(2);
  //canvas.setTextColor(0x0000);  //set the text color as black
  //canvas.setCursor(50, BACKGROUND); //set the cursor at position x=50, y=BACKGROUND
  //canvas.print("Hello There!"); //display "Hello There!" on the screen

  carrier.Buttons.update();

  if (time == 0) {
    title();
  }
  if (time == 200) {
    carrier.display.fillScreen(BACKGROUND);
  }
  if (time > 200) {
    game();
  }

  delay(10);
  time++;
}

void game() {
  controller();

  
  if (sq(x) + sq(y) >= sq(120)) {
    clearPlayer(x, y);
    x *= -.95;
    y *= -.95;
  }

  
  drawProjectile(0, z, 20);

  drawPlayer(x, y);
}

void controller() {
  if (carrier.Buttons.getTouch(TOUCH0)) {
    x += cos30;
    y += 0.5;
  }
  if (carrier.Buttons.getTouch(TOUCH2)) {
    y -= 1;
  }
  if (carrier.Buttons.getTouch(TOUCH4)) {
    x -= cos30;
    y += 0.5;
  }
  if ((carrier.Buttons.getTouch(TOUCH1) && carrier.Buttons.getTouch(TOUCH3))) {
    clearPlayer(x, y);
    x = 0;
    y = 0;
  }
}

void title() {
  carrier.display.setTextSize(2);
  carrier.display.setTextColor(0b1111111111111111);  //set the text color as black
  carrier.display.setCursor(120 - 5.5 * 12, 120); //Needs to be adjusted; not centered perfectly.
  carrier.display.write("Hello World");
}

void drawPlayer(double x, double y) {
  carrier.display.drawRGBBitmap(120 + x - width / 2, 120 - y - height / 2, sprite, width, height);
}

void clearPlayer(double x, double y) {
  carrier.display.fillRect(120 + x - width / 2, 120 - y - height / 2, 10, 10, BACKGROUND);
}


void drawRectangle(double x0, double y0, double x1, double y1) {
  carrier.display.fillRect(120 + x0, 120 - y1, x1 - x0, y1 - y0, 0b1111100000000000);
}

bool collision(double playerX, double playerY, int x0, int y0, int x1, int y1, int w) {
  return (x0 - w < playerX) && (playerX < x1 + w) && (y0 - w < playerY) && (playerY < y1 + w);
}

void drawProjectile(double x, double y, double r) {
  carrier.display.fillCircle(120 + x, 120 - y, r, projectileColor);
  carrier.display.drawCircle(120 + x, 120 - y, r + 1, BACKGROUND);
}

void clearProjectile(double x, double y, double r) {
  carrier.display.fillCircle(120 + x, 120 - y, r + 1, BACKGROUND);
}
