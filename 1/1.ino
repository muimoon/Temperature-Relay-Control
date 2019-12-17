//Programa: Arduino e DHT11 controle de rele por temperatura
//Autor: Arduino e Cia
//MODIFY ON 12/17/2019 BY MIU
//RELAY1,2,3,4 ARRANGE TO 8,9,10,11
//RELAY1,2 FOR HIGH TEMPERATURA(temp_SET MUST OVER THAN 20)
//RELAY3,4 FOR LOW TEMPERATURA(temp_SET MUST LOW THAN 15)

#include "U8glib.h"
#include <DHT.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

int posicao = 0;
int temp_SET = 10;

#define pino_aumenta 5
#define pino_diminui 4
#define pino_rele1 8
#define pino_rele2 9
#define pino_rele3 10
#define pino_rele4 11

#define pino_DHT A0
#define DHTTYPE DHT11

DHT dht(pino_DHT, DHTTYPE);

int temperatura;
int aumenta, diminui;

unsigned long previousMillis = 0;
//Intervalo de medicao temperatura
const long interval = 2000;

void draw()
{
  //Retangulo temperatura atual
  u8g.drawRFrame(0, 17, 84, 46, 2);
  u8g.setFont(u8g_font_fub20);
  //Atualiza a temperatura no display
  u8g.setPrintPos(13, 50);
  u8g.print(temperatura);
  //Circulo grau
  u8g.drawCircle(51, 33, 3);
  u8g.drawStr( 56, 50, "C");

  //Box superior amarelo
  u8g.setFont(u8g_font_8x13B);
  u8g.drawRBox(0, 0, 127, 16, 2);
  u8g.setColorIndex(0);
  u8g.drawStr( 21, 13, "TEMPERATURA");
  u8g.setColorIndex(1);

  //box temperatura maxima
  u8g.drawRFrame(86, 17, 42, 46, 2);
  if (temp_SET <= temperatura && temp_SET>=20)
  {
    //Temperatura maior do que a temp. maxima
    u8g.drawRBox(86, 17, 42, 22, 2);
    u8g.setColorIndex(0);
    u8g.drawStr(96, 33, "SET");
    u8g.setColorIndex(1);
    //Aciona saida do rele
    digitalWrite(pino_rele1, HIGH);
    digitalWrite(pino_rele2, HIGH);
    digitalWrite(pino_rele3, LOW);
    digitalWrite(pino_rele4, LOW);
  }
  else if (temp_SET >= temperatura && temp_SET<=15)
    {
    //Temperatura maior do que a temp. maxima
    u8g.drawRBox(86, 17, 42, 22, 2);
    u8g.setColorIndex(0);
    u8g.drawStr(96, 33, "SET");
    u8g.setColorIndex(1);
    //Aciona saida do rele
    digitalWrite(pino_rele1, LOW);
    digitalWrite(pino_rele2, LOW);
    digitalWrite(pino_rele3, HIGH);
    digitalWrite(pino_rele4, HIGH);
  }	
  else
  {
    //Temperatura menor do que a temp. maxima
    u8g.drawRFrame(86, 17, 42, 22, 2);
    u8g.drawStr(96, 33, "SET");
    //Desliga saida do rele
    digitalWrite(pino_rele1, LOW);
    digitalWrite(pino_rele2, LOW);
    digitalWrite(pino_rele3, LOW);
    digitalWrite(pino_rele4, LOW);
  }
  //Atualiza na tela o valor da temp. maxima
  u8g.setPrintPos(100, 55);
  u8g.print(temp_SET);
  u8g.drawCircle(120, 47, 2);
  u8g.setColorIndex(1);
}

void setup(void)
{
  Serial.begin(9600);
  pinMode(pino_rele1, OUTPUT);
  pinMode(pino_rele2, OUTPUT);
  pinMode(pino_rele3, OUTPUT);
  pinMode(pino_rele4, OUTPUT);
  pinMode(pino_aumenta, INPUT_PULLUP);
  pinMode(pino_diminui, INPUT_PULLUP);
  dht.begin();
}

void loop(void)
{
  unsigned long currentMillis = millis();
  //Timer para ler o valor da temperatura
  if (currentMillis - previousMillis >= interval)
  {
    temperatura = dht.readTemperature();
    previousMillis = currentMillis;
  }

  //Testa botao aumenta temperatura
  aumenta = digitalRead(pino_aumenta);
  if (aumenta == 0)
  {temp_SET++;}
  while (digitalRead(pino_aumenta) == 0)
  {delay(10);}

  //testa botao diminui temperatura
  diminui = digitalRead(pino_diminui);
  if (diminui == 0)
  {temp_SET--;}
  while (digitalRead(pino_diminui) == 0)
  {delay(10);}

  u8g.firstPage();
  // Chama a rotina de desenho na tela
  do
  {
    draw();
  }
  while ( u8g.nextPage() );
  delay(50);
}
