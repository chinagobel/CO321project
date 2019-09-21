void setup() {
  // put your setup code here, to run once:
  int i=0;
  String a="";
   Serial.begin(9600);
  uint8_t arr[] ={255,239,1,5,239,255};
  for(i=0;i<6;i++){
    a+=arr[i];
    a+='_';
 // Serial.println(printHex(arr[i],2));
  }
  Serial.println(a);

  Serial.println("+++++++++++++++++++++++");
////++++++++++++++++++++++++++++++++++++++++++++++++++++/////
char array[512];
a.toCharArray(array,512);
char *strings[10];
char *ptr = NULL;
byte index = 0;
   ptr = strtok(array, "_");  // takes a list of delimiters
    while(ptr != NULL)
    {
        strings[index] = ptr;
        index++;
        ptr = strtok(NULL, "_");  // takes a list of delimiters
    }
  
    for(int n = 0; n < index; n++)
   {
    String c=strings[n];
    int ji=c.toInt();
    Serial.println(ji);
   }
/////////////////////////////////////////////////////++++++++++++++++++++++++++++++++++++++++++++++++++

  
}
  

void loop() {
 
  // put your main code here, to run repeatedly:

}


char * printHex(int num, int precision) {
    char tmp[16];
    char format[128];
 
    sprintf(format, "%%.%dX", precision);
 
    sprintf(tmp, format, num);
    return tmp;
}
