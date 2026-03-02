#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

#define SOS_BUTTON_PIN  2
#define BUZZER_PIN      5
#define SAFETY_LED_PIN  6

#define GPS_RX_PIN      3
#define GPS_TX_PIN      4
#define GSM_RX_PIN      7
#define GSM_TX_PIN      8

const String EMERGENCY_NUMBER_1 = "+919876543210"; 
const String EMERGENCY_NUMBER_2 = "+919988776655";

SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN);
SoftwareSerial gsmSerial(GSM_RX_PIN, GSM_TX_PIN);

TinyGPSPlus gps;

bool isSosActive = false;
bool isBuzzerActive = false;
bool isLightOn = false;

unsigned long previousMillisLight = 0;
const long lightFlickerInterval = 200;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
int lastButtonState = HIGH;
int buttonState = HIGH;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Women's Safety Wearable Initializing..."));

  pinMode(SOS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SAFETY_LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(SAFETY_LED_PIN, LOW);

  gsmSerial.begin(9600);
  gpsSerial.begin(9600);

  delay(3000);
  setupGSM();
  
  Serial.println(F("System Ready."));
}

void loop() {
  gpsSerial.listen();
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  int reading = digitalRead(SOS_BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        if (!isSosActive) {
          Serial.println(F("SOS BUTTON PRESSED!"));
          isSosActive = true;
          isBuzzerActive = true;
          isLightOn = true;
          
          digitalWrite(BUZZER_PIN, HIGH);
          digitalWrite(SAFETY_LED_PIN, HIGH);
          
          triggerSOS(); 
        } 
        else {
          Serial.println(F("SOS DEACTIVATED!"));
          isSosActive = false;
          isBuzzerActive = false;
          isLightOn = false;
          digitalWrite(BUZZER_PIN, LOW);
          digitalWrite(SAFETY_LED_PIN, LOW);
        }
      }
    }
  }
  lastButtonState = reading;

  if (isSosActive) {
    if (isBuzzerActive) {
      digitalWrite(BUZZER_PIN, HIGH);
    }

    if (isLightOn) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillisLight >= lightFlickerInterval) {
        previousMillisLight = currentMillis;
        if (digitalRead(SAFETY_LED_PIN) == LOW) {
          digitalWrite(SAFETY_LED_PIN, HIGH);
        } else {
          digitalWrite(SAFETY_LED_PIN, LOW);
        }
      }
    }
  }
}

void triggerSOS() {
  String latitude = "Unavailable";
  String longitude = "Unavailable";

  if (gps.location.isValid()) {
    latitude = String(gps.location.lat(), 6);
    longitude = String(gps.location.lng(), 6);
  } else {
    Serial.println(F("Warning: No GPS Fix acquired yet. Sending default/last known location alert."));
  }

  String mapsLink = "https://maps.google.com/?q=" + latitude + "," + longitude;
  String smsMessage = "EMERGENCY! SOS Alert triggered.\nLocation: " + mapsLink;
  
  Serial.println(F("Sending SMS to Contact 1..."));
  sendSMS(EMERGENCY_NUMBER_1, smsMessage);
  
  Serial.println(F("Sending SMS to Contact 2..."));
  sendSMS(EMERGENCY_NUMBER_2, smsMessage);

  Serial.println(F("SOS Protocol Executed Successfully."));
}

void setupGSM() {
  gsmSerial.listen();
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  
  gsmSerial.println("AT");
  delay(1000);
  updateSerial();
  
  Serial.println(F("GSM Module Initialized."));
}

void sendSMS(String number, String text) {
  gsmSerial.listen();
  
  gsmSerial.print("AT+CMGS=\"");
  gsmSerial.print(number);
  gsmSerial.println("\"");
  delay(1000);
  
  gsmSerial.print(text);
  delay(500);
  
  gsmSerial.write(26);
  delay(5000);
  
  updateSerial();
}

void updateSerial() {
  while (gsmSerial.available()) {
    Serial.write(gsmSerial.read());
  }
}
