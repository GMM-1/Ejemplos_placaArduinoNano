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

const int ledPin = LED_PIN;
const int buttonOnPin = BUTTON_PIN_1;
const int buttonOffPin = BUTTON_PIN_2;
const int buttonMessagePin = BUTTON_MESSAGE_PIN;
const int potPin = POT_PIN;

bool ledStatus = false;
bool showMessage = false;  // This is the new toggle switch

void setup() {
  display.begin();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonOnPin, INPUT_PULLUP);
  pinMode(buttonOffPin, INPUT_PULLUP);
  pinMode(buttonMessagePin, INPUT_PULLUP);
}

void loop() {
  int potValue = analogRead(potPin);

  if (digitalRead(buttonOnPin) == LOW) {
    ledStatus = true;
    digitalWrite(ledPin, HIGH);
    delay(100);
  }

  if (digitalRead(buttonOffPin) == LOW) {
    ledStatus = false;
    digitalWrite(ledPin, LOW);
    delay(100);
  }

  if (digitalRead(buttonMessagePin) == LOW) {
    showMessage = !showMessage;  // Toggle the message display
    delay(100);
  }

  updateDisplay(potValue);
  delay(200);
}

void updateDisplay(int potValue) {
  display.clearDisplay();
  display.setCursor(0, 0);

  display.print("Estado del LED: ");
  display.println(ledStatus ? "ON" : "OFF");

  display.print("Valor Pot: ");
  display.println(potValue);

  // Display the message if showMessage is true
  if (showMessage) {
    display.setTextSize(2);
    display.println("Hello World");
    display.setTextSize(1);
  }

  display.display();
}
