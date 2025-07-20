#include <SoftwareSerial.h>
#include <dht_nonblocking.h>

#define DHTPIN 4        // DHT11 data pin connected to Arduino pin 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Define SoftwareSerial pins for ESP8266
SoftwareSerial esp8266(2, 3); // RX, TX

const char* ssid = "YOUR_SSID";         // βάλτο εδώ
const char* password = "YOUR_PASSWORD"; // βάλτο εδώ

// HTTP server details
const char* host = "httpbin.org";       // Για δοκιμή, μπορείς να βάλεις δικό σου server
const int port = 80;

void setup() {
  Serial.begin(9600);
  esp8266.begin(115200); // ESP8266 default baud (ίσως χρειαστεί να το αλλάξεις σε 9600 αν δεν δουλεύει)

  dht.begin();
  delay(2000);

  Serial.println("Setup started");

  // Reset ESP8266
  sendCommand("AT+RST", 2000);
  // Set WiFi mode to station
  sendCommand("AT+CWMODE=1", 1000);
  // Connect to WiFi
  String cmd = "AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"";
  if (sendCommand(cmd.c_str(), 5000)) {
    Serial.println("WiFi connected");
  } else {
    Serial.println("WiFi connection failed");
  }
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Build HTTP GET request (για απλή demo, στέλνουμε GET με params)
  String getRequest = String("GET /get?temp=") + temperature + "&hum=" + humidity + " HTTP/1.1\r\n" +
                      "Host: " + host + "\r\n" +
                      "Connection: close\r\n\r\n";

  // Connect to server
  String cmd = "AT+CIPSTART=\"TCP\",\"" + String(host) + "\"," + String(port);
  if (!sendCommand(cmd.c_str(), 5000)) {
    Serial.println("Connection to server failed");
    delay(10000);
    return;
  }

  delay(2000);

  // Send length of data
  cmd = "AT+CIPSEND=" + String(getRequest.length());
  if (!sendCommand(cmd.c_str(), 2000)) {
    Serial.println("Failed to start sending data");
    sendCommand("AT+CIPCLOSE", 1000);
    delay(10000);
    return;
  }

  delay(1000);

  // Send actual GET request
  esp8266.print(getRequest);

  // Wait for send confirmation
  if (waitForResponse("SEND OK", 5000)) {
    Serial.println("Data sent successfully");
  } else {
    Serial.println("Send failed");
  }

  // Close connection
  sendCommand("AT+CIPCLOSE", 1000);

  delay(60000); // Μετρήσεις ανά 60 δευτερόλεπτα
}

// Send AT command and wait for "OK"
bool sendCommand(const char* cmd, unsigned long timeout) {
  esp8266.flush();
  while (esp8266.available()) esp8266.read(); // clear buffer

  Serial.print("Sending: ");
  Serial.println(cmd);

  esp8266.println(cmd);

  unsigned long start = millis();
  String response = "";

  while (millis() - start < timeout) {
    while (esp8266.available()) {
      char c = esp8266.read();
      response += c;
    }
    if (response.indexOf("OK") != -1) {
      Serial.print("Response: ");
      Serial.println(response);
      return true;
    }
    if (response.indexOf("ERROR") != -1) {
      Serial.print("Response: ");
      Serial.println(response);
      return false;
    }
  }
  Serial.println("Timeout waiting for response");
  return false;
}

// Wait for specific response
bool waitForResponse(const char* target, unsigned long timeout) {
  unsigned long start = millis();
  String response = "";
  while (millis() - start < timeout) {
    while (esp8266.available()) {
      char c = esp8266.read();
      response += c;
      if (response.indexOf(target) != -1) {
        Serial.println("Received target response");
        return true;
      }
    }
  }
  Serial.println("Timeout waiting for target response");
  return false;
}
