void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  uint8_t a[]={1,239,3,4,239,1,5,5,5,5,5,5,5,7,239,1,6,6,6,6,6,6,6,8,239,1,6,7,8,9,11,67,32,33,90};
  Serial.print(extractfingerdata(a));
}

void loop() {
  // put your main code here, to run repeatedly:

}
String extractfingerdata(uint8_t arr[]){
  String s="";///data string to send to the server
  int i=0;
  int stop=0;
  boolean start=false;//start storing
  for(i;i<500;i++){ ////go through the array
    if(arr[i]==239&&arr[i+1]==1){ ///check wether data packet or not EF01
        stop++;
        start=true;
        i+=9;/////skip the header file to get to the data
      }
      if(stop==3){///stop after 3 EF01 are found
        break;
        }
      if(start){////start storing data into the string
        s+=arr[i];
        s+='_';
        }
        
  }

  /////////////////////////end of the making data string
  return s;
}
