#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "2F_BACK";
const char* password = "0932576037";

int gettimmin() {
  String timeori = gettime();
  String hour1 = timeori.substring(11, 13);
  int hour = hour1.toInt();
  String minute1 = timeori.substring(14, 16);
  int minute = minute1.toInt();
  Serial.println(hour1);
  Serial.println(minute1);
  return hour * 60 + minute;
}

String gettime() {
  String payload;
  String datetime;
  int week;

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // 設定目標 URL
    http.begin(client, "http://worldtimeapi.org/api/timezone/Asia/Taipei");

    // 執行 GET 請求
    int httpCode = http.GET();

    // 檢查回應狀態碼
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  }

  // 處理 JSON 字串
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("Parsing failed: ");
    Serial.println(error.c_str());
    return datetime;
  }

  if (doc.containsKey("datetime")) {
    datetime = doc["datetime"].as<String>();
    Serial.print("Datetime: ");
    Serial.println(datetime);
  }
  if (doc.containsKey("day_of_week")) {
    week = doc["day_of_week"].as<int>();
    Serial.print("Week: ");
    Serial.println(week);
  }

  return datetime+"W"+String(week);
}
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
}

void loop() {
  int timmin = gettimmin();
  Serial.println(timmin);
  delay(6000);
}
