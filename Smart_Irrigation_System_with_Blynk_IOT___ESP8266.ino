/*
 Smart Irrigation System : Blynk IoT

 
The Smart Irrigation System is an automated solution designed to optimize water usage for plants. 
Utilizing soil moisture sensors, it monitors moisture levels and activates a water pump when levels 
drop below a set threshold. Controlled via a mobile app through Blynk, users can manually operate 
the system and receive alerts when soil moisture is low. This system ensures efficient irrigation, 
conserving water while promoting healthier plant growth.

//Author: Wyder Lalangan
// GitHub Repository: https://github.com/WyderL24
// LinkedIn Profile: www.linkedin.com/in/wyderl
// Connect with Me: 
//      E-mail: wyderl24@gmail.com
//       Skype: live:.cid.19f15983a80c2bd1
// Blynk: https://blynk.io/
// ESP8266 Documentation: https://www.esp8266.com/
------------------------------------------*/

#define BLYNK_TEMPLATE_ID "template-id"
#define BLYNK_TEMPLATE_NAME "template-name"
#define BLYNK_AUTH_TOKEN "authentication-token"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define SOIL_MOISTURE_PIN A0     // Analog pin for soil moisture sensor (use A0 on ESP8266)
#define THRESHOLD_MOISTURE 100    // Adjust this value based on your soil moisture sensor readings
#define PUMP_PIN 2                // Digital pin for controlling the pump
#define PUMP_SWITCH V6            // Virtual pin for manual pump control

char auth[] = BLYNK_AUTH_TOKEN;  // Blynk authentication token
char ssid[] = "WiFi Username";    // WiFi SSID
char pass[] = "WiFi Password";    // WiFi Password

BlynkTimer timer;
bool isPumpOn = false;            // Variable to track pump status

void sendSensorData() {
    int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
    Serial.print("Soil Moisture: ");
    Serial.println(soilMoisture);

    // Map the analog sensor values to a percentage (0-100)
    int soilMoisturePercentage = map(soilMoisture, 3500, 4095, 100, 0);
    Serial.print("Soil Moisture Percentage: ");
    Serial.println(soilMoisturePercentage);

    // Send soil moisture data to Blynk
    Blynk.virtualWrite(V5, soilMoisturePercentage);

    // Control the pump based on soil moisture level and manual switch
    if (isPumpOn || soilMoisturePercentage < THRESHOLD_MOISTURE) {
        digitalWrite(PUMP_PIN, HIGH); // Turn on the pump

        // Send alert if the pump is activated automatically
        if (!isPumpOn) {
            Blynk.logEvent("moisture_alert", "Soil moisture is below the threshold!");
            Serial.println("Soil moisture is below the threshold!");
        }
    } else {
        if (!isPumpOn) {
            digitalWrite(PUMP_PIN, LOW); // Turn off the pump
        }
    }
}

BLYNK_WRITE(PUMP_SWITCH) {
    isPumpOn = param.asInt();
    Serial.println(isPumpOn ? "Pump manually turned ON" : "Pump manually turned OFF");
}

void setup() {
    Serial.begin(9600);
    pinMode(PUMP_PIN, OUTPUT); // Set pump pin as an output

    Blynk.begin(auth, ssid, pass);
    timer.setInterval(3000L, sendSensorData); // Check soil moisture every 3 seconds

    // Initialize switch widget
    Blynk.virtualWrite(PUMP_SWITCH, isPumpOn);
    Blynk.syncVirtual(PUMP_SWITCH);
}

void loop() {
    Blynk.run();
    timer.run();
}
