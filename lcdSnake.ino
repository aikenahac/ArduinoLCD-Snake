#include <SPI.h>
#include <TFT.h>

enum GameState{Logo, Game, GameOver} gameState;

#define cs   10
#define dc   8
#define rst  9

TFT tft = TFT(cs, dc, rst);

byte mapGrid[800];

byte x = 5;
byte y = 5;
byte startSnakeLenght = 4;
byte snakeLenght = startSnakeLenght;

byte appleX = 40;
byte appleY = 40;

enum SnakeDir{Up, Down, Right, Left} snakeDir = Right;

void setup() {
  gameState = 0;
  
  tft.begin();
  tft.background(10, 10, 250);
  Serial.begin(9600);

}

void loop() {
  
  if (gameState == Logo) {
      logo();
  }
  else if (gameState == Game) {
      game();
  }
  else if (gameState = GameOver) {
    gameOver();
  }
  
}

void gameOver() {
  tft.stroke(0, 255, 0);
  tft.fill(0, 255, 0);
  
  tft.setTextSize(2);
  tft.text("GAME OVER", 28, 40);
  
  char pointChars[4];
  String pointVal = String(snakeLenght);
  pointVal.toCharArray(pointChars, 4);
  
  tft.setTextSize(2);
  tft.text("Score:", 37, 60);
  tft.text(pointChars, 107, 60);
  
  tft.setTextSize(1);
  tft.text("Left to restart", 15, 110);
  
  while(true) {
    if (analogRead(A1) > 600) {
       resetGame();
       gameState = Game;
       break;
    } 
  }
  
}

void resetGame() {
   snakeLenght = startSnakeLenght;
   x = 5;
   y = 5;
   appleX = 40;
   appleY = 40;
   snakeDir = Right;
   resetMap();
}

void resetMap() {
  for (short i = 0; i < 800; i++) {
     mapGrid[i] = 0;
  } 
}

void logo() {
  tft.background(129, 150, 192);
  
  tft.stroke(0, 255, 0);
  tft.fill(0, 255, 0);
  
  tft.setTextSize(4);
  tft.text("Snake", 20, 10);
  
  tft.setTextSize(2);
  tft.text("for Arduino", 12, 45);
  
  tft.setTextSize(1);
  tft.text("Aiken Tine Ahac", 25, 75);
  
  tft.text("Vegova, 2020", 12, 105);
  
  delay(4000);
  tft.background(129, 150, 192);
  tft.setTextSize(2);
  tft.text("Drag left", 20, 45);
  tft.text("to start", 30, 65);
  
  
  while(true) {
    if (analogRead(A1) > 600) {
      gameState = Game;
      break;
    }
  }
  

}

  
void game() {
  tft.background(129, 150, 192);
  
  if (appleX == 40 && appleY == 40) {
     spawnApple(); 
  }
  
  if (x == appleX && y == appleY) {
    appleX = 40;
    appleY = 40;
    
    snakeLenght++;
  }
  Serial.println(snakeLenght);
  
  handleInput();
  movement();
  timerDecrease();
  updateMap();
  draw();
  
  delay(250);
}

void spawnApple() {
  appleX = (byte)random(1, 31);
  appleY = (byte)random(1, 24);
  Serial.print("RANDOM: x=");
  Serial.print(appleX);
  Serial.print(", y=");
  Serial.print(appleY);
  
}

void handleInput() {
  
  if (analogRead(A1) > 600) {
     snakeDir = Down; 
  }
  else if(analogRead(A1) < 400) {
     snakeDir = Up; 
  }
  else if(analogRead(A0) > 600) {
     snakeDir = Right; 
  }
  else if(analogRead(A0) < 400) {
     snakeDir = Left; 
  }
  
  Serial.print("A0:");
  Serial.print(analogRead(A0));
  Serial.print("\n");
  Serial.print("A1:");
  Serial.print(analogRead(A1));
  Serial.print("\n");
}

void updateMap() {
   byte xi = 0;
   byte yi = 0;
  
   for (short i = 0; i < 800; i++) {
     
      if (x == xi && mapGrid[i] > 0 && y == yi) {
         gameState = GameOver; 
      }
      
      if (xi == x && yi == y) {
         mapGrid[i] = snakeLenght; 
      }
      
      xi++;
      if (xi == 32) {
        xi = 0;
        yi++; 
      }
  } 
}

void timerDecrease() {
  for (short i = 0; i < 800; i++) {
    if (mapGrid[i] > 0) {
         mapGrid[i] -= 1; 
    }
  } 
}

void movement() {
  
  if (snakeDir == Up) {
    if (y != 0) {
      y-=1;
    }
    else {
      y = 24; 
    }
  } 
  
  if (snakeDir == Down) {
    if (y != 24) {
      y++;
    }
    else {
      y = 0;
   } 
  }
  
  if (snakeDir == Right) {
    if (x != 31) {
      x++;
    }
    else {
      x = 0;
   } 
  }
  
  
  if (snakeDir == Left) {
    if (x != 0) {
      x-=1;
    }
    else {
      x = 31;
    }
    
  } 
  
  Serial.print("Movement x:");
  Serial.print(x);
  Serial.print(", y:");
  Serial.print(y);
  Serial.print("\n");
}

void wallCollision() {
  if (x == 32) {
     x = 0; 
     Serial.println("A");
     return;
  }
  else if (x == 0) {
     x = 32; 
     Serial.println("B");
     return;
  }
  else if (y == 25) {
     y = 0; 
     Serial.println("C");
     return;
  }
  else if (y == 0) {
     y = 25; 
     Serial.println("D");
     return;
  }
}


void draw() {
  drawApple(appleX, appleY);
  
  byte x = 0;
  byte y = 0;
  
  for (short i = 0; i < 800; i++) {
    if (mapGrid[i] > 0) {
      drawSnakePoint(x, y);
    }
     
    x++;
    if (x == 32) {
      x = 0;
      y++; 
    }
  } 
}

void drawApple(byte x, byte y) {
  tft.stroke(200, 20, 20);
  tft.fill  (200, 20, 20);
  
  tft.rect((x * 5) + 0, (y * 5) + 2, 5, 2);
  
  tft.rect((x * 5) + 1, (y * 5) + 4, 3, 1);
  
  tft.rect((x * 5) + 1, (y * 5) + 1, 3, 1);
  
  tft.stroke(20, 200, 20);
  tft.rect((x * 5) + 1, (y * 5) + 0, 2, 1);
}

void drawSnakePoint(byte x, byte y) {
  const byte tileWidth = 5;

  tft.stroke(20, 100, 20);
  tft.fill  (20, 100, 20);
  tft.rect(x * 5, y * 5, 5, 5);
}
