//#include <Wire.h>
//pi,raspberry
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <LiquidCrystal.h>



const int rs = 4, en = 5, d4 = 6, d5 = 7, d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

TinyGPS gps;

String ssid ="EDDY-RPi";
String password="IEEEosaSPIE";

SoftwareSerial softSerial(2, 3); // RX, TX

String data;
String server = "192.168.42.1";
String uri = "/add.php";

float speed = 0.0;
float flat = 0.0;
float flon = 0.0;
unsigned long age;

float bat1 = 0;
/*float bat2 = 0;
float bat3 = 0;
float bat4 = 0;
*/
//************* config multiplexor
// 2^3  2^2  2^1  2^0 
// s3 | s2 | s1 | s0 | N
// 0    0   0     0     0
// 0    0   0     1     1
// 0    0   1     0     2
// 0    0   1     1     3
// 0    1   0     0     4
// 0    0   1     1     5 

#define BAT_MUX_CHANNEL 0
#define AMP1_MUX_CHANNEL 1
const int sensA0 = A0;
const int muxS0 = 9;
const int muxS1 = 10;
const int muxS2 = 11;
const int muxS3 = 12;
int sensA1 = A1;
int sensA2 = A2;
int sensA3 = A3;

int sensA4 = A4;
int sensA5 = A5;
int sensA6 = A6;
int sensA7 = A7;
float amp1 = 0;
float amp2 = 0;
float amp3 = 0;
float amp4 = 0;
float amp5 = 0;
float amp6 = 0;
float amp7 = 0;
float amp8 = 0;

int mVperAmp = 66.00;
int ACSoffset = 2500.00;


int SetMuxChannel(byte channel){
  digitalWrite(muxS0, bitRead(channel, 0));
  digitalWrite(muxS1, bitRead(channel, 1));
  digitalWrite(muxS2, bitRead(channel, 2));
  digitalWrite(muxS3, bitRead(channel, 3));
}

int readMux(byte channel)
{
  // seleccionar el canal para leer
  SetMuxChannel(channel);
  delay(1);
  return analogRead(sensA0);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);

  softSerial.begin(9600);
  pinMode(muxS0, OUTPUT);
  pinMode(muxS1, OUTPUT);
  pinMode(muxS2, OUTPUT);
  pinMode(muxS3, OUTPUT);
  reset();
  connectWifi();
}

void reset() {
  softSerial.println("AT+RST");
  delay(1000);
   lcd.setCursor(0, 0);
  if(softSerial.find("OK")) lcd.print("Module Reset    ");
}

void connectWifi() {
  String cmd = "AT+CWJAP=\"" +ssid+"\",\"" + password + "\"";
  softSerial.println(cmd);
  delay(5000);
  if(softSerial.find("OK")) {
     lcd.setCursor(0, 0);
    lcd.print("Connected!      ");
  }else{
    lcd.setCursor(0, 0);
    lcd.print("Cannot Connect  ");
    connectWifi();
  }
}


void loop () {
  bool newData = false;
  for (unsigned long start = millis(); millis() - start < 100;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      if (gps.encode(c)) // Nueva secuencia recibida
      newData = true;
    }
  }

  if (newData) {
    speed = gps.f_speed_kmph();
    gps.f_get_position(&flat, &flon, &age);
  }
  
  bat1 = (float)readMux(BAT_MUX_CHANNEL) * 60.00 / 1024.00;
  amp1 = (((float)readMux(AMP1_MUX_CHANNEL) * 5000.00 / 1024.00) - ACSoffset) / mVperAmp;
  amp2 = (((float)analogRead(sensA1) * 5000.00 / 1024.00) - ACSoffset) / mVperAmp;
  amp3 = (((float)analogRead(sensA2) * 5000.00 / 1024.00) - ACSoffset) / mVperAmp;
  amp4 = (((float)analogRead(sensA3) * 5000.00 / 1024.00) - ACSoffset) / mVperAmp;
  amp5 = (((float)analogRead(sensA4) * 5000.00 / 1024.00) - ACSoffset) / mVperAmp;
  amp6 = (((float)analogRead(sensA5) * 5000.00 / 1024.00) - ACSoffset) / mVperAmp;
  amp7 = (((float)analogRead(sensA6) * 5000.00 / 1024.00) - ACSoffset) / mVperAmp;
  amp8 = (((float)analogRead(sensA7) * 5000.00 / 1024.00) - ACSoffset) / mVperAmp;
  //lcd.setCursor(0, 0);
  //lcd.print((int)bat1); lcd.print("% ");

  //lcd.setCursor(0, 1);
  //lcd.print((int)speed); lcd.print(" Km/h ");
  // data sent must be under this form //name1=value1&name2=value2.
  data = "batV1=" + String(bat1,2) + 
+
  "&batA1=" + String(amp1,2) +
  "&batA2=" + String(amp2,2) +
  "&batA3=" + String(amp3,2) +
  "&batA4=" + String(amp4,2) +
  "&batA5=" + String(amp1,2) +
  "&batA6=" + String(amp2,2) +
  "&batA7=" + String(amp3,2) +
  "&batA8=" + String(amp4,2) +
  "&vel=" + String(speed,2) +
  "&flat=" + String(flat,6) +
  "&flon=" + String(flon,6);
  // Serial.println(data);
  httppost();
  delay(200);
}

void httppost () {
  softSerial.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.
  if(softSerial.find("OK")) {
    // Serial.println("TCP connection ready");
    digitalWrite(LED_BUILTIN, HIGH);
  }
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);

  String postRequest =
  "POST " + uri + " HTTP/1.0\r\n" +
  "Host: " + server + "\r\n" +
  //"Accept: *" + "/" + "*\r\n" +
  //"Content-Length: " + data.length() + "\r\n" +
  "Content-Type: application/x-www-form-urlencoded\r\n" +
  "Content-Length: " + data.length() + "\r\n" +
  "\r\n" + data;
  
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
  softSerial.print(sendCmd);
  softSerial.println(postRequest.length());
  delay(250);
  
  if(softSerial.find(">")) {
    // Serial.println("Sending..");
    softSerial.print(postRequest);
    if(softSerial.find("SEND OK")) {
      // Serial.println("Packet sent");
      while (softSerial.available()) {
        String tmpResp = softSerial.readString();
        // Serial.println(tmpResp);
      }
      // close the connection
      softSerial.println("AT+CIPCLOSE");
    }
  }
}