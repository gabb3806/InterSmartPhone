#include <SoftwareSerial.h>
SoftwareSerial bluetooth(2,3);
String Mystr=""; // 안드로이드로부터 받는 문자열
String pwd="0000"; // 초기 비밀번호 = 0000

int opn = 5; // 문 열림 핀
int clo = 6; // 문 닫힘 핀

void setup() {
  Serial.begin(9600); // 시리얼 속도
  bluetooth.begin(9600); // 블루투스 속도
  pinMode (opn, OUTPUT); 
  pinMode (clo, OUTPUT); 
}

void loop() {
  blue();   
  if(!Mystr.equals("")) { // Mystr에 문자열이 있으면
    if(pwd==""){ // 비밀번호가 초기화되면
      pwd = Mystr;
      Serial.println("비밀번호를 설정합니다.");
      Serial.println("비밀번호 = " + pwd);
    }
    else if(Mystr=="change") { // 받은 문자열이 change면
      digitalWrite (clo, HIGH);
      digitalWrite (opn, LOW);  
      Serial.println("현재 비밀번호를 입력하세요.");
      change();  // 현재 비밀번호를 입력하고 change함수 호출
    }
    else if(Mystr == pwd) { // 받은 문자열이 현재 비밀번호와 일치하면 문 열림
      Serial.println("문이 열립니다."); // 받은 문자열이 현재 비밀번호와 일치하지 않으면 문 닫힘
      digitalWrite (clo, LOW);
      digitalWrite (opn, HIGH);
      bluetooth.println("1");
    }
    else if(Mystr=="close"){
      Serial.println("문이 닫힙니다.");
      digitalWrite (clo, HIGH);
      digitalWrite (opn, LOW);
    }
    else{
      Serial.println("비밀번호가 틀렸습니다."); // 받은 문자열이 현재 비밀번호와 일치하지 않으면 문 닫힘
      digitalWrite (clo, HIGH);
      digitalWrite (opn, LOW);
      bluetooth.println("6");
    }
    Mystr=""; // Mystr 초기화
  }
}

void blue() {
  while(bluetooth.available()){ // 블루투스에서 넘어온 데이터가 있으면
    char Mychar = (char)bluetooth.read(); // 데이터를 읽어 Mychar에 저장
    Mystr+=Mychar;
    delay(5);
  }
}

void change() {
  Mystr="";
  int i;
  
  for(i=0; i<5; i++) { 
    while(Mystr.equals("")) { // 안드로이드로부터 문자열 받기
      blue();
    }
    if(Mystr.equals(pwd)) { // 받은 문자열이 비밀번호와 일치하면
      pwd=""; // 비밀번호 초기화
      Serial.println("비밀번호가 초기화됩니다. 다시 설정하세요.");
      bluetooth.println("3"); 
      return;
    }else if(Mystr.equals("stop")){
      Serial.println("취소합니다.");
      return;
    }
    else { // 받은 문자열이 비밀번호와 일치하지 않으면
      if(i == 4) { // 5회를 초과하면 return
        Serial.println("비밀번호가 틀렸습니다. 5회를 초과하였습니다.");
        bluetooth.println("5"); 
        return;
      }
      else { // 1~4회까지 오류 메시지 출력
        Serial.print("비밀번호가 틀렸습니다. 다시 입력하세요. ");
        Serial.print(i+1);
        Serial.println("회 오류");
        bluetooth.println("4"); 
        Mystr="";
      }
    }
  }  
}

