#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Keypad.h>
#include <Servo.h>
#include <IRremote.h>

#define DHTPIN A0
#define DHTTYPE DHT11

#define SOIL_MOISTURE_PIN A1
#define LDR_PIN A2
#define IR_PIN 3
#define SERVO_PIN 9
#define RELAY_PIN 8

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

IRrecv irrecv(IR_PIN);
decode_results results;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int plantationType = 0; // 1: Fruits, 2: Herbs, 3: Cacti, 4: Flowers

bool isFanOn = false;
int servoPosition = 0; // 0: OFF, 90: ON

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  dht.begin();
  myServo.attach(SERVO_PIN);
  pinMode(RELAY_PIN, OUTPUT);
  irrecv.enableIRIn(); // Start the IR receiver

  selectPlantationType();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  int ldrValue = analogRead(LDR_PIN);

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print("%");

  bool isArtificialLight = false;
  if (irrecv.decode(&results)) {
    isArtificialLight = true;
    irrecv.resume(); // Receive the next value
  }

  bool isDaytime = ldrValue < 500 && !isArtificialLight; // Adjust threshold as needed

  if ((isDaytime && temperature > 27) || (!isDaytime && temperature > 21)) {
    if (!isFanOn) {
      digitalWrite(RELAY_PIN, HIGH);
      isFanOn = true;
      lcd.setCursor(0, 1);
      lcd.print("Fan: ON ");
    }
  } else {
    if (isFanOn) {
      digitalWrite(RELAY_PIN, LOW);
      isFanOn = false;
      lcd.setCursor(0, 1);
      lcd.print("Fan: OFF");
    }
  }

  handleWatering(soilMoisture);

  delay(2000);
}

void selectPlantationType() {
  lcd.clear();
  lcd.print("Select Type:");
  lcd.setCursor(0, 1);
  lcd.print("1:Fruits 2:Herbs");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("3:Cacti 4:Flowers");
  delay(2000);

  char key = keypad.getKey();
  while (key != '1' && key != '2' && key != '3' && key != '4') {
    key = keypad.getKey();
  }

  plantationType = key - '0';
  lcd.clear();
  lcd.print("Type Selected: ");
  lcd.print(key);
  delay(2000);
}

void handleWatering(int soilMoisture) {
  int targetPosition = 0; // 0: OFF, 90: ON

  if (plantationType == 1 && soilMoisture < 400) {
    targetPosition = 90; // Watering ON
  } else if (plantationType == 1 && soilMoisture > 600) {
    targetPosition = 0; // Watering OFF
  } else if (plantationType == 2 && soilMoisture < 300) {
    targetPosition = 90; // Watering ON
  } else if (plantationType == 2 && soilMoisture > 500) {
    targetPosition = 0; // Watering OFF
  } else if (plantationType == 3 && soilMoisture < 200) {
    targetPosition = 90; // Watering ON
  } else if (plantationType == 3 && soilMoisture > 400) {
    targetPosition = 0; // Watering OFF
  } else if (plantationType == 4 && soilMoisture < 350) {
    targetPosition = 90; // Watering ON
  } else if (plantationType == 4 && soilMoisture > 550) {
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
