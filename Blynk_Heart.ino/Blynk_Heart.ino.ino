#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "ZTE_2.4G_4KTfYa";     // Enter your Wi-Fi network name
const char* password = "afaq4321";         // Enter your Wi-Fi password
const char* serverIP = "192.168.1.6";      // IP address of your Node.js server
const int serverPort = 3000;                // Port number of your Node.js server

int PulseSensorPurplePin = A0;      // Pulse Sensor PURPLE WIRE connected to ANALOG PIN A0
int Threshold = 550;                // Determine which Signal to "count as a beat" and which to ignore.
unsigned long lastBeatTime = 0;     // Stores the time of the last beat
unsigned long currentBeatTime;      // Stores the time of the current beat
int beatCounter = 0;                // Counts the beats within a specified time period
float beatsPerMinute;               // Holds the calculated beats per minute

const int numReadings = 10;         // Number of readings to average
float bpmReadings[numReadings];     // Array to store BPM readings
int bpmIndex = 0;                   // Index of the current reading
boolean readingsComplete = false;   // Flag to indicate if readings are complete

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

    String url = "http://" + String(serverIP) + ":" + String(serverPort) + "/data";
    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int Signal = analogRead(A0);
    
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
    
    if (!readingsComplete) {
      bpmReadings[bpmIndex] = beatsPerMinute;
      bpmIndex = (bpmIndex + 1) % numReadings;
      
      // Check if all readings are complete
      if (bpmIndex == 0) {
        readingsComplete = true;
      }
    }
    
    if (readingsComplete) {
      // Calculate the average BPM
      float sum = 0;
      for (int i = 0; i < numReadings; i++) {
        sum += bpmReadings[i];
      }
      float averageBPM = sum / numReadings;

      String category = getCategory(averageBPM);  // Get the category based on average BPM value

      String requestBody = "category=" + category + "&bpm=" + String(averageBPM);
      int httpResponseCode = http.POST(requestBody);

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

      readingsComplete = false; // Reset the flag for the next set of readings
    }

    http.end();
  }

  delay(200); // Wait for 200 milliseconds before sending the next request
}

String getCategory(float bpm) {
  if (bpm < 60) {
    return "Low";
  } else if (bpm >= 60 && bpm < 100) {
    return "Normal";
  } else if (bpm >= 100 && bpm < 120) {
    return "Elevated";
  } else {
    return "High";
  }
}
