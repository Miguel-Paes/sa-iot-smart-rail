#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client;         // cria o objeto para o wifi
PubSubClient mqtt(client);// cria o objeto para o mqtt usando wifi

// define o nome da rede
const String SSID = "FIESC_IOT_EDU";
// define a senha da rede
const String PASS = "8120gv08";

// define o endereço do broker
const String brokerURL = "test.mosquitto.org";
// define a porta do broker
const int brokerPort = 1883;

// usuário e senha que farão conexão com o servidor
const String brokerUser = ""; // variavel para o user do broker
const String brokerPass = ""; // variavel para a senha do borker

void setup() {
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
  mqtt.setServer(brokerURL.c_str(), brokerPort);
  Serial.println("Conectando no Broker");
  
  // cria um nome que começa com s4-
  String boardID = "S4-"; 

  // junta o "s4-" com um numero aleatorio hexadecimal
  boardID += String(random(0xffff), HEX);

  // tenta conectar ao Broker
  // enquanto não estiver conectado mostra "."
  while(!mqtt.connect(boardID.c_str())){
    Serial.print(".");
    delay(200);
  }
  Serial.println("Conectado com sucesso!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
