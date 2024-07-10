#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Bounce2.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const unsigned char paddle [] PROGMEM = {
  B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
  B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
  B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111,
  B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111
};

const unsigned char brick [] PROGMEM = {
  B11111111, B11111111, B11111111, B11111111
};

const unsigned char ball [] PROGMEM = {
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c
};



int paddleX = 56;
int paddleY = 56;
int paddleLives = 3;
int ballX = 0;
int ballY = 0;
bool ballActive = false;
unsigned long lastCollisionTime = 0;

int score = 0;
bool bricksBroken[4][5] = {false}; 

int potPin = A0;
int potVal = 0;

int ledPins[] = {13, 12, 11}; 
int buzzerPin = 10;
bool buzzerActive = false;
int ldrPin = A10;
int ldrValue = 0;
int threshold = 500;
int buttonPin8 = 8;
int buttonPin7 = 7;
int buttonPin6 = 6;
int selected = 0;
int entered = -1;
float dususHizi = 1;
long long int sonzamansut =0;
long long int sonzamanskor = 0;
int choice = 0 ;

bool ballHitPaddle = false; 
const int brickColumnCount = 6; 

int firstBricks[2][brickColumnCount] = {
  {1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1}
};

int secondBricks[3][brickColumnCount] = {
  {1, 1, 1, 1, 1, 1},
  {0, 0, 1, 1, 0, 0},
  {1, 1, 1, 1, 1, 1}
};

int thirdBricks[4][brickColumnCount] = {
  {0, 0, 1, 1, 0, 0},
  {1, 1, 0, 0, 1, 1},
  {1, 1, 0, 0, 1, 1}, 
  {0, 0, 1, 1, 0, 0}
};


int (*currentBricks)[6] = firstBricks; 
int currentRowCount = 2; 



void setup() {
  for (int i = 0; i < paddleLives ; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], HIGH);
  }

  for (int i = 22; i < 43; i++) {
    pinMode(i, OUTPUT);
  }


  pinMode(buzzerPin, OUTPUT);

  pinMode(ldrPin, INPUT);

  pinMode(buttonPin8, INPUT_PULLUP);
  pinMode(buttonPin7, INPUT_PULLUP);
  pinMode(buttonPin6, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.display();
  delay(1000); 

}


void loop() {
  switch (choice) {
    case 1:
      if (checkAllBricksCleared()) {
        araEkran();
        nextLevel(); 
      } else {
        choice = oyun(false); 
      }
      break;
    case 2:
      if (checkAllBricksCleared()) {
        araEkran();
        nextLevel(); 
      }else {
        choice = oyun(true);
      }
      break;
    case 3:
      choice = menu();
      break;
    default:
      choice = menu();
      break;
  }
}


  // Seviye atlama kontrolü ve ayarlama 
bool checkAllBricksCleared() {
  for (int i = 0; i < currentRowCount; i++) {
    for (int j = 0; j < brickColumnCount; j++) {
      if (currentBricks[i][j] == 1) {
        return false; // Eğer hala tuğla varsa, false döndür
      }
    }
  }
  return true; // Tüm tuğlalar temizlenmişse, true döndür
}


void araEkran() {
  display.clearDisplay(); 
  display.setTextSize(2); 
  display.setTextColor(WHITE); 
  display.setCursor(5, 20);
  display.println("Next Level"); 
  display.display();
  delay(5000); 
}


int ballDirX = 1;
int ballDirY = 1;
int level = 1; 
const int ballSize = 2;

void nextLevel() {

  ballActive = false; 
  ballX = (SCREEN_WIDTH - ballSize) / 2; 
  ballY = (SCREEN_HEIGHT - ballSize) / 2; 

  level++;

  if (currentBricks == firstBricks) {
    currentBricks = secondBricks;
    currentRowCount = 3;
      araEkran(); 

  } else if (currentBricks == secondBricks) { 
    currentBricks = thirdBricks;
    currentRowCount = 4;
    araEkran(); 

     } else { 
    display.clearDisplay(); 
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.println("YOU WIN!");
    display.setTextSize(2); 
    display.setCursor(10, 30); 
    display.print("Score: "); 
    display.println(score);
    display.display();
    delay(10000); 
    resetGame(); 
    level = 1; 
    choice = 3; 
  }
}





int menu() {

    resetSegmentDisplay(22); 
    resetSegmentDisplay(29); 
    resetSegmentDisplay(36); 

  while (true) {
    int down = digitalRead(buttonPin8);
    int up = digitalRead(buttonPin7);
    int enter = digitalRead(buttonPin6);

    if (up == LOW && down == LOW) {
    }
    if (up == LOW) {
      selected = selected + 1;
      delay(200);
    }
    if (down == LOW) {
      selected = selected - 1;
      delay(200);
    }
    if (enter == LOW) {
      entered = selected;
    }

    const char *options[2] = {
      " BASLAT ",
      " CIKIS ",
    };

    if (entered == -1) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println(F("OYUN MENUSU"));
      display.println("");
      for (int i = 0; i < 2; i++) {
        if (i == selected) {
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
            display.println(options[i]);
        } else if (i != selected) {
            display.setTextColor(SSD1306_WHITE);
            display.println(options[i]);
        }
      }
    } else if (entered == 0) {
        entered = -1;
        return 1;
    } else if (entered == 1) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(10, 20);
        display.println(F("OYUNUMUZA GOSTERDIGINIZ"));
        display.println(F("ILGI ICIN TESEKURLER"));
        display.display();
        delay(2000);
        return 0;
    }
    display.display();
  }
}

void SayiYaz(int sayi, int pinSay) {
  switch (sayi)
  {
    case 0 :
      digitalWrite(pinSay + 4, LOW);
      digitalWrite(pinSay + 3, LOW);
      digitalWrite(pinSay + 2, LOW);
      digitalWrite(pinSay + 1, LOW);
      digitalWrite(pinSay, LOW);
      digitalWrite(pinSay + 5, LOW);
      digitalWrite(pinSay + 6, HIGH);
      break;
    case 1:
      digitalWrite(pinSay + 4, HIGH);
      digitalWrite(pinSay + 3, HIGH);
      digitalWrite(pinSay + 2, LOW);
      digitalWrite(pinSay + 1, LOW);
      digitalWrite(pinSay, HIGH);
      digitalWrite(pinSay + 5, HIGH);
      digitalWrite(pinSay + 6, HIGH);
      break;
    case 2:
      digitalWrite(pinSay + 4, LOW);
      digitalWrite(pinSay + 3, LOW);
      digitalWrite(pinSay + 2, HIGH);
      digitalWrite(pinSay + 1, LOW);
      digitalWrite(pinSay, LOW);
      digitalWrite(pinSay + 5, HIGH);
      digitalWrite(pinSay + 6, LOW);
      break;
    case 3:
      digitalWrite(pinSay + 4, HIGH);
      digitalWrite(pinSay + 3, LOW);
      digitalWrite(pinSay + 2, LOW);
      digitalWrite(pinSay + 1, LOW);
      digitalWrite(pinSay, LOW);
      digitalWrite(pinSay + 5, HIGH);
      digitalWrite(pinSay + 6, LOW);
      break;
    case 4:
      digitalWrite(pinSay + 4, HIGH);
      digitalWrite(pinSay + 3, HIGH);
      digitalWrite(pinSay + 2, LOW);
      digitalWrite(pinSay + 1, LOW);
      digitalWrite(pinSay, HIGH);
      digitalWrite(pinSay + 5, LOW);
      digitalWrite(pinSay + 6, LOW);
      break;
    case 5:
      digitalWrite(pinSay + 4, HIGH);
      digitalWrite(pinSay + 3, LOW);
      digitalWrite(pinSay + 2, LOW);
      digitalWrite(pinSay + 1, HIGH);
      digitalWrite(pinSay, LOW);
      digitalWrite(pinSay + 5, LOW);
      digitalWrite(pinSay + 6, LOW);
      break;
    case 6:
      digitalWrite(pinSay + 4, LOW);
      digitalWrite(pinSay + 3, LOW);
      digitalWrite(pinSay + 2, LOW);
      digitalWrite(pinSay + 1, HIGH);
      digitalWrite(pinSay, LOW);
      digitalWrite(pinSay + 5, LOW);
      digitalWrite(pinSay + 6, LOW);
      break;
    case 7:
      digitalWrite(pinSay + 4, HIGH);
      digitalWrite(pinSay + 3, HIGH);
      digitalWrite(pinSay + 2, LOW);
      digitalWrite(pinSay + 1, LOW);
      digitalWrite(pinSay, LOW);
      digitalWrite(pinSay + 5, HIGH);
      digitalWrite(pinSay + 6, HIGH);
      break;
    case 8:
      digitalWrite(pinSay + 4, LOW);
      digitalWrite(pinSay + 3, LOW);
      digitalWrite(pinSay + 2, LOW);
      digitalWrite(pinSay + 1, LOW);
      digitalWrite(pinSay, LOW);
      digitalWrite(pinSay + 5, LOW);
      digitalWrite(pinSay + 6, LOW);
      break;
    case 9:
      digitalWrite(pinSay + 4, HIGH);
      digitalWrite(pinSay + 3, LOW);
      digitalWrite(pinSay + 2, LOW);
      digitalWrite(pinSay + 1, LOW);
      digitalWrite(pinSay, LOW);
      digitalWrite(pinSay + 5, LOW);
      digitalWrite(pinSay + 6, LOW);
      break;
  }
}




const int paddleWidth = 32;
const int paddleHeight = 4;


void resetSegmentDisplay(int basePin) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(basePin + i, HIGH); 
  }
}

int originalFirstBricks[2][6] = {
  {1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1}
};

int originalSecondBricks[3][6] = {
  {1, 1, 0, 0, 1, 1},
  {0, 0, 1, 1, 0, 0},
  {1, 1, 0, 0, 1, 1}
};

int originalThirdBricks[4][6] = {
  {0, 0, 1, 1, 0, 0},
  {1, 1, 0, 0, 1, 1},
  {1, 1, 0, 0, 1, 1},
  {0, 0, 1, 1, 0, 0}
};


void resetGame() {

  currentBricks = firstBricks;
  currentRowCount = 2;
    
  memcpy(firstBricks, originalFirstBricks, sizeof(firstBricks));
  memcpy(secondBricks, originalSecondBricks, sizeof(secondBricks));
  memcpy(thirdBricks, originalThirdBricks, sizeof(thirdBricks));

  score = 0;
  paddleLives  = 3;
  dususHizi = 1;


  for (int i = 0; i < paddleLives ; i++) {
    digitalWrite(ledPins[i], HIGH); 
  }

  ballActive = false;
  ballHitPaddle = false;
 

  paddleX = 56;
  ballX = 0;
  ballY = 0;
}

const unsigned char newBrick[] PROGMEM = {
  B11111111, B11111111, B11111111,
  B11111111, B11111111, B11111111,
  B11111111, B11111111, B11111111,
  B11111111, B11111111, B11111111,
  B11111111, B11111111, B11111111,
  B11111111, B11111111, B11111111,
  B11111111, B11111111, B11111111,
  B11111111, B11111111, B11111111
};

const int newBrickWidth = 21; 
const int newBrickHeight = 8; 
const int brickSpacingX = 2; 
const int brickSpacingY = 2; 



int oyun(boolean umtfert) {
  
  int say = score;
  int basamakSay = 0;

  while (say > 0  ) {
    say = say / 10;
    basamakSay++;
  }

  if (basamakSay == 1 ) {
    SayiYaz(score, 22);
  }else if (basamakSay == 2) {
    int bas1 = score % 10;
    int bas2 = score / 10;
    SayiYaz(bas1, 22);
    SayiYaz(bas2, 29);
  }

  else if (basamakSay == 3) {
    int bas1 = score % 10;
    int bas2 = (score / 10) % 10;
    int bas3 = score / 100;
    SayiYaz(bas1, 22);
    SayiYaz(bas2, 29);
    SayiYaz(bas3, 36);
  }

  potVal = analogRead(potPin);
  paddleX = map(potVal, 0, 1023, 0, 112);
  ldrValue = analogRead(ldrPin);

  if (!ballActive) {
 ballX = paddleX + (paddleWidth - 8) / 2; 
    ballY = paddleY - 8; 

    if(level == 1){
    
      ballDirX =2; 
      ballDirY =2;
  
    }else if (level==2){
        ballDirX *=1.2; 
        ballDirY *=1.2;
    }else if(level==3){
       ballDirX *=1.2; 
        ballDirY *=1.2;
    }

    ballActive = true; 
  
  }

  

  if (ballActive) {
    ballX += ballDirX;
    ballY += ballDirY;

    if (ballX <= 0 || ballX >= SCREEN_WIDTH - ballSize-1) { 
      ballDirX = -ballDirX;
    }
    if (ballY <= 0) { 
      ballY = 0; 
      ballDirY = -ballDirY; 
    }
    if (ballY >= SCREEN_HEIGHT - ballSize) { 

      if (!ballHitPaddle) {

        paddleLives --;
        if (paddleLives < 3) {
          digitalWrite(ledPins[paddleLives ], LOW); 
        }
        if (paddleLives <= 0) {
          display.clearDisplay();
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.setCursor(10, 10);
          display.println("GAME OVER");
          display.setTextSize(2); 
          display.setCursor(10, 30);
          display.print("Score: ");
          display.println(score);
          display.display();
          delay(2000);
          score = 0;
          paddleLives  = 3;
          dususHizi = 1;
          level=1;
          for (int i = 0; i < paddleLives ; i++) {
            digitalWrite(ledPins[i], HIGH);
          }
        resetGame();

          return 0;
        }
        digitalWrite(ledPins[paddleLives], LOW);
        ballActive = false;
      }
    }
  }

  if (ballActive) {
    display.clearDisplay();
    display.drawBitmap(paddleX, paddleY, paddle, 32, 4, WHITE);
    display.drawBitmap(ballX, ballY, ball, 8, 8, WHITE);

    //paletin top ile kontrolü    
    if (ballY + 8 >= paddleY && ballY <= paddleY + paddleHeight && ballX + 8 >= paddleX && ballX <= paddleX + paddleWidth) { 
      ballY = paddleY - 8; 
      ballDirY = -ballDirY; 
      ballHitPaddle = true; 
    }

    // tuğla nın top ile kontrolü ve çizilmesi
 int brickOffsetX = (SCREEN_WIDTH / 2) - ((newBrickWidth + brickSpacingX) * brickColumnCount) / 2; 
 int brickOffsetY = 1; 

     for (int i = 0; i < currentRowCount; i++) {
      for (int j = 0; j < brickColumnCount; j++) {
        
        if (currentBricks[i][j] == 1) { 
         int brickX = brickOffsetX + j * (newBrickWidth + brickSpacingX); 
         int brickY = brickOffsetY + i * (newBrickHeight + brickSpacingY);
          display.drawBitmap(brickX, brickY, newBrick, newBrickWidth, newBrickHeight, WHITE); 

          if (ballX + ballSize >= brickX && ballX <= brickX + newBrickWidth &&
              ballY + ballSize >= brickY && ballY <= brickY + newBrickHeight) { 
            currentBricks[i][j] = 0; 
            ballDirY = -ballDirY; 
            score++; 
          }
        }
      }
    }

  if (checkAllBricksCleared()) {
    nextLevel(); 
  }

    if (ballHitPaddle) {
      ballHitPaddle = false;
    }

    // Skoru ekrana yazdır
/*    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Score: ");
    display.println(score);
    display.print("Lives: ");
    display.println(paddleLives);
    */display.display();
      
  }

  return 1;
}
