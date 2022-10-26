#include <SPI.h>
#include <can.h>
#include <mcp2515.h>

// Definiciones
#define ANALOG_INPUT 0    //< pin del LM35

// LM35 //
/* El sensor LM35 es capaz de medir temperaturas entre -55°C a 150°C,
 *  1°C equivale a 10mV y soporta tensiones entre 4V y 30V.
 */

// ADC Arduino //
/* El ADC de Arduino puede leer valores entre 0 y 1023.
 *  Para obtener la temperatura es necesario hacer una conversión.
 *  Se pueden utilizar distintas fuentes para 
 *  Para tener más resolución en las mediciones se puede establecer el
 *  valor de referencia para la entrada 1023, en este caso se utilizará
 *  la referencia interna de 1V1, lo que permite medir temperaturas de
 *  hasta 110°C.
 */

 // MCP2515
 /* Se alimenta con 5V.
  *  Librería descargada de https://github.com/autowp/arduino-mcp2515
  *  
  */

// Variables globales
float temp;   //< Variable para almacenar el valor obtenido del sensor
char select;   //< Variable para almacenar el valor leído por serie
MCP2515 mcp2515(53);   //< Modulo CAN tiene el CS conectado a 53
struct can_frame tx_msg;
int led_status;   // Estado del LED 
struct can_frame rx_msg;

// Declaración de funciones
bool send_temperature_data(void);
bool send_led_status(void);
bool set_led_status_on(bool);
bool set_led_status_off(bool);

// Configuracion e inicializacion
void setup() 
{
  analogReference(INTERNAL);    //< Referencia para la entrada analógica
  Serial.begin(9600);    //< Configuración del puerto serie

  SPI.begin();    //< Comenzar la comunicación SPI
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);    //< Velocidad de transmisión y frecuencia del oscilador (por default mete 16MHZ)
  mcp2515.setNormalMode();    //< Establecer modo de funcionamiento

  pinMode(LED_BUILTIN, OUTPUT);   //< Defino pin del LED como salida
  digitalWrite(LED_BUILTIN, HIGH);   //< Apago el LED
}

// Main loop
/**
void loop() 
{
  if (send_temperature_data())
  {
    Serial.print("Temperature was sent, it value is: ");
    Serial.print(temp);
    Serial.println();
  }
  else
  {
    Serial.print("\nError!");
   }
  
  delay(500);

  if (send_led_status())
  {
    Serial.print("Led state was sent. it value is: ");
    Serial.print(led_status);
    Serial.println();
  }
  else
  {
    Serial.print("\nError!");
  }

  delay(500);
}

**/

void loop(void)
{
     if (mcp2515.readMessage(&rx_msg) == MCP2515::ERROR_OK)
    {
      Serial.print("CAN ID: ");
      Serial.print(rx_msg.can_id, HEX); // print ID
      Serial.print(" "); 
      Serial.print("CAN DLC: ");
      Serial.print(rx_msg.can_dlc, HEX); // print DLC
      Serial.print(" ");
      Serial.print("CAN DATA: ");
    
      for (int i = 0; i<rx_msg.can_dlc; i++)
      {  // print the data
        Serial.print((char)rx_msg.data[i]);
        Serial.print(" ");
      }

      Serial.println(); 
    }
}

bool send_temperature_data(void)
{
  temp = analogRead(ANALOG_INPUT);    //< Lectura del valor del sensor
  temp = (1.1 * temp * 100.0) / 1024.0;

  char temp_formatted[5];
  dtostrf(temp,3,2,temp_formatted);

  tx_msg.can_id = 0x036;
  tx_msg.can_dlc = 8;   //< Longitud de la trama en bytes
  tx_msg.data[0] = temp_formatted[0];
  tx_msg.data[1] = temp_formatted[1];
  tx_msg.data[2] = temp_formatted[2];
  tx_msg.data[3] = temp_formatted[3];
  tx_msg.data[4] = temp_formatted[4];
  tx_msg.data[5] = 0;
  tx_msg.data[6] = 0;
  tx_msg.data[7] = 0;

  MCP2515::ERROR rc = mcp2515.sendMessage(&tx_msg);

  if (rc == MCP2515::ERROR_OK)
  {
    return true;
  }
  else if (rc == MCP2515::ERROR_ALLTXBUSY) 
  {
    Serial.print("\nAll TX buffers busy!");
    return false;
  }
  else
  {
    return false;
  }
}

bool send_led_status(void)
{
  led_status = digitalRead(LED_BUILTIN);

  tx_msg.can_id = 0x046;
  tx_msg.can_dlc = 1;   //< Longitud de la trama en bytes
  tx_msg.data[0] = ((led_status == HIGH) ? '1' : '0');
  tx_msg.data[1] = '0';
  tx_msg.data[2] = '0';
  tx_msg.data[3] = '0';
  tx_msg.data[4] = '0';
  tx_msg.data[5] = '0';
  tx_msg.data[6] = '0';
  tx_msg.data[7] = '0';

  MCP2515::ERROR rc = mcp2515.sendMessage(&tx_msg);

  if (rc == MCP2515::ERROR_OK)
  {
    return true;
  }
  else if (rc == MCP2515::ERROR_ALLTXBUSY) 
  {
    Serial.print("\nAll TX buffers busy!");
    return false;
  }
  else
  {
    return false;
  }
}
