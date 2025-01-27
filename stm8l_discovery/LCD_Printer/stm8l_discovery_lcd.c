/**
  ******************************************************************************
  * @file    stm8l_discovery_lcd.c
  * @author  Microcontroller Division
  * @version V1.2.0
  * @date    09/2010
  * @brief   This file includes driver for the glass LCD Module mounted on
  *          STM8L1526 discovery board MB915A.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8l_discovery_lcd.h"

#if (defined(_WIN32) || defined(WIN32) || defined(WIN64) || defined(_WIN64))
  #define sleep _sleep
#endif


/* LCD BAR status: We don't write directly in LCD RAM for save the bar setting */
uint8_t t_bar[2]={0x0,0X0};

/*  =========================================================================
                                 LCD MAPPING
    =========================================================================
        A
     _  ----------
COL |_| |\   |J  /|
       F| H  |  K |B
     _  |  \ | /  |
COL |_| --G-- --M--
        |   /| \  |
       E|  Q |  N |C
     _  | /  |P  \|
DP  |_| -----------
        D

 An LCD character coding is based on the following matrix:
      { E , D , P   , J  }
      { M , C , COL , H  }
      { B , A , K   , N  }
      { G , F , Q   , DP }

 The character 'A' for example is:
  -------------------------------
LSB   { 1 , 0 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
MSB   { 1 , 1 , 0 , 0   }
      -------------------
  'A' =  F    E   0   0 hexa

*/

/* Constant table for cap characters 'A' --> 'Z' */
const uint16_t CapLetterMap[26]=
    {
        /* A      B      C      D      E      F      G      H      I  */
        0xFE00,0x6711,0x1d00,0x4711,0x9d00,0x9c00,0x3f00,0xfa00,0x0011,
        /* J      K      L      M      N      O      P      Q      R  */
        0x5300,0x9844,0x1900,0x5a42,0x5a06,0x5f00,0xFC00,0x5F04,0xFC04,
        /* S      T      U      V      W      X      Y      Z  */
        0xAF00,0x0411,0x5b00,0x18c0,0x5a84,0x00c6,0x0052,0x05c0
    };

/* Constant table for number '0' --> '9' */
const uint16_t NumberMap[10]=
    {
        /* 0      1      2      3      4      5      6      7      8      9  */
        0x5F00,0x4200,0xF500,0x6700,0xEa00,0xAF00,0xBF00,0x04600,0xFF00,0xEF00
    };

static void LCD_Conv_Char_Seg(uint8_t* c,bool point,bool column,uint8_t* digit);

/**
  * @brief  Configures the LCD GLASS relative GPIO port IOs and LCD peripheral.
  * @param  None
  * @retval None
  */
void LCD_GLASS_Init(void)
{

 /* Enable LCD/RTC clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);

  #ifdef USE_LSE
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);
  #else
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);
  #endif

  /* Initialize the LCD */
  LCD_Init(LCD_Prescaler_1, LCD_Divider_31, LCD_Duty_1_4,
                                   LCD_Bias_1_3, LCD_VoltageSource_Internal);

  /* Mask register
  For declare the segements used.
  in the Discovery we use 0 to 23 segments. */
  LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);
  LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xFF);
  LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0xff);

  /* To set contrast to mean value */
  LCD_ContrastConfig(LCD_Contrast_3V0);

  LCD_DeadTimeConfig(LCD_DeadTime_0);
  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);

  /* Enable LCD peripheral */
  LCD_Cmd(ENABLE);
}

/**
  * @brief  Setting bar on LCD, writes bar value in LCD frame buffer
  * @param  None
  * @retval None
  */
void LCD_bar()
{
/* bar0 bar2 */
  LCD_WriteRAM_withOP(LCD_RAMRegister_11, OP_AND, 0x5f);
  LCD_WriteRAM_withOP(LCD_RAMRegister_11, OP_OR, t_bar[0]&0xa0);

/*bar1 bar3 */
  LCD_WriteRAM_withOP(LCD_RAMRegister_8, OP_AND, 0xf5);
  LCD_WriteRAM_withOP(LCD_RAMRegister_8, OP_OR, t_bar[1]&0x0a);
}

/**
  * @brief  Converts an ascii char to the a LCD digit.
  * @param  c: a char to display.
  * @param  point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  column : flag indicating if a column has to be add in front
  *         of displayed character.
  *         This parameter can be: COLUMN_OFF or COLUMN_ON.
    * @param 	digit array with segment
  * @retval None
  */
static void LCD_Conv_Char_Seg(uint8_t* c,bool point,bool column, uint8_t* digit)
{
  uint16_t ch = 0 ;
  uint8_t i,j;

  switch (*c)
    {
    case ' ' :
      ch = 0x00;
      break;

    case '*':
      ch = star;
      break;


    case 'm' :
      ch = C_mMap;
      break;

    case 'n' :
      ch = C_nMap;
      break;

    case '-' :
      ch = C_minus;
      break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      ch = NumberMap[*c-0x30];
      break;

    default:
      /* The character c is one letter in upper case*/
      if ( (*c < 0x5b) && (*c > 0x40) )
      {
        ch = CapLetterMap[*c-'A'];
      }
      /* The character c is one letter in lower case*/
      if ( (*c <0x7b) && ( *c> 0x60) )
      {
        ch = CapLetterMap[*c-'a'];
      }
      break;
  }

  /* Set the digital point can be displayed if the point is on */
  if (point)
  {
    ch |= 0x0008;
  }

  /* Set the "COL" segment in the character that can be displayed if the column is on */
  if (column)
  {
    ch |= 0x0020;
  }

  for (i = 12,j=0 ;j<4; i-=4,j++)
  {
    digit[j] = (ch >> i) & 0x0f; //To isolate the less signifiant dibit
  }
}

/**
  * @brief  This function writes a char in the LCD frame buffer.
  * @param  ch: the character to dispaly.
  * @param  point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  column: flag indicating if a column has to be add in front
  *         of displayed character.
  *         This parameter can be: COLUMN_OFF or COLUMN_ON.
  * @param  position: position in the LCD of the caracter to write [0:7]
  * @retval None
  * @par    Required preconditions: The LCD should be cleared before to start the
  *         write operation.
  */
void LCD_GLASS_WriteChar(uint8_t* ch, bool point, bool column, uint8_t position)
{
  uint8_t digit[4];     /* Digit frame buffer */

/* To convert displayed character in segment in array digit */
  LCD_Conv_Char_Seg(ch,point,column,digit);

  switch (position)
  {
    /* Position 1 on LCD (Digit1)*/
    case 1:
      LCD_WriteRAM_withOP(LCD_RAMRegister_0, OP_AND, 0x0fc);
      LCD_WriteRAM_withOP(LCD_RAMRegister_0, OP_OR, (uint8_t)(digit[0]& 0x03)); // 1M 1E

      LCD_WriteRAM_withOP(LCD_RAMRegister_2, OP_AND, 0x3f);
      LCD_WriteRAM_withOP(LCD_RAMRegister_2, OP_OR, (uint8_t)((digit[0]<<4) & 0xc0)); // 1G 1B

      LCD_WriteRAM_withOP(LCD_RAMRegister_3, OP_AND, 0x0cf);
      LCD_WriteRAM_withOP(LCD_RAMRegister_3, OP_OR, (uint8_t)(digit[1]<<4 & 0x30)); // 1C 1D

      LCD_WriteRAM_withOP(LCD_RAMRegister_6, OP_AND, 0xf3);
      LCD_WriteRAM_withOP(LCD_RAMRegister_6, OP_OR, (uint8_t)(digit[1]&0x0c)); // 1F 1A

      LCD_WriteRAM_withOP(LCD_RAMRegister_7, OP_AND, 0x0fc);
      LCD_WriteRAM_withOP(LCD_RAMRegister_7, OP_OR, (uint8_t)(digit[2]&0x03)); // 1Col 1P

      LCD_WriteRAM_withOP(LCD_RAMRegister_9, OP_AND, 0x3f);
      LCD_WriteRAM_withOP(LCD_RAMRegister_9, OP_OR, (uint8_t)((digit[2]<<4) & 0xc0)); // 1Q 1K

      LCD_WriteRAM_withOP(LCD_RAMRegister_10, OP_AND, 0xcf);
      LCD_WriteRAM_withOP(LCD_RAMRegister_10, OP_OR, (uint8_t)((digit[3]<<2)& 0x30)); // 1DP 1N

      LCD_WriteRAM_withOP(LCD_RAMRegister_13, OP_AND, 0xf3);
      LCD_WriteRAM_withOP(LCD_RAMRegister_13, OP_OR, (uint8_t)((digit[3]<<2) & 0x0c)); // 1H 1J
      break;

    /* Position 2 on LCD (Digit2)*/
    case 2:
      LCD_WriteRAM_withOP(LCD_RAMRegister_0, OP_AND, 0x0f3);
      LCD_WriteRAM_withOP(LCD_RAMRegister_0, OP_OR, (uint8_t)((digit[0]<<2)&0x0c)); // 2M 2E

      LCD_WriteRAM_withOP(LCD_RAMRegister_2, OP_AND, 0xcf);
      LCD_WriteRAM_withOP(LCD_RAMRegister_2, OP_OR, (uint8_t)((digit[0]<<2)&0x30)); // 2G 2B

      LCD_WriteRAM_withOP(LCD_RAMRegister_3, OP_AND, 0x3f);
      LCD_WriteRAM_withOP(LCD_RAMRegister_3, OP_OR, (uint8_t)((digit[1]<<6) & 0xc0)); // 2C 2D

      LCD_WriteRAM_withOP(LCD_RAMRegister_6, OP_AND, 0xfc);
      LCD_WriteRAM_withOP(LCD_RAMRegister_6, OP_OR, (uint8_t)((digit[1]>>2)&0x03)); // 2F 2A

      LCD_WriteRAM_withOP(LCD_RAMRegister_7, OP_AND, 0xf3);
      LCD_WriteRAM_withOP(LCD_RAMRegister_7, OP_OR, (uint8_t)((digit[2]<<2)& 0x0c)); // 2Col 2P

      LCD_WriteRAM_withOP(LCD_RAMRegister_9, OP_AND, 0xcf);
      LCD_WriteRAM_withOP(LCD_RAMRegister_9, OP_OR, (uint8_t)((digit[2]<<2)&0x30)); // 2Q 2K

      LCD_WriteRAM_withOP(LCD_RAMRegister_10, OP_AND, 0x3f);
      LCD_WriteRAM_withOP(LCD_RAMRegister_10, OP_OR, (uint8_t)((digit[3]<<4)& 0xC0)); // 2DP 2N

      LCD_WriteRAM_withOP(LCD_RAMRegister_13, OP_AND, 0xfc);
      LCD_WriteRAM_withOP(LCD_RAMRegister_13, OP_OR, (uint8_t)((digit[3])& 0x03)); // 2H 2J
      break;

    /* Position 3 on LCD (Digit3)*/
    case 3:
      LCD_WriteRAM_withOP(LCD_RAMRegister_0, OP_AND, 0xcf);
      LCD_WriteRAM_withOP(LCD_RAMRegister_0, OP_OR, (uint8_t)(digit[0]<<4) & 0x30); // 3M 3E

      LCD_WriteRAM_withOP(LCD_RAMRegister_2, OP_AND, 0xf3);
      LCD_WriteRAM_withOP(LCD_RAMRegister_2, OP_OR, (uint8_t)(digit[0]) & 0x0c); // 3G 3B

      LCD_WriteRAM_withOP(LCD_RAMRegister_4, OP_AND, 0xfc);
      LCD_WriteRAM_withOP(LCD_RAMRegister_4, OP_OR, (uint8_t)(digit[1]) & 0x03); // 3C 3D

      LCD_WriteRAM_withOP(LCD_RAMRegister_5, OP_AND, 0x3f);
      LCD_WriteRAM_withOP(LCD_RAMRegister_5, OP_OR, (uint8_t)(digit[1]<<4) & 0xc0); // 3F 3A

      LCD_WriteRAM_withOP(LCD_RAMRegister_7, OP_AND, 0xcf);
      LCD_WriteRAM_withOP(LCD_RAMRegister_7, OP_OR, (uint8_t)(digit[2]<<4)&0x30); // 3Col 3P

      LCD_WriteRAM_withOP(LCD_RAMRegister_9, OP_AND, 0xf3);
      LCD_WriteRAM_withOP(LCD_RAMRegister_9, OP_OR, (uint8_t)(digit[2]) & 0x0C);  // 3Q 3K

      LCD_WriteRAM_withOP(LCD_RAMRegister_11, OP_AND, 0xfc);
      LCD_WriteRAM_withOP(LCD_RAMRegister_11, OP_OR, (uint8_t)(digit[3]>>2) & 0x03 ); // 3DP 3N

      LCD_WriteRAM_withOP(LCD_RAMRegister_12, OP_AND, 0x3f);
      LCD_WriteRAM_withOP(LCD_RAMRegister_12, OP_OR, (uint8_t)(digit[3]<<6) & 0xc0); // 3H 3J
      break;

    /* Position 4 on LCD (Digit4)*/
    case 4:
      LCD_WriteRAM_withOP(LCD_RAMRegister_0, OP_AND, 0x3f);
      LCD_WriteRAM_withOP(LCD_RAMRegister_0, OP_OR, (uint8_t)(digit[0]<<6) & 0xc0); // 4M 4E

      LCD_WriteRAM_withOP(LCD_RAMRegister_2, OP_AND, 0xfc);
      LCD_WriteRAM_withOP(LCD_RAMRegister_2, OP_OR, (uint8_t)(digit[0]>>2) & 0x03); // 4G 4B

      LCD_WriteRAM_withOP(LCD_RAMRegister_4, OP_AND, 0xf3);
      LCD_WriteRAM_withOP(LCD_RAMRegister_4, OP_OR, (uint8_t)(digit[1]<<2) & 0x0C); // 4C 4D

      LCD_WriteRAM_withOP(LCD_RAMRegister_5, OP_AND, 0xcf);
      LCD_WriteRAM_withOP(LCD_RAMRegister_5, OP_OR, (uint8_t)(digit[1]<<2) & 0x30); // 4F 4A

      LCD_WriteRAM_withOP(LCD_RAMRegister_7, OP_AND, 0x3f);
      LCD_WriteRAM_withOP(LCD_RAMRegister_7, OP_OR, (uint8_t)(digit[2]<<6) & 0xC0); // 4Col 4P

      LCD_WriteRAM_withOP(LCD_RAMRegister_9, OP_AND, 0xfc);
      LCD_WriteRAM_withOP(LCD_RAMRegister_9, OP_OR, (uint8_t)(digit[2]>>2) & 0x03 ); // 4Q 4K

      LCD_WriteRAM_withOP(LCD_RAMRegister_11, OP_AND, 0xf3);
      LCD_WriteRAM_withOP(LCD_RAMRegister_11, OP_OR, (uint8_t)(digit[3]) & 0x0C); // 4DP 4N

      LCD_WriteRAM_withOP(LCD_RAMRegister_12, OP_AND, 0xcf);
      LCD_WriteRAM_withOP(LCD_RAMRegister_12, OP_OR, (uint8_t)(digit[3]<<4) & 0x30); // 4H 4J
      break;

    /* Position 5 on LCD (Digit5)*/
    case 5:
      LCD_WriteRAM_withOP(LCD_RAMRegister_1, OP_AND, 0xfc);
      LCD_WriteRAM_withOP(LCD_RAMRegister_1, OP_OR,  (uint8_t)(digit[0]) & 0x03); // 5M 5E

      LCD_WriteRAM_withOP(LCD_RAMRegister_1, OP_AND, 0x3f);
      LCD_WriteRAM_withOP(LCD_RAMRegister_1, OP_OR,  (uint8_t)(digit[0]<<4) & 0xc0); // 5G 5B

      LCD_WriteRAM_withOP(LCD_RAMRegister_4, OP_AND, 0xcf);
      LCD_WriteRAM_withOP(LCD_RAMRegister_4, OP_OR, (uint8_t)(digit[1]<<4) & 0x30); // 5C 5D

      LCD_WriteRAM_withOP(LCD_RAMRegister_5, OP_AND, 0xf3);
      LCD_WriteRAM_withOP(LCD_RAMRegister_5, OP_OR,  (uint8_t)(digit[1]) & 0x0c); // 5F 5A

      // 5 DP 5 COL not used

      LCD_WriteRAM_withOP(LCD_RAMRegister_8, OP_AND, 0xfe);
      LCD_WriteRAM_withOP(LCD_RAMRegister_8, OP_OR,  (uint8_t)(digit[2]) & 0x01); //  5P

      LCD_WriteRAM_withOP(LCD_RAMRegister_8, OP_AND, 0x3f);
      LCD_WriteRAM_withOP(LCD_RAMRegister_8, OP_OR,  (uint8_t)(digit[2]<<4) & 0xc0); // 5Q 5K

      LCD_WriteRAM_withOP(LCD_RAMRegister_11, OP_AND, 0xef);
      LCD_WriteRAM_withOP(LCD_RAMRegister_11, OP_OR,  (uint8_t)(digit[3]<<2) & 0x10); // 5N

      LCD_WriteRAM_withOP(LCD_RAMRegister_12, OP_AND, 0xf3);
      LCD_WriteRAM_withOP(LCD_RAMRegister_12, OP_OR,  (uint8_t)(digit[3]<<2) & 0x0C); // 5H 5J
      break;

    /* Position 6 on LCD (Digit6)*/
    case 6:
      LCD_WriteRAM_withOP(LCD_RAMRegister_1, OP_AND, 0xf3);
      LCD_WriteRAM_withOP(LCD_RAMRegister_1, OP_OR,  (uint8_t)(digit[0]<<2) & 0x0C); // 6M 6E

      LCD_WriteRAM_withOP(LCD_RAMRegister_1, OP_AND, 0xcf);
      LCD_WriteRAM_withOP(LCD_RAMRegister_1, OP_OR,  (uint8_t)(digit[0]<<2) & 0x30); // 6G 6B

      LCD_WriteRAM_withOP(LCD_RAMRegister_4, OP_AND, 0x3f);
      LCD_WriteRAM_withOP(LCD_RAMRegister_4, OP_OR, (uint8_t)(digit[1]<<6) & 0xc0); // 6C 6D

      LCD_WriteRAM_withOP(LCD_RAMRegister_5, OP_AND, 0xfc);
      LCD_WriteRAM_withOP(LCD_RAMRegister_5, OP_OR,  (uint8_t)(digit[1]>>2) & 0x03); // 6F 6A

      LCD_WriteRAM_withOP(LCD_RAMRegister_8, OP_AND, 0xfb);
      LCD_WriteRAM_withOP(LCD_RAMRegister_8, OP_OR,  (uint8_t)(digit[2]<<2) & 0x04); //  6P

      // 6 DP 6COL not used
      LCD_WriteRAM_withOP(LCD_RAMRegister_8, OP_AND, 0xcf);
      LCD_WriteRAM_withOP(LCD_RAMRegister_8, OP_OR,  (uint8_t)(digit[2]<<2) & 0x30); // 6Q 6K

      LCD_WriteRAM_withOP(LCD_RAMRegister_11, OP_AND, 0xbf);
      LCD_WriteRAM_withOP(LCD_RAMRegister_11, OP_OR,  (uint8_t)(digit[3]<<4) & 0x40); // 6N

      LCD_WriteRAM_withOP(LCD_RAMRegister_12, OP_AND, 0xfc);
      LCD_WriteRAM_withOP(LCD_RAMRegister_12, OP_OR, (uint8_t)(digit[3]) & 0x03); // 6H	6J
      break;

      default:
              break;
  }

/* Refresh LCD  bar */
    LCD_bar();
}

/**
  * @brief  This function writes a char in the LCD RAM.
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @retval None
  */
void LCD_GLASS_DisplayString(uint8_t* ptr)
{
  uint8_t i = 0x01;

    LCD_GLASS_Clear();
  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (i < 8))
  {
    /* Display one character on LCD */
    LCD_GLASS_WriteChar(ptr, false, false, i);

    /* Point on the next character */
    ptr++;

    /* Increment the character counter */
    i++;
  }
}

/**
  * @brief  This function writes a char in the LCD RAM.
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @retval None
  * @par    Required preconditions: Char is ASCCI value "Ored" with decimal point or Column flag
  */
void LCD_GLASS_DisplayStrDeci(uint16_t* ptr)
{
  uint8_t i = 0x01;
    uint8_t char_tmp;

    LCD_GLASS_Clear();
  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (i < 8))
  {
    char_tmp = (*ptr) & 0x00ff;

    switch ((*ptr) & 0xf000)
    {
      case DOT:
          /* Display one character on LCD with decimal point */
          LCD_GLASS_WriteChar(&char_tmp, POINT_ON, COLUMN_OFF, i);
          break;
      case DOUBLE_DOT:
          /* Display one character on LCD with decimal point */
          LCD_GLASS_WriteChar(&char_tmp, POINT_OFF, COLUMN_ON, i);
          break;
      default:
          LCD_GLASS_WriteChar(&char_tmp, POINT_OFF, COLUMN_OFF, i);
          break;
    }/* Point on the next character */
    ptr++;

    /* Increment the character counter */
    i++;
  }
}

/**
  * @brief  This function Clear the whole LCD RAM.
  * @param  None
  * @retval None
  */
void LCD_GLASS_Clear(void)
{
  uint8_t counter = 0;

  for (counter = 0; counter <= LCD_RAMRegister_13; counter++)
  {
    LCD_WriteRAM(counter, LCD_RAM_RESET_VALUE);
  }
}

/**
  * @brief  Display a string in scrolling mode
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @param  nScroll: Specifies how many time the message will be scrolled
  * @param  ScrollSpeed : Speciifes the speed of the scroll, low value gives
  *         higher speed
  * @retval None
  * @par    Required preconditions: The LCD should be cleared before to start the
  *         write operation.
  */
void LCD_GLASS_ScrollSentence(uint8_t* ptr, uint16_t nScroll, uint16_t ScrollSpeed)
{
  uint8_t Repetition;
  uint8_t Char_Nb;
  uint8_t* ptr1;
  uint8_t str[7]="";
  uint8_t Str_size;

  if (ptr == 0) return;

/* To calculate end of string */
  for (ptr1=ptr,Str_size = 0 ; *ptr1 != 0; Str_size++,ptr1++) ;

  ptr1 = ptr;

  LCD_GLASS_DisplayString(ptr);
  sleep(1);

/* To shift the string for scrolling display*/
  for (Repetition=0; Repetition<nScroll; Repetition++)
  {
    for (Char_Nb=0; Char_Nb<Str_size; Char_Nb++)
    {
      *(str) =* (ptr1+((Char_Nb+1)%Str_size));
      *(str+1) =* (ptr1+((Char_Nb+2)%Str_size));
      *(str+2) =* (ptr1+((Char_Nb+3)%Str_size));
      *(str+3) =* (ptr1+((Char_Nb+4)%Str_size));
      *(str+4) =* (ptr1+((Char_Nb+5)%Str_size));
      *(str+5) =* (ptr1+((Char_Nb+6)%Str_size));
      LCD_GLASS_Clear();
      LCD_GLASS_DisplayString(str);

      sleep(1);
    }
  }

}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
