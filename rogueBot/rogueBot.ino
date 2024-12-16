#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;

//Called on booting up
void setup() {
  Serial.begin(9600);

  delay(2000);

  carrier.noCase();
  carrier.begin();
}

//Set math constants
const double root2 = sqrt(2);
const double cos30 = sqrt(3) / 2;

//Set game constants
const uint16_t BACKGROUND = 0x1F;

const int playerWidth = 10;
const int playerHeight = 10;

//Set Sprite Data
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

//Projectile Class
class Projectile {
  public:
    double x;
    double y;
    double angle;
    double collisionRadius;
    double displayRadius;
    double color;

    Projectile(double x, double y, double angle, double collisionRadius = 10, double displayRadius = 10, uint16_t color = 0b1111100000011111) {
      this->x = x;
      this->y = y;
      this->angle = angle * PI / 180;
      this->collisionRadius = collisionRadius;
      this->displayRadius = displayRadius;
      this->color = color;
    };

    void move(double px = 1) {
      x += px * cos(angle);
      y += px * sin(angle);
    };

    bool checkCollision() {
      return false;
    };

    bool onScreen() {
      return (sq(x) + sq(y) < sq(128));
    };

    void draw() {
      carrier.display.fillCircle(120 + x - cos(angle), 120 - y + sin(angle), displayRadius, BACKGROUND);
      carrier.display.fillCircle(120 + x, 120 - y, displayRadius, color);
    };

    void clear(double r) {
      carrier.display.fillCircle(120 + x, 120 - y, r + 1, BACKGROUND);
    };
};

//Player Class
class Player {
  public:
    double x;
    double y;
    double width;
    double height;

    Player(double x, double y, double width, double height) {
      this->x = x;
      this->y = y;
      this->width = width;
      this->height = height;
    }

    void draw() {
      carrier.display.drawRGBBitmap(120 + x - playerWidth / 2, 120 - y - playerHeight / 2, sprite, playerWidth, playerHeight);
    }

    void clear() {
      carrier.display.fillRect(120 + x - playerWidth / 2, 120 - y - playerHeight / 2, 10, 10, BACKGROUND);
    }

    void move() {
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
      if (sq(x) + sq(y) >= sq(120)) {
        this->clear();
        x *= -.95;
        y *= -.95;
      }
    }
};

//Set non-constant game variables
double playerX = 0;
double playerY = 0;
int time = 0;
Projectile proj1(0, 0, 35);
Player player(0, 0, 10, 10);

//called repeatedly
void loop() {

  carrier.Buttons.update();

  if (time == 0) {
    title();
  }
  if (time == 200) {
    gameStart();
  }
  if (time > 200) {
    game();
  }

  delay(10);
  time++;
}

void title() {
  carrier.display.fillScreen(BACKGROUND);
  carrier.display.setTextSize(2);
  carrier.display.setTextColor(0b1111111111111111);  //set the text color as black
  carrier.display.setCursor(120 - 5.5 * 12, 120); //Needs to be adjusted; not centered perfectly.
  carrier.display.write("Hello World");
}

void gameStart() {
  carrier.display.fillScreen(BACKGROUND);
}

void game() {
  player.draw();
  player.move();

  proj1.draw();
  proj1.move();
}

/*
void drawRectangle(double x0, double y0, double x1, double y1) {
  carrier.display.fillRect(120 + x0, 120 - y1, x1 - x0, y1 - y0, 0b1111100000000000);
}

bool collision(int x0, int y0, int x1, int y1, int w) {
  return (x0 - w < playerX) && (playerX < x1 + w) && (y0 - w < playerY) && (playerY < y1 + w);
}
*/
