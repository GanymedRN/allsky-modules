
// Program to respond to Alpaca device discovery



#include <Arduino.h>

#include "WiFi.h"
#include "AsyncUDP.h"

#include "credentials.h"


//---------------
//  Globals
//---------------
unsigned const int localPort = 32227;   // The Alpaca Discovery test port
unsigned const int alpacaPort = 4567;   // The (fake) port that the Alpaca API would be available on
const char *ssid = _SSID;               // SSID
const char *password = _PASSWORD;       // network password

AsyncUDP udp;                           // UDP object
AsyncUDP txUdp;
IPAddress remote_ip_addr;
uint16_t  remote_udp_port = 11004;

//==============================================================================================
void setup()
{
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("WiFi Failed");
    while (1)
      delay(1000);
  }

  // Connect was successfull
  Serial.print("\nConnected to IP Address: ");
  Serial.println(WiFi.localIP());


  // Listen to UDP port 32227 for Alpaca broadcast message
  if (udp.listen(32227))
  {
    Serial.println("Listening for discovery requests...");
    udp.onPacket([](AsyncUDPPacket packet)
    {
      Serial.print("Received UDP Packet of Type: ");
      Serial.print(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast" : "Unicast");
      Serial.print(", From: ");
      remote_ip_addr = packet.remoteIP();
      Serial.print(packet.remoteIP());
      Serial.print(":");
      // remote_udp_port = packet.remotePort();
      Serial.print(packet.remotePort());
      Serial.print(", To: ");
      Serial.print(packet.localIP());
      Serial.print(":");
      Serial.print(packet.localPort());
      Serial.print(", Length: ");
      Serial.print(packet.length());
      Serial.print(", Data: ");
      Serial.write(packet.data(), packet.length());
      Serial.println();

      // No undersized packets allowed
      if (packet.length() < 16)
        return;

      //Compare packet to Alpaca Discovery string

      if (strncmp("alpacadiscovery1", (char *)packet.data(), 16) != 0)
        return;

      
      // send a reply, to the IP address and port that sent us the packet we received
      // on a real system this would be the port the Alpaca API was on
      // String myMessage = "{\"AlpacaPort\":" + String(alpacaPort) + "}";
      // char myMessage_c [64];
      // myMessage.toCharArray(myMessage_c,sizeof(myMessage_c) );
      // Serial.print("Respond with: ");
      // // Serial.println(myMessage_c);
      // Serial.println("my Test message");
      // // packet.printf(myMessage_c);
      packet.printf("my Test message");
      Serial.println("Sent: my Test message");

      // packet.printf("{\"Alpaca Discovery:\":%d}", alpacaPort);

      // txUdp.connect(remote_ip_addr, remote_udp_port);
      // txUdp.print("my Test");
      // txUdp.close();

      // packet.printf("{\"AlpacaPort\": %s}", String(alpacaPort));

      // String myMessage = "My Message";
      // char udp_tx_buf[64];
      // myMessage.toCharArray(udp_tx_buf, sizeof(udp_tx_buf));
      
      // Serial.println("\nResponse 1: ");
      // uint8_t resp_buf[32];
      // int resp_len = snprintf((char *)resp_buf, sizeof(resp_buf), "{\"AlpacaPort\":%d}", remote_ip_addr);
      // udp.writeTo(resp_buf, resp_len, remote_ip_addr, remote_udp_port);


      // Serial.print("\nResponding with \"");
      // Serial.print(myMessage);
      // Serial.print("\" to ");
      // Serial.print(remote_ip_addr);
      // Serial.print(":");
      // Serial.print(remote_udp_port);
      // Serial.println();

      // udp.connect(remote_ip_addr, remote_udp_port);
      // udp.print(udp_tx_buf);
      // udp.close();

      // delay(500);

      // udp.connect(remote_ip_addr, remote_udp_port);
      // udp.print(udp_tx_buf);
      // udp.close();


    });

  } // if (udp.listen(32227))




} // setup



//==============================================================================================
void loop()
{

} // loop



