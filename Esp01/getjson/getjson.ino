#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "2F_BACK";
const char* password = "0932576037";

String gettime(){
  String payload;
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // 設定目標 URL
    http.begin(client,"http://worldtimeapi.org/api/timezone/Asia/Taipei");

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

    delay(6000);  // 延遲一分鐘再次發送請求
  }
  //處理json字串
  String datetime;
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("Parsing failed: ");
    Serial.println(error.c_str());
    return "E";
  }
  if (doc.is<JsonObject>()) {
    JsonObject jsonObject = doc.as<JsonObject>();
    String datetime = jsonObject["datetime"];

    Serial.print("Datetime: ");
    Serial.println(datetime);
    
  }
  return datetime;
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
  gettime();
  delay(6000);
}
