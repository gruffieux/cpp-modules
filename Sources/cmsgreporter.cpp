#include <cmsgreporter.h>
#include <cmsg.h>

MsgReporter::MsgReporter()
{
	FilteredMsg = new int = 0;
	AddLine = false;
	MessagePack = new List();
}

MsgReporter::MsgReporter(char *FileName, int size)
{
	int i;

	FilteredMsg = new int[size];
	for (i = 0; i < size; i++) FilteredMsg[i] = 0;
	AddLine = false;
	LogFileName = FileName;
	MessagePack = new List[size];
	fopen_s(&LogFile, LogFileName, "w");
	fclose(LogFile);
}

MsgReporter::MsgReporter(MsgReporter &model)
{
	FilteredMsg = model.GetFilteredMsg();
	AddLine = model.GetAddLine();
	LogFile = model.GetLogFile();
	FilePosition = *model.GetFilePosition();
	LogFileName = *model.GetLogFileName();
	MsgDate = *model.GetMsgDate();
	MessagePack = model.GetMessagePack();
}

MsgReporter::~MsgReporter()
{
	delete [] MessagePack;
	delete [] FilteredMsg;
}

void MsgReporter::AddMsgPack(const int MsgType, Str *MsgPack, const int total, int color)
{
	int i;

	for (i = 0; i < total; i++)
		MessagePack[MsgType].AddElement(new Msg(color, MsgPack[i]));
}

void MsgReporter::WriteMessage(Str Message)
{
	if (!fopen_s(&LogFile, LogFileName, "r+t"))
	{
		fsetpos(LogFile, &FilePosition);
		fprintf(LogFile, ReplaceCharByString(Message, "%", "%%"));
		fgetpos(LogFile, &FilePosition);
		fclose(LogFile);
	}
}

Str MsgReporter::BuildDatedMessage(Str Message)
{
	Str DateString, HourString, DatedMsg;

	MsgDate.goDate();

	DateString = MsgDate.getDateString(Clock::DATE_FORMAT, LOCALE_SYSTEM_DEFAULT, NULL, "dd/MM/yyyy");
	HourString = MsgDate.getDateString(Clock::HOUR_FORMAT, LOCALE_SYSTEM_DEFAULT, NULL, "HH:mm:ss");
	DatedMsg = BuildString("%s, %s: %s\n", DateString.Get(), HourString.Get(), Message.Get());

	return DatedMsg;
}

void MsgReporter::SetAndWriteMessage(const int MsgType, int MsgID)
{
	Str DatedMsg;

	if (!FilteredMsg[MsgType])
	{
		DatedMsg = BuildDatedMessage(*MessagePack[MsgType].GetElement(MsgID)->getName());
		WriteMessage(DatedMsg);
	}
}

void MsgReporter::SetAndWriteMessage(const int MsgType, Str Message)
{
	Str DatedMsg;

	if (!FilteredMsg[MsgType])
	{
		DatedMsg = BuildDatedMessage(Message);
		WriteMessage(DatedMsg);
	}
}

void MsgReporter::SetAndWriteMessage(Str Message)
{
	Str DatedMsg = BuildDatedMessage(Message);

	WriteMessage(DatedMsg);
}

void MsgReporter::CheckMaxLine()
{
	if (ElementCount > MAX_FILE_LINE)
		if (GetLastToFirst())
			RemoveElement(ElementCount-1, 1);
		else
			RemoveElement(0, 1);
}

bool MsgReporter::AddMessage(const int MsgType, int MsgID)
{
	if (!FilteredMsg[MsgType])
	{
		AddLine = true;
		if (!AddElement(MessagePack[MsgType].GetElement(MsgID))) return false;
		CheckMaxLine();
		SetAndWriteMessage(MsgType, MsgID);
		return true;
	}

	return false;
}

bool MsgReporter::AddMessage(const int MsgType, Str Message, int color)
{
	if (!FilteredMsg[MsgType])
	{
		AddLine = true;
		AddElement(new Msg(color, Message));
		CheckMaxLine();
		SetAndWriteMessage(MsgType, Message);
		return true;
	}

	return false;
}

void MsgReporter::FilterMessage(const int MsgType)
{
	if (FilteredMsg[MsgType])
		FilteredMsg[MsgType] = 0;
	else
		FilteredMsg[MsgType] = 1;
}
