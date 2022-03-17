#include <Servo.h>   //載入函式庫，這是內建的，不用安裝

Servo myservo_top;  // 建立SERVO物件
Servo myservo_bottom;  // 建立SERVO物 件


void setup() {
  myservo_top.attach(8);  // 設定要將伺服馬達接到哪一個PIN腳
  myservo_bottom.attach(10);
  pinMode(3, OUTPUT); 
}

void loop() {   
  digitalWrite(3, HIGH);       // sets the digital pin 13 on
  delay(1000);                  // waits for a second
  digitalWrite(3, LOW);        // sets the digital pin 13 off
  delay(1000);    
  
  myservo_top.write(0);  //旋轉到0度，就是一般所說的歸零
  myservo_bottom.write(0);
  delay(1000);
  myservo_top.write(90);  //旋轉到0度，就是一般所說的歸零
  myservo_bottom.write(90);
  delay(1000);
//  myservo.write(180); //旋轉到180度
//  delay(1000);
//  myservo.write(90);
//  delay(1000);
}
