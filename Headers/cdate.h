////////////////////////////////////
///	\file cdate.h
///	\brief Date class header
///
///	\date 08.10.2008
///	\author Gabriel Ruffieux
////////////////////////////////////

//////////////////////////////////////////////////////////////
///	\brief Implements methods to compare and manipulate date
///
///	Date format: dd.mm.yyyy\n
///	Hour format: hh:mm:ss
class Date
{
private:
  int day, month, year, hour, minute, second;

  bool parseDate(char *format);
  bool parseTime(char *format);
  int compareDate(Date date);
  int compareTime(Date time);
  bool dateIsEmpty() {return day == 0 && month == 0 && year == 0;}
  bool timeIsEmpty() {return hour == 0 && minute == 0 && second == 0;}
public:
  Date();
  Date(char *format);
  Date(int day, int month, int year, int hour=0, int minute=0, int second=0);
  Date(Date &model);
  ~Date();

  static Date currentDate();

  void setDay(int day) {this->day = day;}
  void setMonth(int month) {this->month = month;}
  void setYear(int year) {this->year = year;}
  void setMinute(int minute) {this->minute = minute;}
  void setSecond(int second) {this->second = second;}
  void setHour(int hour) {this->hour = hour;}
  int getDay() {return day;}
  int getMonth() {return month;}
  int getYear() {return year;}
  int getHour() {return hour;}
  int getMinute() {return minute;}
  int getSecond() {return second;}

  void fwdHour(int h);
  void rewHour(int h);
  void fwdDay(int d);
  void rewDay(int d);
  void fwdMonth(int m);
  void rewMonth(int m);
  void fwdYear(int y) {year++;}
  void rewYear(int y) {year--;}
  bool parse(char *format);
  int compare(Date date);

  bool isLeapYear();
  bool isEmpty() {return dateIsEmpty() && timeIsEmpty();}
  int lastDayOfMonth();

  friend bool operator==(Date d1, Date d2) {return d1.compare(d2) == 0;}
  friend bool operator>(Date d1, Date d2) {return d1.compare(d2) == 1;}
  friend bool operator<(Date d1, Date d2) {return d1.compare(d2) == -1;}
  friend bool operator>=(Date d1, Date d2) {return d1.compare(d2) == 0 || d1.compare(d2) == 1;}
  friend bool operator<=(Date d1, Date d2) {return d1.compare(d2) == 0 || d1.compare(d2) == -1;}
};
