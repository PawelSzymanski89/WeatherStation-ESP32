#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>
#include <DHT.h>
#include <DHT_U.h>
//#include <WiFi.h>
#include <MQ135.h>


//NETWORK SETTIGS
const char* ssid     = "Klan S 2.4";
const char* password = "K123654789s";

//DHT11 SENSOR SETTINGS
#define DHTTYPE DHT11
uint8_t DHTPin = 32;
DHT dht(DHTPin, DHTTYPE);
float Temperature;
float Humidity;
float TemperatureLast;
float HumidityLast;

//AIR SENSOR
uint8_t AIR = 25;
MQ135 gasSensor = MQ135(AIR);
int ppm;
int ppmLast;

//RGB LED SETTINGS
#define RED_PIN 27
#define GREEN_PIN 26
#define BLUE_PIN 14

//DISPLAY SETTINGS
uint8_t __CS = 15;
uint8_t __DC = 2;
TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC);

#define WHITE   0xFFFF
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0

//INITIALIZATION
//--------------------------------------------------
void setup() {
  pinMode(DHTPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Weather Station Begin! ");
  dht.begin();
  tft.begin();

  //  Serial.println("NETWORK-CONNECTING");
  //  WiFi.begin(ssid, password);
  //  while (WiFi.status() != WL_CONNECTED) {
  //    setRGB(21, 0, 255);
  //  }
}

//MAIN LOOP FUNCTION
//--------------------------------------------------
void loop() {
  delay(1000);
  markLoopByBlink();
  dispSensors();
  displayLedStatus();
}

//DISPLAY LED STATUS
//--------------------------------------------------
void displayLedStatus() {
  //WIFI
  //  if (WiFi.status() == WL_CONNECTED) blinkRGB(0, 255, 0);
  //  else blinkRGB(21, 0, 255);
  if(ppm < 450) setRGB(0,255,0);
  else if(ppm > 450 && ppm < 600)setRGB(228,95,0);
  else if(ppm > 600 && ppm < 1000)setRGB(255,0,0);
  else blinkRGB(255, 0, 0);
}

//SENSOR FUNCTIONS
//--------------------------------------------------
void dispSensors() {
  Temperature = dht.readTemperature();
  delay(1000);
  Humidity = dht.readHumidity();
  delay(1000);
  ppm = gasSensor.getPPM();
  delay(1000);

  if (Temperature != TemperatureLast || Humidity != HumidityLast || ppm != ppmLast) {
    if ((String)Temperature != "nan" && (String)Humidity != "nan") {
      Serial.println("T: " + (String)Temperature + "  H: " + (String)Humidity);
      tft.fillScreen();
      tft.setCursor(2, 2);
      tft.setTextColor(WHITE);
      tft.setTextSize(1);
      tft.println("TEMPERATURE:");

      tft.setCursor(2, 12);
      tft.setTextColor(BLUE);
      tft.setTextSize(2);
      tft.print((String)Temperature + " C");

      tft.setCursor(2, 32);
      tft.setTextColor(WHITE);
      tft.setTextSize(1);
      tft.println("HUMIDITY:");

      tft.setCursor(2, 42);
      tft.setTextColor(RED);
      tft.setTextSize(2);
      tft.println( (String)Humidity + " %");

      tft.setCursor(2, 62);
      tft.setTextColor(WHITE);
      tft.setTextSize(1);
      tft.println("PPM LEVEL:");

      tft.setCursor(2, 72);
      tft.setTextColor(YELLOW);
      tft.setTextSize(2);
      tft.println( (String)ppm);
     
      TemperatureLast = Temperature;
      HumidityLast = Humidity;
      ppmLast = ppm;
    } else Serial.println("nan captured");
  }
}


//LED FUNCTIONS
//--------------------------------------------------
void blinkRGB(int r, int g, int b) {
  setRGB(r, g, b);
  delay(1000);
  offRGB();
}

void setRGB(int r, int g, int b) {
  digitalWrite(RED_PIN, r);
  digitalWrite(GREEN_PIN, g);
  digitalWrite(BLUE_PIN, b);
}

void offRGB() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}

void markLoopByBlink() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
}
