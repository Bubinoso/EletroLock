#include <SPI.h> // biblioteca do SPI
#include <MFRC522.h> // biblioteca do RFID

#define SS_PIN 10
#define RST_PIN 9
#define LED_B 4 // led verde
#define LED_R 5 // led vermelho
#define releTrava 6 //pino do rele no arduino

MFRC522 mfrc522(SS_PIN, RST_PIN); // define os pinos a serem usados pelo módulo RFID

void setup()
{
  Serial.begin(9600); // inicia a comunicação para o serial
  SPI.begin(); // inicia o SPI
  mfrc522.PCD_Init(); // inicia o módulo RFID
  pinMode(LED_B, OUTPUT); // define o led verde como output
  pinMode(LED_R, OUTPUT); // define o led vermelha como output
  pinMode(releTrava, INPUT); // define o rele como input
}

void liberaAcesso()
{
  digitalWrite(releTrava, 1);
  digitalWrite(LED_B, HIGH);
  delay(3000);
  digitalWrite(LED_B, LOW);
}

void acessoNegado()
{
  Serial.println("Acesso não autorizado");
  digitalWrite(LED_R, HIGH);
  delay(3000);
  digitalWrite(LED_R, LOW);
}

void nivelAcesso1()
{
  Serial.println("Nível de acesso 1 autorizado");
  liberaAcesso();
}

void nivelAcesso2()
{
  Serial.println("Nível de acesso 2 autorizado");
  liberaAcesso();
}

void nivelAcesso3()
{
  Serial.println("Nível de acesso 3 autorizado");
  liberaAcesso();
}

void loop() {

  if (mfrc522.PICC_IsNewCardPresent()) 
  {
  }

  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  String content = ""; // variável que o UID será armazenada e verificada

  for (byte i = 0; i < mfrc522.uid.size; i++) // bloco onde converterá o valor do Serial aproximado para o UID em hexadecimal
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println();
  Serial.println("UID da Etiqueta:" + content);
  Serial.print("Mensagem : " );
  content.toUpperCase(); // coloca o conteúdo da variável em caixa alta (capslock)

  if (content.substring(1) == "XX XX XX XX")   // se o serial for esses: mostra que foi autorizado no monitor acendendo o led verde. se não: mostra que não foi autorizado e acende o led vermelho
    {
      nivelAcesso1();
    }
  else if (content.substring(1) == "YY YY YY YY")
    {
      nivelAcesso2();
    }
  else if (content.substring(1) == "ZZ ZZ ZZ ZZ")
    {
      nivelAcesso3();
    }
  else
    {
      acessoNegado();
    }
}


