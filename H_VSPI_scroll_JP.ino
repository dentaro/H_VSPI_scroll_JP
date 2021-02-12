//文字表示には、mgo-tecさんのライブラリが必要です
#include "ESP32_SPIFFS_ShinonomeFNT.h"
#include "ESP32_SPIFFS_UTF8toSJIS.h"

//こちらは、でんたろう改造ライブラリです。（もとはmgo-tecさんの）
#include "ESP32_LCD_ILI9341_SPI.h"
 
const char* UTF8SJIS_file = "/font/Utf8Sjis.tbl"; //UTF8 Shift_JIS 変換テーブルファイル名を記載しておく
const char* Shino_Zen_Font_file = "/font/shnmk16.bdf"; //全角フォントファイル名を定義
const char* Shino_Half_Font_file = "/font/shnm8x16.bdf"; //半角フォントファイル名を定義
 
const uint8_t CS_SD = 5;//4VSPI //SD card CS ( Chip Select )

//VSPI(KIIRO,DOMO)
const uint8_t SCLK_OLED =  18; //SCLK
const uint8_t MOSI_OLED =  23; //MOSI (Master Output Slave Input)
const uint8_t MISO_OLED =  19; //これは実際は使っていない。MISO (Master Input Slave Output)
const uint8_t CS_OLED = 22;
const uint8_t DC_OLED =  17; //OLED DC(Data/Command)
const uint8_t RST_OLED =  16; //OLED Reset

//HSPI(BENI)
//const uint8_t SCLK_OLED =  14; //SCLK
//const uint8_t MOSI_OLED =  13; //MOSI (Master Output Slave Input)
//const uint8_t MISO_OLED =  12; //これは実際は使っていない。MISO (Master Input Slave Output)
//const uint8_t CS_OLED = 15;
//const uint8_t DC_OLED =  4; //OLED DC(Data/Command)
//const uint8_t RST_OLED =  2; //OLED Reset

const uint8_t lcd_led_pin = 32;
 
ESP32_LCD_ILI9341_SPI LCD(SCLK_OLED, MISO_OLED, MOSI_OLED, CS_OLED, DC_OLED, RST_OLED, lcd_led_pin);

ESP32_SPIFFS_ShinonomeFNT SFR;

String test_str[2];
uint8_t test_buf[2][80][16] = {};
uint16_t test_sj_length[2];
 
uint8_t Scl_Buf[ 2 ][ 16 ][ 640 ] = {};
 
//red (0-31), green (0-63), blue (0-31)
uint8_t red = 31, green = 63, blue = 31;
uint8_t V_size = 1, H_size = 1;
uint8_t num;

void setup() {

  Serial.begin(115200);
 
  //※この関数を使う場合はStrDirect_ShinoFNT_readALL関数を使う場合限定。
  //３つのファイルを同時に開くため、後でクローズすることが必要。
  
  SFR.SPIFFS_Shinonome_Init3F(UTF8SJIS_file, Shino_Half_Font_file, Shino_Zen_Font_file); //ライブラリ初期化。3ファイル同時に開く
 
  //LCD.ILI9341_Init(true, 70000000);//VSPI
  LCD.ILI9341_Init(true, 100000000);//HSPI
  LCD.Disp_Rotation(253);
  LCD.Display_Clear(0, 0, 239, 319);
  LCD.Brightness(255); //LCD LED Full brightness
 
  num = 0;
  test_str[num] = "Ｍ５ｓｔａｃｋで 16×16 pixel 日本語漢字（東雲フォント）の表示できた～～！　";
  num = 1;
  test_str[num] = "半角もいけます。ｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿｧｨｩｪｫｬｭｮ 1234567890 ABCDEFG abcdef";
 
  for(num = 0; num < 2; num++){
    test_sj_length[num] = SFR.StrDirect_ShinoFNT_readALL(test_str[num], test_buf[num]);
    Serial.printf("test_sj_length[%d] = %d\r\n", num, test_sj_length[num]);
  }
 
  num = 0;
  uint8_t max_txt = 40;
  uint32_t scl_speed = 0;
  H_size = 2, V_size = 5;
  red = 0; green = 63;  blue = 31;
  LCD.Scrolle_Font_SetUp(num, max_txt, test_sj_length[num], H_size, V_size, scl_speed, red, green, blue);
 
  num = 1;
  scl_speed = 10;
  H_size = 6, V_size = 2;
  red = 31; green = 5;  blue = 15;
  LCD.Scrolle_Font_SetUp(num, max_txt, test_sj_length[num], H_size, V_size, scl_speed, red, green, blue);
  
  SFR.SPIFFS_Shinonome_Close3F(); //StrDirect_ShinoFNT_readALL関数を使う場合、必ず閉じておく
}
 
void loop() {
// ArduinoOTA.handle();

  num = 0;
  int16_t x0 = 0, y0 = 0;
  LCD.Scrolle_HVsizeUp_8x16_Font_DisplayOut(num, x0, y0, test_buf[num], Scl_Buf[num]);
 
  num = 1;
  x0 = 0; y0 = 100;
  LCD.Scrolle_HVsizeUp_8x16_Font_DisplayOut(num, x0, y0, test_buf[num], Scl_Buf[num]);

}
