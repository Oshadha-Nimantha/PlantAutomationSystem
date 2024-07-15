#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Keypad.h>
#include <Servo.h>

#define DHTPIN A0
#define DHTTYPE DHT11

#define SOIL_MOISTURE_PIN A1
#define SERVO_PIN 9
#define RELAY_PIN 2
int moistVal = 0;
int soilMoisture = 0;

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

// 1x4 4-Key Membrane Switch Keypad
const byte ROWS = 1;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','4'}
};
byte rowPins[ROWS] = {10}; // Change pin as needed
byte colPins[COLS] = {9, 8, 7, 6}; // Change pins as needed

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int plantationType = 0; // 1: Fruits and Veg , 2: Herbs, 3: Cacti, 4: Flowers

bool isFanOn = false;
int servoPosition = 0; // 0: OFF, 90: ON
bool ok = false;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.backlight();
  dht.begin();
  myServo.attach(SERVO_PIN);
  myServo.write(0);
  pinMode(RELAY_PIN, OUTPUT);
  selectPlantationType();
}

void loop() {
  if (ok) {
    moisture();
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    moistVal = analogRead(SOIL_MOISTURE_PIN);
    Serial.println(moistVal);
    soilMoisture = (moistVal/8); //calculate percent for probes about 1 - 1.5 inches apart
    Serial.print(soilMoisture);
    Serial.println("% Moisture ");

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(humidity);
    lcd.print("%");

    if (temperature > 27) {
      if (!isFanOn) {
        digitalWrite(RELAY_PIN, HIGH);
        isFanOn = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Fan: ON ");
        Serial.print("Fan on");
        delay(2000);
        lcd.clear();
      }
    } else if (temperature <= 27) {
      if (isFanOn) {
        digitalWrite(RELAY_PIN, LOW);
        isFanOn = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Fan: OFF");
        delay(2000);
        lcd.clear();
      }
    }
    Serial.println(temperature);
    handleWatering(soilMoisture);

    delay(2000);
  }
}

void selectPlantationType() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select Plantaion");
  lcd.setCursor(0, 1);
  lcd.print("Type :");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1:F&V 2:Herbs");
  lcd.setCursor(0, 1);
  lcd.print("3:Cacti 4:Flowers");

  char key = keypad.getKey();
  while (key != '1' && key != '2' && key != '3' && key != '4') {
    key = keypad.getKey();
  }

  plantationType = key - '0';
  lcd.clear();
  lcd.print("Type Selected: ");
  lcd.print(key);
  delay(2000);
  ok = true;
}

void handleWatering(int soilMoisture) {
  int targetPosition = 0; // 0: OFF, 90: ON
  //Dry soil : 1023
  //Wet soil : 0

  if (plantationType == 1 && soilMoisture >= 600) {
    targetPosition = 90; // Watering ON
  } else if (plantationType == 1 && soilMoisture <= 400) {
    targetPosition = 0; // Watering OFF
  } else if (plantationType == 2 && soilMoisture >= 500) {
    targetPosition = 90; // Watering ON
  } else if (plantationType == 2 && soilMoisture <= 400) {
    targetPosition = 0; // Watering OFF
  } else if (plantationType == 3 && soilMoisture >= 800) {
    targetPosition = 90; // Watering ON
  } else if (plantationType == 3 && soilMoisture <= 700) {
    targetPosition = 0; // Watering OFF
  } else if (plantationType == 4 && soilMoisture >= 600) {
    targetPosition = 90; // Watering ON
  } else if (plantationType == 4 && soilMoisture <= 500) {
    targetPosition = 0; // Watering OFF
  }

  if (servoPosition != targetPosition) {
    myServo.write(targetPosition);
    servoPosition = targetPosition;

    if (servoPosition == 90) {
      lcd.setCursor(0, 1);
      lcd.print("Watering: ON ");
    } else {
      lcd.setCursor(0, 1);
      lcd.print("Watering: OFF");
    }
  }
}

void moisture() {
  // Implementation of moisture function, if any.
}