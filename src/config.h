#define EEPROM_SIZE 20
#define DEVICE_NAME "Gilsa Plug"
#define statictoken true

// #define TOKEN "VE4ZMWlSAt546FSSSHdD"  //1
char TOKEN[20] = {0};

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
#define LOOP_SLEEP_TIME 1
#define LONG_PRESS_ITERATIONS 5000
#define CONNECTION_BLINK_ITERATIONS 10000
#define CHECK_CONNETCTION_ITERATIONS 25000

// Input Constants
#define leftSwitchInput GPIO0
#define midSwitchInput GPIO2
#define rightSwitchInput GPIO1

// Output Constants
#define midSwitchRelay GPIO5
#define linkLed GPIO14
#define redLed GPIO15

// Serial configurations
#define serialEnabled true
#define serialBuadrate 115200

// Wifi configuration
#define resetWifiEnabled false
#define autoGenerateSSID false
#define configPortalTimeout 180

// Server Configuration
int mqttPort = 1883;
char dmiotServer[] = "platform.dmiot.ir";

// We assume that all GPIOs are LOW
bool gpioState[] = {false, false, false, false};

// Device token
// #ifdef statictoken
// #define notoken 1
// #else
// char TOKEN[20] = {0};
// #endif
