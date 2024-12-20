#include <Arduino_MKRIoTCarrier.h>
#include <list>
MKRIoTCarrier carrier;

//Called on booting up -----------------------------------
void setup() {
  Serial.begin(9600);

  delay(2000);

  carrier.noCase();
  carrier.begin();
}

//set colors
const uint16_t BLACK = 0;
const uint16_t BROWN = 0b0101100100000000;
const uint16_t GRAY_ = 0b1000010000010000;
const uint16_t RED__ = 0b1110000001000001;
const uint16_t YELOW = 0b1110011100000001;
const uint16_t ORNGE = 0b1111110000000001;
const uint16_t TEAL_ = 0b0000110100011110;
const uint16_t SKIN_ = 0b1111111110011100;
const uint16_t WHITE = 0xFFFF;

const uint16_t BACK_ = 0x1F;
const uint16_t BACKGROUND = 0x1F;

//Set math constants/functions ---------------------------
const double root2 = sqrt(2);
const double cos30 = sqrt(3) / 2;

int sign(double x) {
  return ((x > 0) - (x < 0)) / 2;
}

//Set game constants----------------------------------------

//player
const int playerWidth = 19;
const int playerHeight = 19;

//enemy
const int enemyHeight = 20;
const int enemyWidth = 9;
const int enemyRadius = 80; //The radius from the player that the AI tries to maintain.
const int fireRate = 100;
const int enemySpawnDistance = 80;

//projectile
const int projCollisionRadius = 4;
const int projDisplayRadius = 4;
const uint16_t projColor = 0b1111100000011111;

//Img data ------------------------------------------
/*
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
*/
/*
const uint16_t playerImg[] PROGMEM = {
  BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, RED__, RED__, RED__, RED__, RED__, RED__, RED__, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, RED__, RED__, RED__, RED__, RED__, RED__, RED__, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, RED__, RED__, BLACK, BLACK, BLACK, RED__, RED__, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, RED__, RED__, BLACK, BLACK, BLACK, RED__, RED__, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, RED__, RED__, BLACK, BLACK, BLACK, RED__, RED__, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, RED__, RED__, RED__, RED__, RED__, RED__, RED__, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, RED__, RED__, RED__, RED__, RED__, RED__, RED__, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, GRAY_, GRAY_, YELOW, YELOW, BLACK, BLACK, YELOW, YELOW, BLACK, BLACK, GRAY_, GRAY_, GRAY_, BACK_, BACK_, BACK_,
  BACK_, BACK_, GRAY_, BACK_, GRAY_, YELOW, YELOW, BLACK, BLACK, YELOW, YELOW, BLACK, BLACK, GRAY_, GRAY_, BACK_, GRAY_, BACK_, BACK_,
  BACK_, BACK_, GRAY_, BACK_, GRAY_, YELOW, YELOW, BLACK, BLACK, YELOW, YELOW, BLACK, BLACK, GRAY_, GRAY_, BACK_, GRAY_, BACK_, BACK_,
  BACK_, BACK_, GRAY_, BACK_, BACK_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, BACK_, BACK_, GRAY_, BACK_, BACK_,
  BACK_, BACK_, GRAY_, BACK_, BACK_, BACK_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, BACK_, BACK_, BACK_, GRAY_, BACK_, BACK_,
  BACK_, GRAY_, BACK_, GRAY_, BACK_, BACK_, BACK_, BACK_, ORNGE, YELOW, ORNGE, BACK_, BACK_, BACK_, BACK_, GRAY_, BACK_, GRAY_, BACK_,
  BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, ORNGE, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_,
};
*/
const uint16_t playerImg[] PROGMEM = {
  BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, RED__, RED__, RED__, RED__, RED__, RED__, RED__, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, RED__, RED__, RED__, RED__, RED__, RED__, RED__, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, RED__, RED__, BLACK, BLACK, BLACK, RED__, RED__, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, RED__, RED__, BLACK, BLACK, BLACK, RED__, RED__, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, RED__, RED__, BLACK, BLACK, BLACK, RED__, RED__, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, RED__, RED__, RED__, RED__, RED__, RED__, RED__, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, RED__, RED__, RED__, RED__, RED__, RED__, RED__, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, GRAY_, BACK_, BACK_, BACK_, BACK_,
}
const uint16_t playerAttackingImg[] PROGMEM = {
  BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND,
  BACKGROUND, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, BACKGROUND,
  BACKGROUND, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, BACKGROUND,
  BACKGROUND, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, BACKGROUND,
  BACKGROUND, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, BACKGROUND,
  BACKGROUND, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b1111111111100000, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, 0b0000011111111111, BACKGROUND,
  BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND, BACKGROUND
};
const uint16_t enemyImgLeft[] PROGMEM = {
  BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, BACK_, BACK_,
  BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BACK_,
  BACK_, BACK_, SKIN_, SKIN_, SKIN_, SKIN_, SKIN_, BACK_, BACK_,
  BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BACK_,
  BACK_, BACK_, BLACK, BLACK, SKIN_, BLACK, BLACK, BACK_, BACK_,
  BACK_, BACK_, SKIN_, SKIN_, SKIN_, SKIN_, SKIN_, BACK_, BACK_,
  BACK_, BLACK, BLACK, WHITE, BLACK, WHITE, BLACK, BLACK, BACK_,
  BACK_, BLACK, BLACK, WHITE, BLACK, WHITE, BLACK, BLACK, BACK_,
  BACK_, BLACK, BLACK, WHITE, BLACK, WHITE, BLACK, BLACK, BACK_,
  BACK_, BLACK, BLACK, BLACK, WHITE, BLACK, BLACK, BLACK, BACK_,
  BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BACK_,
  BACK_, SKIN_, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BACK_,
  BACK_, BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, GRAY_, BACK_,
  BACK_, BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, GRAY_, BACK_,
  BACK_, BACK_, BLACK, BLACK, BACK_, BLACK, BLACK, GRAY_, BACK_,
  BACK_, BACK_, BLACK, BLACK, BACK_, BLACK, BLACK, BACK_, BACK_,
  BACK_, BACK_, BLACK, BLACK, BACK_, BROWN, BROWN, BACK_, BACK_,
  BACK_, BACK_, BROWN, BROWN, BACK_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_,
};
const uint16_t enemyImgRight[] PROGMEM = {
  BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, BACK_, BACK_,
  BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BACK_,
  BACK_, BACK_, SKIN_, SKIN_, SKIN_, SKIN_, SKIN_, BACK_, BACK_,
  BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BACK_,
  BACK_, BACK_, BLACK, BLACK, SKIN_, BLACK, BLACK, BACK_, BACK_,
  BACK_, BACK_, SKIN_, SKIN_, SKIN_, SKIN_, SKIN_, BACK_, BACK_,
  BACK_, BLACK, BLACK, WHITE, BLACK, WHITE, BLACK, BLACK, BACK_,
  BACK_, BLACK, BLACK, WHITE, BLACK, WHITE, BLACK, BLACK, BACK_,
  BACK_, BLACK, BLACK, WHITE, BLACK, WHITE, BLACK, BLACK, BACK_,
  BACK_, BLACK, BLACK, BLACK, WHITE, BLACK, BLACK, BLACK, BACK_,
  BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BACK_,
  BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, GRAY_, BACK_,
  BACK_, SKIN_, BLACK, BLACK, BLACK, BLACK, BLACK, GRAY_, BACK_,
  BACK_, BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, GRAY_, BACK_,
  BACK_, BACK_, BLACK, BLACK, BACK_, BLACK, BLACK, BACK_, BACK_,
  BACK_, BACK_, BLACK, BLACK, BACK_, BLACK, BLACK, BACK_, BACK_,
  BACK_, BACK_, BROWN, BROWN, BACK_, BLACK, BLACK, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, BACK_, BROWN, BROWN, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_,
};
const uint16_t enemyImgStill[] PROGMEM = {
  BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_,
  BACK_, BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, BACK_, BACK_,
  BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BACK_,
  BACK_, BACK_, SKIN_, SKIN_, SKIN_, SKIN_, SKIN_, BACK_, BACK_,
  BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BACK_,
  BACK_, BACK_, BLACK, BLACK, SKIN_, BLACK, BLACK, BACK_, BACK_,
  BACK_, BACK_, SKIN_, SKIN_, SKIN_, SKIN_, SKIN_, BACK_, BACK_,
  BACK_, BLACK, BLACK, WHITE, BLACK, WHITE, BLACK, BLACK, BACK_,
  BACK_, BLACK, BLACK, WHITE, BLACK, WHITE, BLACK, BLACK, BACK_,
  BACK_, BLACK, BLACK, WHITE, BLACK, WHITE, BLACK, BLACK, BACK_,
  BACK_, BLACK, BLACK, BLACK, WHITE, BLACK, BLACK, BLACK, BACK_,
  BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BACK_,
  BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BACK_,
  BACK_, SKIN_, BLACK, BLACK, BLACK, BLACK, BLACK, GRAY_, BACK_,
  BACK_, BACK_, BLACK, BLACK, BLACK, BLACK, BLACK, GRAY_, BACK_,
  BACK_, BACK_, BLACK, BLACK, BACK_, BLACK, BLACK, GRAY_, BACK_,
  BACK_, BACK_, BLACK, BLACK, BACK_, BLACK, BLACK, BACK_, BACK_,
  BACK_, BACK_, BLACK, BLACK, BACK_, BLACK, BLACK, BACK_, BACK_,
  BACK_, BACK_, BROWN, BROWN, BACK_, BROWN, BROWN, BACK_, BACK_,
  BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_, BACK_,
};
const uint16_t projImg[] PROGMEM = {
  BACKGROUND, BACKGROUND,         BACKGROUND,         BACKGROUND,         BACKGROUND,         BACKGROUND,         BACKGROUND,         BACKGROUND,
  BACKGROUND, BACKGROUND,         BACKGROUND,         0b1111100000011111, 0b1111100000011111, BACKGROUND,         BACKGROUND,         BACKGROUND,
  BACKGROUND, BACKGROUND,         0b1111100000011111, 0b1111100000011111, 0b1111100000011111, 0b1111100000011111, BACKGROUND,         BACKGROUND,
  BACKGROUND, 0b1111100000011111, 0b1111100000011111, 0b1111100000011111, 0b1111100000011111, 0b1111100000011111, 0b1111100000011111, BACKGROUND,
  BACKGROUND, 0b1111100000011111, 0b1111100000011111, 0b1111100000011111, 0b1111100000011111, 0b1111100000011111, 0b1111100000011111, BACKGROUND,
  BACKGROUND, BACKGROUND,         0b1111100000011111, 0b1111100000011111, 0b1111100000011111, 0b1111100000011111, BACKGROUND,         BACKGROUND,
  BACKGROUND, BACKGROUND,         BACKGROUND,         0b1111100000011111, 0b1111100000011111, BACKGROUND,         BACKGROUND,         BACKGROUND,
  BACKGROUND, BACKGROUND,         BACKGROUND,         BACKGROUND        , BACKGROUND,         BACKGROUND,         BACKGROUND,         BACKGROUND,
};

//Sprite classes -------------------------------------------------------
class Sprite{
  public:
    double x;
    double y;
    int health;
    double width;
    double height;
    Sprite(double x, double y, double width, double height, int health) {
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

//Player Class
class Player: public Sprite {
  public:
    int countdown;

    Player(double x, double y, int health=5):Sprite(x,y,playerWidth,playerHeight,health){
      countdown = 0;
    }

    void reset(bool resetHealth = true) {
      this->x = 0;
      this->y = 0;
      if (resetHealth) {
        this->health = 5;
      }
      this->countdown = 0;
    }

    void draw() {
      if (isAttacking()) {
        carrier.display.drawRGBBitmap(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, playerAttackingImg, this->width, this->height);
      }
      else {
        carrier.display.drawRGBBitmap(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, playerImg, this->width, this->height);
      }
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
        double angleToOrigin = atan2(this->y, this->x);
        this->x -= 1.001 * cos(angleToOrigin);
        this->y -= 1.001 * sin(angleToOrigin);
        this->clearBorder();
      }
      if (carrier.Buttons.getTouch(TOUCH3) || carrier.Buttons.getTouch(TOUCH1) && countdown < 0) {
        countdown = 100;
      }

      countdown--;
    }

    bool isAttacking() {
      return countdown >= 50;
    }

    void takeDamage(int damage){
      this->health-=damage;
      carrier.leds.setPixelColor(4 - this->health, 0, 0, 0);
      carrier.leds.show();
    }
};

//Projectile Class
class Projectile: public Sprite {
  public:
    double angle;
    double cosA;
    double sinA;

    Projectile(double x, double y, double angle):Sprite(x, y, projDisplayRadius*2, projDisplayRadius*2,health) {
      this->cosA = cos(angle);
      this->sinA = sin(angle);
    };

    void move(double px = 1) {
      this->x += px * this->cosA;
      this->y += px * this->sinA;
    };

    void draw() {
      /*
      carrier.display.fillCircle(120 + this->x - cos(angle), 120 - this->y + sin(angle), projDisplayRadius, BACKGROUND);
      carrier.display.fillCircle(120 + this->x, 120 - this->y, projDisplayRadius, projColor);
      */
      carrier.display.drawRGBBitmap(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, projImg, this->width, this->height);
    };

    void clear() {
      /*
      carrier.display.fillCircle(120 + this->x, 120 - this->y, projDisplayRadius + 1, BACKGROUND);
      */
      carrier.display.fillRect(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, this->width, this->height, BACKGROUND);
    };
};


class Enemy: public Sprite{
  public:
    int fireOffset;
    bool moving;

    Enemy(double x, double y, int fireOffset):Sprite(x, y, enemyWidth, enemyHeight, health){
      this->fireOffset = fireOffset;
      this->moving = false;
    }

    void enemyAI(Player player, std::list<Projectile>& projs, int time) {
      double angle = atan2(player.y - this->y, player.x - this-> x);
      double dist = this->distanceTo(player);

      //movement code
      if (dist > enemyRadius + 5) {
        this->moveToPlayer(player, .6);
        moving = true;
      } else if (dist < enemyRadius) {
        this->moveToPlayer(player, -.4);
        moving = true;
      } else {
        moving = false;
      }

      if (sq(this->x) + sq(this->y) >= sq(115)) {
        this->move(angle, 1.001);
        this->clearBorder();
      }
      
      //shooting code
      if ((time + this->fireOffset) % fireRate == 0) {
        projs.emplace_back(Projectile(x, y, angle));
      }
    }
    
    void move(double angle, double px = 1) {
      this->x += px * cos(angle);
      this->y += px * sin(angle);
    }
    void moveXY(double x, double y, double px = 1) {
      double normalizer = px * sqrt(sq(x) + sq(y));
      this-> x += x * normalizer;
      this-> y += y * normalizer;
    }
    void moveToPlayer(Player player, double px = 1) {
      double dist = this->distanceTo(player);
      //double normalizer = px * sqrt(sq(x) + sq(y));
      this->x += (player.x - this->x) * px / dist;
      this->y += (player.y - this->y) * px / dist;
    }

    void clearBorder() {
      carrier.display.drawRect(120 + this->x - (this->width+2) / 2, 120 - this->y - (this->height+2) / 2, (this->width+2), (this->height+2), BACKGROUND);
    }

    void draw(int time) {
      if (this->moving) {
        if ((time + this->fireOffset) % 16 > 8) {
          carrier.display.drawRGBBitmap(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, enemyImgLeft, this->width, this->height);
        } else {
          carrier.display.drawRGBBitmap(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, enemyImgRight, this->width, this->height);
        }
      } else {
        carrier.display.drawRGBBitmap(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, enemyImgStill, this->width, this->height);
      }
    }

    void clear() {
      carrier.display.fillRect(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, this->width, this->height, BACKGROUND);
    }
};

//Game Variables -----------------------------------------
int time = 0;
bool respawn = false;
int level = 3; //CHANGE TO ZERO
Player player(0, 0);
std::list<Projectile> projs;
std::list<Enemy> enemies;

void createEnemies(int numEnemies) {
  double angleDifference = 2 * PI / numEnemies;
  for (int i = 0; i < numEnemies; i++) {
    double angle = angleDifference * i;
    enemies.emplace_back(Enemy(enemySpawnDistance * cos(angle), enemySpawnDistance * sin(angle), time));
  }
}

//called repeatedly
void loop() {
  carrier.Buttons.update();

  if (time == 0) {
    title();
  }
  if (level == 0) {
    if (time == 50) {
      carrier.leds.setPixelColor(4, 10, 0, 0);
      carrier.leds.show();
    } else if (time == 80) {
      carrier.leds.setPixelColor(3, 10, 0, 0);
      carrier.leds.show();
    } else if (time == 110) {
      carrier.leds.setPixelColor(2, 10, 0, 0);
      carrier.leds.show();
    } else if (time == 140) {
      carrier.leds.setPixelColor(1, 10, 0, 0);
      carrier.leds.show();
    } else if (time == 170) {
      carrier.leds.setPixelColor(0, 10, 0, 0);
      carrier.leds.show();
    }
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
  //canvas.setTextSize(2);
  //canvas.setCursor(50, BACKGROUND); //set the cursor at position x=50, y=BACKGROUND
  //canvas.print("Hello There!"); //display "Hello There!" on the screen
  carrier.display.fillScreen(BACKGROUND);
  carrier.display.setTextSize(2);
  carrier.display.setTextColor(0b1111111111111111);
  carrier.display.setCursor(45, 45);
  if (level == 0 && !respawn) {
    carrier.display.write("Roguebot has");
    carrier.display.setCursor(45, 65);
    carrier.display.write("escaped!");
  }
  carrier.display.setCursor(45, 105);
  carrier.display.write("Level ");
  //carrier.display.write(std::to_string(level + 1));
  if (level < 9) {
    carrier.display.write(level + 49); //print number.
  } else {
    carrier.display.write(level + 56); //print letter starting at capital A.
  }
  
}

void gameStart() {
  carrier.display.fillScreen(BACKGROUND);

  projs.clear();
  enemies.clear();
  player.reset(respawn);

  //enemies.emplace_back(Enemy(100, 100, time));
  createEnemies(level + 1);
}

void game() {
  
  player.draw();
  player.move();

  
  for (Projectile& proj : projs) {
    proj.draw();
    proj.move();

    if(proj.distanceTo(player) <= (player.width / 2) + projCollisionRadius) {
      proj.clear();
      player.takeDamage(1);
      if (player.health == 0) {
        level = 0;
        respawn = true;
        time = -1;
        return;
      }
    }
  }

  projs.remove_if([] (Projectile& proj) {
    return (proj.distanceTo(player) <= (player.height/2)+projCollisionRadius) || (sq(proj.x) + sq(proj.y) > sq(140));
  });

  for (Enemy& enemy : enemies) {
    enemy.enemyAI(player, projs, time);
    enemy.draw(time);

    if ((enemy.distanceTo(player) <= (player.height/2)+enemy.width) && player.isAttacking()) {
      enemy.clear();
    }
  }

  enemies.remove_if([] (Enemy& enemy) {
    return (enemy.distanceTo(player) <= (player.height/2)+enemy.width) && player.isAttacking();
  });

  if (enemies.size() == 0) {
    level++;
    respawn = false;
    time = -1;
    return;
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