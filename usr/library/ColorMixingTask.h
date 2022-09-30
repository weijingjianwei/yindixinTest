#ifndef COLOR_MIXING_H_
#define COLOR_MIXING_H_

#include <stdint.h>
#include <stdbool.h>

#include <appConfig.h>
#include <softTimerTask.h>
#include <taskManager.h>
#include <colorFormula.h>

typedef enum{
  FADING_MODE_DERIVATIVE = 0,   /* Derivative Mode, the intensity would change in nonlinear mode; the intensity changes is softer*/
  FADING_MODE_PROPORATION,      /* Proportional mode��theintensity linear dimming*/
}FadingMode_t;


typedef struct{
  uint8_t device;              /* 1: means realplum(IND83205) , 2: rugby(IND83204)  3: realplumPro(IND82209),4: rugbyPro*/
  uint8_t ledDriverType;       /* 0: meta RGB LED  others: multi-leds with time division */
  uint16_t version;            /* main Version 0x0102 means V1.02       */
}LibVersionInfo_t;


typedef enum{
  COLOR_MODE_PWM = 0,
  COLOR_MODE_RGB,
  COLOR_MODE_HSL,
  COLOR_MODE_RGBL,
  COLOR_MODE_sRGBL,
  COLOR_MODE_XYY,
  COLOR_MODE_LUV,
  COLOR_MODE_ACCURATE_RGBL,
  COLOR_MODE_ACCURATE_sRGBL,
  COLOR_MODE_ACCURATE_XYY,
  COLOR_MODE_ACCURATE_LUV,
}ColorMode_t;

typedef struct{
  ColorMode_t  colorMode;
  union{
      struct{
        uint16_t pwmR;
        uint16_t pwmG;
        uint16_t pwmB;
      }PWM;
      struct{ 
        uint8_t red;
        uint8_t green;
        uint8_t blue;
      }RGB;
      struct{
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint16_t level;
      }RGBL;
      struct{
        uint16_t hue;
        uint8_t saturation;
        uint16_t level;
      }HSL;
      struct{
        uint16_t x;
        uint16_t y;
        uint16_t Y;
      }XYY;
      struct{
        uint16_t u;
        uint16_t v;
        uint16_t L;
      }LUV;
  };
  IntensityRatio_t iRatio; /*   intensity ratio for RGB*/
}ColorParam_t;

/*
 *  get color mixing library version 
 *  @return LibVersionInfo_t
 */
LibVersionInfo_t CLM_GetColorLibraryVersion(void);

/*
 * set Fading mode 
 *  @param [in] FADING_MODE_DERIVATIVE: derivative mode more accurate at low brightness, FADING_MODE_PROPORATION: proporation in whole range
 *  @return none
 */
void CLM_SetFadingMode(FadingMode_t mode);

/*
 *  Set max limited intensity level for all of color control commands excepts CLM_SetPWMs command, this function often is used to limit the led brightness when Led is hot for led over hot protection.
 *  @param [in] intensity: 0-1024 ,means 0% -100%
 *  @return 0
 */
int8_t CLM_SetLimitedMaxIntensity(uint16_t intensity);

/*
 *  Get max limited intensity level see "CLM_SetLimitedMaxIntensity" function for the specific function introduction .
 *  @param [out] intensity: 0-1024 ,means 0% -100%
 *  @return 0
 */
int8_t CLM_GetLimitedMaxIntensity(uint16_t *intensity);


/*
 *  Set accurate color xyY output with calibration parameters
 *  @param [in] iRatio:0-255 for 100%-125% of max Intensity, for accurate calibrated color Intensity compensation, set all 0 for normal color control
 *  @param [in] temperatureR,G,B: -40-100 in 1C
 *  @param [in] x: 0-65535
 *  @param [in] y: 0-65535
 *  @param [in] Y: 0-100
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetAccurateXYY(LedNum_t ledIndex,IntensityRatio_t iRatio, ColorTemperature_t temperature, uint16_t x, uint16_t y, uint8_t Y, uint16_t transitionTime);

/*
 *  Set accurate color L'u'v output with calibration parameters
 *  @param [in] iRatio:0-255 for 100%-125% of max Intensity, for accurate calibrated color Intensity compensation, set all 0 for normal color control
 *  @param [in] temperatureR,G,B: -40-100 in 1C
 *  @param [in] u': 0-65535
 *  @param [in] v': 0-65535
 *  @param [in] level: 0-100
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetAccurateLUV(LedNum_t ledIndex,IntensityRatio_t iRatio, ColorTemperature_t temperature, uint16_t u, uint16_t v, uint8_t level, uint16_t transitionTime);
/*
 *  Set accurate color RGBL output with calibration parameters
 *  @param [in] iRatio:0-255 for 100%-125% of max Intensity, for accurate calibrated color Intensity compensation, set all 0 for normal color control
 *  @param [in] temperatureR,G,B: -40-100 in 1C
 *  @param [in] red: 0-255
 *  @param [in] green: 0-255
 *  @param [in] blue: 0-255
 *  @param [in] level: 0-100
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetAccurateRGBL(LedNum_t ledIndex,IntensityRatio_t iRatio, ColorTemperature_t temperature, uint8_t red, uint8_t green, uint8_t blue,uint8_t level, uint16_t transitionTime);

/*
 *  Set Accurate sRGBL output
 *  @param [in] iRatio:0-255 for 100%-125% of max Intensity, for accurate calibrated color Intensity compensation, set all 0 for normal color control
 *  @param [in] temperatureR,G,B: -40-100 in 1C
 *  @param [in] red: 0-255
 *  @param [in] green: 0-255
 *  @param [in] blue: 0-255
 *  @param [in] level: 0-100
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetAccurateSRGBL(LedNum_t ledIndex,IntensityRatio_t iRatio, ColorTemperature_t temperature, uint8_t red, uint8_t green, uint8_t blue,uint8_t level, uint16_t transitionTime);


/*
 *  Set xyY output 
 *  @param [in] temperatureR,G,B: -40-100 in 1C
 *  @param [in] x: 0-65535
 *  @param [in] y: 0-65535
 *  @param [in] Y: 0-1023
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetXYY(LedNum_t ledIndex,ColorTemperature_t temperature, uint16_t x, uint16_t y, uint16_t Y, uint16_t transitionTime);

/*
 *  Set Lu'v' output 
 *  @param [in] temperatureR,G,B: -40-100 in 1C
 *  @param [in] u': 0-65535
 *  @param [in] v': 0-65535
 *  @param [in] level: 0-1023
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetLUV(LedNum_t ledIndex,ColorTemperature_t temperature, uint16_t u, uint16_t v, uint16_t level, uint16_t transitionTime);
/*
 *  Set RGBL output 
 *  @param [in] temperatureR,G,B: -40-100 in 1C
 *  @param [in] red: 0-255
 *  @param [in] green: 0-255
 *  @param [in] blue: 0-255
 *  @param [in] level: 0-1023
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetRGBL(LedNum_t ledIndex,ColorTemperature_t temperature, uint8_t red, uint8_t green, uint8_t blue,uint16_t level, uint16_t transitionTime);

/*
 *  Set sRGBL output 
 *  @param [in] temperatureR,G,B: -40-100 in 1C
 *  @param [in] red: 0-255
 *  @param [in] green: 0-255
 *  @param [in] blue: 0-255
 *  @param [in] level: 0-1023
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetSRGBL(LedNum_t ledIndex,ColorTemperature_t temperature, uint8_t red, uint8_t green, uint8_t blue,uint16_t level, uint16_t transitionTime);


/*
 *  Set hue saturation level output 
 *  @param [in] temperatureR,G,B: -40-100 in 1C
 *  @param [in] hue: 0-65535
 *  @param [in] saturation: 0-255
 *  @param [in] level: 0-1023
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetHSL(LedNum_t ledIndex,ColorTemperature_t temperature, uint16_t hue, uint8_t saturation, uint16_t level, uint16_t transitionTime);
/*
 *  Set RGB output 
 *  @param [in] temperatureR,G,B: -40-100 in 1C
 *  @param [in] red: 0-255
 *  @param [in] green: 0-255
 *  @param [in] blue: 0-255
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetRGB(LedNum_t ledIndex,ColorTemperature_t temperature, uint8_t red, uint8_t green, uint8_t blue, uint16_t transitionTime);

/*
 * Directly Set PWMs output , only for test purpose,don't use for color mixing parameters
 *  @param [in] PWM_R: 0-65535 Red   Channel PWM value
 *  @param [in] PWM_G: 0-65535 Green Channel PWM value
 *  @param [in] PWM_B: 0-65535 Blue  Channel PWM value
 *  @param [in] transitionTime: 0-65535ms
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetPWMs(LedNum_t ledIndex,uint16_t pwmR, uint16_t pwmG, uint16_t pwmB, uint16_t transitionTime);

/*
 * Directly Get current PWMs value 
 *  @param [out] PWM_R: 0-65535 Red   Channel PWM value
 *  @param [out] PWM_G: 0-65535 Green Channel PWM value
 *  @param [out] PWM_B: 0-65535 Blue  Channel PWM value
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_GetCurrentPWMs(LedNum_t ledIndex,uint16_t *pwmR, uint16_t *pwmG, uint16_t *pwmB);

/*
 * Get current Fading  status, color compensition only runs when fading is finished.
 *  @param [out] 0: fading is runing or invalid ledIndex, 1: fading is finished
 */
uint8_t CLM_FadingIsFinished(LedNum_t ledIndex);

/*
 * Get current color control parameters , this function is purpose to use for color compensation together with CLM_FadingIsFinished function.
 *  @param [out] ColorParam_t, read color mode then get Color Param.
 *  @return Real point. 0 :FAIL
 */ 
ColorParam_t const *CLM_GetCurrentColorParams(LedNum_t ledIndex);


/*
 * Set Led physical param : xy coordinate, intensity in Lumin (for example)
 *  @param [in] type: LED type
 *  @param [in] temperatureR,G,B: -40-100 in 1C
 *  @param [in] coordinate: Led coordinate for example:P(0.3333,0.3333) = P(21845,21845);
 *  @param [in] intensity: 0-65535 in mcd (for example)
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetLedPhyParams(LedNum_t ledIndex,ColorTemperature_t temperature,LedcolorParam_t *param);

/*
 * Get Led physical param : x,y coordinate, intensity in Lumin (for example)
 *  @param [in] type: LED type
 *  @param [in] temperatureR,G,B: current temperature -40-100 in 0.1C
 *  @param [Out] coordinate: Led coordinate for example:P(0.3333,0.3333) = P(21845,21845);
 *  @param [Out] intensity: 0-65535 in Lumin (for example)
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_GetLedPhyParams(LedNum_t ledIndex,ColorTemperature_t temperature,LedcolorParam_t *param);

/*
 *  In order to make the stability of light intensity  in different temperature, Red has maximum intensity degration
 *  @param [in] redIntensity:   0-65535 in Lumin (for example)
 *  @param [in] greenIntensity: 0-65535 in Lumin (for example)
 *  @param [in] blueIntensity:  0-65535 in Lumin (for example)
 *  @return 0
 */
int8_t CLM_SetMinimumIntensity(uint16_t redIntensity,uint16_t greenIntensity,uint16_t blueIntensity );

/*
 *  In order to make the stability of light intensity  in different temperature
 *  @param [out] redIntensity:   0-65535 in Lumin (for example)
 *  @param [out] greenIntensity: 0-65535 in Lumin (for example)
 *  @param [out] blueIntensity:  0-65535 in Lumin (for example)
 *  @return 0
 */
int8_t CLM_GetMinimumIntensity(uint16_t *redIntensity,uint16_t *greenIntensity,uint16_t *blueIntensity);

/*
 * Set calibration point physical param : x,y coordinate
 *  @param [in] coordinate: Led whitePoint coordinate for example:P(0.3333,0.3333) = P(21845,21845);
 *  @return 0
 */
int8_t CLM_SetWhitePointParams(Coordinate_t const *coordinate);

/*
 * Get calibration point physical param : x,y coordinate
 *  @param [in] coordinate: Led coordinate for example:P(0.3333,0.3333) = P(21845,21845);
 *  @return coordinate
 */
uint8_t CLM_GetWhitePointParams(Coordinate_t *coordinate);

/*
 * Set Led PN volt @specifical temperature in mV
 *  @param [in] temperatureR,G,B:  -40-100 in 1C
 *  @param [in] volt_R: Red Led PN volt (mV);
 *  @param [in] volt_G: Green Led PN volt (mV);
 *  @param [in] volt_B: Blue Led PN volt (mV);
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetLedPNVolts(LedNum_t ledIndex,ColorTemperature_t temperature, int16_t volt_R, int16_t volt_G, int16_t volt_B);

/*
 * Get Led PN volt @25C in mV
 *  @param [in] temperatureR,G,B: -40-100 in 1C
 *  @param [out] volt_R: Red Led PN volt (mV);
 *  @param [out] volt_G: Green Led PN volt (mV);
 *  @param [out] volt_B: Blue Led PN volt (mV);
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_GetLedPNVolts(LedNum_t ledIndex,ColorTemperature_t temperature, int16_t *volt_R, int16_t *volt_G, int16_t *volt_B);


/*
 *  Set typical max red, green and blue intensity
 *  @param [in]  ledIndex : led index
 *  @param [in] redIntensity:   0-65535 in Lumin (for example)
 *  @param [in] greenIntensity: 0-65535 in Lumin (for example)
 *  @param [in] blueIntensity:  0-65535 in Lumin (for example)
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetTypicalMaxIntensity(LedNum_t ledIndex,uint16_t redIntensity,uint16_t greenIntensity,uint16_t blueIntensity);

/*
 *  get typical max red, green and blue intensity
 *  @param [in]  ledIndex : led index
 *  @param [out] redIntensity:   0-65535 in Lumin (for example)
 *  @param [out] greenIntensity: 0-65535 in Lumin (for example)
 *  @param [out] blueIntensity:  0-65535 in Lumin (for example)
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_GetTypicalMaxIntensity(LedNum_t ledIndex,uint16_t *redIntensity,uint16_t *greenIntensity,uint16_t *blueIntensity);

/*
 *  whitePoint intensity calculating, the room temperature is MUST when doing white point calculating.
 *  @param [in] temperatureR,G,B: -40-100 in 1C
 *  @param [in] whitePoint: white point coordinatefor example:P(0.3333,0.3333) = P(21845,21845);
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_CalculatingWhitePoint(LedNum_t ledIndex,ColorTemperature_t temperature,Coordinate_t *whitePoint);

/*
 * Note that obsolute function, don't use in new project
 * White Point Calibration: Set Led PWM parameters for calculating max intensity of R,G,B
 *  @param [in] maxPWM_R: at White Point
 *  @param [in] maxPWM_G: at White Point
 *  @param [in] maxPWM_B: at White Point
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_SetWhitePointPWMs(LedNum_t ledIndex,uint16_t maxPWM_R,uint16_t maxPWM_G,uint16_t maxPWM_B);

/*
 * Note that obsolute function, don't use in new project
 * White Point Calibration: Get Led max PWM parameters of R,G,B
 *  @param [out] maxPWM_R: at White Point
 *  @param [out] maxPWM_G: at White Point
 *  @param [out] maxPWM_B: at White Point
 *  @return 0:OK. others :FAIL
 */
uint8_t CLM_GetWhitePointPWMs(LedNum_t ledIndex,uint16_t *maxPWM_R,uint16_t *maxPWM_G,uint16_t *maxPWM_B);

/*
 * storage color settings to flash
 *  @return 0,OK,others:NG
 */
int8_t CLM_StoragColorParams(void);


void CLM_TaskHandler(void);
void CLM_Init(void);

#endif