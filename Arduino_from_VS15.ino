int ledBlue = 5;
int ledYellow = 6;
int ledRed = 7;
int oldValue = 0;

int ledVec[3] = {ledBlue, ledYellow, ledRed};
bool ledState[3] = {true, true, true};
bool ledMode[3] = {false, false, false};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  for(int i=0;i<3;i++)
  {
    pinMode(ledVec[i],OUTPUT);
    digitalWrite(ledVec[i], HIGH);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available() == 0);

  String str = Serial.readString();
  int cmd = str.toInt();

  if(cmd > 2)
  {
    cmd = 2;
  }
  if(cmd < -1)
  {
    cmd = -1;
  }

  if(cmd != -1)  
  {
      if(cmd == oldValue)
      {
        ledMode[cmd] = !ledMode[cmd];
      }

      if(!ledMode[cmd])
      {
        ledState[cmd] = !ledState[cmd];
        digitalWrite(ledVec[cmd],ledState[cmd]);
      }
  }
  else
  {
    for(int i=0;i<3;i++)
    {
      ledMode[i] = false;
      ledState[i] = false;
      digitalWrite(ledVec[i],ledState[i]);
    }
  }

  oldValue = cmd;
}
