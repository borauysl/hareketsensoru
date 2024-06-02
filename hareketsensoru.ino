#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // lcd ekranı için nesne oluşturuldu
Servo motor; // motorun tanımlanması burda
int motionSensorPin = 8; // hareket sensörü pin numarası
int ledPin = 13; // led pin numarası
unsigned long previousMillis = 0; // önceki süre
bool isMotionDetected = false; // hareket var mii =?

// hareket algılandığında ledin yanıp sönme süresi (milisaniye cinsinden)
const long interval = 200; // 0.5 saniye
const long displayTime = 1500; // lcd'de mesajın görüneceği süre (1.5 saniye)

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(motionSensorPin, INPUT); // hareket sensörü pininin giriş olarak ayarlanması
  motor.attach(9); // motorun pin numarası
  pinMode(ledPin, OUTPUT); // led pininin çıkış olarak ayarlanması
}

void loop() {
  int motion = digitalRead(motionSensorPin); // hareket sensöründen okuma yapılıyor
  
  if (motion == HIGH) { // hareket algılandıysa
    isMotionDetected = true; // hareket algılandı olarak işaretle
    unsigned long currentMillis = millis(); // Geçerli zamanı al
    if (currentMillis - previousMillis >= displayTime) {
      lcd.clear(); // lcd ekranı temizleniyo
      lcd.setCursor(0, 0); // İlk satıra cursor konumlandırılıyo
      lcd.print("Harket algiladi!"); // lcd ekranına yazdırma yapılıyo
      lcd.setCursor(0,1);
      lcd.print("Motor donuyor!");

      previousMillis = currentMillis;
    }
    
    // ledi yarım saniye aralıklarla 5 saniye boyunca yanıp söndür
    if (currentMillis - previousMillis <= displayTime) {
      if ((currentMillis - previousMillis) % interval == 0) { // 0.5 saniye aralıklarla
        digitalWrite(ledPin, !digitalRead(ledPin)); // LED'in durumunu tersine çevir
      }
    }
    
    // motoru 0-90 derece arasında döndür
    int angle = map(currentMillis % 2000, 0, 2000, 0, 90); // 2 saniyede bir dönüş yapacak
    motor.write(angle);
    
  } else { 
    if (isMotionDetected) { 
      lcd.clear(); // lcd ekranı temizleniyo
      lcd.setCursor(0, 0); 
      lcd.print("Hareketlilik yok"); // lcd ekranına yazdırma yapılıyo
      lcd.setCursor(0,1);
      lcd.print("Motor donmuyor!");
      digitalWrite(ledPin, HIGH); // ledi kapat
      isMotionDetected = false; 
    }
  }
}
