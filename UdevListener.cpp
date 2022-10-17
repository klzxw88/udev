#include "UdevListener.h"
#include "UdevEvent.h"

#include <glib.h>
#include <libudev.h>
#include <stdarg.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <cstring>
#include <chrono>
#include <thread>
#include <iostream>

#define memzero(x,l) (std::memset((x), 0, (l)))
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

using namespace DevAttributes;

std::atomic<bool> UdevListener::m_listenerRunning = false;

UdevListener::UdevListener(): m_udev(nullptr) {
}

UdevListener::~UdevListener() {
	stopListener();
}

UdevListener *UdevListener::getInstance() {
    static UdevListener _instance;
    return &_instance;
}

bool UdevListener::initListener() {
	std::cout << "UdevListener::" << __FUNCTION__ << std::endl;
    bool initDone = false;
    if (init()) {
        if (startListener()) {
            enumerate_devices();
        }
        initDone = true;
    }
    return initDone;
}

bool UdevListener::startListener() {
	std::cout << "UdevListener::" << __FUNCTION__ << std::endl;
	m_listenerRunning = true;
    m_listenerThread = std::thread(&UdevListener::threadStart,this);
    return true;
}

bool UdevListener::stopListener() {
	std::cout << "UdevListener::" << __FUNCTION__ << std::endl;
	if (!m_listenerRunning) {
		return false;
	}
    udev_unref(m_udev);
    m_udev = nullptr;
	if (m_listenerThread.joinable()) {
		try {
			m_listenerRunning = false;
    		m_listenerThread.join();
       	}
       	catch (std::exception &e) {
			std::cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " caught system_error: " << e.what() << std::endl;
       	}
	}
    return true;
}

/*  To get the new udev instances
*/
bool UdevListener::init() {
	std::cout << "UdevListener::" << __FUNCTION__ << std::endl;
    m_udev = udev_new();
    if (!m_udev) {
		std::cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " udev_new() failed" << std::endl;
        return false;
    }
    return true;
}

void UdevListener::enumerate_devices() {
	std::cout << "UdevListener::" << __FUNCTION__ << std::endl;
    if (m_udev == nullptr) {
		std::cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " m_udev is null" << std::endl;
        return;
    }
    enumerate_subsystem_devices("usb");
    enumerate_subsystem_devices("sound");
    enumerate_subsystem_devices("typec");
}

void UdevListener::threadStart() {

    struct udev_monitor* monitor = NULL;
    int fd_ep;
    int fd_udev = -1;
    struct epoll_event ep_udev;

    fd_ep = epoll_create1(EPOLL_CLOEXEC);
    if (fd_ep < 0) {
		std::cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " error creating epoll fd: " << strerror(errno) << std::endl;
        goto out;
    }
    monitor = udev_monitor_new_from_netlink(m_udev, "udev");
    if (monitor == NULL) {
		std::cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " no socket" << std::endl;
        goto out;
    }

    //if (udev_monitor_filter_add_match_subsystem_devtype(monitor, "usb", "usb_device") < 0 ||
    //        udev_monitor_filter_add_match_subsystem_devtype(monitor, "sound", NULL) < 0) {
    if (udev_monitor_filter_add_match_subsystem_devtype(monitor, "usb", "usb_device") < 0 ||
            udev_monitor_filter_add_match_subsystem_devtype(monitor, "sound", NULL) < 0 ||
            udev_monitor_filter_add_match_subsystem_devtype(monitor, "typec", NULL) < 0) {
		std::cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " filter failed" << std::endl;
        goto out;
    }

    //if (udev_monitor_set_receive_buffer_size(monitor, 128*1024*1024) < 0) {
    if (udev_monitor_set_receive_buffer_size(monitor, 128*1024) < 0) {
		std::cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " failed to set buffer size" << std::endl;
        goto out;
    }

    if (udev_monitor_enable_receiving(monitor) < 0) {
		std::cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " bind failed" << std::endl;
        goto out;
    }
    fd_udev = udev_monitor_get_fd(monitor);
    memzero(&ep_udev, sizeof(struct epoll_event));
    ep_udev.events = EPOLLIN;
    ep_udev.data.fd = fd_udev;
    if (epoll_ctl(fd_ep, EPOLL_CTL_ADD, fd_udev, &ep_udev) < 0) {
		std::cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " fail to add fd to epoll: " << strerror(errno) << std::endl;
        goto out;
    }

    for (;;) {
        int fdcount;
        struct epoll_event ev[4];
        struct udev_device *device;

		if (!m_listenerRunning) {
			goto out;
		}
        fdcount = epoll_wait(fd_ep, ev, ARRAY_SIZE(ev), 100);

        for (int i = 0; i < fdcount; i++) {
            if (ev[i].data.fd == fd_udev && ev[i].events & EPOLLIN) {
                device = udev_monitor_receive_device(monitor);
                if (device == NULL) {
					std::cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << "no device from socket" << std::endl;
                    continue;
                }
                UdevEvent *pNE = new (std::nothrow) UdevEvent;
                if (pNE) {
                    pNE->parser(device, true);
                    //onEvent(pNE);
                }
                else {
					std::cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " memory failure for PdmNetlinkEvent" << std::endl;
                }
				delete pNE;
                udev_device_unref(device);
            }
        }
    }

    out:
    if (fd_ep >= 0) {
        close(fd_ep);
    }
    udev_monitor_unref(monitor);
}

bool UdevListener::checkExternalUsbDevice(std::string &path) {

	std::cout << "UdevListener::" << __FUNCTION__ << " line:" << __LINE__ << " Device PATH = [" << path.c_str() << "]" << std::endl;
    std::size_t found = path.find("usb");
    if (found == std::string::npos) {
		std::cout << "UdevListener::" << __FUNCTION__ << " line:" << __LINE__ << " Device PATH = [" << path << "] not found" << std::endl;
        return false;
    }

    for (auto devPath : m_usbDevicePath) {
        if (path.compare(0,devPath.length(),devPath) == 0) {
            return true;
        }
    }
    m_usbDevicePath.push_back(path);
	std::cout << "UdevListener::" << __FUNCTION__ << " line:" << __LINE__ << " Device PATH = [" << path << "] pushed inside list" << std::endl;
    return true;
}

void UdevListener::enumerate_subsystem_devices(std::string subSys) {
    if (m_udev == nullptr) {
		std::cout << "UdevListener::" << __FUNCTION__ << " line:" << __LINE__ << " m_udev is null" << std::endl;
        return;
    }
    struct udev_enumerate* enumerate = udev_enumerate_new(m_udev);
	std::cout << "UdevListener::" << __FUNCTION__ << std::endl;
    udev_enumerate_add_match_subsystem(enumerate, subSys.c_str());

    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry* devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry* entry;

    udev_list_entry_foreach(entry, devices) {
        const char* path = udev_list_entry_get_name(entry);
        struct udev_device* device = udev_device_new_from_syspath(m_udev, path);
        std::string pathfinder = path;
        struct udev_device* parent_dev = udev_device_get_parent(device);

        if (parent_dev) {
			std::cout << "UdevListener::" << __FUNCTION__ << " line:" << __LINE__ << " Parent device " << path << std::endl;
            UdevEvent *pNE = new UdevEvent;
            pNE->parser(device, false);
			delete pNE;
            //onEvent(pNE);
        }
        udev_device_unref(device);
    }

    udev_enumerate_unref(enumerate);
}
