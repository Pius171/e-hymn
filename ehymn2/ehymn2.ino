/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Create a WiFi access point and provide a web server on it. */


//things to work on tomorrow
//mp3 player
// password change tomorro with input
//volume control
// convert image to icon file
// dfmp3.loop();
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <DFMiniMp3.h>


DynamicJsonDocument hymns(1024);
JsonObject obj = hymns.as<JsonObject>();
// forward declare the notify class, just the name
// I am not using the notify class, but it part of the DFMiniMp3 template
class Mp3Notify;
typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3;

// instance a DfMp3 object,
//
DfMp3 dfmp3(Serial1);

class Mp3Notify {
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action) {
    if (source & DfMp3_PlaySources_Sd) {
      Serial.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb) {
      Serial.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash) {
      Serial.print("Flash, ");
    }
    Serial.println(action);
  }
  static void OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode) {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }
  static void OnPlayFinished([[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track) {
    Serial.print("Play finished for #");
    Serial.println(track);
  }
  static void OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
    PrintlnSourceAction(source, "inserted");
  }
  static void OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
    PrintlnSourceAction(source, "removed");
  }
};


#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)

#else
#define debug(x)
#define debugln(x)
#endif


String ssid = "E-hymn";
String password = "";

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);  // make this random
DNSServer dnsServer;
ESP8266WebServer server(80);
const int BUSY_PIN = 5;



void server_handles() {

  server.on("/get_pass", []() {
    char buffer[100];
    sprintf(buffer, "{\"ssid\": \"%s\",\"pass\": \"%s\"}", ssid.c_str(), password.c_str());
    server.send(200, "text/plain", buffer);
  });

  server.on("/changedetails.html", []() {
    send_file("changedetails.html", "text/html");
  });

  server.on("/", []() {
    send_file("/index.html", "text/html");
  });

  // CSS file
  server.on("/style.css", []() {
    send_file("/style.css", "text/css");
  });


  server.on("/index.js", []() {
    send_file("/index.js", "application/javascript");
  });

  server.on("/hymns.json", []() {
    send_file("/hymns.json", "application/json");
  });

  server.on("/pius.png", []() {
    send_file("/pius.png", "img/png");
  });

  server.on("/list.min.js", []() {
    send_file("/list.min.js", "application/javascript");
  });
  server.on("/inputs.html", []() {
    send_file("/inputs.html", "text/html");
  });
  server.on("/pius.ico", []() {
    send_file("/pius.ico", "img/ico");
  });

  server.on("/input", []() {
    // gets input from input.html page
    String response = server.arg("plain");
    debug("input_response: ");
    debugln(response);
    if (response == "entrance") {
      debugln("playing entrance hymn");
      dfmp3.playMp3FolderTrack(atoi(hymns[response]));
    }

    if (response == "offertory") {
      debugln("playing offertory hymn");
      dfmp3.playMp3FolderTrack(atoi(hymns[response]));
    }

    if (response == "communion") {
      debugln("playing communion hymn");
      dfmp3.playMp3FolderTrack(atoi(hymns[response]));
    }

    if (response == "closing") {
      debugln("playing closing hymn");
      dfmp3.playMp3FolderTrack(atoi(hymns[response]));
    }

    if (response == "our father latin") {
      debugln("playing cour father in latin");
    }

    if (response == "our father english") {
      debugln("playing our father in english");
      //dfmp3.playMp3FolderTrack(hymn number);
    }
    if (response == "creed english") {
      debugln("playing the creed in english");
      //dfmp3.playMp3FolderTrack(hymn number);
    }
    if (response == "creed latin") {
      debugln("playing the creed in latin");
      //dfmp3.playMp3FolderTrack(hymn number);
    }

    if (response == "gloria english") {
      debugln("playing Gloria in english");
      //dfmp3.playMp3FolderTrack(hymn number);
    }
    if (response == "gloria latin") {
      debugln("playing Gloria in latin");
      //dfmp3.playMp3FolderTrack(hymn number);
    }
      int volume = atoi(response.c_str());
    if(volume!=0)// atoi returns 0 if string is not a number
    // so if it is a number then it must be the volume
    {
       dfmp3.setVolume(volume);      
    }
    bool NotBusy = digitalRead(BUSY_PIN);  // this represents the busy pin of the dfmini
    if (response == "play-pause") {
      if (NotBusy) {

        dfmp3.start();
        debugln("music resumed");
      }

      else {
        dfmp3.pause();
        debugln("music paused");
      }
    }
  });

  server.on("/hymns", []() {
    hymns["entrance"] = server.arg("entrance");
    hymns["offertory"] = server.arg("offertory");
    hymns["communion"] = server.arg("communion");
    hymns["closing"] = server.arg("closing");

    // save file to LITTLEFS
    String output;
    serializeJson(hymns, output);
    debug(output);
    save("/hymn_numbers.json",output);

  });

server.on("/details", []() {
   hymns["ssid"]=server.arg("ssid");
    hymns["pass"] = server.arg("new-pass");
    char buffer[100];
    sprintf(buffer, "new password is %s", password.c_str());

        String output;
    serializeJson(hymns, output);
    debug(output);
    save("/hymn_numbers.json",output);
    ESP.restart();
  });
}



void send_file(const char* filename, String content_type) {
  File file1 = LittleFS.open(filename, "r");
  if (!file1) {
    char buffer[100];
    sprintf(buffer, "failed to open %s for reading", filename);
    debugln(buffer);

  } else {
    server.streamFile(file1, content_type);
    char buffer[100];
    sprintf(buffer, "%s file sent", filename);
    debugln(buffer);
  }
}

void save(const char* filename, String message){
      String path = filename;

    Serial.printf("Writing file: %s\n", path);

    File file = LittleFS.open(path, "w");
    if (!file) {
      char buffer[100];
      sprintf(buffer,"failed to open %s for writing",filename);
      debugln(buffer);
      return;
    }
    if (file.print(message)) {
      char buffer[100];
      sprintf(buffer,"%s written",filename);
      debugln(buffer);
      
    } else {
      char buffer[100];
      sprintf(buffer,"%s write failed",filename);
      debugln(buffer);
    }
    file.close();
}



void setup() {
  delay(1000);
  Serial.begin(115200);
  dfmp3.begin();
  pinMode(BUSY_PIN, INPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  // mount LITTLEFS
  if (!LittleFS.begin()) {
    debugln("LittleFS mount failed");
    return;
  }

  String path = "/hymn_numbers.json";
  String hymn_numbers = "";

  debug("reading file ");
  debugln(path);

  File file = LittleFS.open(path, "r");
  if (!file) {
    debugln("Failed to open file for reading");
    debugln("this is probally first usage, so the file does not exist");

  } else {
    debug("Read from file: ");
    while (file.available()) {
      hymn_numbers += file.readString();
    }
    debugln(hymn_numbers);
    deserializeJson(hymns, hymn_numbers);  // convert the string to json object
    file.close();
  }

  ssid = (hymns.containsKey("ssid")) ? hymns["ssid"] + "_E-hymn" : "E-hymn";
  password = (hymns.containsKey("pass")) ? hymns["pass"]+"" : "12345678";
  
  debugln(ssid);
  debugln(password);
  WiFi.softAP(ssid, password);

  // modify TTL associated  with the domain name (in seconds)
  // default is 60 seconds
  dnsServer.setTTL(300);
  // set which return code will be used for all other domains (e.g. sending
  // ServerFailure instead of NonExistentDomain will reduce number of queries
  // sent by clients)
  // default is DNSReplyCode::NonExistentDomain
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);

  // start DNS server for a specific domain name
  dnsServer.start(DNS_PORT, "www.ehymn.com", apIP);
  debugln();
  debug("Configuring access point...");



  IPAddress myIP = WiFi.softAPIP();
  debug("AP IP address: ");
  debugln(myIP);
  server_handles();
  server.begin();
  debugln("HTTP server started");
}

void loop() {
  dnsServer.processNextRequest();

  server.handleClient();
}