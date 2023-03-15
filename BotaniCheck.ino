#include <HqcDht11.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <IRremote.h>  // Include the IRremote library

#define IR_BUTTON_PLAY_PAUSE 64
#define IR_BUTTON_DOWN 7
#define IR_BUTTON_UP 9
#define IR_BUTTON_FUNC 71

String _ABVAR_1_reset = "                ";
double _ABVAR_2_humidity = 0.0 ;
Dht11 dht11_pin_6(6);


const int AirValue = 600;   //you need to replace this value with Value_1
const int WaterValue = 350;  //you need to replace this value with Value_2
int soil_moisture_value = 0;
int soil_moisture_percent = 0;


bool plant_hum_status = true;
bool plant_temp_status = true;

// IR Reciever pin
int IR_RECEIVE_PIN = 7;
String ir_input = "";

const int ORCHIDS = 1;
const int STAGHORN_FERN = 2;
const int CALATHEAS = 3;
const int AIR_PLANT = 4;
const int ALOCASIA = 5;
const int PHILODENDRON = 6;
const int SUCCULENTS = 7;
const int CACTUS = 8;
const int ALOE_VERA = 9;
const int SNAKE_PLANT = 10;

const String LOW_HUM = "low";
const String HIGH_HUM = "high";


int selected_type = 0;
int type_index = 0;


// RGB LED pins
int redPin = 8;
int greenPin = 9;
int bluePin = 10;

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

String getPlantNameByType(int type) {
    switch (type) {
      case ORCHIDS: {
        return "Orchids         ";
        break;
      }
      case STAGHORN_FERN: {
        return "Staghorn fern   ";
        break;
      }
      case CALATHEAS: {
        return "Calatheas       ";
        break;
      }
      case ALOCASIA: {
        return "Alocasia        ";
        break;
      }
      case AIR_PLANT: {
        return "Air plants      ";
        break;
      }
      case PHILODENDRON: {
        return "Philodendron    ";
        break;
      }
      case SUCCULENTS: {
        return "Succulents      ";
        break;
      }
      case CACTUS: {
        return "Cactus          ";
      }
      case ALOE_VERA: {
        return "Aloe Vera       ";
        break;
      }
      case SNAKE_PLANT: {
        return "Snake Plant     ";
        break;
      }
      default: {
        return "";
      }
    }
}

String getPlantHumidity(int type, double value) {
    switch (type) {
      case ORCHIDS: {
        return value < 60 ? LOW_HUM : (value > 80 ? HIGH_HUM : "");
        break;
      }
      case STAGHORN_FERN: {
        return value < 70 ? LOW_HUM : (value > 80 ? HIGH_HUM : "");
        break;
      }
      case CALATHEAS:
      case ALOCASIA: {
        return value < 50 ? LOW_HUM : (value > 60 ? HIGH_HUM : "");
        break;
      }
      case AIR_PLANT: {
        return value < 50 ? LOW_HUM : (value > 70 ? HIGH_HUM : "");
        break;
      }
      case PHILODENDRON: {
        return value < 60 ? LOW_HUM : (value > 70 ? HIGH_HUM : "");
        break;
      }
      case SUCCULENTS:
      case CACTUS:
      case ALOE_VERA: {
        return value < 40 ? LOW_HUM : (value > 50 ? HIGH_HUM : "");
        break;
      }
      case SNAKE_PLANT: {
        return value < 30 ? LOW_HUM : (value > 50 ? HIGH_HUM : "");
        break;
      }
      default: {
        return "";
      }
    }
}


String getPlantMoisture(int type, double value) {
    switch (type) {
      case ORCHIDS: {
        return value < 60 ? LOW_HUM : (value > 80 ? HIGH_HUM : "");
        break;
      }
      case STAGHORN_FERN: {
        return value < 70 ? LOW_HUM : (value > 80 ? HIGH_HUM : "");
        break;
      }
      case CALATHEAS:
      case ALOCASIA: {
        return value < 50 ? LOW_HUM : (value > 60 ? HIGH_HUM : "");
        break;
      }
      case AIR_PLANT: {
        return value < 50 ? LOW_HUM : (value > 70 ? HIGH_HUM : "");
        break;
      }
      case PHILODENDRON: {
        return value < 60 ? LOW_HUM : (value > 70 ? HIGH_HUM : "");
        break;
      }
      case SUCCULENTS:
      case CACTUS:
      case ALOE_VERA: {
        // updated
        return value < 21 ? LOW_HUM : (value > 40 ? HIGH_HUM : "");
        break;
      }
      case SNAKE_PLANT: {
        return value < 30 ? LOW_HUM : (value > 50 ? HIGH_HUM : "");
        break;
      }
      default: {
        return "";
      }
    }
}

double _ABVAR_3_temperature = 0.0 ;
// For these LCD controls to work you MUST replace the standard LCD library with 'New LCD' from...
// https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home
// Direct download https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/LiquidCrystal_V1.2.1.zip
// Your project will not compile until this is done.
//
//                             RS  EN  d0  d1  d2  d3  LED 
LiquidCrystal lcd_I2C_Parallel(12, 11,  5,  4,  3,  2,  7, POSITIVE);
void __ardublockDigitalWrite(int pinNumber, boolean status)
{
  pinMode(pinNumber, OUTPUT);
  digitalWrite(pinNumber, status);
}

void setup()
{
  // Setup humidity and temperature sensor
  dht11_pin_6.init();

  // Setup LCD monitor
  Serial.begin(9600);

  lcd_I2C_Parallel.begin (16, 2);
  lcd_I2C_Parallel.setBacklight(HIGH);

  // Setup the RGB LED
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Setup IR Reciever
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  selected_type = 0;
  type_index = 0;
}

void loop()
{ 
  soil_moisture_value = analogRead(A0);  //put Sensor insert into soil
  soil_moisture_percent = map(soil_moisture_value, AirValue, WaterValue, 0, 100);
  Serial.println("----------");
  Serial.println(soil_moisture_percent);

  // ---------------
  ir_input = "";

  // turn of the light if plant type is not selected
  if (selected_type == 0) {
    setColor(0, 0, 0);
  }
 
  if (selected_type == 0 && type_index == 0) {
      lcd_I2C_Parallel.setCursor( (1) - 1, (1) - 1 );
      lcd_I2C_Parallel.print( "Use arrows to   " );
      lcd_I2C_Parallel.setCursor( (1) - 1, (2) - 1 );
      lcd_I2C_Parallel.print( "select plant    " );
  }
  
  // handle IR Reciever
  if (IrReceiver.decode()) {
    int command = IrReceiver.decodedIRData.command;
    // Serial.println(IrReceiver.decodedIRData.command);
    switch (command) {
      case IR_BUTTON_PLAY_PAUSE: {
        Serial.println("Pressed on button play/pause");
        if (type_index != 0) {
          selected_type = type_index; 
          type_index = 0;
        }
        break;
      }
      case IR_BUTTON_UP: {
        Serial.println("Pressed on button up");
        type_index = (type_index + 1) % 11;
        if (type_index == 0) {
          type_index = 1;
        }
        break;
      }
      case IR_BUTTON_DOWN: {
        Serial.println("Pressed on button down");
        type_index = (type_index - 1) % 11; 
        if (type_index <= 0) {
          type_index = 1;
        }
        break;
      }
      case IR_BUTTON_FUNC: {
        Serial.println("Pressed on button func");
        type_index = 0;
        selected_type = 0;
      }
      default: {
        Serial.println(command);
        Serial.println("Button not recognized");
      }
    }
    IrReceiver.resume();
  }

  if (type_index != 0) {
    lcd_I2C_Parallel.setCursor( (1) - 1, (1) - 1 );
    lcd_I2C_Parallel.print(getPlantNameByType(type_index));
    lcd_I2C_Parallel.setCursor( (1) - 1, (2) - 1 );
    lcd_I2C_Parallel.print("Set using \"play\"");
  }

  // do not continue if the plant type is not selected
  if (selected_type == 0) {
    return;
  }

  _ABVAR_2_humidity = dht11_pin_6.getHumidity() ;
  _ABVAR_3_temperature = dht11_pin_6.getTemperature() ;

  lcd_I2C_Parallel.setCursor( (1) - 1, (1) - 1 );
  String current_humidity_level = getPlantHumidity(selected_type, _ABVAR_2_humidity);
  String current_moisture_level = getPlantMoisture(selected_type, soil_moisture_percent);

  if (current_moisture_level == LOW_HUM) {
    lcd_I2C_Parallel.print( "I'm thirsty :(  " );
    plant_hum_status = false;
  } else if (current_moisture_level == HIGH_HUM) {
    lcd_I2C_Parallel.print( "Too much water  " );
    plant_hum_status = false;
  } else {
    lcd_I2C_Parallel.print( "I'm fine :)     ");
    plant_hum_status = true;
  }

  lcd_I2C_Parallel.setCursor( (1) - 1, (2) - 1 );
  if ( _ABVAR_3_temperature < 10 )
  {
    lcd_I2C_Parallel.print( "I'm cold :(     " );
    plant_temp_status = false;
  } else if (_ABVAR_3_temperature > 30) {
    lcd_I2C_Parallel.print( "I's hot :(      " );
    plant_temp_status = false;
  } else {
    lcd_I2C_Parallel.print( "Temperature OK  ");
    plant_temp_status = true;
  }

  if (plant_hum_status && plant_temp_status) {
    setColor(0, 255, 0); // green
  } else {
    setColor(255, 0, 0); // red
  }
}
