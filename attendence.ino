#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define SS_PIN 10
#define RST_PIN 9
byte readCard[4];
String MasterTag = "A7674765";
String tagID = "";
MFRC522 mfrc522(SS_PIN, RST_PIN);
SoftwareSerial SIM900A(2, 3); 
LiquidCrystal_I2C lcd(0x3F, 16, 2); 

void setup() {
  SIM900A.begin(9600);    
  Serial.begin(9600);       
  Serial.println("Text Message Module Ready & Verified");

  sendATCommand("AT", true);  
  delay(100);
  sendATCommand("AT+CMGF=1", true); 
  delay(100);
  sendATCommand("AT+CNMI=2,2,0,0,0", true);  
  delay(100);
  
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println("--------------------------");
  Serial.println(" Access Control ");
  Serial.println("Scan Your Card>>");
  lcd.init(); 
  lcd.backlight();
}

void loop() {
  
  if (Serial.available() > 0) {
    char command = Serial.read();
  }

  if (SIM900A.available() > 0) {
    Serial.write(SIM900A.read());  
  }


  lcd.setCursor(1, 0);
  lcd.print("Scan Your Card");
    lcd.setCursor(1, 1);
  lcd.print("   Tap It>>");


while (getID()) {
     
    if (tagID == MasterTag){
      Serial.println("Sending");
     
      SendMessage();
          lcd.clear();
      lcd.setCursor(2, 0);
              lcd.print("Your Attendence is present");

      lcd.setCursor(2, 1);
  
                lcd.print("is present");

      delay(2000);
          lcd.clear();

      Serial.println("--------------------------");
    }
    else{
       lcd.clear();
        lcd.setCursor(2, 0);
       
  lcd.print("Tap Again/");
          lcd.setCursor(2, 1);
  lcd.print("Its wrong NFC");

      delay(2000);
        lcd.clear();


      }
    delay(2000);


    
    Serial.println("Scan Your Card>>");
  }
  
}

void sendATCommand(String command, boolean waitForResponse) {
  SIM900A.println(command);
  if (waitForResponse) {
    while (SIM900A.available()) {
      Serial.write(SIM900A.read());
    }
    Serial.println();
  }
}
boolean getID() 
{

  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) {
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); 
  return true;
}
void SendMessage() {
  Serial.println("Sending Message...");
  SIM900A.println("AT+CMGS=\"+916362690632\"\r");  // Set recipient number
  delay(1000);
  SIM900A.println("Your Kid is Present In Class ");  // Message content
  delay(1000);
  SIM900A.println((char)26);  // Send message
  Serial.println("Message sent successfully");
}
