#include <stdio.h>

#include <libusb-1.0/libusb.h>
#include "loader.h"
#include <stdlib.h>
#include <assert.h>



int main(){
    libusb_context *context = NULL; //libusb session
    libusb_device **devs;

    int rtn; //return value
    ssize_t count;

    rtn = libusb_init(&count); //starting a library session

    if(rtn < 0){
        perror("libusb_int error");
        exit(1);
    }

    libusb_set_debug(count, 3); //setting verbosity level
    
    count = libusb_get_device_list(count, &devs);
    if(count < 0){
        perror("Failed to get devices");

    }
    printf("\n%d Devices in list", count);
   
    ssize_t i;
    //next step is to iterate through the list and print them
    
    
    printf("\nline37"); 
    struct libusb_device_handle *devH = NULL;
    printf("\nline39:");
    devH = libusb_open_device_with_vid_pid(context, SWITCH_VID, SWITCH_PID);
    printf("\nLine 41")
    if(devH == NULL){
        perror("Could not connect to switch!");
        exit(1);
    }else{
        printf("\nSwitch conncted!");
    }

    uint8_t       bmReqType = 0;   // the request type (direction of transfer)
    uint8_t            bReq = 0;   // the request field for this packet
    uint16_t           wVal = 0;   // the value field for this packet
    uint16_t         wIndex = 0;   // the index field for this packet
    unsigned char*   data =   NULL;   // the data buffer for the in/output data
    uint16_t           wLen = 0;   // length of this setup packet 
    unsigned int     to = 0;       // timeout duration (if transfer fails)
    
    // transfer the setup packet to the USB device
    

    uint8_t chip_buffer[RCM_CHIP_LEN];
    memset(&chip_buffer, 0, sizeof(chip_buffer));

    rtn = libusb_bulk_transfer(devH, RCM_EP1_IN,data,wLen, RCM_CHIP_LEN, chip_buffer);
    if(rtn < 0)
    {
        perror("failed to get device ID");
        exit(1);
    }
    printf("\nDevice ID: ");



    libusb_exit(NULL);

    return 0;



}


printDev(libusb_device *dev){
    struct libusb_device_descriptor  desc;
    int r = libusb_get_device_descriptor(dev, &desc);
    if(r < 0){
        perror("Failed to get dev descriptor");
        
    }
    printf("\nNumber of possible configurations: %d", desc.bNumConfigurations);
    printf("\nDevice Class: %d", desc.bDeviceClass); 
    printf("\nVendorID: %d", desc.idVendor);
    printf("\nProductID: %d", desc.idProduct);
    struct libusb_config_descriptor *config;

    libusb_get_config_descriptor(dev, 0, &config);
    printf("\nInterfaces: %d", config->bNumInterfaces);

    const struct libusb_interface *inter;
    const struct libusb_interface_descriptor *interdesc;
    const struct libusb_endpoint_descriptor *endpointdesc;

    for(int i = 0; i < config->bNumInterfaces; i++){
        inter = &config->interface[i];
        printf("\nNumber of alternate settings: %d", inter->num_altsetting);
        
        for(int j = 0; j < inter->num_altsetting; j++){
            interdesc = &inter->altsetting[j];
            printf("\nInterface Number: %d", interdesc->bInterfaceNumber);
            printf("Nyum of endpoints: %d\n", interdesc->bNumEndpoints);
            for(int k = 0; k < interdesc->bNumEndpoints; k++){
                 endpointdesc = &interdesc->endpoint[k];
                 printf(" Desc Type: %d ", endpointdesc->bDescriptorType);
                 printf(" EP Addr: %d\n", endpointdesc->bEndpointAddress);
            }
        }
    }
    
    printf("\n\n");
    libusb_free_config_descriptor(config);

}
