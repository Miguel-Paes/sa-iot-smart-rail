//ARQUIVO S1 - MIGUEL PAES DA SILVA - SMART RAIL (SR)

#include <WiFi.h>
const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

void setup() {
  Serial.begin(115200); //Configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS); //Tenta conectar na rede
  Serial.println("Conectando ao WiFI");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
    Serial.println("Conectado com Sucesso!");
}

void loop() {

}
