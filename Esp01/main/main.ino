#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Wi-Fi 連線資訊
const char* ssid = "2F_BACK";
const char* password = "0932576037";

// GitHub 上 JSON 檔案的網址
const char* jsonUrl = "https://raw.githubusercontent.com/your-username/your-repo/your-file.json";

//world time API
const char* timeurl = "http://worldtimeapi.org/api/timezone/Asia/Taipei";

// 控制器的 GPIO 腳位
const int motorPin1 = 2;  // 控制正轉
const int motorPin2 = 3;  // 控制反轉

bool isWatering = false;  // 是否正在澆水
int wateringStartTime;  // 澆水開始時間
int wateringDuration;  // 澆水持續時間

String getreservation() {
    // 建立 WiFiClient 物件，用於發送 HTTP 請求
  WiFiClient client;

  // 發送 GET 請求並取得回應
  HTTPClient http;
  http.begin(client, jsonUrl);
  int httpCode = http.GET();
  String jsonStr;

  if (httpCode == HTTP_CODE_OK) {
    // 讀取回應內容
    jsonStr = http.getString();

    
  }
  http.end();
  return jsonStr;
}

int gettimeweek() {
  String timeori = gettime();
  String week1="0"+timeori[33];
  int week=(week1).toInt();
  return(week);
}

int gettimemin() {
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

// 開始澆水
void startWatering() {
  digitalWrite(motorPin1, HIGH);  // 馬達正轉
  digitalWrite(motorPin2, LOW);

  wateringStartTime = millis() / 1000;
  isWatering = true;
}

// 停止澆水
void stopWatering() {
  digitalWrite(motorPin1, LOW);  // 馬達反轉
  digitalWrite(motorPin2, HIGH);

  isWatering = false;
}

void setup() {
  // 連接到 Wi-Fi
  WiFi.begin(ssid, password);

  // 等待連線成功
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // 連線成功
  Serial.println("Connected to WiFi");

  // 設定 GPIO 腳位為輸出模式
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
}

void loop() {
  int week=gettimeweek();
  int minute=gettimemin();
  String res1=getreservation();
  
  //解reservation json obj
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, res1);
  if(error){
    Serial.println("decode failed");
    return;
  }
  JsonArray reservations = doc["reservations"];
  for (JsonObject reservation : reservations) {
    int startTime = reservation["start_time"];
    wateringDuration = reservation["duration"];

    // 取得當前時間
    int currentTime = millis() / 1000;

    // 檢查是否應該開始澆水
    if (!isWatering && minute == startTime) {
      startWatering();
    }
    else if(!isWatering && minute>startTime && minute<(startTime+wateringDuration)){
      startWatering();
    }//若已經超過開始時間一樣開始澆水

    // 檢查是否應該停止澆水
    if (isWatering && currentTime - wateringStartTime >= wateringDuration) {
      stopWatering();
    }
  }


  // 每分鐘執行一次
  delay(60000);
}
