#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "2F_BACK";
const char* password = "0932576037";

JsonObject gettime(){
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

  }
  //處理json字串
  String datetime;
  DynamicJsonDocument doc2(1024);
  JsonObject dt=doc2.to<JsonObject>();
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("Parsing failed: ");
    Serial.println(error.c_str());
    return dt;
  }
  if (doc.is<JsonObject>()) {
    JsonObject jsonObject = doc.as<JsonObject>();
    String datetime = jsonObject["datetime"];

    Serial.print("Datetime: ");
    Serial.println(datetime);
    
    
    String year1=datetime.substring(0,4);
    int year=year1.toInt();
    dt["year"]=year;

    String month1=datetime.substring(5,7);
    int month=month1.toInt();
    dt["month"]=month;

    String day1=datetime.substring(8,10);
    int day=day1.toInt();
    dt["day"]=day;

    String hour1=datetime.substring(11,13);
    int hour=hour1.toInt();
    Serial.println(hour);
    dt["hour"]=hour;

    String minute1=datetime.substring(14,16);
    int minute=minute1.toInt();
    dt["minute"]=minute;

    String second1=datetime.substring(17,19);
    int second=second1.toInt();
    dt["second"]=second;

    int week=jsonObject["day_of_week"];
    dt["week"]=week;
    
  }
  String hh;
  serializeJson(dt, hh);
  Serial.println(hh);
  return dt;
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
  JsonObject timenow=gettime();
  String hh;
  serializeJson(timenow, hh);
  Serial.println(hh);
  delay(6000);
}
