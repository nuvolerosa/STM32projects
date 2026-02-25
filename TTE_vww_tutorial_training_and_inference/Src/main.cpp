/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   main.cpp
 *
 * Reference papers:
 *  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
 *  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
 *  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
 * Contact authors:
 *  - Wei-Chen Wang, wweichen@mit.edu
 *  - Wei-Ming Chen, wmchen@mit.edu
 *  - Ji Lin, jilin@mit.edu
 *  - Ligeng Zhu, ligeng@mit.edu
 *  - Song Han, songhan@mit.edu
 *  - Chuang Gan, ganchuang@csail.mit.edu
 *
 * Target ISA:  ARMv7E-M
 * -------------------------------------------------------------------- */

#include "main.h"
#include "camera.h"
#include "lcd.h"
#include "profile.h"
#include "stdio.h"
#include "string.h"
#include "testing_data/golden_data.h"
#include "testing_data/images.h"
extern "C" {
#include "genNN.h"
#include "tinyengine_function.h"
}
#define SHOWIMG

#include "stm32746g_discovery.h"

static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
static void MX_GPIO_Init(void);

#define IMAGE_H 80
#define IMAGE_W 80
#define INPUT_CH 160
#define OUTPUT_CH 2
#define IMAGES 6

void SystemClock_Config(void);
void StartDefaultTask(void const *argument);

//// Nicole memory experiments (to be defined better)
//#define IMAGE_SIZE       0x8000       // 32 KB
//// First sector start
//#define IMAGE_BASE_ADDR  0x08050000
//#define IMAGE1_ADDR  (IMAGE_BASE_ADDR)
//#define IMAGE2_ADDR  (IMAGE1_ADDR + IMAGE_SIZE)
//#define IMAGE3_ADDR  (IMAGE2_ADDR + IMAGE_SIZE)
//#define IMAGE4_ADDR  (IMAGE3_ADDR + IMAGE_SIZE)
//
//// Find the right sector in flash
//uint32_t GetSector(uint32_t Address) {
//    uint32_t sector = 0;
//
//    if(Address < 0x08008000)       sector = FLASH_SECTOR_0;
//    else if(Address < 0x08010000)  sector = FLASH_SECTOR_1;
//    else if(Address < 0x08018000)  sector = FLASH_SECTOR_2;
//    else if(Address < 0x08020000)  sector = FLASH_SECTOR_3;
//    else if(Address < 0x08040000)  sector = FLASH_SECTOR_4;
//    else if(Address < 0x08080000)  sector = FLASH_SECTOR_5;
//    else                           sector = FLASH_SECTOR_6;
//
//    return sector;
//}
//
//// this function save the fames to flash
//uint32_t Flash_Write_Data(uint32_t StartSectorAddress, uint16_t *Data, uint16_t numberofhalfwords)
//{
//
//	static FLASH_EraseInitTypeDef EraseInitStruct;
//	uint32_t SECTORError;
//	int sofar=0;
//
//	/* Unlock the Flash to enable the flash control register access *************/
//    HAL_FLASH_Unlock();
//
//    /* Erase the user Flash area */
//
//	  /* Get the number of sector to erase from 1st sector */ //PROBLEM: library not present
//	  uint32_t StartSector = GetSector(StartSectorAddress);
//	  uint32_t EndSectorAddress = StartSectorAddress + numberofhalfwords*4;
//	  uint32_t EndSector = GetSector(EndSectorAddress);
//
//	  /* Fill EraseInit structure*/
//	  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
//	  EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
//	  EraseInitStruct.Sector        = StartSector;
//	  EraseInitStruct.NbSectors     = (EndSector - StartSector) + 1;
//
//	  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
//	     you have to make sure that these data are rewritten before they are accessed during code
//	     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
//	     DCRST and ICRST bits in the FLASH_CR register. */
//	  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
//	  {
//		  return HAL_FLASH_GetError ();
//	  }
//
//	  /* Program the user Flash area word by word
//	    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
//
//	   while (sofar<numberofhalfwords)
//	   {
//	     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, StartSectorAddress, Data[sofar]) == HAL_OK)
//	     {
//	    	 StartSectorAddress += 2;  // use StartPageAddress += 4 for word and 8 for double word
//	    	 sofar++;
//	     }
//	     else
//	     {
//	       /* Error occurred while writing data in Flash memory*/
//	    	 return HAL_FLASH_GetError ();
//	     }
//	   }
//
//	  /* Lock the Flash to disable the flash control register access (recommended
//	     to protect the FLASH memory against possible unwanted operation) *********/
//	  HAL_FLASH_Lock();
//
//	   return 0;
//
//}


// End Nicole experiment

signed char out_int[OUTPUT_CH]; // probs to get 0 or 1

float labels[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
void train(int cls) {
  char cbuf[20];
  for (int i = 0; i < 10; i++) {
    if (i == cls) {
      labels[i] = 1.0f;
    } else
      labels[i] = 0.0f;
  }
  invoke(labels);
}

void invoke_new_weights_givenimg(signed char *out_int8) {
  invoke_inf();
  signed char *output = (signed char *)getOutput();
  for (int i = 0; i < 10; i++)
    out_int8[i] = output[i];
}

#define BUTTON1_Pin GPIO_PIN_0
#define BUTTON1_GPIO_Port GPIOA
#define BUTTON2_Pin GPIO_PIN_10
#define BUTTON2_GPIO_Port GPIOF

#define RES_W 128
#define RES_H 120

uint16_t *RGBbuf;
#define ENABLE_TRAIN

int main(void) {
  char buf[150];
  char showbuf[150];

  CPU_CACHE_Enable();
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
  lcdsetup();

  int camErr = initCamera();
  uint32_t start, end, starti, endi, time;

  StartCapture();
  signed char *input = getInput();

  RGBbuf = (uint16_t *)&input[128 * 128 * 4];
  int t_mode = 1; // hardcoded to 0, was 0 before
  while (1) {
    starti = HAL_GetTick();
    HAL_Delay(10);
    ReadCapture();
    StartCapture();

    // Check hardfault

    int ret = DecodeandProcessAndRGB(RES_W, RES_H, input, RGBbuf, 1);
    for (int i = 0; i < 128 * 8 * 3; i++) {
      input[120 * 128 * 3 + i] = -128;
    }
    for (int i = 0; i < RES_W; i++) {
      for (int j = 0; j < RES_W; j++) {
        uint8_t red = (int32_t)input[(128 * i + j) * 3] + 128;
        uint8_t green = (int32_t)input[(128 * i + j) * 3 + 1] + 128;
        uint8_t blue = (int32_t)input[(128 * i + j) * 3 + 2] + 128;

        uint16_t b = (blue >> 3) & 0x1f;
        uint16_t g = ((green >> 2) & 0x3f) << 5;
        uint16_t r = ((red >> 3) & 0x1f) << 11;

        RGBbuf[j + RES_W * i] = (uint16_t)(r | g | b);
      }
    }

    loadRGB565LCD(10, 10, RES_W, RES_W, RGBbuf, 2);
    endi = HAL_GetTick();

    uint8_t button0;

    char s[1];
    s[0] = 'c';
    recieveChar(s);
    if (s[0] == '3')
      t_mode = 1;
    if (s[0] == '4')
      t_mode = 0;
    if (t_mode == 1) {

    	drawBlackBackground(270, 481, 1, 273);

    	button0 = BSP_PB_GetState(BUTTON_KEY) == GPIO_PIN_SET;
    	if (button0 == 1){

        	// Reset black screen
        	drawBlackBackground(270, 481, 1, 273);
        	// Show instruction 1
        	sprintf(showbuf, "Collect 3");
        	displaystring(showbuf, 273, 10);
        	sprintf(showbuf, "frames");
        	displaystring(showbuf, 273, 40);
        	//
        	sprintf(showbuf, "Frame 1");
        	displaystring(showbuf, 273, 80);
        	HAL_Delay(1000);
        	StartCapture();
        	ReadCapture();
        	DecodeandProcessAndRGB(RES_W, RES_H, input, RGBbuf, 1);

        	// Save the frame to flash

        	// 1) Find number of words for RGFBuf
        	// Flash_Write_Data(IMAGE1_ADDR, RGBbuf, 4096);
        	// Retrive from flash for testing and show it in the screen with >> loadRGB565LCD(10, 10, RES_W, RES_W, flash_frame, 2);

//			// Count down before entering training
//			for (int i = 5; i > 0; i--) {
//				sprintf(showbuf, "Starting in");
//				displaystring(showbuf, 273, 10);
//				sprintf(showbuf, "Press =>");
//				displaystring(showbuf, 273, 150);
//				sprintf(showbuf, "Person");
//				displaystring(showbuf, 273, 175);
//				sprintf(showbuf, "Release =>");
//				displaystring(showbuf, 273, 200);
//				sprintf(showbuf, "Other");
//				displaystring(showbuf, 273, 225);
//				sprintf(showbuf, "...%d ", i);
//				displaystring(showbuf, 273, 50);
//				HAL_Delay(1000);        // wait 1 second
//			}
//			drawBlackBackground(270, 481, 1, 273);
//			button0 = BSP_PB_GetState(BUTTON_KEY) == GPIO_PIN_SET;
//
//
//			// ASSIGN TRAINING CLASS
//			int label = 0;
//			if (button0) {
//			  sprintf(showbuf, "Train cls 1");
//			  label = 1;
//			} else if (!button0) {
//			  sprintf(showbuf, "Train cls 0");
//			  label = 0;
//			}
//
//			// this part of the code is making inference
//			start = HAL_GetTick();
//			invoke_new_weights_givenimg(out_int);
//
//			// MANIPULATE ANSWER
//			int answer_right = 0; // this is just initialisation
//			int p;
//			if (out_int[0] > out_int[1]) { // p(0) > p(1) --> prediction is NOT a person
//			  p = 0;
//			  if (label == 1)
//				answer_right = 0; // if required label = 1 --> answer is wrong
//			  else
//				answer_right = 1; // if required label = 1 --> answer is right
//			} else {
//			  p = 1;
//			  if (label == 1)
//				answer_right = 1;
//			  else
//				answer_right = 0;
//			}
//			end = HAL_GetTick();
//			detectResponse(answer_right, 0, t_mode, p, label);

//			ReadCapture();
//			StartCapture();
//			DecodeandProcessAndRGB(RES_W, RES_H, input, RGBbuf, 1);
//			displaystring(showbuf, 273, 10);
//			start = HAL_GetTick();
//			train(label); // here it is training
//			end = HAL_GetTick();
//			sprintf(showbuf, "Train done ");
//			displaystring(showbuf, 273, 10);
//			detectResponse(answer_right, end - start, t_mode, p, label);
//			drawBlackBackground(270, 481, 1, 273);
    	}
    }else if(t_mode == 0){
        start = HAL_GetTick();
        invoke_new_weights_givenimg(out_int);
        int person = 0;
        if (out_int[0] > out_int[1]) {
          person = 1;
        } else {
          person = 0;
        }
        end = HAL_GetTick();
        sprintf(showbuf, " Inference ");
        displaystring(showbuf, 273, 10);
        time = end - start;
        detectResponse(person, time, t_mode, 0, 0);
    }
  }

  while (1) {
  }
}
void SystemClock_Config(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if (ret != HAL_OK) {
    while (1) {
      ;
    }
  }

  ret = HAL_PWREx_EnableOverDrive();
  if (ret != HAL_OK) {
    while (1) {
      ;
    }
  }

  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if (ret != HAL_OK) {
    while (1) {
      ;
    }
  }
}
static void Error_Handler(void) {

  BSP_LED_On(LED1);
  while (1) {
  }
}

static void CPU_CACHE_Enable(void) {

  SCB_EnableICache();

  SCB_EnableDCache();
}

static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin,
                    GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOI, ARDUINO_D7_Pin | ARDUINO_D8_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_Port, LCD_BL_CTRL_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(LCD_DISP_GPIO_Port, LCD_DISP_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(DCMI_PWR_EN_GPIO_Port, DCMI_PWR_EN_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOG, ARDUINO_D4_Pin | ARDUINO_D2_Pin | EXT_RST_Pin,
                    GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = OTG_HS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_HS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_TXD1_Pin | RMII_TXD0_Pin | RMII_TX_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ULPI_D7_Pin | ULPI_D6_Pin | ULPI_D5_Pin | ULPI_D3_Pin |
                        ULPI_D2_Pin | ULPI_D1_Pin | ULPI_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SPDIF_RX0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF8_SPDIFRX;
  HAL_GPIO_Init(SPDIF_RX0_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = OTG_FS_VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_VBUS_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = Audio_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Audio_INT_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = OTG_FS_P_Pin | OTG_FS_N_Pin | OTG_FS_ID_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ARDUINO_D7_Pin | ARDUINO_D8_Pin | LCD_DISP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = uSD_Detect_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(uSD_Detect_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_BL_CTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_BL_CTRL_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = TP3_Pin | NC2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ARDUINO_SCK_D13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(ARDUINO_SCK_D13_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DCMI_PWR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DCMI_PWR_EN_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LCD_INT_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ULPI_NXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(ULPI_NXT_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ARDUINO_D4_Pin | ARDUINO_D2_Pin | EXT_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ULPI_STP_Pin | ULPI_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_MDC_Pin | RMII_RXD0_Pin | RMII_RXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_RXER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RMII_RXER_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RMII_REF_CLK_Pin | RMII_MDIO_Pin | RMII_CRS_DV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ULPI_CLK_Pin | ULPI_D0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ARDUINO_MISO_D12_Pin | ARDUINO_MOSI_PWM_D11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BUTTON1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON1_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BUTTON2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON2_GPIO_Port, &GPIO_InitStruct);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {

  while (1) {
  }
}
#endif
