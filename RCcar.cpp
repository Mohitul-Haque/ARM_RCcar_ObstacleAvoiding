#include "mbed.h"
 
PwmOut ledR(p25);
PwmOut ledL(p24);
DigitalOut trigger(p6); 
DigitalOut myled(LED1); //monitor trigger 
DigitalOut myled2(LED2); //monitor echo 
DigitalIn  echo(p7);
DigitalOut BrkR(p19);
DigitalOut BrkL(p20);
DigitalOut RevR(p18);
DigitalOut RevL(p17);

      
Serial pc(USBTX, USBRX);
Serial device(p9, p10); 
 

int distance = 0; 
int correction = 0; 
Timer sonar; 
 
int main()
{
    ledL.period(0.001f);
ledR.period(0.001f);
    sonar.reset(); 
    sonar.start(); 
    while (echo==2) {};     
myled2 = 0; 
// stop timer     
sonar.stop(); 
 //read timer 
    int correction = sonar.read_us(); 
    pc.printf("Approximate software overhead timer delay is %d uS\n\r",correction); 
//    
    unsigned char rx;
    pc.printf("Bluetooth Start\r\n");
    
    device.baud(9600);
    while(1) {
 
    pc.printf("Approximate software overhead timer delay is %d uS\n\r",correction);
    // trigger sonar to send a ping         
    trigger = 1;         
    myled = 1;         
    myled2 = 0;         
    sonar.reset();         
    wait_us(10.0);         
    trigger = 0;         
    myled = 0; 
    //wait for echo high         
    while (echo==0) {};         
    myled2=echo; 
    //echo high, so start timer         
    sonar.start(); 
    //wait for echo low         
    while (echo==1) {}; 
    //stop timer and read value         
    sonar.stop(); 
    //subtract software overhead timer delay and scale to cm         
    float distance = (sonar.read_us()-correction)/58.0;        
     myled2 = 0; 
     pc.printf("distance is %f cm \n\r",distance); 
    //wait so that any echo(s) return before sending another ping         
    wait(0.2);
    
    if (device.readable()) 
            {
            
                    rx = device.getc();
            pc.printf("\n Received %x \nSending : %x",rx,rx);
            
            }
    
    if (distance > 15){
 
        
            //turn right
             if (rx == 4 || rx == 5){
                    BrkR = 0;
                    BrkL = 0; 
                    ledL.write(0.10f);
                    ledR.write(0.10f);
                    RevR = 1;
                    RevL = 0; 
                    pc.printf("Hello");       
                 } 
                 
             //turn left  
            if (rx == 6 || rx == 7){
                    BrkR = 0;
                    BrkL = 0; 
                    RevR = 0;
                    RevL = 1;   
                    ledR.write(0.10f); 
                    ledL.write(0.10f); 
                    pc.printf("Hello1");   
                 }
                 
             //go forwards
            if (rx == 1 || rx == 0){
                    BrkR = 0;
                    BrkL = 0;
                    RevR = 0;
                    RevL = 0;  
                    ledL.write(0.10f);
                    ledR.write(0.1385f); 
                    pc.printf("Hello2");        
                 }
                 
              //reverse  
            if (rx == 2 || rx == 3){
                    BrkR = 0;
                    BrkL = 0; 
                    ledL.write(0.155f);
                    ledR.write(0.10f); 
                    RevR = 1;
                    RevL = 1;  
                    pc.printf("Hello3"); 
                 }  
         }
         else {
             
            //reverse  
            if (rx == 2 || rx == 3){
                    BrkR = 0;
                    BrkL = 0; 
                    ledL.write(0.155f);
                    ledR.write(0.10f); 
                    RevR = 1;
                    RevL = 1;  
                    pc.printf("Hello3"); 
                 }
                 else{
                        BrkR = 1;
                        BrkL = 1; 
                     }
             
             
             }
    }
}
 