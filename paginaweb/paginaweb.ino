#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal.h> 

// Iniciar el display
#define RS 25
#define EN 26
#define D4 27
#define D5 14
#define D6 19
#define D7 18

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
const char* ssid = "Nombre de la red";
const char* password = "Password";

WebServer server(80);

// Variables para el estado de los parqueos
bool parqueo_1 = false;
bool parqueo_2 = false;
bool parqueo_3 = false;
bool parqueo_4 = false;
bool parqueo_5 = false;
bool parqueo_6 = false;
bool parqueo_7 = false;
bool parqueo_8 = false;

// Recibir datos de la Tiva C
HardwareSerial SerialTIVA1(1);
HardwareSerial SerialTIVA2(2);

// Código HTML
String htmlResponse = "<!DOCTYPE html> <html>\n";
 "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
   "<meta charset=\"UTF-8\">\n";
  "<title>Parqueo App</title>\n";
  "  <style>\n";
  "    /* CSS Styling */\n";
  "    body {\n";
  "      font-family: 'Roboto', sans-serif;\n";
  "      background-color: #f2f2f2;\n";
  "      background-image: linear-gradient(to bottom right, #b491e3, #7579ff);\n";
  "      color: #333;\n";
  "      margin: 0;\n";
  "      padding: 20px;\n";
  "      display: flex;\n";
  "      align-items: center;\n";
  "      justify-content: center;\n";
  "      height: 100vh;\n";
  "    }\n";
  "\n";
  "    h1 {\n";
  "      text-align: center;\n";
  "      color: #8a00b8;\n";
  "      font-size: 48px;\n";
  "      margin-bottom: 30px;\n";
  "      text-transform: uppercase;\n";
  "      letter-spacing: 2px;\n";
  "    }\n";
  "\n";
  "    .parking-lot {\n";
  "      display: flex;\n";
  "      flex-wrap: wrap;\n";
  "      justify-content: center;\n";
  "      margin-top: 50px;\n";
  "    }\n";
  "\n";
  "    .parking-spot {\n";
  "      width: 200px;\n";
  "      height: 200px;\n";
  "      border-radius: 10px;\n";
  "      margin: 10px;\n";
  "      display: flex;\n";
  "      align-items: center;\n";
  "      justify-content: center;\n";
  "      font-size: 24px;\n";
  "      font-weight: bold;\n";
  "      cursor: pointer;\n";
  "      box-shadow: 0 0 10px rgba(0, 0, 0, 0.2);\n";
  "      transition: background-color 0.3s ease;\n";
  "    }\n";
  "\n";
  "    .vacant {\n";
  "      background-color: #99ccff;\n";
  "      color: #fff;\n";
  "    }\n";
  "\n";
  "    .occupied {\n";
  "      background-color: #ff6666;\n";
  "      color: #fff;\n";
  "    }\n";
  "  </style>\n";
  "</head>\n";
  "<body>\n";
  "<h1>Parqueo App</h1>\n";
  "  <div class=\"parking-lot\">\n";
  "<div id=\"spot1\" class=\"parking-spot\">PARQUEO 1</div>\n";
  "<div id=\"spot2\" class=\"parking-spot\">PARQUEO 2</div>\n";
  "<div id=\"spot3\" class=\"parking-spot\">PARQUEO 3</div>\n";
  "<div id=\"spot4\" class=\"parking-spot\">PARQUEO 4</div>\n";
  "<div id=\"spot5\" class=\"parking-spot\">PARQUEO 5</div>\n";
  "<div id=\"spot6\" class=\"parking-spot\">PARQUEO 6</div>\n";
  "<div id=\"spot7\" class=\"parking-spot\">PARQUEO 7</div>\n";
  "<div id=\"spot8\" class=\"parking-spot\">PARQUEO 8</div>\n";
  "  </div>\n";
  "\n";
  "function updateColor() {"
"  var xhttp = new XMLHttpRequest();"
"  xhttp.onreadystatechange = function() {"
"    if (this.readyState == 4 && this.status == 200) {"
"      document.getElementById('circle').className = this.responseText;"
"    }"
"  };"
"  xhttp.open('GET', '/color', true);"
"  xhttp.send();"
"  setTimeout(updateColor, 1000);"
"}"
"updateColor();"
"</script>"
"</head>"
"<body>"
"<div id='circle' class='circle'></div>"
"</body>"
"</html>"
}

void handleRoot() {
  server.send(200, "text/html", htmlResponse);
}

void handleParqueo1() {
  if (parqueo_1) {
    server.send(200, "text/plain", "occupied");
  } else {
    server.send(200, "text/plain", "available");
  }
}

void handleParqueo2() {
  if (parqueo_2) {
    server.send(200, "text/plain", "occupied");
  } else {
    server.send(200, "text/plain", "available");
  }
}

void handleParqueo3() {
  if (parqueo_3) {
    server.send(200, "text/plain", "occupied");
  } else {
    server.send(200, "text/plain", "available");
  }
}

void handleParqueo4() {
  if (parqueo_4) {
    server.send(200, "text/plain", "occupied");
  } else {
    server.send(200, "text/plain", "available");
  }
}

void handleParqueo5() {
  if (parqueo_5) {
    server.send(200, "text/plain", "occupied");
  } else {
    server.send(200, "text/plain", "available");
  }
}

void handleParqueo6() {
  if (parqueo_6) {
    server.send(200, "text/plain", "occupied");
  } else {
    server.send(200, "text/plain", "available");
  }
}

void handleParqueo7() {
  if (parqueo_7) {
    server.send(200, "text/plain", "occupied");
  } else {
    server.send(200, "text/plain", "available");
  }
}

void handleParqueo8() {
  if (parqueo_8) {
    server.send(200, "text/plain", "occupied");
  } else {
    server.send(200, "text/plain", "available");
  }
}

void setup() {
  
  const char* ssid = "Nombre de la red";
  const char* password = "Password";
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/parqueo1", handleParqueo1);
  server.on("/parqueo2", handleParqueo2);
  server.on("/parqueo3", handleParqueo3);
  server.on("/parqueo4", handleParqueo4);
  server.on("/parqueo5", handleParqueo5);
  server.on("/parqueo6", handleParqueo6);
  server.on("/parqueo7", handleParqueo7);
  server.on("/parqueo8", handleParqueo8);

  server.begin();
}

void loop() {
  server.handleClient();
  while (SerialTIVA1.available()) {
    String inData = SerialTIVA1.readStringUntil('\n');
    if (inData[0] == 'P') {
      int parqueoNum = inData[1] - '0';
      bool parqueoState = (inData[2] - '0') == 1;
      switch (parqueoNum) {
        case 1:
          parqueo_1 = parqueoState;
          break;
        case 2:
          parqueo_2 = parqueoState;
          break;
        case 3:
          parqueo_3 = parqueoState;
          break;
        case 4:
          parqueo_4 = parqueoState;
          break;
        case 5:
          parqueo_5 = parqueoState;
          break;
        case 6:
          parqueo_6 = parqueoState;
          break;
        case 7:
          parqueo_7 = parqueoState;
          break;
        case 8:
          parqueo_8 = parqueoState;
          break;
      }
    }
  }
  int occupiedCount = parqueo_1 + parqueo_2 + parqueo_3 + parqueo_4 + parqueo_5 + parqueo_6 + parqueo_7 + parqueo_8;
  int availableCount = 8 - occupiedCount;

  lcd.setCursor(9, 0); 
  lcd.print(String("PARQUEOS"));  
  lcd.setCursor(3,1);
  lcd.print(String(availableCount)); 
}