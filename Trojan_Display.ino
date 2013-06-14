/*
Sketch fuer das ansteuern der Trojan Leuchttafel mittels Spectrum Analyzer
Shield und Transistor Shield

Code Original von Bliptronics.com
Ben Moyes 2010

Modifiziert von 
Stefan Witschi 2013
*/
   
//For spectrum analyzer shield, these three pins are used.
//You can move pinds 4 and 5, but you must cut the trace on the shield and re-route from the 2 jumpers. 
const int spectrumReset=5;
const int spectrumStrobe=4;
const int spectrumAnalog=0;  //0 for left channel, 1 for right.

// Spectrum analyzer read values will be kept here.
int Spectrum[7];

// Setzt den Modus true=Musikgesteuert false=Led an
boolean musicMode = true;


const int button = 2;   // Button um zwischen Sound und Beleuchtungsmodus umzuschalten
const int led = 11;     // LED Streifen

int buttonState = 0;             // the current reading from the input pin
int prevButtonState = 0;         // previous button reading
int counter = 0;                 // counter

void setup() {
      Serial.begin(9600);

pinMode(led ,OUTPUT); 	
//  pinMode(Button, INPUT_PULLUP);
pinMode(button ,INPUT); 	

  byte Counter;

  //Setup pins to drive the spectrum analyzer. 
  pinMode(spectrumReset, OUTPUT);
  pinMode(spectrumStrobe, OUTPUT);

  //Init spectrum analyzer
  digitalWrite(spectrumStrobe,LOW);
    delay(1);
  digitalWrite(spectrumReset,HIGH);
    delay(1);
  digitalWrite(spectrumStrobe,HIGH);
    delay(1);
  digitalWrite(spectrumStrobe,LOW);
    delay(1);
  digitalWrite(spectrumReset,LOW);
    delay(5);
  // Reading the analyzer now will read the lowest frequency.
  
}

void loop() {

int buttonState = digitalRead(button);

// Debouncing
if (prevButtonState != buttonState) {
if(buttonState == LOW){
  counter++;
    }
}
prevButtonState = buttonState;

if(counter %2 == 0 ){
  musicMode = false;}
  else{
  musicMode = true;
 } 

//Serial.print("buttonState = ");
//Serial.println(buttonState);

// Serial.println(musicMode);

if (musicMode == true)
{

  int Counter, Counter2, Counter3;
    
  showSpectrum();
//  delay(15);  //We wait here for a little while until all the values to the LEDs are written out.
              //This is being done in the background by an interrupt
 }             
else{
digitalWrite(led, HIGH);    
    }

}


// FUNCTIONS

// Read 7 band equalizer.
void readSpectrum()
{
  // Band 0 = Lowest Frequencies.
  byte Band;
  for(Band=0;Band <7; Band++)
  {
    Spectrum[Band] = (analogRead(spectrumAnalog) + analogRead(spectrumAnalog) ) >>1; //Read twice and take the average by dividing by 2
    digitalWrite(spectrumStrobe,HIGH);
    digitalWrite(spectrumStrobe,LOW);     
  }
}


void showSpectrum()
{
  //Not I don;t use any floating point numbers - all integers to keep it zippy. 
   readSpectrum();
   byte Band, BarSize, MaxLevel;
   static unsigned int  Divisor = 10, ChangeTimer=0; //, ReminderDivisor,
   unsigned int works, Remainder;
  
  MaxLevel = 0; 
        
  for(Band=0;Band< 3;Band++)//We only graph the lowest 5 bands here, there is 2 more unused!
  {
  //If value is 0, we don;t show anything on graph
     works = Spectrum[Band]/Divisor;	//Bands are read in as 10 bit values. Scale them down to be 0 - 5
     if(works > MaxLevel)  //Check if this value is the largest so far.
       MaxLevel = works;                       
// Serial.println(works);
//Serial.println(Spectrum[Band]);
 
 if (works > 8){
     digitalWrite(led, HIGH);
 }
 if (works < 8){
     digitalWrite(led, LOW);
 }


     for(BarSize=1;BarSize <=5; BarSize++)  
        {
//        if(	works > BarSize) LP.setLEDFast(	LP.Translate(Band,BarSize-1),BarSize*6,31-(BarSize*5),0);
//          else if ( works == BarSize) LP.setLEDFast(	LP.Translate(Band,BarSize-1),BarSize*6,31-(BarSize*5),0); //Was remainder
//            else LP.setLEDFast(	LP.Translate(Band,BarSize-1),5,0,5);
        }
  }
  
 // Adjust the Divisor if levels are too high/low.
 // If  below 4 happens 20 times, then very slowly turn up.
  if (MaxLevel >= 10)
  {
    Divisor=Divisor+1;
    ChangeTimer=0;
  }
  else
    if(MaxLevel < 9)
    {
      if(Divisor > 8)
        if(ChangeTimer++ > 10)
        {
          Divisor--;
          ChangeTimer=0;
        }
    }
    else
    {
      ChangeTimer=0; 
    }
  }




 
    
