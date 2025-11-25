#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"

WiFiClientSecure wifiClient;
WiFiClient client;         // cria o objeto para o wifi
PubSubClient mqtt(client);// cria o objeto para o mqtt usando wifi

const String topico = "SmartRail/S4/Trem/Vel"; // nome do topico

// declaração de variaveis
const byte ledVermelho = 23;
const byte ledVerde = 24;
const byte ledAzul = 25;
const int motorA1 = 2; 
const int motorA2 = 3;  
const int motorB1 = 4;
const int motorB2 = 5;

// ----------------------- abaixo do loop/funções principais do sensor-----------------------

// funcao para controlar leds
void controladorLeds(byte velocidade){
  if(velocidade > 0) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
  } else if(velocidade < 0){
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);
  } else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, LOW);
  }
}

// funcao callback
void callback(char* topic, byte* payload, unsigned long length){
  String mensagemRecebida = "";
  for(int i = 0; i < length; i++){
    mensagemRecebida += (char) payload[i];
  }
  byte int velocidade = mensagemRecebida.toInt();
  Serial.println(mensagemRecebida);
  // fazer o controle de leds aqui
  controladorLeds(velocidade);
}

// funcao para conectar wifi
void conectarWifi(){
  Serial.println("Conectando no WiFi");
  // Loop para conectar no wifi
  while(WiFi.status() != WL_CONNECTED){
    ledcWrite(ledVermelho, 150);
    ledcWrite(ledVerde, 150);
    ledcWrite(ledAzul, 0);
    delay(1000);
  }
  Serial.println("\n Conectado com sucesso!");
}

// funcao configurar servidor do broker/porta
String configurarServidorBroker(){
  // configura o servidor do broker/porta
  mqtt.setServer(BROKER_URL, BROKER_PORT);
  Serial.println("Conectando no Broker");
  
  // cria um userID
  String userID = "S4-NATHAN"; 

  // junta o "s4-" com um numero aleatorio hexadecimal
  userID += String(random(0xffff), HEX);
  return userID;
}

// funcao para conectar broker
void conectarBroker(){
  String userID = configurarServidorBroker();
  // Loop para conectar no broker
  while(!mqtt.connected()){
    if(mqtt.connect(userID.c_str(), BROKER_USR_NAME, BROKER_USR_PASS)){
      Serial.print('.');
      ledcWrite(ledVermelho, 150);
      ledcWrite(ledAzul, 150);
      ledcWrite(ledVerde, 0);
      delay(2000);
    } else { // caso dê erro de conexao com broker
      ledcWrite(ledVermelho, 255);
      ledcWrite(ledAzul, 0);
      ledcWrite(ledVerde, 0);
      delay(2000);
    }
  }
}

// funcao do motor A para mover para frente
void paraFrenteMotorA(){
  digitalWrite(motorA2, HIGH);
  analogWrite(motorA2, 127);

  digitalWrite(motorA1, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
  delay(2000);
}

// funcao do motor B para dar re

void darReMotorB(){
  digitalWrite(motorB2, HIGH);
  analogWrite(motorB2, 127);

  digitalWrite(motorB1, LOW);
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);

  delay(2000);
}

// funcao para parar todos os motores
void pararMotores(){
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);

  delay(2000);
}

// funcao do motor A para dar re
void darReMotorA(){;
  digitalWrite(motorA2, HIGH);
  analogWrite(motorA2, 127);

  digitalWrite(motorA1, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);

  delay(2000);
}

// funcao do motor B para mover para frente
void paraFrenteMotorB(){
  digitalWrite(motorB1, HIGH);
  ledcWrite(motorB1, 127);

  digitalWrite(motorB2, LOW);
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);

  delay(2000);
}

// funcao principal
void setup() {
  //define os pinos dos motores
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  // define os motores como entrada analogico
  ledcAttach(motorA1, 5000, 8);
  ledcAttach(motorA2, 5000, 8);
  ledcAttach(motorB1, 5000, 8);
  ledcAttach(motorB2, 5000, 8);

  // define os pinos dos leds
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);

  // define os leds como entrada analogico
  ledcAttach(ledVermelho, 5000, 8);
  ledcAttach(ledVerde, 5000, 8);
  ledcAttach(ledAzul, 5000, 8);

  //configuração do wifi
  wifiClient.setInscure();
  Serial.begin(115200); // configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS); // tenta conectar na rede
  
  //  conectando no wifi
  conectarWifi();

  // conectando no broker
  conectarBroker();

  mqtt.publish(topico.c_str());
  mqtt.publish("SmartRail/S4/Trem/Vel");
  mqtt.publish("SmartRail/S4/Trem/Controle_led_rgb_s4");
  mqtt.setCallback(callback);
  Serial.println("Conectado com sucesso!");
}

void loop() {
  String mensagem = "";
  if(Serial.available() > 0){
    mensagem = Serial.readStringUntil('\n');
    Serial.print("Mensagem digitada: ");
    Serial.println(mensagem);
    mensagem = "Nathan: " + mensagem;
    //envia mensagem
    mqtt.publish("miguel123", mensagem.c_str()); 
  }  
  mqtt.loop();

  paraFrenteMotorA();
  darReMotorB();
  pararMotores();
  darReMotorA();
  paraFrenteMotorB();
  pararMotores();
  callback(byte velocidade);
}