#ifndef _UdevListener_H
#define _UdevListener_H

#include "Common.h"

#include <thread>
#include <list>
#include <atomic>

class PdmNetlinkEvent;

class UdevListener {
private:
    static std::atomic<bool> m_listenerRunning;
    struct udev* m_udev;
    std::thread m_listenerThread;
    std::list<std::string> m_usbDevicePath;

    UdevListener();
  	UdevListener(const UdevListener& src) = delete;
  	UdevListener& operator=(const UdevListener& rhs) = delete;

public:
    static UdevListener *getInstance();
    ~UdevListener();

    bool initListener();
    bool startListener();
    bool stopListener();
    //virtual void onEvent(PdmNetlinkEvent *evn) = 0;

private:
    bool init();
    void threadStart();
    void enumerate_devices();
    void enumerate_subsystem_devices(std::string subSys);
    bool checkExternalUsbDevice(std::string &path);
};

#endif //_UdevListener_H
