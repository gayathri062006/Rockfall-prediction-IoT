#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <LiquidCrystal_I2C.h>

Adafruit_MPU6050 mpu;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buzzerPin = 8;
const int ledSafe   = 10;
const int ledDanger = 9;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.begin();              // MPU6050 init
  lcd.init(); lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledSafe, OUTPUT);
  pinMode(ledDanger, OUTPUT);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);   // get acceleration

  // simple slope angle from X‑axis acceleration (approx)
  float angle = atan2(a.acceleration.x, a.acceleration.z) * 57.3; // degree

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Slope:");
  lcd.print(angle,1);
  lcd.print(" deg");

  if (abs(angle) > 30) {        // threshold for dangerous tilt
    digitalWrite(ledDanger, HIGH);
    digitalWrite(ledSafe, LOW);
    tone(buzzerPin, 1000);
    lcd.setCursor(0,1);
    lcd.print("ROCKFALL RISK!");
  } else {
    digitalWrite(ledDanger, LOW);
    digitalWrite(ledSafe, HIGH);
    noTone(buzzerPin);
    lcd.setCursor(0,1);
    lcd.print("Safe slope    ");
  }

  delay(200);
}
