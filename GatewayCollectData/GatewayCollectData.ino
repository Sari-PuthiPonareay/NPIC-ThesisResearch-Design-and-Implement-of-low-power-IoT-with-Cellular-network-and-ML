#include <SPI.h>
#include <LoRa.h>

#define ss 15
#define rst 2
#define dio0 4

byte MasterNode = 0xFF;
byte Node1 = 0xBB;
byte Node2 = 0xCC;
byte Node3 = 0xDD;

String SenderNode = "";
String outgoing;

byte msgCount = 0;
String incoming = "";
unsigned long freq = 433E6;  //set Frequency
unsigned long previousMillis = 0;
unsigned long int previoussecs = 0;
unsigned long int currentsecs = 0;
unsigned long currentMillis = 0;
int interval = 1;
int Secs = 0;

//Node1
float TDS;
float Turbidity;
float Temp;
float pH;

//Node2
float TDS1;
float Turbidity1;
float Temp1;
float pH1;

//Node3
float TDS2;
float Turbidity2;
float Temp2;
float pH2;


void setup() {

  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Master Node");

  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(freq)) {
    Serial.println("Starting LoRa Fail!!!!!");
    while (1);
  }
}

void loop() {

  currentMillis = millis();
  currentsecs = currentMillis / 1000;
  if ((unsigned long)(currentsecs - previoussecs) >= interval) {
    Secs = Secs + 1;
    Serial.println(Secs);
    if (Secs >= 16) {
      Secs = 0;
    }

    //Node1
    if ((Secs >= 1) && (Secs <= 5)) {
      String message = "10";
      sendMessage(message, MasterNode, Node1);
    }

    //Node2
    if ((Secs >= 6) && (Secs <= 10)) {
      String message = "20";
      sendMessage(message, MasterNode, Node2);
    }

    //Node3
    if ((Secs >= 11) && (Secs <= 15)) {
      String message = "30";
      sendMessage(message, MasterNode, Node3);
    }

    previoussecs = currentsecs;
  }

  onReceive(LoRa.parsePacket());
}

void sendMessage(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();
  LoRa.write(otherNode);
  LoRa.write(MasterNode);
  LoRa.write(msgCount);
  LoRa.write(outgoing.length());
  LoRa.print(outgoing);
  LoRa.endPacket();
  msgCount++;
}

void onReceive(int packetSize) {
  if (packetSize == 0)
    return;

  int recipient = LoRa.read();
  byte sender = LoRa.read();
  if (sender == 0xBB)
    SenderNode = "Node1: ";
  if (sender == 0xCC)
    SenderNode = "Node2: ";
  if (sender == 0xDD)
    SenderNode == "Node3: ";
  byte incomingMsgId = LoRa.read();
  byte incomingLength = LoRa.read();

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {
    ;
    return;
  }

  if (recipient != Node1 && recipient != MasterNode) {
    ;
    return;
  }

  //Read Data from Node1
  if (sender = 0xBB) {
    String t1 = getValue(incoming, ',', 0);  //TDS
    String t2 = getValue(incoming, ',', 1);  //Temp
    String t3 = getValue(incoming, ',', 2);  //Turbidity
    String p1 = getValue(incoming, ',', 3);  //pH

    TDS = t1.toInt();
    Temp = t2.toInt();
    Turbidity = t3.toInt();
    pH = p1.toInt();
    incoming = "";

    //Print TDS Value
    Serial.print("TDS1:");
    Serial.print(TDS);
    Serial.println(" ppm");

    //Pint Temp Value
    Serial.print("Temp1: ");
    Serial.print(Temp);
    Serial.println(" C");

    //Print Turbidity Value
    Serial.print("Tubidity1: ");
    Serial.println(Turbidity);

    //Print pH
    Serial.print("pH1: ");
    Serial.println(pH);
  }

  //Read Data from Node2
  if (sender == 0xCC) {
    String t4 = getValue(incoming, ',', 0);  // TDS1
    String t5 = getValue(incoming, ',', 1);  // Temp1
    String t6 = getValue(incoming, ',', 2);  // Turbidity1
    String p2 = getValue(incoming, ',', 3);  // pH1

    TDS1 = t4.toInt();
    Temp1 = t5.toInt();
    Turbidity1 = t6.toInt();
    pH1 = p2.toInt();
    incoming = "";

    //Print TDS Value
    Serial.print("TDS2:");
    Serial.print(TDS1);
    Serial.println(" ppm");

    //Pint Temp Value
    Serial.print("Temp2: ");
    Serial.print(Temp1);
    Serial.println(" C");

    //Print Turbidity Value
    Serial.print("Tubidity2: ");
    Serial.println(Turbidity1);

    //Print pH
    Serial.print("pH2: ");
    Serial.println(pH1);
  }

  if (sender == 0xDD) {
    String t7 = getValue(incoming, ',', 0);  // TDS2
    String t8 = getValue(incoming, ',', 1);  // Temp2
    String t9 = getValue(incoming, ',', 2);  // Turbidity2
    String p3 = getValue(incoming, ',', 3);  // pH2

    TDS2 = t7.toInt();
    Temp2 = t8.toInt();
    Turbidity2 = t9.toInt();
    pH2 = p3.toInt();
    incoming = "";

    //Print TDS Value
    Serial.print("TDS3:");
    Serial.print(TDS2);
    Serial.println(" ppm");

    //Pint Temp Value
    Serial.print("Temp3: ");
    Serial.print(Temp2);
    Serial.println(" C");

    //Print Turbidity Value
    Serial.print("Tubidity3: ");
    Serial.println(Turbidity2);

    //Print pH
    Serial.print("pH3: ");
    Serial.println(pH2);
  }
  Serial.println("Not Receive anything");
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}