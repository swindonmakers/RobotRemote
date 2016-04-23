#ifndef RobotWifi_h
#define RobotWifi_h

#include <ESP8266Wifi.h>

#define ROBOTWIFI_SSID "Robots"
#define ROBOTWIFI_PASS "robotsrule"

class RobotWifi
{
public:
	RobotWifi();

	void host();
	void join();

private:

};

#endif // !RobotWifi_h