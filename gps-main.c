#include <wiringPi.h>
main ()
{
  int switch_state,count,serial,length;
  char sentence[255];//storage for nmea sentence
  wiringPiSetup () ;//setups the  gpio pins 
  pinMode (1, INPUT) ;//declares gpio 18 as input 
  count = 0;
  serial=serialOpen("/dev/ttyAMA0",4800);
  while(serialDataAvail(serial)!=-1)//if the gps has succesfully concted with the raspi do the following 
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

int get_type(char *inData){//function the returns the type of the  sentence
	int pos,output;
	char temp[6];//temporary array to put in the actuall type of the sentence 
	for(pos = 0;pos <= 6;pos++){
		temp[pos] = inData[pos];//copies the type of  the sentence to the temporary array
	}
	if(strcmp(temp,"$GPGGA") == 0){//tests if the type of the sentence is gpgga
                output = 1;//returns 1 if it is gpgga
        }
	else if(strcmp(temp,"$GPGSA") == 0){// tests if the type of the sentence is gpgsa
		output = 2;//returns 2 if its gpgsa 
	}
	else if(strcmp(temp,"$GPGSV") == 0){// tests if the type of the sentence is gpgsv .
                output = 3;//returns 3 if it is gpgsv
        }
	else if(strcmp(temp,"$GPRMC") == 0){// tests if the type of the sentence is gprmc
                output = 4;//returns 4 if it is gprmc
        }
	else 
		output = 0;//returns 0 if the type of the sentence is either corrupt or not supported .
	return output;//returns the output of the function 
}


void get_fix_time(char *inData,char *fix_time){
	int type = get_type(*inData);//get the sentence type 
	int digits;//position index in the fix_time array
	int count;//counts the 2 digits cycle to fill the fix_time array
       	int pos;//index to the inData array
	 if(type == 1 || type == 4){//test if type of the sentence is either gpgga or gprmc
		count = 0;//initiate count
		digits = 0;//initiate digits
		for(pos = 7;pos <= 12; pos++){//loop for finding the specific area in the sentence that holds the fix time 
			digits++;//continues to the next char of the fix time 
			if(count==2){//check if  2 digits of the fix time have  been passed to the array of the fix time and adds ':' to the next  element of the array 
				fix_time[digits]=':';//add ':' to an element of the fix time array
				count = 0;//resets 2 digits cycle
			}
			else {//otherwise
				fix_time[digits] = inData[pos];//places the current digit to the fix time array
				count++;//continues to the next digit of the 2 digits cycle 
			}
		}
	}
}
