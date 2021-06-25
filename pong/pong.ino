//Declarations for OLED Screen
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//Our own definitions for the game board
#define LED 10
#define B1 12
#define B2 11
#define PEIZO 8
#define JOY_X A7
#define JOY_Y A6
////////////////////////////////////////////////////////////////////////
int randX;
int randY;
int randX1;
int randY1;
int x = 1;
int y = 1;
int j = 1;

int lineL1 = 20;
int lineL2 = 40;

int lineR1 = 20;
int lineR2 = 40;
///////////////////////////////////////////////////////////////////////
int ship1 = 54;
int ship2 = 54;

int rockX[10];
int rockY[10];
int randRockX[10];
int randRockY[10];
int rockState = 1;






/////////////////////////////////////////////////////////////////////////

int buttonState1;
int buttonState2;
int lastState1, lastState2;

int score1 = 0;
int score2 = 0;

int pause = 0;

int pauseState = 0;

int gameSelection = 0;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //Start up and initialize the display

  randomSeed(analogRead(0));
  
  pinMode(LED,OUTPUT);
  pinMode(B1,INPUT);
  pinMode(B2,INPUT);
  pinMode(PEIZO,OUTPUT);
  pinMode(JOY_X,INPUT);
  pinMode(JOY_Y,INPUT);

  //Clear the display buffer
  display.clearDisplay();

  //Always need to call this to make anything show on the screen
  display.display();

  Serial.begin(9600);
}

void loop() {
  

  display.setTextSize(1);       // Set the text size to 2
  display.setTextColor(WHITE);  // Draw white text - only option
  display.setCursor(10, 10);      // Start at top-left corner

  display.clearDisplay();

  lastState1 = buttonState1;
  lastState2 = buttonState2;
  
  buttonState1 = digitalRead(B1);
  buttonState2 = digitalRead(B2);

  int joyXvalue = analogRead(JOY_X);
  int joyYvalue = analogRead(JOY_Y);


  
  if (buttonState1 == HIGH && buttonState2 == HIGH && lastState1 == LOW && lastState2 == LOW) //PAUSE BUTTON
  {
    pauseState = 0;
  }

  
  if (pauseState == 1) //////////////////////////////////////////////////////////////////////////////////////////////////////// pong ///////////////
  {
  
    if (joyXvalue < 460 && lineL1 > 0) //going up
    {
      lineL1-=2;
      lineL2-=2;
    }
    if (joyXvalue > 560 && lineL2 < 64) //going down
    {
      lineL1+=2;
      lineL2+=2;
    }
  
    if (buttonState2 == HIGH && buttonState1 == LOW && lineR1 > 0) //going up
    {
      lineR1-=2;
      lineR2-=2;
    }
    if (buttonState1 == HIGH && buttonState2 == LOW && lineR2 < 64)//going down
    {
      lineR1+=2;
      lineR2+=2;
    }
      
    if (x == 1)
    {
      randX = random(1, 128);
      randY = random(1, 64);
      x = 0;
    }
    if (y == 1)
    {
      randX1 = random(2, 4);
      randY1 = random(2, 4);
      y = 0;
    }
    
  
    if (randX > -5 && randX < 5) //hit left side
    {
      randX1 = random(2, 5);
      score2++;
      randX = 64;
      randY = 32;
      pause = 1;
    }
    if (randX > 123 && randX < 133) //hit right side
    {
      randX1 = random(-2, -5);
      score1++;
      randX = 64;
      randY = 32;
      pause = 1;
    }
    if (randY > -5 && randY < 5) //hit top
    {
      randY1 = random(2, 4);
    }
    if (randY > 59 && randY < 69) //hit bottom
    {
      randY1 = random(-2, -5);
    }
  
    
  
    randX += randX1;
    randY += randY1;
  
    if (randX == 10 && randY >= lineL1 && randY <= lineL2) //ball is touching left paddle
    {
      randX1 = random(2, 4);
      //Serial.println("left hit");
    }
    if (randX == 118 && randY >= lineR1 && randY <= lineR2) //ball is touching right paddle
    {
      randX1 = random(-2, -4);
      //Serial.println("right hit");
    }
    
  
    /*Serial.print(randX);
    Serial.print(" ");
    Serial.println(randY);*/
   
  
    display.print("       ");
    display.print(score1);
    display.print("  ");
    display.print(score2);
  
    display.drawCircle(randX, randY, 1, WHITE);
  
    display.drawLine(10, lineL1, 10, lineL2, WHITE);
    display.drawLine(118, lineR1, 118, lineR2, WHITE);
  
    display.drawRect(0, 0, 128, 64, WHITE);
  
    for (int i = 0; i < 64 ; i+=5)
    {
      display.drawLine(64, i, 64, i+2, WHITE);
    }
  
  
  
    
    if (pause == 1)
    {
      delay(500);
      pause = 0;
    }
  
  }//end pause ting
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (pauseState == 2) //////////////////////////////////////////////////////////////////////////////////////////// space race ////////////////////
  {
    display.clearDisplay();
    if (joyXvalue > 550 && ship1 < 58) //up
    {
      ship1++;
    }
    if (joyXvalue < 460) //down
    {
      ship1--;
    }
    if (buttonState1 == HIGH && buttonState2 == LOW && ship2 < 58)
    {
      ship2++;
    }
    if (buttonState2 == HIGH && buttonState1 == LOW)
    {
      ship2--;
    }
    
    for (int p = 0; p < 10; p++)
    {
      if (rockState == 1)
      {
        randRockX[p] = random(1,3);
        randRockY[p] = random(-1,1);
        rockY[p] = random(5, 59);
        rockX[p] = -15;
        rockState = 0;
      }
  
      rockY[p] += randRockY[p];
      rockX[p] += randRockX[p];
  
      if (rockX[p] >= 135 && rockX[p] <= 150)
      {
        rockState = 1;
      }
      else if (rockY[p] >= -15 && rockY[p] <= -5)
      {
        rockState = 1;
      }
      else if (rockY[p] >= 70 && rockY[p] <= 80)
      {
        rockState = 1;
      }
      
      display.drawCircle(rockX[p], rockY[p], 1, WHITE);
    }

    

    display.fillRect(30, ship1, 4, 6, WHITE);
    display.fillRect(94, ship2, 4, 6, WHITE);
    
  }
/////////////////////////////////////////////////////////////////////////////////////////////
  if (pauseState == 0)
  {
    display.clearDisplay();
    display.println("       MENU");
    display.println("Pong");
    display.println("Space Race");

    if (joyXvalue < 460 && gameSelection > 0) //joystick go up
    {
      gameSelection--;
      
    }
    if (joyXvalue > 560 && gameSelection < 1) //joystick go down
    {
      gameSelection++;
    }
    if (gameSelection == 0)
    {
      display.drawRect(0, 18, 128, 8, WHITE);
    }
    else if (gameSelection == 1)
    {
      display.drawRect(0, 26, 128, 8, WHITE);
    }
    lastState1 = buttonState1;
    buttonState1 = digitalRead(B1);
    buttonState2 = digitalRead(B2);

    if (buttonState1 == HIGH && buttonState2 == LOW){
      if (gameSelection == 0)
      {
        pauseState = 1;
      }
      else if (gameSelection == 1)
      {
        pauseState = 2;
      }
    }
    

    
    //Serial.println(gameSelection);
    //Serial.println(joyXvalue);
    
  }
  Serial.println(rockState);
  /*Serial.print(buttonState1);
  Serial.print(" ");
  Serial.println(lastState1);*/
  display.display();


}
