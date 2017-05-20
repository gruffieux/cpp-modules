#include <cconfigfile.h>
#include <ckey.h>
#include <cinteger.h>
#include <cdatavalue.h>
#include <cdatafile.h>

ConfigFile::ConfigFile() : DataFileList("config.dat", DataFileList::NORMAL_INDEX)
{
	KeyTotal = 0;
}

ConfigFile::~ConfigFile()
{
	KeyMap.RemoveAllElement(0, true);
}

bool ConfigFile::Open(bool ReadOnly, bool CreateIfNotExist, bool erase)
{
	if (!DataFileList::Open(ReadOnly, CreateIfNotExist, erase))
		return false;

	if (status == OPENED_READONLY || status == OPENED_UPDATE)
		KeyTotal = Read("KeyTotal");

	return true;
}

bool ConfigFile::Close()
{
	if (status == OPENED_EMPTY || status == OPENED_UPDATE)
		Write(KeyMap.GetElementCount(), "KeyTotal");

	return DataFileList::Close();
}

bool ConfigFile::WriteGraphic(DisplayMode *pDisplayMode)
{
	if (!Write(pDisplayMode->GetWidth(), "SCREEN_WIDTH")) return false;
	if (!Write(pDisplayMode->GetHeight(), "SCREEN_HEIGHT")) return false;
	if (!Write(pDisplayMode->GetBit(), "BIT")) return false;
	if (!Write(pDisplayMode->GetWindowed(), "WINDOWED")) return false;

	return true;
}

bool ConfigFile::WriteKeys()
{
	int i, dik, def;

	for (i = 0; i < KeyMap.GetElementCount(); i++)
	{
		dik = Key::getKeyElement(i, &KeyMap)->getDik();
		def = Key::getKeyElement(i, &KeyMap)->getDefault();
		if (!def) def = dik;
		if (!Write(dik, "KEY_DIK", false, Str::Parse(i+1), true)) return false;
		if (!Write(def, "KEY_DEF", false, Str::Parse(i+1), true)) return false;
		if (!WriteString(*Key::getKeyElement(i, &KeyMap)->getName(), "KEY_DESC", false, Str::Parse(i+1), true, 24)) return false;
	}

	return true;
}

bool ConfigFile::WriteSound(BOOL PlayMusic, BOOL PlaySounds)
{
	if (!Write(PlayMusic, "MUSIC_ON")) return false;
	if (!Write(PlaySounds, "SOUNDS_ON")) return false;

	return true;
}

bool ConfigFile::WriteModeFilters()
{
	if (!WriteString(ModeFilters, "MODE_FILTERS"))
		return false;

	return true;
}

void ConfigFile::ReadGraphic(DisplayMode *pDisplayMode)
{
	pDisplayMode->SetWidth(Read("SCREEN_WIDTH"));
	pDisplayMode->SetHeight(Read("SCREEN_HEIGHT"));
	pDisplayMode->SetBit(Read("BIT"));
	pDisplayMode->SetWindowed((short)Read("WINDOWED"));

	pDisplayMode->GenerateName();
}

void ConfigFile::ReadKeys()
{
	int i, dik, def;
	Str Description;

	KeyMap.RemoveAllElement(0, true);

	for (i = 0; i < KeyTotal; i++)
	{
		dik = Read("KEY_DIK", false, Str::Parse(i+1), true);
		def = Read("KEY_DEF", false, Str::Parse(i+1), true);
		Description = ReadString("KEY_DESC", false, Str::Parse(i+1), true);
		KeyMap.AddElement(new Key(dik, def, Description));
	}
}

void ConfigFile::ReadModeFilters()
{
	ModeFilters = ReadString("MODE_FILTERS");
}

bool ConfigFile::IsModeValid(int width, int height)
{
	int i, j, last[2], factor;
	bool b, valid;
	Integer ResX, ResY;
	Str Filter, Content, Res;
	Integer *pInteger;
	List Valids;

	if (!ModeFilters.GetLength())
		return true;

	ModeFilters.Trim();

	i = last[0] = 0;
	valid = true;

	while (!last[0])
	{
		Filter = ExtractString(ModeFilters, ";", i, &last[0]);
		if (Filter.GetLength())
		{
			Valids.AddElement(new Integer(0));
			pInteger = Integer::getIntegerElement(Valids.GetElementCount()-1, &Valids);

			// Filtre d'égalité (=[res1]|[res2])
			if (Filter.GetChar(0) == '=')
			{
				j = last[1] = 0;
				Content = Filter.GetSubString(1);
				if (Content.GetLength())
					while (!last[1])
					{
						Res = ExtractString(Content, "|", j, &last[1]);
						if (Res.GetLength())
						{
							ResX = Integer(Str::ToInt(ExtractString(Res, "x", 0)));
							ResY = Integer(Str::ToInt(ExtractString(Res, "x", 1)));
							if (ResX.GetValue() == width && ResY.GetValue() == height)
							{
								pInteger->SetValue(1);
								break;
							}
						}
						j++;
					}
			}

			// Filtre d'inégalité (![res1]&[res2])
			if (Filter.GetChar(0) == '!')
			{
				j = last[1] = 0;
				pInteger->SetValue(1);
				Content = Filter.GetSubString(1);
				if (Content.GetLength())
					while (!last[1])
					{
						Res = ExtractString(Content, "&", j, &last[1]);
						if (Res.GetLength())
						{
							ResX = Integer(Str::ToInt(ExtractString(Res, "x", 0)));
							ResY = Integer(Str::ToInt(ExtractString(Res, "x", 1)));
							if (ResX.GetValue() == width && ResY.GetValue() == height)
							{
								pInteger->SetValue(0);
								break;
							}
						}
						j++;
					}
			}

			// Filtre de divisibilité (/[facteur1]|[facteur2])
			if (Filter.GetChar(0) == '/')
			{
				j = last[1] = 0;
				Content = Filter.GetSubString(1);
				if (Content.GetLength())
					while (!last[1])
					{
						factor = Str::ToInt(ExtractString(Content, "|", j, &last[1]));
						if (factor)
						{
							ResX = Integer(width);
							ResY = Integer(height);
							if (ResX.IsDivisable(factor) && ResY.IsDivisable(factor))
							{
								pInteger->SetValue(1);
								break;
							}
						}
						j++;
					}
			}

			// Filtre de supériorité et d'infériorité (>[res], >=[res], <[res], <=[res])
			if (Filter.GetChar(0) == '>' || Filter.GetChar(0) == '<')
			{
				if (Filter.GetChar(1) == '=')
					Res = Filter.GetSubString(2);
				else
					Res = Filter.GetSubString(1);
				if (Res.GetLength())
				{
					ResX = Integer(Str::ToInt(ExtractString(Res, "x", 0)));
					ResY = Integer(Str::ToInt(ExtractString(Res, "x", 1)));
					if (Filter.GetChar(1) == '=')
					{
						if (Filter.GetChar(0) == '>')
							b = width * height >= ResX.GetValue() * ResY.GetValue();
						else
							b = width * height <= ResX.GetValue() * ResY.GetValue();
					}
					else
						if (Filter.GetChar(0) == '>')
							b = width * height > ResX.GetValue() * ResY.GetValue();
						else
							b = width * height < ResX.GetValue() * ResY.GetValue();
					if (b)
						pInteger->SetValue(1);
				}
			}
		}

		i++;
	}

	// Vérifie que le mode passe tous les filtres
	for (i = 0; i < Valids.GetElementCount(); i++)
		if (!Integer::getIntegerElement(i, &Valids)->GetValue())
		{
			valid = false;
			break;
		}

	Valids.RemoveAllElement(0, true);

	return valid;
}
