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
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>


DynamicJsonDocument hymns(200);
JsonObject obj = hymns.as<JsonObject>();

#ifndef APSSID
#define APSSID "E-hymn"
#define APPSK "thereisnospoon"
#endif

#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)

#else
#define debug(x)
#define debugln(x)
#endif

/* Set these to your desired hymn_numbers. */
const char *ssid = APSSID;
const char *password = APPSK;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer server(80);




void send_html() {
  File file1 = LittleFS.open("/index.html", "r");
  if (!file1) {
    debugln("Failed to open file1 for reading");

  } else {
    server.streamFile(file1, "text/html");
    debugln("html file sent");
  }
}

void sendcss() {
  File file1 = LittleFS.open("/style.css", "r");
  if (!file1) {
    debugln("Failed to open file1 for reading");

  } else {
    server.streamFile(file1, "text/css");
    debugln(" css file sent");
  }
}

void sendJS() {
  File file1 = LittleFS.open("/index.js", "r");
  if (!file1) {
    debugln("Failed to open file1 for reading");

  } else {
    server.streamFile(file1, "application/javascript");
    debugln(" index.js file sent");
  }
}


void send_hymns() {
  File file1 = LittleFS.open("/hymns.json", "r");
  if (!file1) {
    debugln("Failed to open file1 for reading");

  } else {
    server.streamFile(file1, "application/json");
    debugln(" hymn.json file sent");
  }
}

void send_listJS() {
  File file1 = LittleFS.open("/list.min.js", "r");
  if (!file1) {
    debugln("Failed to open file1 for reading");

  } else {
    server.streamFile(file1, "application/javascript");
    debugln(" list.js file sent");
  }
}

void send_logo() {
  File file1 = LittleFS.open("/pius.png", "r");
  if (!file1) {
    debugln("Failed to open pius.png for reading");

  } else {
    server.streamFile(file1, "image/png");
    debugln(" pius.png file sent");
  }
}

void input() {
  // gets input from input.html page
  String response = server.arg("plain");
  debug("input_response: ");
  debugln(response);
  if (response == "entrance") {
    debugln("playing entrance hymn");
  }

  if (response == "offertory") {
    debugln("playing offertory hymn");
  }

  if (response == "communion") {
    debugln("playing communion hymn");
  }

  if (response == "closing") {
    debugln("playing closing hymn");
  }

  if (response == "our father latin") {
    debugln("playing cour father in latin");
  }

  if (response == "our father english") {
    debugln("playing our father in english");
  }
  if (response == "creed english") {
    debugln("playing the creed in english");
  }
  if (response == "creed latin") {
    debugln("playing the creed in latin");
  }

  if (response == "gloria english") {
    debugln("playing Gloria in english");
  }
  if (response == "gloria latin") {
    debugln("playing Gloria in latin");
  }
  int NotBusy = 0; // this represents the busy pin of the dfmini
  if (response == "play-pause") {
    if (NotBusy) {
      debugln("resume music");
    }

    else {
      debugln("pause music");
    }
  }
}  
  void get_data() {
    hymns["entrance"] = server.arg("entrance");
    hymns["offertory"] = server.arg("offertory");
    hymns["communion"] = server.arg("communion");
    hymns["closing"] = server.arg("closing");

    // save file to LITTLEFS
    String output;
    serializeJson(hymns, output);
    debug(output);

    String path = "/hymn_numbers.json";

    Serial.printf("Writing file: %s\n", path);

    File file = LittleFS.open(path, "w");
    if (!file) {
      debugln("Failed to open file for writing");
      return;
    }
    if (file.print(output)) {
      debugln("File written");
    } else {
      debugln("Write failed");
    }
    file.close();
    return;
  }

  void send_inputs_page() {
    File file1 = LittleFS.open("/inputs.html", "r");
    if (!file1) {
      debugln("Failed to open inputs.html for reading");

    } else {
      server.streamFile(file1, "text/html");
      debugln("inputs.html file sent");
    }
  }

  void setup() {
    delay(1000);
    Serial.begin(115200);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
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
    /* You can remove the password parameter if you want the AP to be open. */


    IPAddress myIP = WiFi.softAPIP();
    debug("AP IP address: ");
    debugln(myIP);
    server.on("/", send_html);
    server.on("/style.css", sendcss);
    server.on("/index.js", sendJS);
    server.on("/list.min.js", send_listJS);
    server.on("/hymns.json", send_hymns);
    server.on("/pius.png", send_logo);
    server.on("/hymns", get_data);  // get the hymn numbers inputed in the webpage
    server.on("/input", input);
    server.on("/inputs.html", send_inputs_page);


    // mount LITTLEFS
    if (!LittleFS.begin()) {
      debugln("LittleFS mount failed");
      return;
    }

    server.begin();
    debugln("HTTP server started");

    String path = "/hymn_numbers.json";
    String hymn_numbers = "";

    debug("reading file ");
    debugln(path);

    File file = LittleFS.open(path, "r");
    if (!file) {
      debugln("Failed to open file for reading");
      debugln("this is probally first usage, so the file does not exist");
      return;
    }

    debug("Read from file: ");
    while (file.available()) {
      hymn_numbers += file.readString();
    }
    debugln(hymn_numbers);
    deserializeJson(hymns, hymn_numbers);  // convert the string to json object
    file.close();
  }

  void loop() {
    dnsServer.processNextRequest();

    server.handleClient();
  }