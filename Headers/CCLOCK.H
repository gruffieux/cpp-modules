#ifndef CCLOCK
#define CCLOCK

#include <time.h>
#include <windows.h>
#include <cstr.h>

class Clock
{
private:
	int counter, pulse;
	bool go;
	SYSTEMTIME date;
	clock_t ticks;
public:
	enum TIME_FORMAT
	{
		NO_TIME_FORMAT,
		DATE_FORMAT,
		HOUR_FORMAT
	};
	Clock();
	~Clock();
	void decreaseCounter(int amount) {counter -= amount;}
	int getCounter() {return counter;}
	SYSTEMTIME getDate() {return date;}
	Str getDateString(TIME_FORMAT TimeFormat, LCID Locale, DWORD dwFlags, Str format);
	int getPulse() {return pulse;}
	clock_t getTicks() {return ticks;}
	void goDate() {GetLocalTime(&date);}
	bool goCounter(double pps, int start, int end, bool reverse);
	void increaseCounter(int amount) {counter += amount;}
	bool isGoing() {return go;}
	void makeDateFromString(char *data, char locate);
	static int random(int min, int max) {return rand() % (max + 1 - min) + min;}
	void setCounter(int Clock_counter) {counter = Clock_counter;}
	void stopCounter() {go = false;}
	static void useRandom() {srand((int)time(NULL));}
};

#endif
