

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>

#define WIFI_SSID "DoorWay_IoT"     
#define WIFI_PASSWORD "55555555" 

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

#define AUTHOR_EMAIL "towfiq.uctccse@gmail.com"
#define AUTHOR_PASSWORD "bguycagpwljhzwtq";
#define RECIPIENT_EMAIL "towfiq.abflit@gmail.com"

SMTPSession smtp;

void smtpCallback(SMTP_Status status);

void setup(){
  pinMode(2,OUTPUT);//out
  pinMode(0,INPUT);//in
  Serial.begin(9600);
  Serial.println();
  Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    digitalWrite(2,0);
    Serial.print(".");
    delay(200);
    digitalWrite(2,1);
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();


}

void loop(){

  if(digitalRead(0)){ 
    
   digitalWrite(2,0); //LED On
    delay(700);
    digitalWrite(2,1);//LED Off
    delay(700);
     digitalWrite(2,0);
    delay(700);
    digitalWrite(2,1);
    delay(700);
     digitalWrite(2,0);
    delay(700);
    digitalWrite(2,1);
    delay(700);
     digitalWrite(2,0);
    delay(700);
    digitalWrite(2,1);
    delay(700);


  
    if(!digitalRead(0))
      return;
    delay(200);

    if(digitalRead(0)){
      digitalWrite(2,0);
      sendEmail();
      delay(5000);
      digitalWrite(2,1);
    }
      
      
  }
  
  

}



void sendEmail(){
    smtp.debug(1);
  smtp.callback(smtpCallback);
  ESP_Mail_Session session;


  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  SMTP_Message message;

  message.sender.name = "My IoT Device";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "IoT Device notification";
  message.addRecipient("Receiver Device", RECIPIENT_EMAIL);

  /*Send HTML message*/
  String htmlMsg = "<div style=\"color:#2f4468;\"><h1>Alert! Detected someone!!</h1><p>- Sent from IoT Device\n</p></div>";
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return;

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
}


/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
    }
    Serial.println("----------------\n");
  }
}   
