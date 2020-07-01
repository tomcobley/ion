#ifndef BATTERY_H
#define BATTERY_H

#include "sys.h"
#include "log.h"
#include "./curl.h"

#define SYSTEM_SUCCESS (0)
#define MAX_BUFFER_SIZE (2042)


#define BATTERY_INFO__MACOS ("pmset -g batt > ./temp/batteryinfo.txt")
#define BATTERY_INFO__LINUX ("upower -i `upower -e \
  | grep 'BAT'` \
  | egrep 'state|percentage' > ./temp/batteryinfo.txt")

#define DELETE_TEMP_CONTENTS ("cd ./temp && rm batteryinfo.txt")
#define BATTERY_INFO_PATH ("./temp/batteryinfo.txt")
#define BATTERY_LOG_PATH ("./log/batterylog.txt")
#define BATTERY_ANALYSIS_PATH ("./log/batteryanalysis.csv")

void free_battery(battery_t *battery);

battery_t *alloc_battery(void);

#endif
