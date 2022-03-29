
// Device token
#define TOKEN "YfN0IJPU5Y6lMl7her2o"

// GPIO bindings
#define GPIO0 0
#define GPIO1 1
#define GPIO2 2
#define GPIO3 3
#define GPIO4 4
#define GPIO5 5
#define GPIO9 9
#define GPIO10 10
#define GPIO11 11
#define GPIO12 12
#define GPIO13 13
#define GPIO15 15
#define GPIO13 13
#define GPIO14 14
#define GPIO16 16

// Constants
#define LONG_PRESS_ITERATIONS 600000
#define CONNECTION_BLINK_ITERATIONS 100000
#define CHECK_CONNETCTION_ITERATIONS 5000000

// Input Constants
#define leftSwitchInput GPIO0
#define midSwitchInput GPIO3
#define rightSwitchInput GPIO1

// Output Constants
#define leftSwitchRelay GPIO5
#define midSwitchRelay GPIO14
#define rightSwitchRelay GPIO12
#define linkLed GPIO13

// Serial configurations
#define serialEnabled false
#define serialBuadrate 115200

// Wifi configuration
#define resetWifiEnabled false
#define customIpEnabled false
#define callbackEnabled false
#define autoGenerateSSID false
#define portalTimeout 60

char wifiSSID[20] = "DM Wall T1";

// Server Configuration
int mqttPort = 1883;
char dmiotServer[] = "platform.dmiot.ir";

// We assume that all GPIOs are LOW
bool gpioState[] = {false, false, false, false};
