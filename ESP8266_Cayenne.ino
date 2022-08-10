//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#define VIRTUAL_CHANNEL 1
#define ACTUATOR_PIN 4 

char ssid[] = "_JU5TU51";
char wifiPassword[] = "free4all";

int trigger = 16;
int echo = 5;

//Info auntentikasi dari website Cayenne
char username[] = "dfbe2330-1343-11ed-bbc1-5d0b0fa0a668";
char password[] = "9dbe1f9e0f73e314fa37c76047f9ff8d9a691ca0";
char clientID[] = "fcb7a0f0-1344-11ed-8df2-dd50487e509b";

long T;
float distanceCM;
unsigned long lastMillis = 0;

void setup() {
  //Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(115200);
  Cayenne.begin(username, password, clientID);
  pinMode(ACTUATOR_PIN, OUTPUT);
}

void loop() {
  Cayenne.loop();
  digitalWrite(trigger, LOW);
  delay(1);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  T = pulseIn(echo, HIGH);
  distanceCM = T * 0.034;
  distanceCM = distanceCM / 2;
  Serial.print("Distance in cm: ");
  Serial.println(distanceCM);
  Serial.println("Centimeter:");
  delay(500);
  // Upload setiap data dengan interval waktu 10detik
  if(millis() - lastMillis > 10000) {
    lastMillis = millis();
    //Menuliskan data ke web informasi cayenne
    Cayenne.virtualWrite(0, distanceCM);
  }
}

  CAYENNE_IN(VIRTUAL_CHANNEL)
  {
    int value = getValue.asInt();
    CAYENNE_LOG("Channel %d, pin %d, value %d", VIRTUAL_CHANNEL, ACTUATOR_PIN, value);
    // nilai value yang diterima dari slider web cayenne yang selanjutnya diteruskan ke digital pin
    digitalWrite(ACTUATOR_PIN, value);
  }

  CAYENNE_OUT_DEFAULT()
   {
     // Meneruskan data ke cayenne melalui channel 1
     Cayenne.virtualWrite(1, millis());
   }
