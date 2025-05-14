#include <Adafruit_NeoPixel.h>
#include <ezButton.h>

#define VRX_PIN  A1 // Arduino pin connected to VRX pin
#define VRY_PIN  A0 // Arduino pin connected to VRY pin
#define SW_PIN   2  // Arduino pin connected to SW  pin

#define NUM_LEDS 8

// Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// NEO_GRB -> Ordre des couleurs (dépend de la led adressable matérielle)
// NEO_KHZ800 -> Fréquence du signal de données (dépend de la led adressable matérielle)
Adafruit_NeoPixel horizontale(NUM_LEDS, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel verticale(NUM_LEDS, 7, NEO_GRB + NEO_KHZ800);
ezButton button(SW_PIN);

void setup() {
  Serial.begin(9600);
  button.setDebounceTime(50); // set debounce time to 50 milliseconds

  horizontale.begin();
  verticale.begin();

  horizontale.setBrightness(42);
  verticale.setBrightness(42);

  horizontale.show();
  verticale.show();

  debugLeds();
}

uint8_t get2bit(uint8_t pack, uint8_t pos){
  return (pack >> pos * 2) & 0b11; // & 0b11 => Utilisation d'un masquage
}

void set2bit(uint8_t & pack, uint8_t pos, uint8_t val){
  pack &= ~(0b11 << pos * 2);
  pack |= (0b11 & val) << pos * 2;
}

int xValue;
int yValue;
int command;

void getJoystick(uint8_t & infos)
{
  // read analog X and Y analog values
  xValue = analogRead(VRX_PIN); ; // To store value of the X axis
  yValue = analogRead(VRY_PIN); // To store value of the Y axis
  // Serial.println("xValue : " + (String)xValue + " | yValue : " + (String)yValue);
  infos = 0x00;

  // check left/right commands
  // Liste des paliers : 50 ; 195 ; 320 ; 460 | 650 ; 742 ; 950
  
  // Left
  if (xValue < 505 && xValue > 320)
    set2bit(infos, 3, 0b00);
  else if (xValue < 320 && xValue > 195)
    set2bit(infos, 3, 0b01);
  else if (xValue < 195 && xValue > 50)
    set2bit(infos, 3, 0b10);
  else if (xValue < 50)
    set2bit(infos, 3, 0b11);
  // Right
  else if (xValue < 650 && xValue > 495)
    set2bit(infos, 2, 0b00);
  else if (xValue < 742 && xValue > 650)
    set2bit(infos, 2, 0b01);
  else if (xValue < 950 && xValue > 742)
    set2bit(infos, 2, 0b10);
  else if (xValue > 950)
    set2bit(infos, 2, 0b11);

  // UP
  if (yValue < 505 && yValue > 320)
    set2bit(infos, 1, 0b00);
  else if (yValue < 320 && yValue > 195)
    set2bit(infos, 1, 0b01);
  else if (yValue < 195 && yValue > 50)
    set2bit(infos, 1, 0b10);
  else if (yValue < 50)
    set2bit(infos, 1, 0b11);
  // DOWN 850-860
  else if (yValue < 650 && yValue > 495)
    set2bit(infos, 0, 0b00);
  else if (yValue < 742 && yValue > 650)
    set2bit(infos, 0, 0b01);
  else if (yValue < 950 && yValue > 742)
    set2bit(infos, 0, 0b10);
  else if (yValue > 950)
    set2bit(infos, 0, 0b11);

  // NOTE: AT A TIME, THERE MAY BE NO COMMAND, ONE COMMAND OR TWO COMMANDS
}

void debugLeds(){
  for(int i = 0; i < NUM_LEDS; i++){
    horizontale.setPixelColor(i,random(0,255),random(0,255),random(0,255));
    verticale.setPixelColor(i,random(0,255),random(0,255),random(0,255));
    
    horizontale.show();
    delay(50);
    verticale.show();

    horizontale.setPixelColor(i,0,0,0);
    verticale.setPixelColor(i,0,0,0);
  }
  horizontale.show();
  delay(50);
  verticale.show();
}

void updateHorizontale(uint8_t & infos){
  uint32_t col = horizontale.Color(0,215,42);
    // Left
    horizontale.setPixelColor(3,255,0,0);
    if (get2bit(infos,3) == 0b00){
      horizontale.setPixelColor(0,0,0,0);
      horizontale.setPixelColor(1,0,0,0);
      horizontale.setPixelColor(2,0,0,0);
    } else if (get2bit(infos,3) == 0b01){
      horizontale.setPixelColor(0,0,0,0);
      horizontale.setPixelColor(1,0,0,0);
      horizontale.setPixelColor(2,col);
    } else if (get2bit(infos,3) == 0b10){
      horizontale.setPixelColor(0,0,0,0);
      horizontale.setPixelColor(1,col);
      horizontale.setPixelColor(2,col);
    } else if (get2bit(infos,3) == 0b11){
      horizontale.setPixelColor(0,col);
      horizontale.setPixelColor(1,col);
      horizontale.setPixelColor(2,col);
    }
    // Right
      horizontale.setPixelColor(4,255,0,0);
    if (get2bit(infos,2) == 0b00){
      horizontale.setPixelColor(5,0,0,0);
      horizontale.setPixelColor(6,0,0,0);
      horizontale.setPixelColor(7,0,0,0);
    } else if (get2bit(infos,2) == 0b01){
      horizontale.setPixelColor(5,col);
      horizontale.setPixelColor(6,0,0,0);
      horizontale.setPixelColor(7,0,0,0);
    } else if (get2bit(infos,2) == 0b10){
      horizontale.setPixelColor(5,col);
      horizontale.setPixelColor(6,col);
      horizontale.setPixelColor(7,0,0,0);
    } else if (get2bit(infos,2) == 0b11){
      horizontale.setPixelColor(5,col);
      horizontale.setPixelColor(6,col);
      horizontale.setPixelColor(7,col);
    }
    horizontale.show();
}

void updateVerticale(uint8_t & infos){
  uint32_t col = verticale.Color(0,215,42);
  // DOWN
    verticale.setPixelColor(3,255,0,0);
    if (get2bit(infos,0) == 0b00){
      verticale.setPixelColor(0,0,0,0);
      verticale.setPixelColor(1,0,0,0);
      verticale.setPixelColor(2,0,0,0);
    } else if (get2bit(infos,0) == 0b01){
      verticale.setPixelColor(0,0,0,0);
      verticale.setPixelColor(1,0,0,0);
      verticale.setPixelColor(2,col);
    } else if (get2bit(infos,0) == 0b10){
      verticale.setPixelColor(0,0,0,0);
      verticale.setPixelColor(1,col);
      verticale.setPixelColor(2,col);
    } else if (get2bit(infos,0) == 0b11){
      verticale.setPixelColor(0,col);
      verticale.setPixelColor(1,col);
      verticale.setPixelColor(2,col);
    }
    // UP
      verticale.setPixelColor(4,255,0,0);
    if (get2bit(infos,1) == 0b00){
      verticale.setPixelColor(5,0,0,0);
      verticale.setPixelColor(6,0,0,0);
      verticale.setPixelColor(7,0,0,0);
    } else if (get2bit(infos,1) == 0b01){
      verticale.setPixelColor(5,col);
      verticale.setPixelColor(6,0,0,0);
      verticale.setPixelColor(7,0,0,0);
    } else if (get2bit(infos,1) == 0b10){
      verticale.setPixelColor(5,col);
      verticale.setPixelColor(6,col);
      verticale.setPixelColor(7,0,0,0);
    } else if (get2bit(infos,1) == 0b11){
      verticale.setPixelColor(5,col);
      verticale.setPixelColor(6,col);
      verticale.setPixelColor(7,col);
    }

    verticale.show();
}

uint8_t infosMovements;
int bValue;

void loop() {
  button.loop(); // MUST call the loop() function first

  while (button.getState() == 0) {
    button.loop();
    // Serial.println("The button is pressed");
    debugLeds();
  }

  getJoystick(infosMovements);

  // Serial.println(infosMovements, BIN);

  updateHorizontale(infosMovements);
  updateVerticale(infosMovements);

}
