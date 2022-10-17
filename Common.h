#ifndef COMMON_H_
#define COMMON_H_

#include <string>

namespace DevAttributes {

enum DeviceEventType { STORAGE_DEVICE = 0,
                       UNKNOWN_DEVICE
                     };

//device status
    const std::string DEVICE_ADD = "add";
    const std::string DEVICE_REMOVE = "remove";
    const std::string DEVICE_CHANGE = "change";
    const std::string DEVICE_BIND = "bind";
    const std::string DEVICE_UNBIND = "unbind";
    const std::string YES = "1";
    const std::string NO = "0";

    const std::string DEVNUM = "DEVNUM";
    const std::string BUSNUM = "BUSNUM";
    const std::string ACTION  =  "ACTION";
    const std::string DEVLINKS  = "DEVLINKS";
    const std::string DEVNAME  =  "DEVNAME";
    const std::string DEVPATH  =  "DEVPATH";
    const std::string DEVTYPE  =  "DEVTYPE";
    const std::string ID_BUS  =  "ID_BUS";
    const std::string ID_MODEL  =  "ID_MODEL";
    const std::string ID_MODEL_ENC  =  "ID_MODEL_ENC";
    const std::string ID_MODEL_ID  =  "ID_MODEL_ID";
    const std::string ID_PATH  =  "ID_PATH";
    const std::string ID_PATH_TAG  =  "ID_PATH_TAG";
    const std::string ID_REVISION  =  "ID_REVISION";
    const std::string ID_SERIAL  =  "ID_SERIAL";
    const std::string ID_SERIAL_SHORT  =  "ID_SERIAL_SHORT";
    const std::string ID_TYPE  =  "ID_TYPE";
    const std::string ID_USB_DRIVER  =  "ID_USB_DRIVER";
    const std::string ID_USB_INTERFACES  =  "ID_USB_INTERFACES";
    const std::string ID_USB_INTERFACE_NUM  =  "ID_USB_INTERFACE_NUM";
    const std::string ID_VENDOR_ENC  =  "ID_VENDOR_ENC";
    const std::string ID_VENDOR  =  "ID_VENDOR";
    const std::string ID_VENDOR_ID  =  "ID_VENDOR_ID";
    const std::string MAJOR  =  "MAJOR";
    const std::string MINOR  =  "MINOR";
    const std::string SEQNUM  =  "SEQNUM";
    const std::string SUBSYSTEM  =  "SUBSYSTEM";
    const std::string TAGS  =  "TAGS";
    const std::string USEC_INITIALIZED  =  "USEC_INITIALIZED";
    const std::string SPEED = "SPEED";
    const std::string ID_VENDOR_FROM_DATABASE = "ID_VENDOR_FROM_DATABASE";

    const std::string MAX_CHILD = "MAX_CHILD";
    const std::string USB_VERSION = "USB_VERSION";
    const std::string DEVCLASS = "DEVICE_CLASS";
    const std::string DEVSUBCLASS = "DEVICE_SUBCLASS";
    const std::string INTERFACE = "INTERFACE";
    const std::string INTERFACE_COUNT = "INTERFACE_COUNT";
    const std::string PRODUCT_ID = "PRODUCT_ID";

// Unknown device
    const std::string DEV_TYPE_UNKNOWN = "UNKNOWN";

//USB device Speed
    const std::string USB_SUPER_SPEED = "SUPER";
    const std::string USB_HIGH_SPEED = "HIGH";
    const std::string USB_FULL_SPEED = "FULL";
    const std::string USB_LOW_SPEED = "LOW";

//Host Contoller Interface (HCD)
    const std::string XHCI = "xhci";
    const std::string EHCI = "ehci";
    const std::string OHCI = "ohci";
    const std::string UHCI = "uhci";
};

#endif /* COMMON_H_ */
