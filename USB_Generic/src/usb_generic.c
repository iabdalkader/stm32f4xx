#include "usbd_desc.h"
#include "usbd_req.h"
#include "usb_generic.h"
#define IN_EP       0x81  /* EP1 for data IN */
#define OUT_EP      0x01  /* EP1 for data OUT */
#define CMD_EP      0x82  /* EP2 for commands */
#define USB_CONFIG_DESC_SIZE  (32)

static uint8_t  usbd_gen_Init        (void *pdev, uint8_t cfgidx);
static uint8_t  usbd_gen_DeInit      (void *pdev, uint8_t cfgidx);
static uint8_t  usbd_gen_DataIn      (void *pdev, uint8_t epnum);
static uint8_t  usbd_gen_DataOut     (void *pdev, uint8_t epnum);
static uint8_t  *USBD_gen_GetCfgDesc (uint8_t speed, uint16_t *length);

/* Handle to USB OTG device 
   Note: This needs to be global */
USB_OTG_CORE_HANDLE  USB_OTG_dev;

/* USB user callbacks structure */
static struct usb_user_cb user_cb;

/* USB transfer buffer */
static __ALIGN_BEGIN uint8_t usb_xfer_buffer   [MAX_DATA_PACKET_SIZE] __ALIGN_END;

/* Interface class callbacks structure */
static USBD_Class_cb_TypeDef  USBD_gen_cb = {
    usbd_gen_Init,
    usbd_gen_DeInit,
    NULL,
    NULL, 
    NULL,
    usbd_gen_DataIn,
    usbd_gen_DataOut,
    NULL,
    NULL,
    NULL,     
    USBD_gen_GetCfgDesc,
};

static __ALIGN_BEGIN uint8_t usbd_gen_CfgDesc[USB_CONFIG_DESC_SIZE]  __ALIGN_END =
{
    /*Configuration Descriptor*/
    0x09,   /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,  /* bDescriptorType: Configuration */
    USB_CONFIG_DESC_SIZE,           /* wTotalLength:no of returned bytes */
    0x00,
    0x01,   /* bNumInterfaces: 2 interface */
    0x01,   /* bConfigurationValue: Configuration value */
    0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
    0xC0,   /* bmAttributes: self powered */
    0x32,   /* MaxPower 0 mA */

    /*Interface Descriptor */
    0x09,   /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints: One endpoints used */
    0xFF,   /* bInterfaceClass: Vendor Specific */
    0x00,   /* bInterfaceSubClass */
    0x00,   /* bInterfaceProtocol */
    0x00,   /* iInterface: */

    /*Endpoint IN Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType: Endpoint */
    IN_EP,                         /* bEndpointAddress */
    0x02,                              /* bmAttributes: Bulk */
    LOBYTE(MAX_DATA_PACKET_SIZE),       /* wMaxPacketSize: */
    HIBYTE(MAX_DATA_PACKET_SIZE),
    0x00,                              /* bInterval: ignore for Bulk transfer */

    /*Endpoint OUT Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType: Endpoint */
    OUT_EP,                        /* bEndpointAddress */
    0x02,                              /* bmAttributes: Bulk */
    LOBYTE(MAX_DATA_PACKET_SIZE),  /* wMaxPacketSize: */
    HIBYTE(MAX_DATA_PACKET_SIZE),
    0x00,                              /* bInterval: ignore for Bulk transfer */
};


void usb_dev_init(struct usb_user_cb *cb)
{
    /* set user callback struct */
    user_cb = *cb;

    /* init USB OTG Device */
    USBD_Init(&USB_OTG_dev, 
            USB_OTG_FS_CORE_ID, 
            &USR_desc, 
            &USBD_gen_cb, &USR_cb);
}

void usb_dev_send(void *buffer, int length)
{
    int usb_xfer_length;

    /* call user callback to fill buffer */
    user_cb.usb_data_in(usb_xfer_buffer, &usb_xfer_length, user_cb.user_data);

    if (usb_xfer_length) {
        /* Fill IN endpoint fifo with packet */
        DCD_EP_Tx(&USB_OTG_dev, IN_EP, usb_xfer_buffer, usb_xfer_length);
    }
}


static uint8_t  usbd_gen_Init (void *pdev, uint8_t cfgidx)
{
  /* Open EP IN */
  DCD_EP_Open(pdev,
              IN_EP,
              MAX_DATA_PACKET_SIZE,
              USB_OTG_EP_BULK);
  
  /* Open EP OUT */
  DCD_EP_Open(pdev,
              OUT_EP,
              MAX_DATA_PACKET_SIZE,
              USB_OTG_EP_BULK);
  
#if 0
  /* Open Command IN EP */
  DCD_EP_Open(pdev,
              CMD_EP,
              CMD_PACKET_SZE,
              USB_OTG_EP_INT);
#endif

  /* Prepare Out endpoint to receive next packet */
  DCD_EP_PrepareRx(pdev, OUT_EP,
                   (uint8_t*)(usb_xfer_buffer),
                   MAX_DATA_PACKET_SIZE);

  return USBD_OK;
}

/**
  * @brief  usbd_gen_Init
  *         DeInitialize the USB layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  usbd_gen_DeInit (void  *pdev, 
                                 uint8_t cfgidx)
{
  /* Open EP IN */
  DCD_EP_Close(pdev, IN_EP);
  
  /* Open EP OUT */
  DCD_EP_Close(pdev, OUT_EP);
  
//  /* Open Command IN EP */
//  DCD_EP_Close(pdev, CMD_EP);
  return USBD_OK;
}

/**
  * @brief  USBD_gen_GetCfgDesc 
  *         Return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_gen_GetCfgDesc (uint8_t speed, uint16_t *length)
{
    *length = sizeof (usbd_gen_CfgDesc);
    return usbd_gen_CfgDesc;
}

/**
  * @brief  usbd_gen_DataIn
  *         Data sent on non-control IN endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t usbd_gen_DataIn (void *pdev, uint8_t epnum)
{
    int usb_xfer_length;
    usb_xfer_length = ((USB_OTG_CORE_HANDLE*)pdev)->dev.in_ep[epnum].xfer_count;

    user_cb.usb_data_in(usb_xfer_buffer, &usb_xfer_length, user_cb.user_data);

    if (usb_xfer_length) {
        /* Fill IN endpoint fifo with packet */
        DCD_EP_Tx (pdev, IN_EP, usb_xfer_buffer, usb_xfer_length);
    }
    return USBD_OK;
}

/**
  * @brief  usbd_gen_DataOut
  *         Data received on non-control Out endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t usbd_gen_DataOut(void *pdev, uint8_t epnum)
{   
    int usb_xfer_length;

    /* Get the received data length */
    usb_xfer_length = USBD_GetRxCount(pdev, epnum); 

    /* call user callback */
    user_cb.usb_data_out(usb_xfer_buffer, &usb_xfer_length, user_cb.user_data);

    if (usb_xfer_length) {
        /* Fill IN endpoint fifo with first packet */
        DCD_EP_Tx (pdev, IN_EP, usb_xfer_buffer, usb_xfer_length);
    }

    /* Prepare Out endpoint to receive next packet */
    DCD_EP_PrepareRx(pdev, OUT_EP,
                     (uint8_t*)(usb_xfer_buffer),
                     MAX_DATA_PACKET_SIZE);
    return USBD_OK;
}
