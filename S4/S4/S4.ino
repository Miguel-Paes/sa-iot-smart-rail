#include <WiFi.h>
const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

void setup() {
  Serial.begin(115200); // configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS); // tenta conectar na rede
  Serial.println("Conectando no WiFi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(200);
  }
  Serial.println("Conectado com sucesso!");
}

void loop() {
  // put your main code here, to run repeatedly:

}