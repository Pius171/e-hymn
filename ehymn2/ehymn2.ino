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
// receive data from webpage form
// ota
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <FS.h>
#include <LittleFS.h>

#ifndef APSSID
#define APSSID "E-hymn"
#define APPSK  "thereisnospoon"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/


void handleRoot() {
   File file1 = LittleFS.open("/index.html", "r");
    if (!file1) {
      Serial.println("Failed to open file1 for reading");

    }
    else {
      server.streamFile(file1, "text/html");
      Serial.println("html file sent");
    }
  
}

void sendcss(){
    File file1 = LittleFS.open("/style.css", "r");
    if (!file1) {
      Serial.println("Failed to open file1 for reading");

    }
    else {
      server.streamFile(file1, "text/css");
      Serial.println(" css file sent");
    }
}

void sendJS(){
      File file1 = LittleFS.open("/index.js", "r");
    if (!file1) {
      Serial.println("Failed to open file1 for reading");

    }
    else {
      server.streamFile(file1, "application/javascript");
      Serial.println(" css file sent");
    }
}


void send_hymns(){
   File file1 = LittleFS.open("/hymns.json", "r");
    if (!file1) {
      Serial.println("Failed to open file1 for reading");

    }
    else {
      server.streamFile(file1, "application/json");
      Serial.println(" hymn.json file sent");
    }
}

void send_listJS(){
 File file1 = LittleFS.open("/list.min.js", "r");
    if (!file1) {
      Serial.println("Failed to open file1 for reading");

    }
    else {
      server.streamFile(file1, "application/javascript");
      Serial.println(" list.js file sent");
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
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
 

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/style.css", sendcss);
  server.on("/index.js", sendJS);
  server.on("/list.min.js", send_listJS);
  server.on("/hymns.json",send_hymns);

    // mount LITTLEFS
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }

  server.begin();
  Serial.println("HTTP server started");


  
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
