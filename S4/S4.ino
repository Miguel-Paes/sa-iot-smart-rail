#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"

WiFiClientSecure wifiClient;
WiFiClient client;         // cria o objeto para o wifi
PubSubClient mqtt(client);// cria o objeto para o mqtt usando wifi

const String mensagem_ligar[6] = {"Acender", "Ligar", "On", "1", "True"};
const String mensagem_ligar[5] = {"Apagar", "Desligar", "Off", "0", "False"};

const String topico = "TopicoNathan"; // nome do topico

void setup() {
  wifiClient.setInscure();
  Serial.begin(115200); // configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS); // tenta conectar na rede
  Serial.println("Conectando no WiFi");

  // enquanto não estiver conectado mostra "."
  while(WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(200);
  }
  Serial.println("\n Conectado com sucesso!");

  // configura o servidor do broker/porta
  mqtt.setServer(BROKER_URL, BROKER_PORT);
  Serial.println("Conectando no Broker");
  
  // cria um nome que começa com s4-
  String userID = "S4-NATHAN"; 

  // junta o "s4-" com um numero aleatorio hexadecimal
  userID += String(random(0xffff), HEX);

  // tenta conectar ao Broker
  // enquanto não estiver conectado mostra "."
  while(!mqtt.connected()){
    mqtt.connect(userID.c_str(), BROKER_USR_NAME, BROKER_USR_PASS);
    Serial.print('.');
    delay(2000);
  }

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
}

// ----------------------- abaixo do loop-----------------------
void callback(char* topic, byte* payload, unsigned long length){
  String mensagemRecebida = "";
  for(int i = 0; i < length; i++){
    mensagemRecebida += (char) payload[i];
  }
  Serial.println(mensagemRecebida);
  // fazer o controle aqui


}
