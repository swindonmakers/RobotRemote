#include "RobotWifi.h"

RobotWifi::RobotWifi()
{
}

void RobotWifi::join()
{
	WiFi.disconnect(true);
	WiFi.mode(WIFI_STA);
	if (ROBOTWIFI_PASS != NULL)
		WiFi.begin(ROBOTWIFI_SSID, ROBOTWIFI_PASS);
	else
		WiFi.begin(ROBOTWIFI_SSID);

	while (WiFi.status() != WL_CONNECTED) {
		delay(100);
	}
}

void RobotWifi::host()
{
	WiFi.disconnect(true);
	WiFi.mode(WIFI_AP);

	if (ROBOTWIFI_PASS != NULL)
		WiFi.softAP(ROBOTWIFI_SSID, ROBOTWIFI_PASS);
	else
		WiFi.softAP(ROBOTWIFI_SSID);
}