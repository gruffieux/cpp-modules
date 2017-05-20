#ifndef CSQL
#define CSQL

#include <windows.h>
#include <Sqlext.h>
#include <cstr.h>

#define SIMULTANEOUS_REQUEST 5

class SQL
{
private:
	bool connected;
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt[SIMULTANEOUS_REQUEST];
	Str LastSQLError;
public:
	SQL() {connected = false;}
	SQL(SQL &model);
	~SQL() {}
	bool SetLastSQLError(SQLINTEGER error, SQLHSTMT hstmt);
	bool Connection(char *dsn, char *user="", char *password="");
	void Disconnection();
	bool GetConnected() {return connected;}
	SQLHENV Gethenv() {return henv;}
	SQLHDBC Gethdbc() {return hdbc;}
	SQLHSTMT Gethstmt(int n) {return hstmt[n];}
	Str GetLastSQLError() {return LastSQLError;}
};

#endif