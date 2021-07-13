#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <SoftwareSerial.h>

const int PulseWire = 0;
const int LED13 = 13;
int Threshold = 550;         

const int Tx = 2;
const int Rx = 3;

int sum=0;
double avg = 0.0;
int cnt=0;
int maxbpm=0, minbpm=300;

SoftwareSerial BTSerial(Tx, Rx);

PulseSensorPlayground pulseSensor;  

void calcavg(int mybpm){
  if(mybpm == 0)
    return;
  sum += mybpm;
  if (pulseSensor.begin()){
   cnt++;
  }
  avg = (double)sum / (double)cnt;
}

void setup() {  
  Serial.begin(9600);
  BTSerial.begin(9600);       

  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);   
  pulseSensor.setThreshold(Threshold);   

  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");   
  }
}

void loop() {
  if(cnt == 0)
  {
    Serial.println("부팅이 완료될 때까지 잠시만 기다려주십시오...");
  }
  
  int myBPM = pulseSensor.getBeatsPerMinute();

  if(maxbpm < myBPM){
    maxbpm = myBPM;
  }

  if(minbpm > myBPM && myBPM != 0){
    minbpm = myBPM;
  }

  calcavg(myBPM);

  if (pulseSensor.sawStartOfBeat()) {
    Serial.print("현재 박동 수 : ");
    Serial.println(myBPM);
    Serial.print("최대 박동 수 : ");
    Serial.println(maxbpm);
    Serial.print("최소 박동 수 : ");
    Serial.println(minbpm);
    Serial.print("평균 박동 수 : ");
    Serial.println(avg);
    Serial.println();
    BTSerial.print("평균 박동 수 : ");
    BTSerial.println(avg);
    BTSerial.println();
  }

  delay(1000);
  
  if(cnt % 10 == 0 && cnt != 0){
    if(avg > 120.0){
      Serial.println("================심박수가 비정상적으로 높습니다.=================");
      Serial.println("1. 천천히 심호흡을 해보세요.");
      Serial.println("2. 경동맥(목 옆) 마시지를 해보세요.");
      Serial.println("3. 얼굴에 찬 물을 뿌려보세요.");
      Serial.println("의사와 상담이 필요합니다. 위급 상황 시 119로 전화하세요.");
      Serial.println("============================================================");
      BTSerial.println("====심박수가 비정상적으로 높습니다.====");
      BTSerial.println("1. 천천히 심호흡을 해보세요.");
      BTSerial.println("2. 경동맥(목 옆) 마시지를 해보세요.");
      BTSerial.println("3. 얼굴에 찬 물을 뿌려보세요.");
      BTSerial.println("의사와 상담이 필요합니다. 위급 상황 시 119로 전화하세요.");
      BTSerial.println("==================================");
      sum = 0;
      cnt = 0;
      maxbpm = 0;
      minbpm = 300;
      delay(10000);
    }
    if(avg < 60.0){
      Serial.println("================심박수가 비정상적으로 낮습니다.=================");
      Serial.println("의사와 상담이 필요합니다. 위급 상황 시 119로 전화하세요.");
      Serial.println("============================================================");
      BTSerial.println("====심박수가 비정상적으로 낮습니다.====");
      BTSerial.println("의사와 상담이 필요합니다. 위급 상황 시 119로 전화하세요.");
      BTSerial.println("===================================");
      sum = 0;
      cnt = 0;
      maxbpm = 0;
      minbpm = 300;
      delay(10000);
    }
    if(avg >= 60.0 && avg <= 120){
      Serial.println("==================================================");
      Serial.println("==================================================");
      Serial.println("================심박수가 정상입니다.=================");
      Serial.println("==================================================");
      Serial.println("==================================================");
      BTSerial.println("===================================");
      BTSerial.println("===================================");
      BTSerial.println("=========심박수가 정상입니다.=========");
      BTSerial.println("===================================");
      BTSerial.println("===================================");
      sum = 0;
      cnt = 0;
      maxbpm = 0;
      minbpm = 300;
    }
  }
}
