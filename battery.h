#define SYSTEM_SUCCESS (0)
// TODO: change MAX_LINE_SIZE to something more meaningful
#define MAX_LINE_SIZE (1023)
#define CYCLE_LOWER_BOUND (30)
#define CYCLE_UPPER_BOUND (80)

#define BATTERY_INFO__MACOS ("pmset -g batt > ./temp/batteryinfo.txt")
#define BATTERY_INFO__LINUX ("upower -i `upower -e \
  | grep 'BAT'` \
  | egrep 'state|percentage' > ./temp/batteryinfo.txt")

#define DELETE_TEMP_CONTENTS ("cd ./temp && rm batteryinfo.txt")
#define BATTERY_INFO_PATH ("./temp/batteryinfo.txt")
#define BATTERY_LOG_PATH ("./log/batterylog.txt")
#define BATTERY_ANALYSIS_PATH ("./log/batteryanalysis.csv")
#define POWER_ON ("https://maker.ifttt.com/trigger/charge_low/with/key/dMgkSNQqbJM-9NI1ySrKdb")
#define POWER_OFF ("https://maker.ifttt.com/trigger/charge_high/with/key/dMgkSNQqbJM-9NI1ySrKdb")


