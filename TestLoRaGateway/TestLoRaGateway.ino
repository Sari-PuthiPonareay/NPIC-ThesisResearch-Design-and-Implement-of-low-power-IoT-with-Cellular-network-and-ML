#include <SPI.h>
#include <LoRa.h>

const long freq = 866E6;

const int csPin = 5;
const int rstPin = 25;
const int LPin = 26;

const int interval =  5000;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(csPin, rstPin, LPin);

  if (!LoRa.begin(freq)) {
  Serial.println("LoRa Fail");
  while(true);
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();
  Serial.println("LoRa Simple Gateway");
  Serial.println("Only receive messages from nodes");
  Serial.println("Tx: invertIQ enable");
  Serial.println("Rx: invertIQ disable");
  Serial.println();

  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();
}

void loop() {
  if (runEvery(interval)) { // repeat every 5000 millis

    String message = "HeLoRa World! ";
    message += "I'm a Gateway! ";
    message += millis();

    LoRa_sendMessage(message); // send a message

    Serial.println("Send Message!");
  }

}

void LoRa_rxMode(){
  LoRa.disableInvertIQ();               // normal mode
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.enableInvertIQ();                // active invert I and Q signals
}

void LoRa_sendMessage(String message) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

void onReceive(int packetSize) {
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }

  Serial.print("Gateway Receive: ");
  Serial.println(message);
}

void onTxDone() {
  Serial.println("TxDone");
  LoRa_rxMode();
}

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}