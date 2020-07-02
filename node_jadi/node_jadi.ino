#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define SS_PIN D4
#define RST_PIN D3
#define Laser D2
#define Buzzer D1
#define Ldr D0

#define firebaseURl "alatfotocopy-ed11a.firebaseio.com"
#define authCode "ZBULaEWV7HuBvqeJkIdmEIow1cu4YfrLfyaMnVd0"

const char* ssid = "tttttt";
const char* password = "amarfadillah";
float kondisi;
bool sekali = false;
int ada,duakali = 0;



MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class
void setup() {
//   Serial.begin(9600);
//   Serial.println("RFID reading UID");
   pinMode(Ldr, INPUT); //D0
   pinMode(Buzzer, OUTPUT); // D1
   pinMode(Laser, OUTPUT); //laser //D2
   
   digitalWrite(Buzzer, LOW);
   digitalWrite(Laser, LOW);

   Pengaturan_wifi();
   Pengaturan_data();
   SPI.begin();       // Init SPI bus
   mfrc522.PCD_Init(); // Init MFRC522
   
   
}
void loop() {
kondisi = Firebase.getFloat("kondisi");
String content= "";
byte letter;
if(kondisi == 0){
  digitalWrite(Laser, LOW); //D1 
  digitalWrite(Buzzer, LOW);
  Firebase.set("sistem",0); // notif
}
else{
  
  digitalWrite(Laser, HIGH); // laser
}

if ( mfrc522.PICC_IsNewCardPresent())
    {
        if ( mfrc522.PICC_ReadCardSerial())
        {
//           Serial.print("Tag UID:");
           for (byte i = 0; i < mfrc522.uid.size; i++) {
//                  Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
//                  Serial.print(mfrc522.uid.uidByte[i], HEX);
                  content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
                  content.concat(String(mfrc522.uid.uidByte[i], HEX));
            }
          
//            Serial.println();
//            Serial.print("Message : ");
            content.toUpperCase();
            if (content.substring(1) == "9B D4 4A 79" or content.substring(1) == "20 2D 9C 73") //change here the UID of the card/cards that you want to give access
            {
              if (kondisi == 1){
                Firebase.set("kondisi",0);
                digitalWrite(Laser, LOW);
                digitalWrite(Buzzer,LOW);
                Firebase.set("sistem",0); // notif
              }
              else{
                Firebase.set("kondisi",1);
                digitalWrite(Laser, HIGH);
              }
//              Serial.println("Authorized access");
//              Serial.println();
              delay(3000);
            }
 
            else   {
//              Serial.println(" Access denied");
              delay(3000);
            }
            mfrc522.PICC_HaltA();
            
        }
}
if(kondisi == 1){
deteksi();
}
  
}

void Pengaturan_wifi(){
//  Serial.println();
//  Serial.print("Wifi connecting");
//  Serial.print(ssid);

  WiFi.begin(ssid,password);
//
//  Serial.println();
//  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
//    WiFi.begin(ssid,password); // salah
//    Serial.print(".");
  }
  delay(2000);
//  Serial.println(WiFi.localIP());
//  Serial.println("wif.iconnect");
}


void Pengaturan_data(){
  Firebase.begin(firebaseURl,authCode);
  delay(1000);
  float checking = Firebase.getFloat("check");
  while(checking != 1){
    Firebase.begin(firebaseURl,authCode);
    float checking = Firebase.getFloat("check");
    Serial.println(checking);
    delay(1000);
  }

  digitalWrite(Buzzer,HIGH);
  delay(500);
  digitalWrite(Buzzer,LOW);
  delay(500);
  digitalWrite(Buzzer,HIGH);
  delay(500);
  digitalWrite(Buzzer,LOW);
  delay(500);
  
}


void deteksi(){
  ada = digitalRead(Ldr); //ldr
  if(ada == 1){
    if(sekali == true){
      duakali++;
      sekali = false;
    }
  }
  else {
    sekali = true;
  }
  
  if(duakali == 2){
    digitalWrite(Buzzer,HIGH); //bel
    Firebase.set("sistem",1); // notif
    
    duakali =0;
  }
  
}

