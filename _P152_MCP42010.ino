//#######################################################################################################
//#################################### Plugin 152: MCP42010 #############################################
//################################## I use GPIO 12 / 14 / 15 ############################################
//#######################################################################################################
// written by antibill

// Usage:
// (1): Set value to potentiometer (http://xx.xx.xx.xx/control?cmd=digipot,0,255)
// (2): Set value to potentiometer (http://xx.xx.xx.xx/control?cmd=digipot,1,0)

#include <MCP42010.h>
// https://github.com/mensink/arduino-lib-MCP42010
static float Plugin_152_PotDest[2] = {0,0};

#define PLUGIN_152
#define PLUGIN_ID_152         152
#define PLUGIN_NAME_152       "MCP42010"
#define PLUGIN_VALUENAME1_152 "DigiPot0"
#define PLUGIN_VALUENAME2_152 "DigiPot1"



boolean Plugin_152(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_152;
        Device[deviceCount].Type = DEVICE_TYPE_DUMMY;           // SPI pins for ESP8266 are CS=15, CLK=14, MOSI=13
        Device[deviceCount].Ports = 0;
        Device[deviceCount].VType = Sensor_VType::SENSOR_TYPE_DUAL;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = true;
        Device[deviceCount].ValueCount = 2;
        Device[deviceCount].SendDataOption = true;
        Device[deviceCount].TimerOption = true;
        Device[deviceCount].TimerOptional = true;
        Device[deviceCount].GlobalSyncOption = true;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_152);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_152));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_152));
        break;
      }

    case PLUGIN_GET_DEVICEGPIONAMES:
      {
        event->String1 = formatGpioName_bidirectional(F("CS"));
        event->String2 = formatGpioName_bidirectional(F("CLK"));
        event->String3 = formatGpioName_bidirectional(F("MOSI"));
        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {
        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        success = true;
        break;
      }

    case PLUGIN_INIT:
      {
        success = true;
        break;
      }

    case PLUGIN_WRITE:
      {

        String tmpString  = string;
        int argIndex = tmpString.indexOf(',');
        if (argIndex)
          tmpString = tmpString.substring(0, argIndex);

        if (tmpString.equalsIgnoreCase(F("digipot")))
        {
          int pot;
          int value;
          pot = event->Par1;
          value = event->Par2;

          MCP42010 digipot(Settings.TaskDevicePin1[event->TaskIndex], 
                           Settings.TaskDevicePin2[event->TaskIndex], 
                           Settings.TaskDevicePin3[event->TaskIndex]);
          Plugin_152_PotDest[pot] = value;
          digipot.setPot(pot,value);
          success = true;
        }


        break;
      }

    case PLUGIN_READ:
      {

        UserVar[event->BaseVarIndex + 0]= Plugin_152_PotDest[0] ;
        UserVar[event->BaseVarIndex + 1] = Plugin_152_PotDest[1] ;
        success = true;
        break;
      }


  }
  return success;
}
