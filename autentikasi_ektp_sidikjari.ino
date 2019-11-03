#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_Fingerprint.h>
#define pinSDA 10
#define pinRST 9
MFRC522 RFID(pinSDA, pinRST);

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int LedPin = 5;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  RFID.PCD_Init();
  pinMode(LedPin, OUTPUT);

// fingerprint
  while (!Serial);
  delay(100);
  finger.begin(57600);
  if (finger.verifyPassword()) {
  } else {
    while (1) { delay(1); }
  }
  finger.getTemplateCount();
//  akhir fingerprint
}
void loop() {
  getFingerprintIDez();
  delay(50);
  if ( ! RFID.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! RFID.PICC_ReadCardSerial()) {
    return;
  }
  String content = "";
  byte letter;
  for (byte i = 0; i < RFID.uid.size; i++) {
    Serial.print(RFID.uid.uidByte[i] < 0x10 ? "0" : "");
    Serial.print(RFID.uid.uidByte[i], HEX);
    content.concat(String(RFID.uid.uidByte[i] < 0x10 ? "0" : ""));
    content.concat(String(RFID.uid.uidByte[i], HEX));
  }
  String ku = String(Serial.println(+"-"));
  digitalWrite(LedPin, HIGH);
  delay(2500);
  digitalWrite(LedPin, LOW);

  
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_NOFINGER:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      return p;
    case FINGERPRINT_IMAGEFAIL:
      return p;
    default:
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      return p;
    case FINGERPRINT_FEATUREFAIL:
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      return p;
    default:
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    return p;
  } else {
    return p;
  }   
  
  // found a match!
  Serial.print("finger"); Serial.println(finger.fingerID); 
//  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("finger"); Serial.print(finger.fingerID); Serial.println("-");
  digitalWrite(LedPin, HIGH);
  delay(2000);
  digitalWrite(LedPin, LOW); 
//  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
