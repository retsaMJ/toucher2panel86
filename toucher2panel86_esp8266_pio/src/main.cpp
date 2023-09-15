/*#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h> // 引入Servo库

const char *ssid = "SIA028";     // WiFi网络名称
const char *password = ""; // WiFi密码

ESP8266WebServer server(80);
Servo myservo; // 创建Servo对象
int flag = 0; // 初始标志状态（0表示"Off"，1表示"Open"）

void handleRoot() {
  String content = "<html><body>";
  content += "<h1>Hello from ESP8266!</h1>";
  content += "<p>Last Action: " + String((flag == 0) ? "Off" : "Open") + "</p>";
  content += "<p>IP Address: " + WiFi.localIP().toString() + "</p>"; // 打印IP地址
  content += "<form action='/open' method='POST'>";
  content += "<input type='submit' name='action' value='Open'>";
  content += "</form>";
  content += "<form action='/off' method='POST'>";
  content += "<input type='submit' name='action' value='Off'>";
  content += "</form>";
  content += "</body></html>";
  server.send(200, "text/html", content);
}

void handleOpen() {
  myservo.write(150); // 将舵机旋转到正向60度
  delay(1000); // 等待1秒
  myservo.write(90); // 将舵机回到初始位置
  flag = 1; // 设置标志为1，表示"Open"
  server.send(200, "text/plain", "Open action executed.");
  Serial.println("Open action executed.");
}

void handleOff() {
  myservo.write(30); // 将舵机旋转到反向60度
  delay(1000); // 等待1秒
  myservo.write(90); // 将舵机回到初始位置
  flag = 0; // 设置标志为0，表示"Off"
  server.send(200, "text/plain", "Off action executed.");
  Serial.println("Off action executed.");
}

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // 打印IP地址

  myservo.attach(D2); // 将舵机连接到D2引脚
  myservo.write(90); // 初始化舵机位置
  flag = 0; // 初始化标志

  server.on("/", handleRoot);
  server.on("/open", HTTP_POST, handleOpen); // 处理"Open"按钮的POST请求
  server.on("/off", HTTP_POST, handleOff);   // 处理"Off"按钮的POST请求
  server.begin();
}

void loop() {
  server.handleClient();
}
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h> // 引入Servo库

const char *ssid = "SIA028";     // WiFi网络名称
const char *password = "PRECISION2020"; // WiFi密码

ESP8266WebServer server(80);
Servo myservo; // 创建Servo对象
int flag = 0; // 初始标志状态（0表示"Off"，1表示"Open"）

const int switchPin = D3; // 物理开关连接到D3引脚
int switchState = LOW;    // 开关状态
int lastSwitchState = LOW; // 上一次开关状态

void handleRoot() {
  String content = "<html><body>";
  content += "<h1>Hello from ESP8266!</h1>";
  content += "<p>Switch Status: " + String(digitalRead(switchPin) == HIGH ? "On" : "Off") + "</p>";
  content += "<p>Flag Status: " + String((flag == 0) ? "Off" : "Open") + "</p>";
  content += "<p>IP Address: " + WiFi.localIP().toString() + "</p>"; // 打印IP地址
  content += "<form action='/toggle' method='POST'>";
  content += "<input type='submit' name='action' value='" + String((flag == 0) ? "Open" : "Off") + "'>";
  content += "</form>";
  content += "</body></html>";
  server.send(200, "text/html", content);
}

void handleToggle() {
  if (flag == 0) {
    myservo.write(150); // 将舵机旋转到正向60度
    delay(1000); // 等待1秒
    myservo.write(90); // 将舵机回到初始位置
    flag = 1; // 设置标志为1，表示"Open"
  } else {
    myservo.write(30); // 将舵机旋转到反向60度
    delay(1000); // 等待1秒
    myservo.write(90); // 将舵机回到初始位置
    flag = 0; // 设置标志为0，表示"Off"
  }
  server.send(200, "text/plain", "Toggle action executed.");
  Serial.println("Toggle action executed.");
}

void setup() {
  Serial.begin(115200);
  
  pinMode(switchPin, INPUT_PULLUP); // 设置物理开关引脚为输入，启用上拉电阻
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // 打印IP地址

  myservo.attach(D2); // 将舵机连接到D2引脚
  myservo.write(90); // 初始化舵机位置
  flag = 0; // 初始化标志

  server.on("/", handleRoot);
  server.on("/toggle", HTTP_POST, handleToggle); // 处理按钮的POST请求
  server.begin();
}

void loop() {
  server.handleClient();

  // 读取物理开关状态
  switchState = digitalRead(switchPin);

  // 如果开关状态发生变化，切换标志
  if (switchState != lastSwitchState) {
    if (switchState == HIGH) {
      if (flag == 0) {
        myservo.write(150); // 将舵机旋转到正向60度
        delay(1000); // 等待1秒
        myservo.write(90); // 将舵机回到初始位置
        flag = 1; // 设置标志为1，表示"Open"
      } else {
        myservo.write(30); // 将舵机旋转到反向60度
        delay(1000); // 等待1秒
        myservo.write(90); // 将舵机回到初始位置
        flag = 0; // 设置标志为0，表示"Off"
      }
    }
    lastSwitchState = switchState;
  }
}
