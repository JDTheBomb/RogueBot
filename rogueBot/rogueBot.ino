#include <Arduino_MKRIoTCarrier.h>
#include <list>
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

//math functions
int sign(double x) {
  return ((x > 0) - (x < 0)) / 2;
}

//Set game constants
const uint16_t BACKGROUND = 0x1F;

const int playerWidth = 10;
const int playerHeight = 10;

const int attackRadius = 80;

const int fireRate = 100;

//Set Sprite Data
const uint16_t playerImg[] PROGMEM = {
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

const uint16_t enemyImg[] PROGMEM = {
  BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND,
  BACKGROUND, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, BACKGROUND,
  BACKGROUND, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, BACKGROUND,
  BACKGROUND, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, BACKGROUND,
  BACKGROUND, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, 0b1111100000000000, BACKGROUND,
  BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND
};

//Sprite Class
class Sprite{
  public:
    double x;
    double y;
    int health;
    double width;
    double height;
    Sprite(double x=0, double y=0, double width=10, double height=10, int health=100) {
      this->x=x;
      this->y=y;
      this->width=width;
      this->height=height;
      this->health=health;
    };
    double distanceTo(Sprite otherSprite){
      return sqrt(sq(x-otherSprite.x) + sq(y-otherSprite.y));
    };
};

//Projectile Class
class Projectile: public Sprite {
  public:
    double angle;
    double collisionRadius;
    double color;
    double displayRadius;

    Projectile(double x = 0, double y = 0, double angle = 0, double collisionRadius = 10, double displayRadius = 10, uint16_t color = 0b1111100000011111):Sprite(x, y) {
      this->angle = angle * PI / 180;
      this->collisionRadius = collisionRadius;
      this->displayRadius = displayRadius;
      this->color = color;
    };

    void move(double px = 1) {
      this->x += px * cos(angle);
      this->y += px * sin(angle);
    };

    bool checkCollision() {
      return false;
    };

    bool onScreen() {
      return (sq(this->x) + sq(this->y) < sq(128));
    };

    void draw() {
      carrier.display.fillCircle(120 + this->x - cos(angle), 120 - this->y + sin(angle), displayRadius, BACKGROUND);
      carrier.display.fillCircle(120 + this->x, 120 - this->y, displayRadius, color);
    };

    void clear(double r) {
      carrier.display.fillCircle(120 + this->x, 120 - this->y, r + 1, BACKGROUND);
    };
};

//Player Class
class Player: public Sprite {
  public:

    Player(double x=0, double y=0, int health=100, double width = 10, double height = 10):Sprite(x,y,width,height,health){
    }

    void draw() {
      carrier.display.drawRGBBitmap(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, playerImg, this->width, this->height);
    }

    void clear() {
      carrier.display.fillRect(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, this->width, this->height, BACKGROUND);
    }
    void clearBorder() {
      carrier.display.drawRect(120 + this->x - (this->width+2) / 2, 120 - this->y - (this->height+2) / 2, (this->width+2), (this->height+2), BACKGROUND);
    }

    void move() {
      if (carrier.Buttons.getTouch(TOUCH0)) {
        this->x += cos30;
        this->y += 0.5;
      }
      if (carrier.Buttons.getTouch(TOUCH2)) {
        this->y -= 1;
      }
      if (carrier.Buttons.getTouch(TOUCH4)) {
        this->x -= cos30;
        this->y += 0.5;
      }
      if (sq(this->x) + sq(this->y) >= sq(115)) {
        /*
        this->clear();
        this->x *= -.95;
        this->y *= -.95;
        */
        double angleToOrigin = atan2(this->y, this->x);
        this->x -= 1.001 * cos(angleToOrigin);
        this->y -= 1.001 * sin(angleToOrigin);
        this->clearBorder();
      }
    }
};

class Enemy: public Sprite{
  public:
    int fireOffset;

    Enemy(double x=0, double y=0, int fireOffset = 0, double width = 10, double height = 10, int health=100):Sprite(x, y, width, height, health){
      this->fireOffset = fireOffset;
    }

    void attack(Player player, std::list<Projectile>& projs, int time) {
      double angle = atan2(player.y - this->y, player.x - this-> x);
      if (this->distanceTo(player) > attackRadius + 5) {
        move(angle, .6);
      }
      if (this->distanceTo(player) < attackRadius) {
        move(angle, -.4);
      }
      if (sq(this->x) + sq(this->y) >= sq(115)) {
        double angleToOrigin = atan2(this->y, this->x);
        this->x -= 1.001 * cos(angleToOrigin);
        this->y -= 1.001 * sin(angleToOrigin);
        this->clearBorder();
      }

      if ((time - this->fireOffset) % fireRate == 0) {
        fire(angle, projs);
      }
    }

    void fire(double angle, std::list<Projectile>& projs) {
      projs.emplace_back(Projectile(x, y, angle * 180 / PI));
    }
    
    void move(double angle, double px = 1) {
      this->x += px * cos(angle);
      this->y += px * sin(angle);

    }

    void clearBorder() {
      carrier.display.drawRect(120 + this->x - (this->width+2) / 2, 120 - this->y - (this->height+2) / 2, (this->width+2), (this->height+2), BACKGROUND);
    }

    void draw() {
      carrier.display.drawRGBBitmap(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, enemyImg, this->width, this->height);
    }

    void clear() {
      carrier.display.fillRect(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, this->width, this->height, BACKGROUND);
    }
};

////Game Variables
int time = 0;
Player player(0, 0, 10, 10);
std::list<Projectile> projs;
std::list<Enemy> enemies;

//called repeatedly
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
  enemies.emplace_back(Enemy(5));
  projs.emplace_back(Projectile(20, 20, -180));
}

void game() {
  player.draw();
  player.move();

  for (Projectile& proj : projs) {
    proj.draw();
    proj.move();
  }
  for (Enemy& enemy : enemies) {
    enemy.draw();
    enemy.attack(player, projs, time);
  }
}

/*
void drawRectangle(double x0, double y0, double x1, double y1) {
  carrier.display.fillRect(120 + x0, 120 - y1, x1 - x0, y1 - y0, 0b1111100000000000);
}

bool collision(int x0, int y0, int x1, int y1, int w) {
  return (x0 - w < playerX) && (playerX < x1 + w) && (y0 - w < playerY) && (playerY < y1 + w);
}
*/
