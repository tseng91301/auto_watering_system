#include <TimeLib.h>

void loop() {
  // 更新時間
  timeClient.update();

  // 獲取日期和時間
  unsigned long currentTime = timeClient.getEpochTime();

  // 獲取星期幾
  int currentWeekday = weekday(currentTime);

  // 格式化日期字串
  String formattedDate = String(day(currentTime)) + "/" + String(month(currentTime)) + "/" + String(year(currentTime)) + " (";
  
  switch (currentWeekday) {
    case 1:
      formattedDate += "Sunday";
      break;
    case 2:
      formattedDate += "Monday";
      break;
    case 3:
      formattedDate += "Tuesday";
      break;
    case 4:
      formattedDate += "Wednesday";
      break;
    case 5:
      formattedDate += "Thursday";
      break;
    case 6:
      formattedDate += "Friday";
      break;
    case 7:
      formattedDate += "Saturday";
      break;
    default:
      formattedDate += "Unknown";
      break;
  }
  
  formattedDate += ")";

  // 輸出到序列埠
  Serial.print("Current Time: ");
  Serial.println(currentTime);
  Serial.print("Formatted Date: ");
  Serial.println(formattedDate);

  // ... 其他程式碼

  delay(60000);  // 每分鐘執行一次
}
