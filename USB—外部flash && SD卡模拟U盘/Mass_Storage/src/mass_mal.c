/**
  ******************************************************************************
  * @file    mass_mal.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Medium Access Layer interface
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "./sdio/bsp_sdio_sdcard.h"
#include "./flash/fatfs_flash_spi.h" 
#include "mass_mal.h"
#include <stdio.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t Mass_Memory_Size[2];
uint32_t Mass_Block_Size[2];
uint32_t Mass_Block_Count[2];
__IO uint32_t Status = 0;
//#define  sFLASH_ID              0xEF3015     //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
#define  sFLASH_ID              0XEF4017    //W25Q64
extern SD_CardInfo SDCardInfo;	  //用于存储卡的信息
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : MAL_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Init(uint8_t lun)
{
  uint16_t status = MAL_OK;

  switch (lun)
  {
    case 0:
			FLASH_SPI_disk_initialize();
			if(SPI_FLASH_ReadID()==sFLASH_ID)
			{
				printf("flash init succsee\n");
				Status = MAL_OK;
			}
			else
				Status = MAL_FAIL;
      break;
		case 1:
      Status = SD_Init();
      break;
    default:
      return MAL_FAIL;
  }
  return status;
}
/*******************************************************************************
* Function Name  : MAL_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length)
{
  switch (lun)
  {
    case 0:
			Memory_Offset+=(512*4096);//扇区偏移，外部Flash文件系统空间放在外部Flash后面6M空间
			//printf("write add =%d.length=%d\n",Memory_Offset/4096,Transfer_Length/4096);
			SPI_FLASH_SectorErase(Memory_Offset);
			SPI_FLASH_BufferWrite((uint8_t *)Writebuff,Memory_Offset,Transfer_Length);
      break;
		case 1:
			Status =	SD_WriteBlock((uint8_t*)Writebuff, Memory_Offset, Transfer_Length);
			Status = SD_WaitWriteOperation();  
			while(SD_GetStatus() != SD_TRANSFER_OK);
      if ( Status != SD_OK )
      {
        return MAL_FAIL;
      }      
      break;
    default:
      return MAL_FAIL;
  }
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : Buffer pointer
*******************************************************************************/
uint16_t MAL_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length)
{

  switch (lun)
  {
    case 0:
			Memory_Offset+=(512*4096);//扇区偏移，外部Flash文件系统空间放在外部Flash后面6M空间
			//printf("read add =%d.length=%d\n",Memory_Offset/4096,Transfer_Length/4096);
			SPI_FLASH_BufferRead((uint8_t *)Readbuff, Memory_Offset, Transfer_Length);
      break;
		
		case 1:
      SD_ReadBlock((uint8_t*)Readbuff, Memory_Offset, Transfer_Length);
			Status = SD_WaitReadOperation();
      while(SD_GetStatus() != SD_TRANSFER_OK)
      {
      }
      if ( Status != SD_OK )
      {
        return MAL_FAIL;
      }
      break;
    default:
      return MAL_FAIL;
  }
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_GetStatus
* Description    : Get status
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_GetStatus (uint8_t lun)
{
  uint32_t DeviceSizeMul = 0, NumberOfBlocks = 0;
	switch (lun)
  {
    case 0:
		{
			FLASH_SPI_disk_initialize();
			if(SPI_FLASH_ReadID()==sFLASH_ID)
			{
				Mass_Block_Size[0]  =4096;
				Mass_Block_Count[0] =1536;
				Mass_Memory_Size[0] =Mass_Block_Size[0]*Mass_Block_Count[0];
				return MAL_OK;
			}
		}
		case 1:
			if (SD_Init() == SD_OK)
			{
				SD_GetCardInfo(&SDCardInfo);
				SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
				DeviceSizeMul = (SDCardInfo.SD_csd.DeviceSizeMul + 2);

				if(SDCardInfo.CardType == SDIO_HIGH_CAPACITY_SD_CARD)
				{
					Mass_Block_Count[1] = (SDCardInfo.SD_csd.DeviceSize + 1) * 1024;
				}
				else
				{
					NumberOfBlocks  = ((1 << (SDCardInfo.SD_csd.RdBlockLen)) / 512);
					Mass_Block_Count[1] = ((SDCardInfo.SD_csd.DeviceSize + 1) * (1 << DeviceSizeMul) << (NumberOfBlocks/2));
				}
				Mass_Block_Size[1]  = 512;

				Status = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16)); 
				Status = SD_EnableWideBusOperation(SDIO_BusWide_4b); 
				if ( Status != SD_OK )
				{
					return MAL_FAIL;
				}	      

				Mass_Memory_Size[1] = Mass_Block_Count[1] * Mass_Block_Size[1];
				return MAL_OK;
			}
		default:break;
	}
  return MAL_FAIL;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

