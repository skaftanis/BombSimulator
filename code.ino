 #include "Keypad.h"
 #include "pitches.h"

//notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };
    byte numbers[10] = {
    B11111100, B01100000, B11011010, B11110010, B01100110,
    B10110110, B10111110, B11100000, B11111110, B11100110
    };
    int diffuse = 1;
    int norm_delay = 1000;
    boolean boomCaled = false;
    int tonems=200;
    boolean increseCalled=false;
    const byte ROWS = 4; //four rows
    const byte COLS = 4; //four columns
     char keys[ROWS][COLS] =
    {{'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}}; 

    char minuteLeft=0;
    char minuteRight=0;
    char secondLeft = 0;
    char secondRight = 0;
    int count = 0;
    boolean complete =false;
   byte rowPins[ROWS] = {
    11, 10, 9, 8}; //connect to the row pinouts of the keypad
   byte colPins[COLS] = {
    7, 6, 5, 4}; //connect to the column pinouts of the keypad
  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
    
    
    void setup() {

  Serial.begin(9600);
      for(int i = 34; i <=48; i++) {
      pinMode(i, OUTPUT);
    }
    for(int i = 20; i <=33; i++) {
      pinMode(i, OUTPUT);
    }
    pinMode(13, OUTPUT);   
    pinMode(2 , INPUT);
    pinMode(8 , OUTPUT); //ηχείο
    pinMode(3, INPUT);
    pinMode(18, INPUT);
    pinMode(52 , OUTPUT); //red led
    pinMode(53 , OUTPUT); //green led
    attachInterrupt(0, stop, CHANGE);
    attachInterrupt(1, increase_time , CHANGE);
    attachInterrupt(5, boom, CHANGE);
     

    keypad.addEventListener(keypadEvent); //add an event listener for this keypad
    keypad.setDebounceTime(50); //This shows that the debounce time will allow one key press every 250 milliseconds. 
    
while( complete == false)
  {
      int key = keypad.getKey();
/*      if (key != NO_KEY)
      {
     //    keypadEvent(key);
          Serial.print(key);
      } */
    
    }        
    
      
    writeNumber(minuteLeft, 20, 26); //πρωτο αριστερα
    writeNumber(minuteRight, 27, 33); //δευτερο αριστερα
    writeNumber(secondLeft, 34, 40);
    writeNumber(secondRight, 41, 47);
    delay(1000);  
   // digitalWrite(10, HIGH); 
    
    }
     
    void stop()
    {
      digitalWrite(53 , HIGH); 
      exit(0);
    }
    
    void boom()
    {
       digitalWrite(52,HIGH);
       writeNumber(0, 34, 40);
       writeNumber(0, 41, 47);           
       writeNumber(0, 20, 26); //πρωτο αριστερα
       writeNumber(0, 27, 33); //δευτερο αριστερα
       boomCaled=true;
   
    }
 
    void boom_sound() {
      
      // iterate over the notes of the melody:
      for (int thisNote = 0; thisNote < 8; thisNote++) {
    
        // to calculate the note duration, take one second 
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000/noteDurations[thisNote];
        tone(12, melody[thisNote],noteDuration);
    
        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(8);
      }
       exit(0);
    }   
    
    
    void increase_time()
    {
      norm_delay = 100;
      increseCalled=true;
    }
  
    void loop() {
      int i,j;
     // digitalWrite(10, HIGH); 
     int l,n,m;
  for (n=minuteLeft; n>=0; n--) {
    writeNumber(n, 20, 26);
    for (int m=minuteRight; m>=0; m--) {
       if( m == 0 && !boomCaled)
           minuteRight = 9;
           
       writeNumber(m, 27, 33);
      for ( i=secondLeft; i>=0; i--)
      { 
            if( i == 0 && !boomCaled)
                 secondLeft = 5;
            //digitalWrite(53,HIGH);
            writeNumber(i, 34, 40);
            for( j=secondRight; j>=0; j--) {
               writeNumber(j, 41, 47); 
               if ( n == 0 && m == 0 && i== 0 && j == 0)
               {
                 boomCaled = true;
                 break;
               }
               if ( j == 0)
                 secondRight = 9;
               tone(12,500,tonems);            
               delay(norm_delay);  
               if (boomCaled) { boom_sound(); break; }   
               if (   i==1  && increseCalled==false && m==0 && n==0 )  {
                 for (int k=0; k<5; k++) {
                   tone(12,500,100);
                   delay(200);           
                 }

                 norm_delay=0;
               }
             if (i==0 && increseCalled==false && m==0 && n==0) {
                for (int k=0; k<10; k++) {
                   tone(12,500,50);
                   delay(100);
                  
                 }
           }
               
         }  
     
      }
  
     }
     
  }
      digitalWrite(52,HIGH);
      boom_sound();
      exit(0);
      
      
     }
      
    
     
    void writeNumber(int number, int pin1, int pin2) {
      if(number < 0 || number > 9) {
        return;
      }
      
      byte mask = numbers[number];
      byte currentPinMask = B10000000;
      for(int i = pin1; i <= pin2; i++) {
        if(mask & currentPinMask) digitalWrite(i,HIGH);
        else digitalWrite(i,LOW);
      currentPinMask = currentPinMask >> 1;
      }
    }
    
    
void keypadEvent(KeypadEvent key)
{
       digitalWrite(13,HIGH);
      //writeNumber(key-48, 20, 26);
    if (keypad.getState() == PRESSED )
      {        
        //delay(500);
 
        if ( key >= '0' && key <= '9' ) //getKey() returns ASCII code
          {
            if ( count == 0 ) // First minute's digit
            {
                  minuteLeft = key-48;
                  writeNumber(minuteLeft, 20, 26);
            }
            else if ( count == 1) //Second minute's digit
            {
                 minuteRight = key-48;
                 writeNumber(minuteRight, 27, 33);
            }
                 
            else if ( count == 2) // First seconds's digit
             { 
                if( key > '5' )
                      return;
                else
                      secondLeft = key-48;
                
                writeNumber(secondLeft, 34, 40);     
                      
               }
            else if ( count == 3) // Second seconds's digit
            {
              secondRight = key-48;
              writeNumber(secondRight, 41, 47);  
            }   
          count++; 
          if ( count == 4 )
             complete = true;  
          }
     }
     digitalWrite(13,LOW);
        
  } 
  

