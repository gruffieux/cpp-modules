#ifndef CDATAFILELIST
#define CDATAFILELIST

#include <clist.h>
#include <cdatagrouplist.h>
#include <windows.h>
#include <stdio.h>

class DataFileList : public List
{
public:
	enum FILE_STATUS
	{
		CLOSED,
		OPENED_EMPTY,
		OPENED_READONLY,
		OPENED_UPDATE
	};
	enum FILE_INDEX
	{
		NO_INDEX,
		NORMAL_INDEX,
		KEY_INDEX
	};
	DataFileList();
	DataFileList(Str DFL_FileName, int DFL_IndexType=NORMAL_INDEX);
	DataFileList(DataFileList &model);
	~DataFileList();
	bool SetIndexType();
	void SetIndexType(int DFL_IndexType) {IndexType = DFL_IndexType;}
	void SetFileName(Str DFL_FileName) {FileName = DFL_FileName;}
	virtual bool Open(bool ReadOnly, bool CreateIfNotExist, bool erase);
	virtual bool Close();
	bool CreateList();
	bool CreateDataGroupList(Str AttachedIndex, DataGroupList *pDGL);
	bool Write(int value, Str Index="", bool NumIndex=false, Str AttachedIndex="", bool NumAttachedIndex=false, void (CALLBACK *CallBackProc)(int CB_IndexCount)=NULL, int KeyValue=EOF);
	bool WriteString(Str Value, Str Index="", bool NumIndex=false, Str AttachedIndex="", bool NumAttachedIndex=false, int MaxLength=256, void (CALLBACK *CallBackProc)(int CB_IndexCount)=NULL, int KeyValue=EOF);
	int Read(Str Index="", bool NumIndex=false, Str AttachedIndex="", bool NumAttachedIndex=false, void (CALLBACK *CallBackProc)(int CB_IndexCount)=NULL);
	Str ReadString(Str Index="", bool NumIndex=false, Str AttachedIndex="", bool NumAttachedIndex=false, void (CALLBACK *CallBackProc)(int CB_IndexCount)=NULL);
	void ReachFirstPosition() {rewind(pFILE);}
	int ReachLastPosition();
	bool Refresh();
	fpos_t * GetPosition(Str Index="", bool NumIndex=false, Str AttachedIndex="", bool NumAttachedIndex=false);
	int GetCurrentIndex() {return CurrentIndex;}
	int GetIndexCount() {return IndexCount;}
	int GetIndexTotal() {return IndexTotal;}
	int GetIndexType() {return IndexType;}
	bool ErrorOccured() {return LastError.GetLength() > 0;}
	Str GetLastError() {return LastError;}
	FILE_STATUS GetStatus() {return status;}
	Str GetFileName() {return FileName;}
	List* GetAttachedPack() {return &AttachedPack;}
private:
	int CurrentIndex, IndexCount, IndexTotal;
	int IndexType;
	FILE *pFILE;
	Str FileName;
	List AttachedPack;
	Str LastError;
	short StartWrite(Str Index="", bool NumIndex=false, Str AttachedIndex="", bool NumAttachedIndex=false);
	bool WriteIndex(Str Value, bool NumIndex=false);
	bool StartRead(Str Index="", bool NumIndex=false, Str AttachedIndex="", bool NumAttachedIndex=false);
	Str ReadIndex();
	void ReportError(Str ErrMsg="");
	bool CheckOperation(bool writting);
protected:
	FILE_STATUS status;
};

#endif
