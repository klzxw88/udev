#ifndef _UdevEvent_H
#define _UdevEvent_H

#include "Common.h"

#include <libudev.h>
#include <string>
#include <map>

class UdevEvent {
private:
    std::map<std::string,std::string> mdeviceInfo;

    void getDeviceInfo(struct udev_device* dev, bool isInitDone);

public:
    UdevEvent() {
    }
    ~UdevEvent() {
    }

    void parser(struct udev_device* dev, bool isInitDone);
    std::string getDevAttribute(std::string);
};
#endif //_UdevEvent_H
