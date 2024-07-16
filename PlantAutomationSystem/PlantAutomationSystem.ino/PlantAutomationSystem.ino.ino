#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Keypad.h>
#include <Servo.h>

#define DHTPIN A0
#define DHTTYPE DHT11

#define SOIL_MOISTURE_PIN A1
#define SERVO_PIN 9
#define RED_LED 2
#define GREEN_LED 8
int moistVal = 0;
int soilMoisture = 0;

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 11, 10};
byte colPins[COLS] = {7, 6, 5, 4};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int plantationType = 0; // 1: Fruits, 2: Herbs, 3: Cacti, 4: Flowers

bool isTempHigh = false;
int servoPosition = 0; // 0: OFF, 90: ON
bool ok = false;

void setup() {

  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.backlight();
  dht.begin();
  myServo.attach(SERVO_PIN);
  myServo.write(0);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  selectPlantationType();

}

void loop() {
  if (ok){
  float temperature = dht.readTemperature()-3;
  float humidity = dht.readHumidity();
  moistVal = analogRead(SOIL_MOISTURE_PIN);
  
  Serial.println(moistVal);
  soilMoisture = (moistVal/8); //calculate percent for probes about 1 - 1.5 inches apart
  Serial.print(soilMoisture);
  Serial.println("% Moisture ");
  //int soilMoisture = analogRead(SOIL_MOISTURE_PIN);

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print("%");
  
  if (temperature > 27) {
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      isTempHigh = true;
      Serial.print("Temp high");
    
  } else if (temperature <= 27) {
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      isTempHigh = false;
      Serial.print("Temp low");
    
  }
  handleWatering(soilMoisture);
  delay(2000);
  }
}

void selectPlantationType() {
  lcd.clear();
  lcd.print("Select Type:");
  delay(5000);
  lcd.setCursor(0, 0);
  lcd.print("1:Veg&Fru 2:Herbs");
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
  lcd.clear();
}

void handleWatering(int soilMoisture) {
  int targetPosition = 0; // 0: OFF, 90: ON

  //dry soil = 0
  //wet soil = 1023

  if (plantationType == 1 && soilMoisture <= 30) {
    targetPosition = 90; // Watering ON
  } else if (plantationType == 1 && soilMoisture >= 70) {
    targetPosition = 0; // Watering OFF
  } else if (plantationType == 2 && soilMoisture <= 20) {
    targetPosition = 90; // Watering ON
  } else if (plantationType == 2 && soilMoisture >= 60) {
    targetPosition = 0; // Watering OFF
  } else if (plantationType == 3 && soilMoisture <= 50) {
    targetPosition = 90; // Watering ON
  } else if (plantationType == 3 && soilMoisture >= 80) {
    targetPosition = 0; // Watering OFF
  } else if (plantationType == 4 && soilMoisture <= 40) {
    targetPosition = 90; // Watering ON
  } else if (plantationType == 4 && soilMoisture >= 70) {
    targetPosition = 0; // Watering OFF
  }


  if (servoPosition != targetPosition) {
    myServo.write(targetPosition);
    servoPosition = targetPosition;

    if (servoPosition == 90) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Watering: ON ");
      delay(3000);
      lcd.clear();
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Watering: OFF");
      delay(3000);
      lcd.clear();
    }
  }
}