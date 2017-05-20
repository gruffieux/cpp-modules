#include <cdatafilelist.h>
#include <cdatavalue.h>
#include <cdatagroup.h>
#include <cdatafile.h>

DataFileList::DataFileList()
{
	CurrentIndex = IndexCount = IndexTotal = 0;
	IndexType = NORMAL_INDEX;
	status = CLOSED;
	FileName = "";
}

DataFileList::DataFileList(Str DFL_FileName, int DFL_IndexType)
{
	CurrentIndex = IndexCount = IndexTotal = 0;
	IndexType = DFL_IndexType;
	status = CLOSED;
	FileName = DFL_FileName;
}

DataFileList::DataFileList(DataFileList &model)
{
	CurrentIndex = IndexCount = 0;
	IndexTotal = model.GetIndexTotal();
	IndexType = model.GetIndexType();
	status = CLOSED;
	FileName = model.GetFileName();
}

DataFileList::~DataFileList()
{
	RemoveAllElement(0, true);
	AttachedPack.RemoveAllElement(0, true);
}

bool DataFileList::SetIndexType()
{
	Str Ext;

	Ext = ExtractString(FileName, ".", 1);

	if (Ext == (Str)"dat")
	{
		IndexType = NORMAL_INDEX;
		return true;
	}

	if (Ext == (Str)"kdat")
	{
		IndexType = KEY_INDEX;
		return true;
	}

	IndexType = NO_INDEX;

	return false;
}

bool DataFileList::Open(bool ReadOnly, bool CreateIfNotExist, bool erase)
{
	if (ReadOnly)
	{
		if (fopen_s(&pFILE, FileName.Get(), "rb")) return false;
		status = OPENED_READONLY;
	}
	else
		if (erase)
		{
			if (fopen_s(&pFILE, FileName.Get(), "wb+")) return false;
			status = OPENED_EMPTY;
		}
		else
			if (!fopen_s(&pFILE, FileName.Get(), "rb+"))
				status = OPENED_UPDATE;
			else
				if (CreateIfNotExist)
				{
					if (fopen_s(&pFILE, FileName.Get(), "wb+")) return false;
					status = OPENED_EMPTY;
				}
				else
					return false;

	CurrentIndex = IndexCount = 0;
	LastError = "";

	if (!IndexType)
		return true;

	switch (status)
	{
	case OPENED_EMPTY:
		return true;
	case OPENED_READONLY:
		if (!CreateList())
		{
			Close();
			return false;
		}
		IndexTotal = Read("IndexTotal");
		return true;
	case OPENED_UPDATE:
		if (!CreateList())
		{
			Close();
			return false;
		}
		IndexTotal = Read("IndexTotal");
		return true;
	}

	return false;
}

bool DataFileList::Close()
{
	if (status == CLOSED) return false;

	if (IndexType)
		if (status == OPENED_EMPTY || status == OPENED_UPDATE)
			Write(IndexCount, "IndexTotal");

	if (fclose(pFILE)) return false;

	status = CLOSED;

	if (IndexType)
	{
		RemoveAllElement(0, true);
		AttachedPack.RemoveAllElement(0, true);
	}

	return true;
}

bool DataFileList::CreateList()
{
	int i, NumIndex, NumAttachedIndex, MaxLength, type;
	char *buffer;
	fpos_t position;
	Str Index, AttachedIndex;
	DataValue Data;
	DataFile *AttachedElement;

	if (!IndexType) return false;

	RemoveAllElement(0, true);
	AttachedPack.RemoveAllElement(0, true);

	if (!CheckOperation(false)) return false;

	rewind(pFILE);

	Index = ReadIndex();
	AttachedIndex = ReadIndex();
	fgetpos(pFILE, &position);
	CurrentIndex = _getw(pFILE);

	while (CurrentIndex != EOF)
	{
		type = _getw(pFILE);
		if (IndexType == KEY_INDEX) _getw(pFILE);
		switch (type)
		{
		case EOF:
			ReportError();
			break;
		case DataValue::INTEGER:
			Data = DataValue(Str::Parse(_getw(pFILE)), -1, type);
			break;
		case DataValue::STRING:
			MaxLength = _getw(pFILE);
			_getw(pFILE);
			buffer = new char[MaxLength];
			if (fread(buffer, 1, MaxLength, pFILE) != MaxLength)
				ReportError();
			Data = DataValue(buffer, MaxLength, type);
			delete [] buffer;
			break;
		default:
			ReportError("DataFileList::CreateList: unknown value type");
			return false; // type inconnu
		}

		NumIndex = Index.Get(10);
		NumAttachedIndex = AttachedIndex.Get(10);
		AttachedElement = NULL;

		if (NumAttachedIndex)
			AttachedElement = new DataFile(NumAttachedIndex, NULL, Data);
		else
			if (AttachedIndex.GetLength())
				AttachedElement = new DataFile(NULL, AttachedIndex, Data);

		if (AttachedElement)
		{
			i = AttachedPack.SearchElement(AttachedElement, true);
			if (i == -1)
				if (NumAttachedIndex)
				{
					i = AttachedPack.GetElementCount();
					if (!AddElement(new DataFile(NumAttachedIndex, position, Data))) return false;
					if (!AttachedPack.AddElement(AttachedElement)) return false;
				}
				else
					if (AttachedIndex.GetLength())
					{
						i = AttachedPack.GetElementCount();
						if (!AddElement(new DataFile(position, AttachedIndex, Data))) return false;
						if (!AttachedPack.AddElement(AttachedElement)) return false;
					}
			if (i != -1)
				if (NumIndex)
				{
					if (!DataFile::getDataFileElement(i, &AttachedPack)->GetSubIndex()->AddElement(new DataFile(NumIndex, position, Data)))
						return false;
				}
				else
					if (Index.GetLength())
						if (!DataFile::getDataFileElement(i, &AttachedPack)->GetSubIndex()->AddElement(new DataFile(position, Index, Data)))
							return false;
		}
		else
			if (NumIndex)
			{
				if (!AddElement(new DataFile(NumIndex, position, Data)))
					return false;
			}
			else
				if (Index.GetLength())
					if (!AddElement(new DataFile(position, Index, Data)))
						return false;
		
		Index = ReadIndex();
		AttachedIndex = ReadIndex();
		fgetpos(pFILE, &position);
		CurrentIndex = _getw(pFILE);
	}

	CurrentIndex = 0;
	rewind(pFILE);

	return true;
}

bool DataFileList::CreateDataGroupList(Str AttachedIndex, DataGroupList *pDGL)
{
	int i, j, type, KeyValue, length, MaxLength;
	bool NewGroup;
	char *buffer;
	fpos_t *position;
	Str Value;
	DataFile *Element;
	DataGroup *Group;

	if ((IndexType != KEY_INDEX) | (pDGL == NULL) | (CheckOperation(false) == false))
		return false;

	for (i = 0; i < AttachedPack.GetElementCount(); i++)
		if (DataFile::getDataFileElement(i, &AttachedPack)->GetIndex() == AttachedIndex)
		{
			Group = NULL;
			for (j = 0; j < DataFile::getDataFileElement(i, &AttachedPack)->GetSubIndex()->GetElementCount(); j++)
			{
				Element = DataFile::getDataFileElement(j, DataFile::getDataFileElement(i, &AttachedPack)->GetSubIndex());
				position = Element->getPosition();
				if (position)
				{
					fsetpos(pFILE, position);
					_getw(pFILE); // Read index
					type = _getw(pFILE);
					KeyValue = _getw(pFILE);

					NewGroup = !Group || KeyValue != Group->GetKey();

					if (NewGroup)
					{
						Group = new DataGroup(KeyValue);
						Group->getIndexData()->alpha = AttachedIndex;
					}

					// Read value by type
					switch (type)
					{
					case DataValue::INTEGER:
						Group->AddValue(Element->GetIndex(), _getw(pFILE));
						break;
					case DataValue::STRING:
						MaxLength = _getw(pFILE);
						length = _getw(pFILE);
						buffer = new char[MaxLength];
						if (fread(buffer, 1, MaxLength, pFILE) != MaxLength)
							ReportError();
						Value.Cpy(buffer, length);
						delete [] buffer;
						Group->AddValue(Element->GetIndex(), Value, MaxLength);
						break;
					default:
						break;
					}
				}

				if (NewGroup)
					pDGL->AddElement(Group);
			}
		}

	return true;
}

short DataFileList::StartWrite(Str Index, bool NumIndex, Str AttachedIndex, bool NumAttachedIndex)
{
	short result = 0;
	fpos_t *position;

	if (!CheckOperation(true) | !IndexType)
		return 3;

	position = GetPosition(Index, NumIndex, AttachedIndex, NumAttachedIndex);

	if (position)
	{
		fsetpos(pFILE, position);
		CurrentIndex = _getw(pFILE);
		rewind(pFILE);
		fsetpos(pFILE, position);
		result = 1;
	}
	else
	{
		// Nouvel index, on se place à la fin du fichier
		if (Index.GetLength())
		{
			Refresh();
			CurrentIndex = ReachLastPosition();
			Refresh();
		}
		CurrentIndex++;
		WriteIndex(Index, NumIndex);
		WriteIndex(AttachedIndex, NumAttachedIndex);
		result = 2;
	}

	IndexCount++;

	return result;
}

bool DataFileList::WriteIndex(Str Value, bool NumIndex)
{
	int value;

	if (!IndexType)
		return false;

	// On définit si c'est indexé ou pas
	if (NumIndex)
	{
		value = Value.Get(10);
		if (value)
		{
			if (_putw(DataValue::INTEGER, pFILE) == EOF) ReportError();
			if (_putw(value, pFILE) == EOF) ReportError();
		}
		else
			if (_putw(0, pFILE) == EOF) ReportError();
	}
	else
		if (Value.GetLength())
		{
			if (_putw(DataValue::STRING, pFILE) == EOF) ReportError();
			if (_putw(Value.GetLength(), pFILE) == EOF) ReportError();
			if (fputs(Value.Get(), pFILE) == EOF) ReportError();
		}
		else
			if (_putw(0, pFILE) == EOF) ReportError();

	return true;
}

/*
Bloc de données:
- Indexation (>0 si indexé, 0 dans le cas contraire)
	- Identifiant de l'index (si indexé)
- Index attaché (>0 si indexé, 0 dans le cas contraire)
	- Identifiant du de l'index attaché (si attaché)
- Indice de donnée
- Type de valeur (1 pour entier)
- Valeur (entier)
*/
bool DataFileList::Write(int value, Str Index, bool NumIndex, Str AttachedIndex, bool NumAttachedIndex, void (CALLBACK *CallBackProc)(int CB_IndexCount), int KeyValue)
{
	if (IndexType)
	{
		if (!StartWrite(Index, NumIndex, AttachedIndex, NumAttachedIndex)) return false;
		if (_putw(CurrentIndex, pFILE) == EOF) ReportError();
		if (_putw(DataValue::INTEGER, pFILE) == EOF) ReportError();
		if (IndexType == KEY_INDEX && KeyValue != EOF && _putw(KeyValue, pFILE) == EOF) ReportError();
	}

	if (_putw(value, pFILE) == EOF) ReportError();

	if (CallBackProc) CallBackProc(IndexCount);

	return true;
}

/*
Bloc de données:
- Indexation (>0 si indexé, 0 dans le cas contraire)
	- Identifiant de l'index (si indexé)
- Indice de donnée
- Type de valeur (2 pour chaîne)
- Taille maxi de la chaîne
- Taille de la chaîne
- Valeur (chaîne)
*/
bool DataFileList::WriteString(Str Value, Str Index, bool NumIndex, Str AttachedIndex, bool NumAttachedIndex, int MaxLength, void (CALLBACK *CallBackProc)(int CB_IndexCount), int KeyValue)
{
	int FixedMaxLength;
	short result;
	Str FixedValue;

	result = StartWrite(Index, NumIndex, AttachedIndex, NumAttachedIndex);
	if (!result) return false;

	if (result != 3)
	{
		if (_putw(CurrentIndex, pFILE) == EOF) ReportError();
		if (_putw(DataValue::STRING, pFILE) == EOF) ReportError();
		if (IndexType == KEY_INDEX && KeyValue != EOF && _putw(KeyValue, pFILE) == EOF) ReportError();
	}

	// On écrit la chaîne
	// La taille maxi ne peut changer si l'index existe déjà
	if (result == 2 || result == 3)
	{
		if (Value.GetLength() > MaxLength)
			FixedValue.Cpy(Value.Get(), MaxLength);
		else
			FixedValue = Value;
		if (_putw(MaxLength, pFILE) == EOF) ReportError();
		if (_putw(FixedValue.GetLength(), pFILE) == EOF) ReportError();
		if (fwrite(FixedValue.Get(), 1, MaxLength, pFILE) != MaxLength) ReportError();
	}
	else
	{
		Refresh();
		FixedMaxLength = _getw(pFILE);
		Refresh();
		if (Value.GetLength() > FixedMaxLength)
			FixedValue.Cpy(Value.Get(), FixedMaxLength);
		else
			FixedValue = Value;
		if (FixedMaxLength != MaxLength)
			ReportError(BuildString("DataFileList::WriteString: MaxLength couldn't be changed to %d.\nIt has been fixed to %d when added", MaxLength, FixedMaxLength));
		if (_putw(FixedValue.GetLength(), pFILE) == EOF) ReportError();
		if (fwrite(FixedValue.Get(), 1, FixedMaxLength, pFILE) != FixedMaxLength) ReportError();
	}

	if (CallBackProc) CallBackProc(IndexCount);

	return true;
}

bool DataFileList::StartRead(Str Index, bool NumIndex, Str AttachedIndex, bool NumAttachedIndex)
{
	fpos_t *position;

	if (!CheckOperation(false) | !IndexType)
		return false;

	// On se positionne sur le bloc de donnée d'après, si il existe, l'indice d'identification
	position = GetPosition(Index, NumIndex, AttachedIndex, NumAttachedIndex);
	if (position) 
		fsetpos(pFILE, position);
	else
	{
		ReadIndex();
		ReadIndex();
	}

	CurrentIndex = _getw(pFILE);
	IndexCount++;

	return true;
}

Str DataFileList::ReadIndex()
{
	int size;
	char *buffer;
	Str Value;

	if (!IndexType)
	{
		ReportError("DataFileList::ReadIndex: This file is not indexed.");
		return (Str)"";
	}

	// Si c'est indexé on récupère l'identifiant
	switch (_getw(pFILE))
	{
	case EOF:
		ReportError();
		break;
	case 0:
		break;
	case DataValue::INTEGER:
		Value.Cpy(_getw(pFILE));
		break;
	case DataValue::STRING:
		size = _getw(pFILE) + 1;
		buffer = new char[size];
		fgets(buffer, size, pFILE);
		if (!buffer) ReportError("DataFileList::ReadIndex: could not get string into buffer.");
		Value = Str(buffer);
		delete [] buffer;
		break;
	default:
		ReportError("DataFileList::ReadIndex: index type unexcepted.");
		break;
	}

	return Value;
}

int DataFileList::Read(Str Index, bool NumIndex, Str AttachedIndex, bool NumAttachedIndex, void (CALLBACK *CallBackProc)(int CB_IndexCount))
{
	if (IndexType)
	{
		if (!StartRead(Index, NumIndex, AttachedIndex, NumAttachedIndex))
			return EOF;

		// On vérifie le type de valeur
		if (_getw(pFILE) != DataValue::INTEGER)
			ReportError("DataFileList::Read: integer value excepted.");

		if (IndexType == KEY_INDEX)
			_getw(pFILE);
	}

	if (CallBackProc) CallBackProc(IndexCount);
	
	return _getw(pFILE);
}

Str DataFileList::ReadString(Str Index, bool NumIndex, Str AttachedIndex, bool NumAttachedIndex, void (CALLBACK *CallBackProc)(int CB_IndexCount))
{
	int length, MaxLength;
	char *buffer;
	Str Value;

	if (IndexType)
	{
		if (!StartRead(Index, NumIndex, AttachedIndex, NumAttachedIndex))
			return Value;

		// On vérifie le type de valeur
		if (_getw(pFILE) != DataValue::STRING)
			ReportError("DataFileList::ReadString: string value excepted.");

		if (IndexType == KEY_INDEX)
			_getw(pFILE);
	}

	// On récupère la chaîne
	MaxLength = _getw(pFILE);
	length = _getw(pFILE);
	buffer = new char[MaxLength];
	if (fread(buffer, 1, MaxLength, pFILE) != MaxLength) ReportError();
	Value.Cpy(buffer, length);
	delete [] buffer;

	if (CallBackProc) CallBackProc(IndexCount);

	return Value;
}

int DataFileList::ReachLastPosition()
{
	int LastIndex = CurrentIndex;

	if (!CheckOperation(false) | !IndexType)
		return LastIndex;

	while (CurrentIndex != EOF)
	{
		ReadIndex();
		ReadIndex();
		LastIndex = CurrentIndex;
		CurrentIndex = _getw(pFILE);
		_getw(pFILE);
		if (IndexType == KEY_INDEX) _getw(pFILE);
		_getw(pFILE);
	}

	return LastIndex;
}

bool DataFileList::Refresh()
{
	fpos_t position;

	if (!CheckOperation(false)) return false;
	if (fgetpos(pFILE, &position)) return false;
	rewind(pFILE);
	if (fsetpos(pFILE, &position)) return false;

	return true;
}

bool DataFileList::CheckOperation(bool writting)
{
	switch (status)
	{
	case CLOSED:
		ReportError("DataFileList::CheckOperation: The file is closed!");
		return false;
	case OPENED_READONLY:
		if (writting)
		{
			ReportError("DataFileList::CheckOperation: Writting data in ReadOnly mode is forbidden!");
			return false;
		}
		return true;
	}

	return true;
}

void DataFileList::ReportError(Str ErrMsg)
{
	int ErrNo;
	char *buffer;

	if (!ErrMsg.GetLength())
	{
		ErrNo = ferror(pFILE);
		if (ErrNo)
		{
			buffer = new char[255];
			if (!strerror_s(buffer, 255, ErrNo))
				LastError = Str(buffer);
			else
				LastError = "DataFileList::ReportError: Unknown error.";
			delete [] buffer;
		}
	}
	else
		LastError = ErrMsg;
}

fpos_t * DataFileList::GetPosition(Str Index, bool NumIndex, Str AttachedIndex, bool NumAttachedIndex)
{
	int i, j;
	fpos_t *position;
	DataValue Data;
	DataFile *Element, *AttachedElement;

	position = NULL;
	Element = AttachedElement = NULL;

	if (!IndexType)
	{
		ReportError("DataFileList::GetPosition: This file is not indexed");
		return NULL;
	}

	if (!ElementCount)
	{
		if (status != OPENED_EMPTY) ReportError("DataFileList::GetPosition: Index list not created!");
		return NULL;
	}

	if (NumIndex)
		Element = new DataFile(Index.Get(10), NULL, Data);
	else
		if (Index.GetLength())
			Element = new DataFile(NULL, Index, Data);
		else
			return NULL; // Pas d'indexation

	if (NumAttachedIndex)
		AttachedElement = new DataFile(AttachedIndex.Get(10), NULL, Data);
	else
		if (AttachedIndex.GetLength())
			AttachedElement = new DataFile(NULL, AttachedIndex, Data);

	if (AttachedElement)
	{
		i = AttachedPack.SearchElement(AttachedElement, true);
		if (i != -1)
		{
			j = DataFile::getDataFileElement(i, &AttachedPack)->GetSubIndex()->SearchElement(Element, true);
			if (j != -1) position = DataFile::getDataFileElement(j, DataFile::getDataFileElement(i, &AttachedPack)->GetSubIndex())->getPosition();
		}
	}
	else
	{
		i = SearchElement(Element, true);
		if (i != -1) position = DataFile::getDataFileElement(i, this)->getPosition();
	}

	delete Element;
	delete AttachedElement;

	return position;
}
