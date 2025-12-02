 //ARQUIVO S1 - MIGUEL PAES DA SILVA - SMART RAIL (SR)

#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
#include "DHT.h"
#define DHTPIN 25
#define DHTTYPE DHT11

WiFiClientSecure wifiClient;
WiFiClient client; //Cria objeto para wifi
PubSubClient mqtt(client); //Cria objeto para MQTT usando wifi

//Conexão
const char *mqtt_broker = "1acd7ac2686b4748913cce80ad4161df.s1.eu.hivemq.cloud";
const int mqtt_port = 8883; // Porta padrão
const char *mqtt_user = "user_placa_1";
const char *mqtt_password = "Userplaca1";

//Tópicos - Publish
const String topicoTemp = "SmartRail/S1/Temp";
const String topicoUmid = "SmartRail/S1/Umidade";
const String topicoPres = "SmartRail/S1/Presenca1";

//Tópicos - Publish/Subscribe
const String topicoIlum = "SmartRail/S1/Lumn";

DHT sensorDHT(DHTPIN, DHTTYPE);

const byte pinoLdr = A0;
int valorLdr = 0;
const byte pinoTrig = 12;
const byte pinoEcho = 13;

float duracao, distancia, temperatura, umidade = 0;

//declaracao de funcoes 
// funcao que controla os leds de acordo com a temperatura
void controladorLedsTemperatura(float temperature){
  if(temperature > 25) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
    delay(1000);
  } else if(temperature < 25){
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);
    delay(1000);
  } else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, LOW);
    delay(1000);
  }  
}

// funcao que controla os leds de acordo com a umidade
void controladorLedsUmidade(float humidity){
  if(humidity > 1000) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
  } else if(humidity < 1000){
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);
  } else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, LOW);
  }
}

void sensorUmidade(float humidity){
  umidade = sensorDHT.readHumidity();
  Serial.print("Umidade [%]: ");
  Serial.print(umidade);
  client.publish(topicoUmid, umidade.c_str());
  delay(2000);
}

void sensorTemperatura(float temperature){
  temperatura = sensorDHT.readTemperature();
  Serial.print("Temperatura [C]: ");
  Serial.println(temperatura);
  client.publish(topicoTemp, temperatura.c_str());
  delay(2000);
}

void sensorLuminosidade(int valorSensorLdr){
  valorSensorLdr = analogRead(pinoLdr);
  Serial.println("Valor lido pelo LDR = ");
  Serial.println(valorSensorLdr);
  client.publish(topicoIlum, valorSensorLdr.c_str());
  delay(1000);
}

void calcularDistanciaDuracao(float distance, float duration)
{
  duration = pulseIn(pinoEcho, HIGH);
  distance = (duration*0.343)/2;
  Serial.println("Distancia: ");
  Serial.println(distance);
  client.publish(topicoPres, distance.c_str());
  delay(100);
}

// int nome[ = {}]
void callback(char* topic, byte* payload, unsigned long length) {
  String mensagemRecebida = "";
  
  // Converte payload para String
  for(int i = 0; i < length; i++) {
    mensagemRecebida += (char)payload[i];
  }
  
  Serial.print("Mensagem recebida no tópico: ");
  Serial.print(topic);
  Serial.print(" | Conteúdo: ");
  Serial.println(mensagemRecebida);
  
  // Converte para inteiro
  int valorRecebido = mensagemRecebida.toInt();
  
  // Verifica em qual tópico chegou a mensagem
  String topicoStr = String(topic);
  
  if(topicoStr == "SmartRail/S1/Lumn") {
    // Controle de luminosidade remoto
    Serial.print("Controle de iluminação remoto: ");
    Serial.println(valorRecebido);
  }
}

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
  
  // cria um nome que começa com s1-
  String userID = "S1-MIGUEL"; 

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

void setup() {
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoEcho, INPUT);
  pinMode(pinoLdr, INPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);

  // define os leds como entrada analogico
  ledcAttach(ledVermelho, 5000, 8);
  ledcAttach(ledVerde, 5000, 8);
  ledcAttach(ledAzul, 5000, 8);
  
  // configuração do wifi
  wifiClient.setInscure();
  Serial.begin(115200); //Configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS); //Tenta conectar na rede
  
  // conectando no wifi
  conectarWifi();
  //conectando no broker
  conectarBroker();

  mqtt.setCallback(callback);
  mqtt.subscribe(topicoIlum);
  Serial.println("\nConectado com sucesso ao Broker");
}

void loop() {
  mqtt.loop();

  String mensagem = "";
  if(Serial.available() > 0) {
    mensagem = Serial.readStringUntil('\n');
    Serial.print("Mensagem digitada: ");
    Serial.println(mensagem);
    mensagem = "Miguel: " + mensagem;
    mqtt.publish(TOPIC1, mensagem.c_str());
  }
 
  sensorUmidade(temperatura);
  sensorTemperatura(temperatura);
  sensorLuminosidade(valorLdr);
  calcularDistanciaDuracao(distancia, duracao);
  delay(1000);
}
