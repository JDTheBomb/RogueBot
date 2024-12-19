#include <Arduino_MKRIoTCarrier.h>
#include <list>
MKRIoTCarrier carrier;

//Called on booting up
void setup() {
  Serial.begin(9600);

  delay(2000);

  carrier.noCase();
  carrier.begin();

  carrier.leds.setPixelColor(0, 255, 0, 0);
  carrier.leds.setPixelColor(1, 255, 0, 0);
  carrier.leds.setPixelColor(2, 255, 0, 0);
  carrier.leds.setPixelColor(3, 255, 0, 0);
  carrier.leds.setPixelColor(4, 255, 0, 0);
  carrier.leds.show();
}

//Set math constants
const double root2 = sqrt(2);
const double cos30 = sqrt(3) / 2;

//Set game constants
const uint16_t BACKGROUND = 0x1F;

const int playerWidth = 10;
const int playerHeight = 10;

const int attackRadius = 40;

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

    Projectile(double x = 0, double y = 0, double angle = 0, double health=1, double collisionRadius = 10, double displayRadius = 10, uint16_t color = 0b1111100000011111):Sprite(x, y, collisionRadius*2,collisionRadius*2,health) {
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
      carrier.display.fillCircle(120 + this->x - cos(angle), 120 - this->y + sin(angle), this->displayRadius, BACKGROUND);
      carrier.display.fillCircle(120 + this->x, 120 - this->y, this->displayRadius, color);
    };

    void clear() {
      carrier.display.fillCircle(120 + this->x, 120 - this->y, this->displayRadius, BACKGROUND);
    };
};

//Player Class
class Player: public Sprite {
  public:

    Player(double x=0, double y=0, int health=5, double width = 10, double height = 10):Sprite(x,y,width,height,health){
    }

    void draw() {
      carrier.display.drawRGBBitmap(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, playerImg, this->width, this->height);
    }

    void clear() {
      carrier.display.fillRect(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, this->width, this->height, BACKGROUND);
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
      if (sq(this->x) + sq(this->y) >= sq(120)) {
        this->clear();
        this->x *= -.95;
        this->y *= -.95;
      }
    }
    void loosehealth(int health){
      if(1<=this->health){
        carrier.leds.setPixelColor(this->health-1, 0, 0, 0);
        carrier.leds.show();
        this->health-=health;
        
      }else{
        this->health=0;
      }
    }
};

//define player & projs; needed here because the enemy class directly references these objects.
Player player(0, 0, 5, 10);
std::list<Projectile> projs;

class Enemy: public Sprite{
  public:
    int animationState;
    int delay;
    Enemy(double x=0, double y=0, double width = 10, double height = 10, int delay = 0, int health=100):Sprite(x, y, width, height, health){
    }

    void attack() {
      delay++;
      double angleToPlayer = atan2(player.y - this->y, player.x - this-> x);
      if (this->distanceTo(player) < attackRadius) {
        if (delay % fireRate == 0) {
          fire(angleToPlayer);
        }
      } else {
        move(angleToPlayer);
      }
    }

    void fire(double angle) {
      projs.emplace_back(Projectile(x, y, angle * 180 / PI));
    }
    
    void move(double angle, double px = 1) {
      this->x += px * cos(angle);
      this->y += px * sin(angle);
    }

    void draw() {
      //Serial.print(120 + this->x - this->width / 2);
      //Serial.print(" ");
      //Serial.print(120 - this->y - this->height / 2);
      //Serial.print(" ");
      //Serial.print(this->width);
      //Serial.print(" ");
      //Serial.print(this->height);
      //Serial.println();

      carrier.display.drawRGBBitmap(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, enemyImg, this->width, this->height);
    }

    void clear() {
      carrier.display.fillRect(120 + this->x - this->width / 2, 120 - this->y - this->height / 2, this->width, this->height, BACKGROUND);
    }
};

//Game Constants

//Enemy enemy2(4,5);

int time = 0;
std::list<Enemy> enemies;

//called repeatedly
void loop() {
  //Serial.println("test");
  //Serial.println(enemy1.x);
  //Serial.println(enemy1.y);
  //Serial.println(enemy1.health);
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
  enemies.emplace_back(Enemy());
  projs.emplace_back(Projectile(20, 20, -180));
}

void game() {
  
  player.draw();
  player.move();

  /*
  for (Projectile& proj : projs) {
    Serial.println(player.health);
    //Serial.println(proj.distanceTo(player));
    //Serial.println((player.height/2)+proj.collisionRadius);
    if(proj.distanceTo(player)<=(player.height/2)+proj.collisionRadius){
      player.loosehealth(proj.health);
    }else{
      proj.draw();
      proj.move();
    };
    
  }*/
  for (std::list<Projectile>::iterator it = projs.begin(); it != projs.end(); ++it) {
    // Access the current projectile using (*it) or (it->)
    Projectile& proj = *it; // Get a reference for easier use
    //Serial.println(player.health);
    //Serial.println(proj.distanceTo(player));
    //Serial.println((player.height/2)+proj.collisionRadius);
    if (sq(proj.x) + sq(proj.y) <= sq(140)) {
      if(proj.distanceTo(player)<=(player.height/2)+proj.collisionRadius){
        player.loosehealth(proj.health);
        proj.clear();
        projs.erase(it);
      }else{
        proj.draw();
        proj.move();
      };
    }else{
      projs.erase(it);
    }
  }

  
  for (Enemy& enemy : enemies) {
    enemy.draw();
    enemy.attack();
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
