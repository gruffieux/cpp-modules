#include <cclock.h>

Clock::Clock()
{
	counter = pulse = 0;
	go = false;
}

Clock::~Clock()
{
}

Str Clock::getDateString(TIME_FORMAT TimeFormat, LCID Locale, DWORD dwFlags, Str format)
{
	char *buffer = new char[255];

	switch (TimeFormat)
	{
	case DATE_FORMAT:
		GetDateFormat(Locale, dwFlags, &date, format, buffer, 255);
		break;
	case HOUR_FORMAT:
		GetTimeFormat(Locale, dwFlags, &date, format, buffer, 255);
		break;
	}

	return Str(buffer);
}

bool Clock::goCounter(double pps, int start, int end, bool reverse)
{
	int start2 = start;
	int end2 = end;

	if (reverse)
	{
		start = end2;
		end = start2;
	}

	if (!go)
	{
		counter = start;
		go = true;
	}
	
	ticks = clock();

	if (ticks >= pulse + CLOCKS_PER_SEC / pps)
	{
		if (reverse)
			counter--;
		else
			counter++;
		pulse = ticks;
	}

	if (counter == end)
	{
		counter = start;
		go = false;
		return false;
	}
	else
		return true;
}

void Clock::makeDateFromString(char *data, char locate)
{
	char string[255];
	char buffer[4];
	int dmy, i1, i2, i3;

	strcpy_s(string, data);

	i1 = 0;
	while (string[i1] != locate)
		i1++;
	i1 -= 2;

	//Tire le jour de la chaîne et l'ajoute dans la structure de date
	i2 = 0;
	i3 = i1 + 2;
	for (i1; i1 < i3; i1++)
	{
		buffer[i2] = string[i1];
		i2++;
	}
	buffer[i2] = '\0';
	dmy = atoi(buffer);
	date.wDay = dmy;

	//Tire le mois de la chaîne et l'ajoute dans la structure de date
	i1++;
	i2 = 0;
	i3 = i1 + 2;
	for (i1; i1 < i3; i1++)
	{
		buffer[i2] = string[i1];
		i2++;
	}
	buffer[i2] = '\0';
	dmy = atoi(buffer);
	date.wMonth = dmy;

	//Tire l'année de la chaîne et l'ajoute dans la structure de date
	i1++;
	i2 = 0;
	i3 = i1 + 4;
	for (i1; i1 < i3; i1++)
	{
		buffer[i2] = string[i1];
		i2++;
	}
	buffer[i2] = '\0';
	dmy = atoi(buffer);
	date.wYear = dmy;

	date.wDayOfWeek = date.wHour = date.wMinute = date.wSecond = date.wMilliseconds = 0;
}
