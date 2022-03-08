
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
#define midSwitchInput GPIO4
#define leftSwitchInput GPIO14
#define rightSwitchInput GPIO5

// Output Constants
#define midSwitchRelay GPIO15
#define leftSwitchRelay GPIO2
#define rightSwitchRelay GPIO13
#define pinsPullDown GPIO16
#define whiteLed GPIO0

// Server Configuration
int mqttPort = 1883;
char dmiotServer[] = "46.100.58.20";

// We assume that all GPIOs are LOW
bool gpioState[] = {false, false, false, false};
char wifiSSID[20] = "DM Switch 1 pole";
