#include <wiringPi.h>
main ()
{
  int switch_state,count,serial,length;
  char sentence[255];//storage for nmea sentence
  wiringPiSetup () ;//setups the  gpio pins 
  pinMode (1, INPUT) ;//declares gpio 18 as input 
  count = 0;
  
  while((serial=serialOpen("/dev/ttyAMA0",4800))>0)//if the gps has succesfully concted with the raspi do the following 
  {
    switch_state = digitalRead(1);//switch for reconrding  state 
    if(switch_state == 1){
	system("gps-logger");//start the logger
        count=1;// logger is running
    }
    if(switch_state == 0 && count == 1){
	system("gps-stop");//stop the logger
        count=0;//logger is not running
    }
  }

}
void get_sentence(char *inData[255],int *pos){//get the next nmea sentence 
        char inChar = -1;//character read
        pos = 0;//index into array,where to store the character  
        while(serialDataAvail(serial)){//If data is being sent to the seria port  do the following
	       inChar = serialGetchar(serial);//Read the next character 
               //fprintf9stdout,"%c",inChar);
	       inData[pos] = inChar;//save character read into the sentence 
	       pos++;//go to the next character position
	       if(inChar == 13){//if the character read is carriage return  then break loop 
		 break;
	       }
       }
 }

