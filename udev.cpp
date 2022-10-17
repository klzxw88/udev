#include "UdevListener.h"
#include <unistd.h>

#include <iostream>

int main() {
	UdevListener::getInstance()->initListener();
	usleep(10000000);
	UdevListener::getInstance()->stopListener();
	return 0;
}
