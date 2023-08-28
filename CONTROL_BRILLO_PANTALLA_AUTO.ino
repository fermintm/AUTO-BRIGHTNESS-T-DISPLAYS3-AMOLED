#include <Wire.h>
#include "rm67162.h"
#include <BH1750.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();  // Inicializar la pantalla
TFT_eSprite sprite = TFT_eSprite(&tft);
BH1750 lightSensor;


void setup() {
  rm67162_init();
  lcd_setRotation(3);
  sprite.createSprite(536, 240);
  sprite.setSwapBytes(true);
  sprite.setColorDepth(16);
  sprite.setTextDatum(4);

  Wire.begin(43, 44);

  lightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);

  // Configurar el rango de brillo de la pantalla
  // tft.setBrightness(255);  // Brillo máximo inicial

  // Puedes ajustar la dirección I2C del sensor de luz si es necesario
  // lightSensor.begin(0x23); // Dirección I2C alternativa
}

void loop() {

  if (lightSensor.measurementReady()) {
    float lux = lightSensor.readLightLevel();

    if (lux > 40000.0) {
      lightSensor.setMTreg(32);
    }
    else {
      if (lux > 10.0) {
        lightSensor.setMTreg(69);
      }

      else {
        if (lux <= 0) {
          lightSensor.setMTreg(138);
        }
      }
    }

    // Mapear el valor de lux a un rango de brillo para la pantalla
    int brightness = map(lux, 0, 3000, 50, 254);  // Ajusta los valores mínimo y máximo según tus necesidades

    // Limitar el brillo a los valores válidos
    brightness = constrain(brightness, 50, 254);

    // Actualizar el brillo de la pantalla

    lcd_brightness(brightness);

    sprite.fillSprite(TFT_BLACK);

    sprite.setTextColor(TFT_WHITE);
    sprite.setTextFont(4);
    sprite.setCursor(0, 70);
    sprite.print("Light:  ");
    sprite.setTextColor(TFT_CYAN);
    sprite.print(lux , 0);
    sprite.setTextColor(TFT_WHITE);
    sprite.print(" LUX");
    sprite.pushSprite(0, 0);
    lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());

    delay(100);  // Puedes ajustar este valor para controlar la frecuencia de actualización
  }
}
