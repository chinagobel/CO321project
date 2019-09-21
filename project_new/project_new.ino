#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>

#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};


byte rowPins[ROWS] = {22, 24, 26, 28}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {30, 32, 34, 36}; //connect to the column pinouts of the keypad//

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 





SoftwareSerial mySerial(11,12);///(2,3)//11-purpule 12--white

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;

LiquidCrystal LCD(10,9,5,4,3,2);
int myCounter=0;
int enroll=6;//for enroll mode push button
int search=7;//for search mode push button
int up=8;//upload mode button
int back=13;//go back or ok button
String sid;///id number in string
int number;////get the id number
void setup() {
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  pinMode(enroll,INPUT);
  pinMode(search,INPUT);
  pinMode(up,INPUT);
  pinMode(back,INPUT);
  
  
  LCD.begin(16,4);//lcd has 16 columns 4 rows 
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("Select Mode");
  
}

int pushbuttonstate(){
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("Enroll or exit");
  while(1){
   if(digitalRead(enroll)==HIGH){
    return 0;
   }else if(digitalRead(back)==HIGH){
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("Select Mode");
    return 1;
   }
  }
  
}

uint8_t readnumber(void) {////get the id number form serial
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}



void loop() {

  
  if(digitalRead(enroll)==1){
  LCD.clear();
  LCD.setCursor(0,0);//set the cursor
  LCD.print("Enrolling....");
  
  while(1){
  LCD.setCursor(0,1);//second row
  LCD.print("Enter ID number");
  //number=readnumber();
  //id=number;
  //Serial.println(number);
  //id = readnumber(); // changed
  sid=keypadinput();
  id=sid.toInt();
  LCD.setCursor(0,2);
  LCD.print(id);
  while (!  getFingerprintEnroll() );
  
  int c=pushbuttonstate();
  if(c==1){
    break;
  }
  }
  
  }else if(digitalRead(search)==1){
    ////searching goes here
    LCD.setCursor(0,0);
    LCD.print("Searching..");
    while(1){
    ////if id=100 admin////the exit
    int admin=getFingerprintIDez();
    if(admin==100){
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("Select Mode");
      break;
    }
    delay(50);
    }  
    
    }else if(digitalRead(up)==1){
      /////code to upload the attendace
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("Uploading.....");
      for(int i=0;i<10;i++){
        delay(500);
        }
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("Done uploading"); 
      delay(1000);
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("Select Mode");
       
      }
  
  // put your main code here, to run repeatedly:

}


////fingerprint Enroll


uint8_t getFingerprintEnroll() {

  int p = -1;
  //Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  LCD.setCursor(0,3);
  LCD.print("Touch");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  LCD.setCursor(0,3);
  LCD.print("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  LCD.setCursor(0,3);
  LCD.print("Put same finger");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    LCD.setCursor(0,3);
    LCD.print("Stored!        ");
    delay(500);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}




/////searching////

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("Searching..");
  LCD.setCursor(0,1);
  LCD.print(finger.fingerID);
  LCD.setCursor(6,1);
  LCD.print("ID Found");
  //Serial.print("Found ID #");
  //Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}


String keypadinput(){
    String input;
    while(digitalRead(back)==0){////ok button
    char customKey = customKeypad.getKey();
    if (customKey){
                input+=customKey;
               
               }
    }
   // Serial.println(input);
    return input;
}
