

// PIN DEFINITIONS


#define KPAD_1  23
#define KPAD_2  A4
#define KPAD_3  25
#define KPAD_4  24

#define LCD_D4  5
#define LCD_D5  4
#define LCD_D6  3
#define LCD_D7  2
#define LCD_RS  12
#define LCD_EN  11

#define RELAY_A  26
#define RELAY_B  27
#define RELAY_C  28
#define RELAY_D  29

#define WATER_SENSOR_A  30
#define WATER_SENSOR_B  31
#define WATER_SENSOR_C  9

#define BUTTON_DELAY 10

#define PH_SENSOR  A0
#define UV_SENSOR  A1
#define TEMP_SENSOR  10
#define Offset 0.00            //deviation compensate
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
#define DISPLAY_UPDATE_COUNT 10



/*
  S E R I A L  C O M M A N D S   D E F I N I T I O N
*/

#define GET_PH_LEVEL  'p'
#define GET_TEMPERATURE  't'
#define GET_WATERSENSORS 'w'
#define GET_RELAY_STATE  'r'
#define GET_UV_LEVEL  'u'
#define RELAY_A_ON  'a'
#define RELAY_B_ON  'b'
#define RELAY_C_ON  'c'
#define RELAY_D_ON  'd'
#define RELAY_A_OFF  'A'
#define RELAY_B_OFF  'B'
#define RELAY_C_OFF  'C'
#define RELAY_D_OFF  'D'




int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;  

int keypad_arr[4] = {KPAD_1, KPAD_2, KPAD_3, KPAD_4 } ;
int relay_arr[4] = {RELAY_A, RELAY_B, RELAY_C, RELAY_D};

float temperature = 0;
float phlevel = 0;
int uvlevel = 0;

int water_sensor_arr[3] = {WATER_SENSOR_A, WATER_SENSOR_B, WATER_SENSOR_C};
int water_sensor_state[3] = {0,0,0};
int relay_state[4] = {0,0,0,0};

int current_display = 0;

int display_counter = DISPLAY_UPDATE_COUNT;

int current_relay = 0;



#include <LiquidCrystal.h>
#include <OneWire.h>
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
OneWire ds(TEMP_SENSOR);


void setup() {
  //initialize pins
  pinMode(RELAY_A, OUTPUT);
  pinMode(RELAY_B, OUTPUT);
  pinMode(RELAY_C, OUTPUT);
  pinMode(RELAY_D, OUTPUT);
  digitalWrite(RELAY_A, HIGH);
  digitalWrite(RELAY_B, HIGH);
  digitalWrite(RELAY_C, HIGH);
  digitalWrite(RELAY_D, HIGH);

  pinMode(KPAD_1, INPUT);
  pinMode(KPAD_2, INPUT);
  pinMode(KPAD_3, INPUT);
  pinMode(KPAD_4, INPUT);

  pinMode(WATER_SENSOR_A, INPUT);
  pinMode(WATER_SENSOR_B, INPUT);
  pinMode(WATER_SENSOR_C, INPUT);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  Serial.begin(9600);           // start serial for output
  Serial.println("Ready");
  display_clear();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    delay(10);
    char command = Serial.read();
    readDataAndPerformTask(command);
    delay(5);
  }

  delay(100);
 
  
  int presed_button = buttonPressed();

  if (presed_button) buttonPressedActivity(presed_button);
   // change display
  display_counter--;
  if ( !display_counter ) {
    display_counter = DISPLAY_UPDATE_COUNT;
    display_pager();
  }
  delay(10);
  
}


void display_pager () {
  switch (current_display) {
    case 0: // sensor display
      display_sensorReadings();
      break;
    case 1:
      display_waterSensorState();
      break;     
    case 2:
      display_relayState();
      break;        
    case 3:
      display_about();
      break;     
      
  }
}


void display_relayState () {
  lcd.setCursor(0, 0);
  lcd.print("Relay ");

  lcd.setCursor(11, 0);
  lcd.print("A:");
  lcd.setCursor(13, 0);
  if (!digitalRead(RELAY_A)) {
    lcd.print("On ");
  } else {
    lcd.print("Off");
  }


  lcd.setCursor(0, 1);
  lcd.print("B:");
  lcd.setCursor(2, 1);
  if (!digitalRead(RELAY_B)) {
    lcd.print("On ");
  } else {
    lcd.print("Off");
  }

  lcd.setCursor(6, 1);
  lcd.print("C:");
  lcd.setCursor(8, 1);
  if (!digitalRead(RELAY_C)) {
    lcd.print("On ");
  } else {
    lcd.print("Off");
  }

  
  lcd.setCursor(11, 1);
  lcd.print("D:");
  lcd.setCursor(13, 1);
  if (!digitalRead(RELAY_D)) {
    lcd.print("On ");
  } else {
    lcd.print("Off");
  } 


  lcd.blink();

  switch (current_relay) {
    case 0: lcd.setCursor(11, 0); break;
    case 1: lcd.setCursor(0, 1); break;
    case 2: lcd.setCursor(6, 1); break;
    case 3: lcd.setCursor(11, 1); break;
  }
}




void display_waterSensorState () {
  updateWaterLevelSensor();
  lcd.setCursor(0, 0);
  lcd.print("Water Sensor");


  lcd.setCursor(0, 1);
  lcd.print("A:");
  lcd.setCursor(2, 1);
  if (water_sensor_state[0]) {
    lcd.print("On ");
  } else {
    lcd.print("Off");
  }

  lcd.setCursor(6, 1);
  lcd.print("B:");
  lcd.setCursor(8, 1);
  if (water_sensor_state[1]) {
    lcd.print("On ");
  } else {
    lcd.print("Off");
  }

  
  lcd.setCursor(11, 1);
  lcd.print("C:");
  lcd.setCursor(13, 1);
  if (water_sensor_state[2]) {
    lcd.print("On ");
  } else {
    lcd.print("Off");
  } 
}

void display_clear () {
  lcd.setCursor(0, 0);
  lcd.print("                 ");
  lcd.setCursor(0, 1);
  lcd.print("                 ");
  
}


void display_about () {
  lcd.setCursor(0, 0);
  lcd.print("  Hydrophonics");
  lcd.setCursor(0, 1);
  lcd.print("   Cynthia  ");
  
}

void display_sensorReadings () {
  
  temperature = getTemp();
  uvlevel = getUV();
  phlevel = getPH();

  lcd.setCursor(0, 0);
  lcd.print("Temp :");
  lcd.setCursor(7, 0);
  lcd.print(temperature);
  lcd.setCursor(13, 0);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("UV :");
  lcd.setCursor(5, 1);
  lcd.print(uvlevel);


  lcd.setCursor(8, 1);
  lcd.print("Ph:");
  lcd.setCursor(12, 1);
  lcd.print(phlevel);   

  
}



void buttonPressedActivity ( int pressed_button) {
 delay(200);
  switch (pressed_button) {
    case 1: // switch display
      current_display++;
      display_clear();
      if ( current_display > 3 ) current_display = 0;
      display_pager();
      break;
    default:
      buttonCommands(pressed_button);
  }
}

void buttonCommands ( int pressed_button ) {

  if (current_display == 2 ) {
    switch (pressed_button) {
      case 2:
        current_relay++;
        if (current_relay > 3) current_relay = 0;
        break;
      case 3:
        digitalWrite(relay_arr[current_relay], LOW);
        break;
      case 4:
        digitalWrite(relay_arr[current_relay], HIGH);
        break;
    }
  }
  
  
}

void updateWaterLevelSensor () {
  for ( int i=0; i < 3 ; i++ ) {
    int water_sensor = water_sensor_arr[i];
    int sensor_state1 = digitalRead(water_sensor);
    delay(BUTTON_DELAY);
    int sensor_state2 = digitalRead(water_sensor);
    if ( sensor_state1 != sensor_state2) {
      // debounce
      continue; // continue to next button
    } else {
      water_sensor_state[i] = sensor_state2;
    }
  }
}

 

void readDataAndPerformTask (char command) { 
  switch(command)  {
    case RELAY_A_ON : digitalWrite(RELAY_A, HIGH); relay_state[0] = 1; Serial.println("RELAY_A_ON"); break;
    case RELAY_B_ON : digitalWrite(RELAY_B, HIGH); relay_state[1] = 1; Serial.println("RELAY_B_ON"); break;
    case RELAY_C_ON : digitalWrite(RELAY_C, HIGH); relay_state[2] = 1; Serial.println("RELAY_C_ON"); break;
    case RELAY_D_ON : digitalWrite(RELAY_D, HIGH); relay_state[3] = 1; Serial.println("RELAY_D_ON"); break;
    case RELAY_A_OFF : digitalWrite(RELAY_A, LOW); relay_state[0] = 0; Serial.println("RELAY_A_OFF"); break;
    case RELAY_B_OFF : digitalWrite(RELAY_B, LOW); relay_state[1] = 0; Serial.println("RELAY_B_OFF"); break;
    case RELAY_C_OFF : digitalWrite(RELAY_C, LOW); relay_state[2] = 0; Serial.println("RELAY_C_OFF"); break;
    case RELAY_D_OFF : digitalWrite(RELAY_D, LOW); relay_state[3] = 0; Serial.println("RELAY_D_OFF"); break;

    case GET_PH_LEVEL :
        phlevel = getPH();
        Serial.print("PH:");
        Serial.println(phlevel);
        break;
        
    case GET_TEMPERATURE :
        temperature = getTemp();
        Serial.print("TEMP:");
        Serial.println(temperature);
        break;   

    case GET_UV_LEVEL :
        uvlevel = getUV();
        Serial.print("UV:");
        Serial.println(uvlevel);
        break;   
        
     
    case GET_WATERSENSORS :
        updateWaterLevelSensor();
        Serial.print("WATER:");
        Serial.print(water_sensor_state[0]);
        Serial.print(water_sensor_state[1]);
        Serial.println(water_sensor_state[2]);
        break;  
      
    case GET_RELAY_STATE :
        Serial.print("RELAY:");
        Serial.print(relay_state[0]);
        Serial.print(relay_state[1]);
        Serial.print(relay_state[2]);
        Serial.println(relay_state[3]);
  
        break; 
    default:
        Serial.print("NA");
         
  }
}



float getTemp(){
  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search(); 
    return -1000;
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("ERROR: CRC is not valid!"); 
    return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("ERROR: Device is not recognized");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); 

  byte present = ds.reset();
  ds.select(addr); 
  ds.write(0xBE); 

  for (int i = 0; i < 9; i++) { 
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float TRead = ((MSB << 8) | LSB); 
  float Temperature = TRead / 16;

  return Temperature;
}

int getUV() 
{
  int sensorValue;
  int analogValue = analogRead(UV_SENSOR);//connect UV sensors to Analog 0   
  if (analogValue<20)
  {
    sensorValue = 0;
  }
  else
  {
    sensorValue = 0.05*analogValue-1;
  }

  return sensorValue;
}


double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("ERROR: number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
} 
float getPH () {
  static float pHValue,voltage;
  pHValue = 0;

  pHArray[pHArrayIndex++]=analogRead(PH_SENSOR);
  if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
  voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
  pHValue = 3.5*voltage+Offset;
 
  
  return pHValue;

  
}

int buttonPressed () {
  int retval = 0;

  for ( int i = 0; i < 4 ;i++ ) {
    int button = keypad_arr[i];
    int button_state1 = digitalRead(button);
    if (button_state1 == 1 ) continue;
    delay(BUTTON_DELAY);
    int button_state2 = digitalRead(button);
    if ( button_state1 != button_state2) {
      // debounce
      continue; // continue to next button
    } else {
      // button is really really pressed
      // skip other buttons. only 1 button at a time
      retval = i + 1;
      //break;
    }
  }
  
  return retval;
  
}


