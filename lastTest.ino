#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte zero[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte one[] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};

byte two[] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
};

byte three[] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100
};

byte four[] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110
};

byte five[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
unsigned long millisLcd;
unsigned long millisSystem;
unsigned long millisLedBlue;
unsigned long millisLedRed;


const int pin = A0;
int musclePower;

void setup() {
  Serial.begin(9600);

  lcd.setCursor(0, 0);
  lcd.print("Muscle Power :");
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(2, 0);
  digitalWrite(3, 0);

  lcd.begin();
  lcd.createChar(0, zero);
  lcd.createChar(1, one);
  lcd.createChar(2, two);
  lcd.createChar(3, three);
  lcd.createChar(4, four);
  lcd.createChar(5, five);
}

void loop() {

  int value = analogRead(pin);
  musclePower = map(value, 1023, 600, 0, 100);

  lcd.setCursor(0, 0);
  lcd.print("Muscle Power :");
    for (int i = 0; i <= musclePower; i++) {
      updateProgressBar(i, 100, 1);
    }

    if (millis() - millisLcd >= 1400)
    {
      lcd.clear();
      millisLcd = millis();
    }

  Serial.println(value);
  Serial.print("percent = ");
  Serial.println(musclePower);

  if (millis() - millisSystem >= 50)
  {
      updateSystem();

  }


}
void updateSystem() {
  if (musclePower > 10 && musclePower <= 50) {
    if (millis() - millisLedBlue >= 1)
    {
      digitalWrite(2, HIGH);
    }
  }
  else {
    digitalWrite(2, LOW);
  }
  if (musclePower > 50) {
    if (millis() - millisLedRed >= 1)
    {
      digitalWrite(3, HIGH);
    }
  }
  else {
    digitalWrite(3, LOW);
  }
}
void updateProgressBar(unsigned long count, unsigned long totalCount, int lineToPrintOn)
{
  double factor = totalCount / 80.0;        //See note above!
  int percent = (count + 1) / factor;
  int number = percent / 5;
  int remainder = percent % 5;
  if (number > 0)
  {
    lcd.setCursor(number - 1, lineToPrintOn);
    lcd.write(5);
  }

  lcd.setCursor(number, lineToPrintOn);
  lcd.write(remainder);
}
