#include <WiFi.h>
#include <Wire.h>
#include "esp_camera.h"

const char* ssid = "POTS2.4";
const char* password = "POTS@12340987";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  while(!Serial){};

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.begin();
  Serial.println("Server started");
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  while(!client.available()){
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  String s = "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\n\r\n";
  client.print(s);

  camera_fb_t *fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  client.write((uint8_t *)fb->buf, fb->len);
  esp_camera_fb_return(fb);
  Serial.println("Camera captured");
}
