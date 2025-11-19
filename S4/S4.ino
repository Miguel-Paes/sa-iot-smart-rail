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
  // enquanto não estiver conectado no wifi mostra "." e liga o led amarelo
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
  
  // cria um nome que começa com s4-
  String userID = "S4-NATHAN"; 

  // junta o "s4-" com um numero aleatorio hexadecimal
  userID += String(random(0xffff), HEX);
  return userID;
}

// funcao para conectar broker
void conectarBroker(){
  String userID = configurarServidorBroker();
  // enquanto não estiver conectado mostra "."
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

// funcao principal
void setup() {
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

  mqtt.subscribe(topico.c_str()); // inscrever em um topico
  mqtt.subscribe("miguel123");
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
    mqtt.publish("miguel123", mensagem.c_str()); //envia msg
  }  
  mqtt.loop();
  callback(byte velocidade);
}
