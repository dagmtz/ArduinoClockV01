/*
  Arduino Digital Clock V0.1

  Author: 
    Daniel Martinez
    dagmtzs@gmail.com
  
  Date: 
    13/06/2023
*/

#define CLOCK_TO_SERIAL_ENABLED 1
#define CLOCK_TO_DISPLAY_ENABLED 1

#define MILLIS_PER_SECOND 1000
#define SECONDS_PER_MINUTE 60
#define MINUTES_PER_HOUR 60
#define HOURS_PER_DAY 24
#define SECOND_CHANGE_THRESHOLD 200

unsigned int hours = 0;
unsigned int minutes = 0;
unsigned int seconds = 0;
unsigned int msCounter = 0;
bool newSecondFlag = false;

char timeBuffer[9];
char time[9]="00:00:00";

void setup() 
{
  Serial.begin(9600);
  delay(SECOND_CHANGE_THRESHOLD);
  DDRB |= B00111111;
}

void updateBuffer()
{
  msCounter = millis() % 1000;

  if ( msCounter <= SECOND_CHANGE_THRESHOLD && !newSecondFlag )
  {
    seconds++;
    if ( seconds == SECONDS_PER_MINUTE )
    {
      seconds = 0;
      minutes++;
    }

    if ( minutes == MINUTES_PER_HOUR )
    {
      minutes = 0;
      hours++;
    }

    if ( hours == HOURS_PER_DAY )
    {
      hours = 0;
    }

    newSecondFlag = true;
  }

  if ( msCounter > SECOND_CHANGE_THRESHOLD && newSecondFlag )
  {
    newSecondFlag = false;
  }
  
  sprintf(timeBuffer, "%02d:%02d:%02d", hours, minutes, seconds);
}

void writeBCD( uint8_t number )
{
  uint8_t portbBuffer = PORTB;
  
  portbBuffer &= B00110000;
  number &= B00001111;
  portbBuffer |= number;

  PORTB = portbBuffer;
}

void updateDigits()
{
  writeBCD(time[7]);
  delay(3);
}

void displayTime()
{
  if ( time[7] != timeBuffer[7] )
  {
    strcpy(time, timeBuffer);
    #if CLOCK_TO_SERIAL_ENABLED == 1
      Serial.println(timeBuffer);
    #endif
  }
  #if CLOCK_TO_DISPLAY_ENABLED == 1
    updateDigits();
  #endif
}

void loop() 
{

  updateBuffer();
  displayTime();
  
}
