/**
******************************************************************************
* @file  HAL_op.c
* @author  AE team
* @version  V1.5.0
* @date  21/10/2016
* @brief  This file provides all the OP firmware functions.
******************************************************************************
* @copy
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2016 MindMotion</center></h2>
*/

/* Includes ------------------------------------------------------------------*/
#include "HAL_op.h"

/** @addtogroup StdPeriph_Driver
* @{
*/

/** @defgroup OP 
* @brief OP driver modules
* @{
*/

/** @defgroup OP_Private_TypesDefinitions
* @{
*/

/**
* @}
*/

/** @defgroup OP_Private_Defines
* @{
*/

/*******************************************************************************
* @name   : OPAMP_Config
* @brief  : Enable OPAMP
* @param  : None
* @retval : void
*******************************************************************************/
void OPAMP_Config(void)
{
    OPAMP->CSR = 1;
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/*-------------------------(C) COPYRIGHT 2016 MindMotion ----------------------*/
