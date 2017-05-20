#include <csql.h>

SQL::SQL(SQL &model)
{
	int i;

	connected = model.GetConnected();
	henv = model.Gethenv();
	hdbc = model.Gethdbc();
	for (i = 0; i < SIMULTANEOUS_REQUEST; i++)
		hstmt[i] = model.Gethstmt(i);
}

bool SQL::SetLastSQLError(SQLINTEGER error, SQLHSTMT hstmt)
{
	char *message = new char[255];

	if (SQLError(henv, hdbc, hstmt, NULL, &error, (SQLCHAR*)message, 255, NULL) != SQL_SUCCESS) return false;
	LastSQLError = Str(message);

	return true;
}

bool SQL::Connection(char *dsn, char *user, char *password)
{
	int i;
	SQLRETURN result;

	result = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	if (result != SQL_SUCCESS)
	{
		SetLastSQLError(result, NULL);
		return false;
	}
	
	result = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
	if (result != SQL_SUCCESS)
	{
		SetLastSQLError(result, NULL);
		return false;
	}
	
	result = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	if (result != SQL_SUCCESS)
	{
		SetLastSQLError(result, NULL);
		return false;
	}
	
	result = SQLConnect(hdbc, (SQLCHAR *)dsn, SQL_NTS, (SQLCHAR *)user, SQL_NTS, (SQLCHAR *)password, SQL_NTS);
	if (result != SQL_SUCCESS)
	{
		SetLastSQLError(result, NULL);
		return false;
	}
	
	for (i = 0; i < SIMULTANEOUS_REQUEST; i++)
	{
		result = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt[i]);
		if (result != SQL_SUCCESS)
		{
			SetLastSQLError(result, hstmt[i]);
			return false;
		}
	}

	connected = true;

	return true;
}

void SQL::Disconnection()
{
	int i;

	if (!connected) return;

	for (i = 0; i < SIMULTANEOUS_REQUEST; i++)
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt[i]);

	SQLFreeHandle(SQL_HANDLE_ENV, henv);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	SQLDisconnect(hdbc);

	connected = false;
}
