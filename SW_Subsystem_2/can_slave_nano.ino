#include <SPI.h>
#include <can.h>
#include <mcp2515.h>

#define CAN_ID 0x46A
#define PROTOTYPE_CAN_ID 0x47A

#define CMD_ID_TURN_ON_LED 0x0
#define CMD_ID_TURN_OFF_LED 0x1
#define CMD_ID_GET_LED_STATUS 0x2
#define CMD_ID_GET_TEMP 0x3

// Variables globales
MCP2515 mcp2515(10);   //< Modulo CAN tiene el CS conectado a 10
struct can_frame rx_msg;

// Configuración e inicialización
void setup()
{
  Serial.begin(9600);    //< Configuración del puerto serie

  SPI.begin();    //< Comenzar la comunicación SPI
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);    //< Velocidad de transmisión y frecuencia del oscilador (por default mete 16MHZ)
  mcp2515.setFilterMask(MCP2515::MASK0, false, 0x4FF); 
  mcp2515.setFilter(MCP2515::RXF0, false, CAN_ID);
  mcp2515.setFilter(MCP2515::RXF1, false, CAN_ID);
  mcp2515.setFilterMask(MCP2515::MASK1, false, 0x4FF);
  mcp2515.setFilter(MCP2515::RXF2, false, CAN_ID);
  mcp2515.setFilter(MCP2515::RXF3, false, CAN_ID);
  mcp2515.setFilter(MCP2515::RXF4, false, CAN_ID);
  mcp2515.setFilter(MCP2515::RXF5, false, CAN_ID);
  mcp2515.setNormalMode();    //< Establecer modo de funcionamiento

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(6, OUTPUT);
  // start with the LED off
  digitalWrite(6, LOW);
}

void execute_cmd(uint8_t cmd)
{
  if (cmd == CMD_ID_TURN_ON_LED)
  {
    digitalWrite(6, HIGH);
  }
  else if (cmd == CMD_ID_TURN_OFF_LED)
  {
    digitalWrite(6, LOW);
  }
  else if (cmd == CMD_ID_GET_LED_STATUS)
  {
    bool status = digitalRead(6);
    Serial.print("LED status: ");
    Serial.print(status);
    Serial.print("\n");
  }
  else if (cmd == CMD_ID_GET_TEMP)
  {
//    tempC = analogRead(pinLM35);
//    // Calculamos la temperatura con la fórmula
//    tempC = (5.0 * tempC * 100.0)/1024.0;
//    Serial.print("Temp: ");
//    // Envia el dato al puerto serial
//    Serial.print(tempC);
//    Serial.print("\n");
  }
  else
  {
    //nothng
  }
}

// Main loop
void loop()
{
    if (mcp2515.readMessage(&rx_msg) == MCP2515::ERROR_OK)
    {
      Serial.print("CAN CMD: ");
      Serial.print(rx_msg.data[0]);
      execute_cmd(rx_msg.data[0]);
    }
}
