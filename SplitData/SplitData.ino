String readString;
String data;
String v;
String i;
String w;
String h;
int ind1;  // , locations
int ind2;
int ind3;
int ind4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("wait");
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();  //gets one byte from serial buffer
    if (c == '*') {

      ind1 = readString.indexOf(',');
      v = readString.substring(0, ind1);
      ind2 = readString.indexOf(',', ind1 + 1);  //finds location of second ,
      i = readString.substring(ind1 + 1, ind2 + 1);
      ind3 = readString.indexOf(',', ind2 + 1);
      w = readString.substring(ind2 + 1, ind3 + 1);
      ind4 = readString.indexOf(',', ind3 + 1);
      h = readString.substring(ind3 + 1);  //captures remain part of data after last ,
      Serial.print("volt ");
      Serial.println(v);
      Serial.print("amp: ");
      Serial.println(i);
      Serial.print("power: ");
      Serial.println(w);
      Serial.print("fre: ");
      Serial.println(h);





      readString = "";
    } else {

      readString += c;
    }  //makes the string readString
  }
}