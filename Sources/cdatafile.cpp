#include <cdatafile.h>

DataFile::DataFile()
{
	NumIndex = 0;
	position = NULL;
}

DataFile::DataFile(fpos_t DataFile_position, Str DataFile_Index, DataValue DataFile_DataValue)
{
	NumIndex = 0;
	position = DataFile_position;
	Index = DataFile_Index;
	Data = DataFile_DataValue;
}

DataFile::DataFile(int DataFile_NumIndex, fpos_t DataFile_position, DataValue DataFile_DataValue)
{
	NumIndex = DataFile_NumIndex;
	position = DataFile_position;
	Index = Str::Parse(DataFile_NumIndex);
	Data = DataFile_DataValue;
}

DataFile::DataFile(Str Object_Name, Str DataFile_Index, DataValue DataFile_DataValue) : Object(Object_Name)
{
	NumIndex = 0;
	position = NULL;
	Index = DataFile_Index;
	Data = DataFile_DataValue;
}

DataFile::DataFile(DataFile &model)
{
	NumIndex = model.GetNumIndex();
	position = *model.getPosition();
	Index = model.GetIndex();
	SubIndex = *model.GetSubIndex();
	Data = *model.GetData();
}

bool DataFile::compare(Object *pObject)
{
	DataFile *pDataFile = dynamic_cast<DataFile*>(pObject);

	if (pDataFile)
		if (NumIndex)
			return NumIndex == pDataFile->GetNumIndex();
		else
			if (Index.GetLength())
				return Index == pDataFile->GetIndex();

	return false;
}
