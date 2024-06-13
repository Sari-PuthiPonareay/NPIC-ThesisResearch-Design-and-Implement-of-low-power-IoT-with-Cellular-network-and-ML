#include <SPI.h>
#include <LoRa.h>

const long freq = 866E6;

const int csPin = 10;
const int rstPin = 9;
const int Lpin = 2;


void setup() {

  Serial.begin(9600);
  while (!Serial);

  LoRa.setPins(csPin, rstPin, Lpin);

  if (!LoRa.begin(freq)){
    Serial.print("LoRa Fail");
    while (true);
  }

  Serial.println("LoRa Success");
  Serial.println();
  Serial.println("LoRa Node");
  Serial.println("Only receive message for Gateways");
  Serial.println("TX: inverIQ disable");
  Serial.println("RX: invertIQ enable");
  Serial.println();

  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();
}

void loop() {
  if (runEvery(1000)) {
  
  String message = "Hello World ";
  message += "Im a Node";
  message += millis();

  LoRa_sendMessage(message);
  
  Serial.println("Send Message!");
  }

}


void LoRa_rxMode(){
  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
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

  Serial.print("Node Receive: ");
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