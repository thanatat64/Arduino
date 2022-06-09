#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//lcd progressbar
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
//lcd progressbar

unsigned long millisLcd; // ตัว delay lcd
unsigned long millisSystem; // ตัว delay system
unsigned long millisLedWhite; // ตัว delay Led(white)
unsigned long millisLedBlue; // ตัว delay Led(Blue)
unsigned long millisLedRed; // ตัว delay Led(Red)

const int pin = A1;// ขา 1 TL072 C ต่อรับ output เข้า analog arduino เพื่อ read ค่า
int musclePower; // ตัว map ค่า

//mean calculator
#define SMA 10
int avg[SMA];
//int avgSize = 0;

void updateAVG(int value) {//update ค่าเฉลี่ย แล้วนำมาหาค่า mean ทุกๆ 10 ค่า
  for (int i = 0; i < SMA - 1; i++) {
    avg[i] = avg[i + 1];
  }
  avg[SMA - 1] = value;
}
int meanAVG() {
  int sum = 0 ;
  for (int i = 0; i < SMA; i++) {
    sum += avg[i];
  }
  return sum / SMA;
}

void setup() {
  Serial.begin(9600);

  lcd.setCursor(0, 0);
  lcd.print("Muscle Power :");
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(2, 0);//blue
  digitalWrite(3, 0);//red
  digitalWrite(4, 0);//white

  lcd.begin();
  lcd.createChar(0, zero);
  lcd.createChar(1, one);
  lcd.createChar(2, two);
  lcd.createChar(3, three);
  lcd.createChar(4, four);
  lcd.createChar(5, five);


}

void loop() {

  int value = analogRead(pin);//readค่าที่ได้
  musclePower = map(value, 1023, 382, 0, 100);// 0-641 uV
  updateAVG(musclePower);//เอาค่าที่ map เก็บใน array
  musclePower = meanAVG();//คำนวณหาค่าเฉลี่ย
  lcd.setCursor(0, 0);
  lcd.print("EMG Value: ");
  if (musclePower <= 0) {
    musclePower = 0;
    lcd.print("0");
    lcd.print("  %");
  } else if (musclePower > 0 && musclePower <= 100) {
    lcd.print(musclePower);
    lcd.print(" %");
  }
  else if (musclePower > 100) {
    musclePower = 100;
    lcd.print(musclePower);
    lcd.print("%");
  }
  for (int i = 0; i <= musclePower; i++) {
    updateProgressBar(i, 100, 1);
  }

  if (millis() - millisLcd >= 500)
  {
    lcd.clear();
    millisLcd = millis();
  }

  Serial.println(value);
  Serial.print("percent = ");
  Serial.println(musclePower);

  if (millis() - millisSystem >= 1000)
  {
    updateSystem();

  }


}
void updateSystem() {
  if (musclePower > 0 && musclePower <= 33) {
    if (millis() - millisLedWhite >= 1)
    {
      digitalWrite(4, HIGH);
    }
  }
  else {
    digitalWrite(4, LOW);
  }
  if (musclePower > 34 && musclePower <= 67) {
    if (millis() - millisLedBlue >= 1)
    {
      digitalWrite(2, HIGH);
    }
  }
  else {
    digitalWrite(2, LOW);
  }
  if (musclePower > 67) {
    if (millis() - millisLedRed >= 1)
    {
      digitalWrite(3, HIGH);
    }
  }
  else {
    digitalWrite(3, LOW);
  }
}

void updateProgressBar(unsigned long count, unsigned long totalCount, int lineToPrintOn)//method วาดแถบวัดแรง
{
  double factor = totalCount / 80.0;       
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
