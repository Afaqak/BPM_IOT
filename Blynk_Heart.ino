//
//#define heartratePin A0
//#include "DFRobot_Heartrate.h"
//#define BLYNK_PRINT Serial
//#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h>
//#include <SimpleTimer.h>   //including the library of SimpleTimer
//
//DFRobot_Heartrate heartrate(DIGITAL_MODE); 
//
//SimpleTimer timer;
//char auth[] = " 0943b4b2b46c414391967788e8c0d8a0";            // You should get Auth Token in the Blynk App.
//                                           // Go to the Project Settings (nut icon).
//
//char ssid[] = "ZTE_2.4G_4KTfYa";    // Your WiFi credentials.
//char pass[] = "afaq4321";  // Set password to "" for open networks.
//
//void setup() {
//  Serial.begin(115200);
//  Blynk.begin(auth, ssid, pass);
//  timer.setInterval(2000, sendUptime);
//
//}
//
//void sendUptime()
//{
//  
//  uint8_t rateValue;
//  heartrate.getValue(heartratePin); ///< A1 foot sampled values
//  rateValue = heartrate.getRate(); ///< Get heart rate value 
//  if(rateValue)  {
//    Serial.println(rateValue);
//    Blynk.virtualWrite(V0,rateValue);
//  }
//  delay(20);
//  
//}
//void loop() {
//
//  Blynk.run();
//  timer.run();
// 
//}
//




#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "ZTE_2.4G_4KTfYa";     // Enter your Wi-Fi network name
const char* password = "afaq4321"; // Enter your Wi-Fi password
const char* serverIP = "192.168.1.2";      // IP address of your Node.js server
const int serverPort = 3000;                // Port number of your Node.js server
 int httpResponseCode=0;
WiFiClient client;


int PulseSensorPurplePin = A0;  // Pulse Sensor PURPLE WIRE connected to ANALOG PIN A0
int Signal;                    // Holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
unsigned long lastBeatTime = 0;  // Stores the time of the last beat
unsigned long currentBeatTime;   // Stores the time of the current beat
int beatCounter = 0;    // Counts the beats within a specified time period
float beatsPerMinute;   // Holds the calculated beats per minute


void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print the local IP address
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Send data to Node.js server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
            // Send the Signal value to Serial Plotter.

    String url = "http://" + String(serverIP) + ":" + String(serverPort) + "/data";
    http.begin(client, url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Signal=analogRead(A0);
   if (Signal > Threshold) {
    currentBeatTime = millis();
    if (lastBeatTime != 0) {
      unsigned long beatInterval = currentBeatTime - lastBeatTime;
      beatsPerMinute = 60000 / beatInterval;
      lastBeatTime = currentBeatTime;

    }
    lastBeatTime = currentBeatTime;
   }
   Serial.println(beatsPerMinute);
   if(beatsPerMinute>=50 && beatsPerMinute<=100){
   String requestBody = "bpm=" + String(beatsPerMinute);
      httpResponseCode = http.POST(requestBody);
   }
    // Check the response code
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }

  delay(200); // Wait for 5 seconds before sending the next request
}

//int getBPM() {
//  if (Signal > Threshold) {
//    currentBeatTime = millis();
//    if (lastBeatTime != 0) {
//      unsigned long beatInterval = currentBeatTime - lastBeatTime;
//      beatsPerMinute = 60000 / beatInterval;
//      lastBeatTime = currentBeatTime;
//      return int(beatsPerMinute);
//    }
//    lastBeatTime = currentBeatTime;
//  }
//  return 0;  // Return 0 if no beat is detected
//}


//#define BLYNK_TEMPLATE_ID           "TMPL6EnHkVEqk"
//#define BLYNK_TEMPLATE_NAME         "Quickstart Device"
//#define BLYNK_AUTH_TOKEN            "oIWZyChUUuZhwjsOXBAMNE-bMFFNM1vB"
//
///* Comment this out to disable prints and save space */
//#define BLYNK_PRINT Serial
//
//
//#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h>
//
//// Your WiFi credentials.
//// Set password to "" for open networks.
//char ssid[] = "ZTE_2.4G_4KTfYa";
//char pass[] = "afaq4321";
//int PulseSensorPurplePin = A0;  // Pulse Sensor PURPLE WIRE connected to ANALOG PIN A0
//
//int Signal;                    // Holds the incoming raw data. Signal value can range from 0-1024
//int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
//
//unsigned long lastBeatTime = 0;  // Stores the time of the last beat
//unsigned long currentBeatTime;   // Stores the time of the current beat
//
//int beatCounter = 0;    // Counts the beats within a specified time period
//float beatsPerMinute;   // Holds the calculated beats per minute
//
//BlynkTimer timer;
//
//// This function is called every time the Virtual Pin 0 state changes
//BLYNK_WRITE(V0)
//{
//  // Set incoming value from pin V0 to a variable
//  int value = param.asInt();
//
//  // Update state
//  Blynk.virtualWrite(V4, value);
//}
//
//// This function is called every time the device is connected to the Blynk.Cloud
//BLYNK_CONNECTED()
//{
//  // Change Web Link Button message to "Congratulations!"
//  Blynk.setProperty(V4, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
//  Blynk.setProperty(V4, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
//  Blynk.setProperty(V4, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
//}
//
//// This function sends Arduino's uptime every second to Virtual Pin 2.
//void myTimerEvent()
//{
//
//  Signal = analogRead(PulseSensorPurplePin);  // Read the PulseSensor's value.
//  Serial.println(Signal);                      // Send the Signal value to Serial Plotter.
//
//  // Check if a beat is detected
//  if (Signal > Threshold) {
//    currentBeatTime = millis();  // Get the current time of the beat.
//
//    if (lastBeatTime != 0) {
//      unsigned long beatInterval = currentBeatTime - lastBeatTime;  // Calculate the time interval between beats.
//      beatsPerMinute = 60000 / beatInterval;  // Calculate the beats per minute using the beat interval.
//
//      // Print the BPM to the Serial Monitor.
//      Serial.print("BPM: ");
//
//        Blynk.virtualWrite(V4, beatsPerMinute);
//    }
//
//    lastBeatTime = currentBeatTime;  // Update the last beat time with the current beat time.
//  }
//
//  
//}
//
//void setup()
//{
//  // Debug console
//  Serial.begin(115200);
//
//  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
//  // You can also specify server:
//  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
//  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
//
//  // Setup a function to be called every second
//  timer.setInterval(1000L, myTimerEvent);
//}
//
//void loop()
//{
//  Blynk.run();
//  timer.run();
//  // You can inject your own code or combine it with other sketches.
//  // Check other examples on how to communicate with Blynk. Remember
//  // to avoid delay() function!
//}
//
//
