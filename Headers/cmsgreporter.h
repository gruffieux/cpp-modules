#ifndef CMSGREPORTER
#define CMSGREPORTER

#include <clist.h>
#include <cclock.h>
#include <stdio.h>

#define MAX_FILE_LINE 100

class MsgReporter : public List
{
private:
	int *FilteredMsg;
	bool AddLine;
	FILE * LogFile;
	fpos_t FilePosition;
	Str LogFileName;
	List *MessagePack;
	Clock MsgDate;
	void CheckMaxLine();
	Str BuildDatedMessage(Str Message);
public:
	MsgReporter();
	MsgReporter(char *FileName, int size);
	MsgReporter(MsgReporter &model);
	~MsgReporter();
	void SetAddLine(bool EM_AddLine) {AddLine = EM_AddLine;}
	void AddMsgPack(const int MsgType, Str *MsgPack, const int total, int color);
	void WriteMessage(Str Message);
	void SetAndWriteMessage(const int MsgType, int MsgID);
	void SetAndWriteMessage(const int MsgType, Str Message);
	void SetAndWriteMessage(Str Message);
	bool AddMessage(const int MsgType, int MsgID);
	bool AddMessage(const int MsgType, Str Message, int color);
	void FilterMessage(const int MsgType);
	int * GetFilteredMsg() {return FilteredMsg;}
	int GetFilteredMsg(int index) {return FilteredMsg[index];}
	bool GetAddLine() {return AddLine;}
	FILE * GetLogFile() {return LogFile;}
	fpos_t * GetFilePosition() {return &FilePosition;}
	Str * GetLogFileName() {return &LogFileName;}
	List * GetMessagePack() {return MessagePack;}
	List * GetMessagePack(int index) {return &MessagePack[index];}
	Clock * GetMsgDate() {return &MsgDate;}
};

#endif
