#include "lcd.h"

void lcdInit(void)
{
    // LCD initialization
    LCD_Init();
    LCD_LayerInit();

    // LTDC reload configuration
    LTDC_ReloadConfig(LTDC_IMReload);

    // Enable the LTDC
    LTDC_Cmd(ENABLE);

    // Set LCD foreground layer
    LCD_SetLayer(LCD_FOREGROUND_LAYER);

    // Configure the IO Expander
    IOE_Config();

    // Initialize User Button mounted on STM32F429I-DISCO
    STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

    // Display test name on LCD
    LCD_Clear(LCD_COLOR_WHITE);
    LCD_SetBackColor(LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLACK);

    LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*)" Julia's Robot");
    LCD_DisplayStringLine(LCD_LINE_7, (uint8_t*)"      --by Dad");
}
