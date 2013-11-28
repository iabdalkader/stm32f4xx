#include "usbd_usr.h"
#include "usbd_ioreq.h"

USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,
};
/**
* @brief  USBD_USR_Init 
* @param  None
* @retval None
*/
void USBD_USR_Init(void)
{   

}

/**
* @brief  USBD_USR_DeviceReset 
* @param  speed : device speed
* @retval None
*/
void USBD_USR_DeviceReset(uint8_t speed )
{
 switch (speed)
 {
   case USB_OTG_SPEED_HIGH: 
     break;

  case USB_OTG_SPEED_FULL: 
     break;
 default:
     break;
     
 }
}

/**
* @brief  USBD_USR_DeviceConfigured
* @param  None
* @retval Staus
*/
void USBD_USR_DeviceConfigured (void)
{
}


/**
* @brief  USBD_USR_DeviceConnected
* @param  None
* @retval Staus
*/
void USBD_USR_DeviceConnected (void)
{
}


/**
* @brief  USBD_USR_DeviceDisonnected
* @param  None
* @retval Staus
*/
void USBD_USR_DeviceDisconnected (void)
{
}

/**
* @brief  USBD_USR_DeviceSuspended 
* @param  None
* @retval None
*/
void USBD_USR_DeviceSuspended(void)
{
  /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBD_USR_DeviceResumed 
* @param  None
* @retval None
*/
void USBD_USR_DeviceResumed(void)
{
  /* Users can do their application actions here for the USB-Reset */
}


