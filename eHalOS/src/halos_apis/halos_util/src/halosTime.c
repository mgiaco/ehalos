#include <inttypes.h>
#include <string.h>


#include "../inc/halosTime.h"
#include "../inc/halosUtil.h"

void Time_ctor(Time *me, int8_t hour, int8_t minute, int8_t second) {
	me->hour = hour;
	me->minute = minute;
	me->second = second;
}

void Time_increment(Time *me) {
	me->second++;
	if (me->second == 60) {
		me->second = 0;
		me->minute++;
		if (me->minute == 60) {
			me->minute = 0;
			me->hour++;
		}
	}
}

char* Time_toString(Time *me) {

	static char data[] = { "00:00:00" };
	char *tmp;

	if (me->hour <= 9) {
		data[0] = '0';
		tmp = halos_itoa(me->hour);
		strlcpy(&data[1], tmp, 2);
	} else {
		tmp = halos_itoa(me->hour);
		strlcpy(data, tmp, 3);
	}
	data[2] = ':';

	if (me->minute <= 9) {
		data[3] = '0';
		tmp = halos_itoa(me->minute);
		strlcpy(&data[4], tmp, 2);
	} else {
		tmp = halos_itoa(me->minute);
		strlcpy(&data[3], tmp, 3);
	}
	data[5] = ':';

	if (me->second <= 9) {
		data[6] = '0';
		tmp = halos_itoa(me->second);
		strlcpy(&data[7], tmp, 2);
	} else {
		tmp = halos_itoa(me->second);
		strlcpy(&data[6], tmp, 3);
	}

	return data;
}
