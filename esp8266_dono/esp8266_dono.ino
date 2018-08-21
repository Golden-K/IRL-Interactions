#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

//Include the SSL client
#include <WiFiClientSecure.h>
int sizer;
char ssid[] = "hello";       // your network SSID (name)
char password[] = "byebyegoodbye";  // your network key

//Add a SSL client
WiFiClientSecure client;



long checkRedditDueTime;
int checkRedditDelay = 60000; // 60 x 1000 (1 minute)

void setup() {

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

}

String getDonos() {
  String title = "";
  String headers = "";
  String body = "";
  bool finishedHeaders = false;
  bool currentLineIsBlank = true;
  bool gotResponse = false;
  long now;
  int x = 0;

  char host[] = "streamlabs.com";

  if (client.connect(host, 443)) {
    Serial.println("connected");
    
    String URL = "/api/donations?access_token=40EF01F0CC32BC680005";

    Serial.println(URL);
    
    client.println("GET " + URL + " HTTP/1.1");
    client.print("Host: "); client.println(host);
    client.println("User-Agent: ");
    client.println("");

    now = millis();
    // checking the timeout
    while (millis() - now < 1500) {
      while (client.available()) {
        char c = client.read();
        //Serial.print(c);

        if (finishedHeaders) {
          body=body+c;
        } else {
          if (currentLineIsBlank && c == '\r') {
            finishedHeaders = true;
          }
          else {
            headers = headers + c;
          }
        }

        if (c == '6' ) {
          currentLineIsBlank = true;
        }else if (c != '\r') {
          currentLineIsBlank = false;
        }

        //marking we got a response
        gotResponse = true;

      }
      if (gotResponse) {
        Serial.print("I got a response!");
        Serial.print('\n');
        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(body);
        if (root.success()) {
          Serial.print ("YAY!!!");
          JsonArray& donations = root["donations"];
          //if (root.containsKey("donations")) {
            sizer = donations.size();
            Serial.println('\n');
            Serial.print("donations = ");
            Serial.println(sizer);
            //JsonObject& post = root["donations"]["children"][0];
            //if (post.containsKey("data")) { 
              //title = post["data"]["title"].as<String>();
            //}
          //} 
        } else {
          Serial.println("failed to parse JSON");
        }

        break;
      }
    }
  }

  //Serial.println(body);
  
  return body;
}

void loop() {
  long now = millis();
  if(now >= checkRedditDueTime) {
    Serial.println("---------");
    String body = getDonos();
    if(body != "") {
     // Serial.println("Most recent post on /r/" + subredditName);
      //Serial.println(body);
    } else {
     Serial.println("Error getting title."); 
    }
    Serial.println("---------");
    checkRedditDueTime = now + checkRedditDelay;
  }
}
