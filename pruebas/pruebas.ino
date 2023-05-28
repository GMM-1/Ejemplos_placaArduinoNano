#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define LED_PIN 3
#define BUTTON_PIN_1 4
#define BUTTON_PIN_2 5
#define BUTTON_MESSAGE_PIN 6
#define POT_PIN A0
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define i2c_Address 0x3C

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button pins
const int buttonLeftPin = 4;
const int buttonRightPin = 5;
const int buttonRestartPin = 6;

// Paddle variables
const int paddleWidth = 30;
const int paddleHeight = 5;
int paddleX;
int paddleY;

// Ball variables
const int ballSize = 3;
int ballX;
int ballY;
int ballSpeedX;
int ballSpeedY;

// Brick variables
const int brickRows = 4;
const int brickColumns = 6;
const int brickWidth = 20;
const int brickHeight = 8;
bool bricks[brickRows][brickColumns];

// Game state
bool gameRunning = true;

// Frame rate control
unsigned long previousFrameTime = 0;
const int frameDelay = 10; // Milliseconds delay between frames

void setup() {
  // Initialize the display
  display.begin();
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);

  // Set button pins as INPUT
  pinMode(buttonLeftPin, INPUT_PULLUP);
  pinMode(buttonRightPin, INPUT_PULLUP);
  pinMode(buttonRestartPin, INPUT_PULLUP);

  // Initialize paddle position
  paddleX = display.width() / 2 - paddleWidth / 2;
  paddleY = display.height() - paddleHeight - 1;

  // Initialize ball position and speed
  ballX = display.width() / 2;
  ballY = display.height() / 2;
  ballSpeedX = -1;
  ballSpeedY = -1;

  // Initialize bricks
  for (int row = 0; row < brickRows; row++) {
    for (int col = 0; col < brickColumns; col++) {
      bricks[row][col] = true;
    }
  }
}

void loop() {
  if (gameRunning) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousFrameTime >= frameDelay) {
      previousFrameTime = currentMillis;

      // Read button inputs
      bool buttonLeft = digitalRead(buttonLeftPin) == LOW;
      bool buttonRight = digitalRead(buttonRightPin) == LOW;
      bool buttonRestart = digitalRead(buttonRestartPin) == LOW;

      // Move paddle
      if (buttonLeft && paddleX > 0) {
        paddleX--;
      } else if (buttonRight && paddleX + paddleWidth < display.width()) {
        paddleX++;
      }

      // Update ball position
      ballX += ballSpeedX;
      ballY += ballSpeedY;

      // Handle ball collisions
      if (ballX <= 0 || ballX >= display.width() - ballSize) {
        ballSpeedX *= -1; // Reverse X direction
      }
      if (ballY <= 0) {
        ballSpeedY *= -1; // Reverse Y direction
      }

      // Check if ball hits paddle
      if (ballY + ballSize >= paddleY && ballX + ballSize >= paddleX && ballX <= paddleX + paddleWidth) {
        ballSpeedY *= -1; // Reverse Y direction
      }

      // Check if ball hits bricks
      for (int row = 0; row < brickRows; row++) {
        for (int col = 0; col < brickColumns; col++) {
          if (bricks[row][col]) {
            int brickX = col * (brickWidth + 2);
            int brickY = row * (brickHeight + 2);

            if (ballX + ballSize >= brickX && ballX <= brickX + brickWidth &&
                ballY + ballSize >= brickY && ballY <= brickY + brickHeight) {
              bricks[row][col] = false;
              ballSpeedY *= -1; // Reverse Y direction
            }
          }
        }
      }

      // Clear display
      display.clearDisplay();

      // Draw paddle
      display.fillRect(paddleX, paddleY, paddleWidth, paddleHeight, SH110X_WHITE);

      // Draw ball
      display.fillRect(ballX, ballY, ballSize, ballSize, SH110X_WHITE);

      // Draw bricks
      for (int row = 0; row < brickRows; row++) {
        for (int col = 0; col < brickColumns; col++) {
          if (bricks[row][col]) {
            int brickX = col * (brickWidth + 2);
            int brickY = row * (brickHeight + 2);
            display.fillRect(brickX, brickY, brickWidth, brickHeight, SH110X_WHITE);
          }
        }
      }

      // Display updated screen
      display.display();

      // Check for game over
      if (ballY >= display.height()) {
        gameRunning = false;
      }
    }
  } else {
    // Game over state

    // Check if restart button is pressed
    if (digitalRead(buttonRestartPin) == LOW) {
      // Reset game state and restart
      resetGame();
      gameRunning = true;
    }
  }
}

void resetGame() {
  // Clear bricks
  for (int row = 0; row < brickRows; row++) {
    for (int col = 0; col < brickColumns; col++) {
      bricks[row][col] = false;
    }
  }

  // Reset paddle position
  paddleX = display.width() / 2 - paddleWidth / 2;
  paddleY = display.height() - paddleHeight - 1;

  // Reset ball position and speed
  ballX = display.width() / 2;
  ballY = display.height() / 2;
  ballSpeedX=-1;
  ballSpeedY=-1;
}