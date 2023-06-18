#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// Wi-Fi 連線資訊
const char* ssid = "2F_BACK";
const char* password = "0932576037";

// NTP 伺服器設定
const char* ntpServer = "pool.ntp.org";
const int timeZone = 8;  // 時區設定，以 GMT+8 為例

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, timeZone * 3600);

void setup() {
  Serial.begin(115200);
  // 連接到 Wi-Fi
  WiFi.begin(ssid, password);

  // 等待連線成功
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // 連線成功
  Serial.println("Connected to WiFi");

  // 初始化 NTP 客戶端
  timeClient.begin();
  timeClient.update();
}

void loop() {
  // 更新時間
  timeClient.update();

  // 獲取日期和時間
  unsigned long epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  // 獲取當前星期幾
  int currentWeekday = timeClient.getDay();

  // 輸出到序列埠
  Serial.print("Current Weekday: ");
  Serial.println(currentWeekday);
  Serial.print("Current Time: ");
  Serial.println(formattedTime);

  //将epochTime换算成年月日
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  Serial.print("Month day:");
  Serial.println(monthDay);
  int currentMonth = ptm->tm_mon + 1;
  Serial.print("Month:");
  Serial.println(currentMonth);
  //https://blog.csdn.net/veray/article/details/121940328


  // 在這裡進行預約時間和澆水持續時間的比較與澆水控制

  delay(60000);  // 每分鐘更新一次時間
}
