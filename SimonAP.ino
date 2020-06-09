// Importation des différentes bibliothèques
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <analogWrite.h>
#include <pitches.h>
#include <Tone32.h>


const int MAX_LEVEL = 100;
int sequence[MAX_LEVEL];
int sound[MAX_LEVEL];
int gamer_sequence[MAX_LEVEL];
int level = 1;
int note = 0;
int velocity = 2000;
int deuxs = 400 ;//200;
int deuxc = 500 ;//250;
int cinq = 1000 ; //500;
int cent = 200; //100;


// Identifiant Wi-Fi

const char *ssid = "SIMON TFE JEU";
const char *password = "Elias";
int max_connection = 1;


// Déclaration des différentes LEDs

const int ledRed =13;             //Led Rouge
const int ledWhite = 12;             //Led Blanc
const int ledBlue = 15;             //Led Jaune
const int ledYellow = 14;             //Led Bleu

boolean RedWebPage = HIGH ;
boolean WhiteWebPage = HIGH ;
boolean BlueWebPage = HIGH ;
boolean YellowWebPage = HIGH ;
boolean StartWebPage = HIGH;

// Création du AsyncWebServer object sur le port "80"
AsyncWebServer server(80);


String processor(const String& var){
  Serial.println(var +" = "+ String(level));
  if (var =="level")
  {
   return String(level); 
  }

  return String();
}


void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  pinMode(12, OUTPUT); //white led
  pinMode(13, OUTPUT); //red led
  pinMode(14, OUTPUT); //yellow led
  pinMode(15, OUTPUT); //blue led

  pinMode(32, INPUT_PULLUP); //button 1
  pinMode(33, INPUT_PULLUP); //button 2
  pinMode(34, INPUT_PULLUP); //button 3
  pinMode(35, INPUT_PULLUP); //button 4

  pinMode(4, OUTPUT); //buzzer
  pinMode(36, INPUT_PULLUP); //button Start



  // Initialisation du SPIFFS
  
  if(!SPIFFS.begin(true)){
    Serial.println("Une erreur est survenu");
    return;
  }



  // Connexion au Wi-Fi
  
  WiFi.softAP(ssid, password, max_connection);
 
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  




   // Direction de la page WEB
   
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });


  // Direction vers le fichier style.css
  
  server.on("/Style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/Style.css", "text/css");
  });


  // Demarrer le simon
  
  server.on("/Start", HTTP_GET, [](AsyncWebServerRequest *request){
        StartWebPage = LOW;
        Serial.println("boutonWeb start pressé");
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
   server.on("/rouge", HTTP_GET, [](AsyncWebServerRequest *request){
       RedWebPage = LOW;
       Serial.println("boutonWeb rouge pressé");
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
   server.on("/bleu", HTTP_GET, [](AsyncWebServerRequest *request){
   BlueWebPage = LOW;
   Serial.println("boutonWeb bleu pressé");
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
   server.on("/blanc", HTTP_GET, [](AsyncWebServerRequest *request){
   WhiteWebPage = LOW;
   Serial.println("boutonWeb blanc pressé");
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
   server.on("/jaune", HTTP_GET, [](AsyncWebServerRequest *request){
   YellowWebPage = LOW;
   Serial.println("boutonWeb jaune pressé");
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Démarrer le serveur
  server.begin();

  
}


void loop() {
  if (level == 1) {
    generate_sequence();
    
    for (int i = 12; i >= 15; i--) { //flashing leds
      digitalWrite(i, HIGH);
      Serial.println("led" + String(i) + " allumée");
      delay(cent);
      digitalWrite(i, LOW);
      Serial.println("led" + String(i) + " éteinte");
      delay(cent);
    }
  }

  if (level != 1 || StartWebPage == LOW) { //start button == button 1
    show_sequence();
    get_sequence();
    StartWebPage = HIGH;
  }
}

void generate_sequence() {
  randomSeed(millis()); //true random

  for (int i = 0; i < MAX_LEVEL; i++) {
    sequence[i] = random(12, 16);

    switch (sequence[i]) { //convert color to sound
      case 15:
        note = 349; //Fa
        break;
      case 14:
        note = 329; //Mi
        break;
      case 13:
        note = 293; //Re
        break;
      case 12:
        note = 261; //Do
        break;
    }
    sound[i] = note;
  }
//    Serial.println(void generate_sequence());
}

void show_sequence() {
  digitalWrite(12, LOW);
  Serial.println("ledWhite éteinte");
  digitalWrite(13, LOW);
  Serial.println("ledRed éteinte");
  digitalWrite(14, LOW);
  Serial.println("ledYellow éteinte");
  digitalWrite(15, LOW);
  Serial.println("ledBlue éteinte");
  BlueWebPage = HIGH;
  YellowWebPage = HIGH;
  WhiteWebPage = HIGH;
  RedWebPage = HIGH;

  for (int i = 0; i < level; i++) {
    digitalWrite(sequence[i], HIGH);
    Serial.println("led" + String(sequence[i]) + "allumée");
    tone(3, sound[i]);
    delay(velocity);
    digitalWrite(sequence[i], LOW);
    Serial.println("led" + String(sequence[i]) + "éteinte");
    noTone(3);
    delay(deuxs);
  }
//    Serial.println(void show_sequence());
}

void get_sequence() {
  int flag = 0; //flag correct sequence

  for (int i = 0; i < level; i++) {
    flag = 0;

    while (flag == 0) {

      if (WhiteWebPage == LOW) {
        Serial.println("bouton Blanc pressé");
        digitalWrite(12, HIGH);
        Serial.println("ledWhite allumée");
        tone(3, 261); //Do
        delay(velocity);
        noTone(3);
        gamer_sequence[i] = 12;
        flag = 1;
        delay(deuxs);

        if (gamer_sequence[i] != sequence[i]) {
          wrong_sequence();
          return;
        }
        digitalWrite(12, LOW);WhiteWebPage == HIGH;
        Serial.println("ledWhite éteinte");
      }

      if (RedWebPage == LOW) {
        Serial.println("bouton Rouge pressé");
        digitalWrite(13, HIGH);
        Serial.println("ledRed allumée");
        tone(3, 293); //Re
        delay(velocity);
        noTone(3);
        gamer_sequence[i] = 13;
        flag = 1;
        delay(deuxs);

        if (gamer_sequence[i] != sequence[i]) {
          wrong_sequence();
          return;
        }
        digitalWrite(13, LOW);RedWebPage == HIGH;
        Serial.println("ledRed éteinte");
      }

      if (YellowWebPage == LOW) {
        Serial.println("bouton jaune pressé");
        digitalWrite(14, HIGH);
        Serial.println("ledYellow allumée");
        tone(3, 329); //Mi
        delay(velocity);
        noTone(3);
        gamer_sequence[i] = 14;
        flag = 1;
        delay(deuxs);

        if (gamer_sequence[i] != sequence[i]) {
          wrong_sequence();
          return;
        }
        digitalWrite(14, LOW);YellowWebPage == HIGH;
        Serial.println("ledYellow éteinte");
      }

      if (BlueWebPage == LOW) {
        Serial.println("bouton Bleu pressé");
        digitalWrite(15, HIGH);
        Serial.println("ledBlue allumée");
        tone(3, 349); //Fa
        delay(velocity);
        noTone(3);
        gamer_sequence[i] = 15;
        flag = 1;
        delay(deuxs);

        if (gamer_sequence[i] != sequence[i]) {
          wrong_sequence();
          return;
        }
        digitalWrite(15, LOW);BlueWebPage == HIGH;
        Serial.println("ledBlue éteinte");
      }
    }
  }
  right_sequence();
//  Serial.println(void get_sequence());
}

void right_sequence() {
  Serial.println("Manche suivante");
//  digitalWrite(12, LOW);
//  digitalWrite(13, LOW);
//  digitalWrite(14, LOW);
//  digitalWrite(15, LOW);
//  delay(deuxc);
//
//  digitalWrite(12, HIGH);
//  digitalWrite(13, HIGH);
//  digitalWrite(14, HIGH);
//  digitalWrite(15, HIGH);
//  delay(cinq);
//
//  digitalWrite(12, LOW);
//  digitalWrite(13, LOW);
//  digitalWrite(14, LOW);
//  digitalWrite(15, LOW);
//  delay(cinq);

  if (level < MAX_LEVEL) {
    level++;
  }
  velocity -= 50; //increases difficulty
}

void wrong_sequence() {
  Serial.println("Perdu");
  for (int i = 0; i < 3; i++) {
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    digitalWrite(14, HIGH);
    digitalWrite(15, HIGH);
    tone(3, 233);
    delay(deuxc);

    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    digitalWrite(14, LOW);
    digitalWrite(15, LOW);
    noTone(3);
    delay(deuxc);
  }
  level = 1;
  velocity = 2000;
}
