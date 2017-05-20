#define DIRECTINPUT_VERSION 0x0800

#include <ckey.h>
#include <dinput.h>

Key::Key(int dik)
{
	this->dik = dik;
	def = dik;
}

Key::Key(int dik, int def, Str name) : Object(name)
{
	this->dik = dik;
	this->def = def;
}

char Key::character(bool upperCase)
{
	switch (dik)
	{
	case DIK_0: return '0';
	case DIK_1: return '1';
	case DIK_2: return '2';
    case DIK_3: return '3';
    case DIK_4: return '4';
    case DIK_5: return '5';
    case DIK_6: return '6';
    case DIK_7: return '7';
    case DIK_8: return '8';
    case DIK_9: return '9';
	case DIK_A: return (upperCase == true) ? 'A' : 'a';
	case DIK_B: return (upperCase == true) ? 'B' : 'b';
	case DIK_C: return (upperCase == true) ? 'C' : 'c';
	case DIK_D: return (upperCase == true) ? 'D' : 'd';
	case DIK_E: return (upperCase == true) ? 'E' : 'e';
	case DIK_F: return (upperCase == true) ? 'F' : 'f';
    case DIK_G: return (upperCase == true) ? 'G' : 'g';
    case DIK_H: return (upperCase == true) ? 'H' : 'h';
    case DIK_I: return (upperCase == true) ? 'I' : 'i';
    case DIK_J: return (upperCase == true) ? 'J' : 'j';
    case DIK_K: return (upperCase == true) ? 'K' : 'k';
    case DIK_L: return (upperCase == true) ? 'L' : 'l';
    case DIK_M: return (upperCase == true) ? 'M' : 'm';
    case DIK_N: return (upperCase == true) ? 'N' : 'n';
    case DIK_O: return (upperCase == true) ? 'O' : 'o';
    case DIK_P: return (upperCase == true) ? 'P' : 'p';
    case DIK_Q: return (upperCase == true) ? 'Q' : 'q';
    case DIK_R: return (upperCase == true) ? 'R' : 'r';
    case DIK_S: return (upperCase == true) ? 'S' : 's';
    case DIK_T: return (upperCase == true) ? 'T' : 't';
    case DIK_U: return (upperCase == true) ? 'U' : 'u';
    case DIK_V: return (upperCase == true) ? 'V' : 'v';
    case DIK_W: return (upperCase == true) ? 'W' : 'w';
    case DIK_X: return (upperCase == true) ? 'X' : 'x';
    case DIK_Y: return (upperCase == true) ? 'Y' : 'y';
	case DIK_Z: return (upperCase == true) ? 'Z' : 'z';
	case DIK_COMMA: return ',';
	case DIK_SPACE: return ' ';
	default: return '\0';
	}
}

Str Key::label()
{
	switch (dik)
	{
	case DIK_0: return Str("0");
	case DIK_1: return Str("1");
	case DIK_2: return Str("2");
    case DIK_3: return Str("3");
    case DIK_4: return Str("4");
    case DIK_5: return Str("5");
    case DIK_6: return Str("6");
    case DIK_7: return Str("7");
    case DIK_8: return Str("8");
    case DIK_9: return Str("9");
    case DIK_A: return Str("A");
    case DIK_ABNT_C1: return Str("ABNT_C1");
    case DIK_ABNT_C2: return Str("ABNT_C2");
    case DIK_ADD: return Str("ADD");
    case DIK_APOSTROPHE: return Str("APOSTROPHE");
    case DIK_APPS: return Str("APPS");
    case DIK_AT: return Str("AT");
    case DIK_AX: return Str("AX");
    case DIK_B: return Str("B");
    case DIK_BACK: return Str("BACK");
    case DIK_BACKSLASH: return Str("BACKSLASH");
    case DIK_C: return Str("C");
    case DIK_CALCULATOR: return Str("CALCULATOR");
    case DIK_CAPITAL: return Str("CAPITAL");
    case DIK_COLON: return Str("COLON");
    case DIK_COMMA: return Str("COMMA");
    case DIK_CONVERT: return Str("CONVERT");
    case DIK_D: return Str("D");
    case DIK_DECIMAL: return Str("DECIMAL");
    case DIK_DELETE: return Str("DELETE");
    case DIK_DIVIDE: return Str("DIVIDE");
    case DIK_DOWN: return Str("DOWN");
    case DIK_E: return Str("E");
    case DIK_END: return Str("END");
    case DIK_EQUALS: return Str("EQUALS");
    case DIK_ESCAPE: return Str("ESCAPE");
    case DIK_F: return Str("F");
    case DIK_F1: return Str("F1");
    case DIK_F2: return Str("F2");
    case DIK_F3: return Str("F3");
    case DIK_F4: return Str("F4");
    case DIK_F5: return Str("F5");
    case DIK_F6: return Str("F6");
    case DIK_F7: return Str("F7");
    case DIK_F8: return Str("F8");
    case DIK_F9: return Str("F9");
    case DIK_F10: return Str("F10");
    case DIK_F11: return Str("F11");
    case DIK_F12: return Str("F12");
    case DIK_F13: return Str("F13");
    case DIK_F14: return Str("F14");
    case DIK_F15: return Str("F15");
    case DIK_G: return Str("G");
    case DIK_GRAVE: return Str("GRAVE");
    case DIK_H: return Str("H");
    case DIK_HOME: return Str("HOME");
    case DIK_I: return Str("I");
    case DIK_INSERT: return Str("INSERT");
    case DIK_J: return Str("J");
    case DIK_K: return Str("K");
    case DIK_KANA: return Str("KANA");
    case DIK_KANJI: return Str("KANJI");
    case DIK_L: return Str("L");
    case DIK_LBRACKET: return Str("LBRACKET");
    case DIK_LCONTROL: return Str("LCONTROL");
    case DIK_LEFT: return Str("LEFT");
    case DIK_LMENU: return Str("LMENU");
    case DIK_LSHIFT: return Str("LSHIFT");
    case DIK_LWIN: return Str("LWIN");
    case DIK_M: return Str("M");
    case DIK_MAIL: return Str("MAIL");
    case DIK_MEDIASELECT: return Str("MEDIASELECT");
    case DIK_MEDIASTOP: return Str("MEDIASTOP");
    case DIK_MINUS: return Str("MINUS");
    case DIK_MULTIPLY: return Str("MULTIPLY");
    case DIK_MUTE: return Str("MUTE");
    case DIK_MYCOMPUTER: return Str("MYCOMPUTER");
    case DIK_N: return Str("N");
    case DIK_NEXT: return Str("NEXT");
    case DIK_NEXTTRACK: return Str("NEXTTRACK");
    case DIK_NOCONVERT: return Str("NOCONVERT");
    case DIK_NUMLOCK: return Str("NUMLOCK");
    case DIK_NUMPAD0: return Str("NUMPAD0");
    case DIK_NUMPAD1: return Str("NUMPAD1");
    case DIK_NUMPAD2: return Str("NUMPAD2");
    case DIK_NUMPAD3: return Str("NUMPAD3");
    case DIK_NUMPAD4: return Str("NUMPAD4");
    case DIK_NUMPAD5: return Str("NUMPAD5");
    case DIK_NUMPAD6: return Str("NUMPAD6");
    case DIK_NUMPAD7: return Str("NUMPAD7");
    case DIK_NUMPAD8: return Str("NUMPAD8");
    case DIK_NUMPAD9: return Str("NUMPAD9");
    case DIK_NUMPADCOMMA: return Str("NUMPADCOMMA");
    case DIK_NUMPADENTER: return Str("NUMPADENTER");
    case DIK_NUMPADEQUALS: return Str("NUMPADEQUALS");
    case DIK_O: return Str("O");
    case DIK_OEM_102: return Str("OEM_102");
    case DIK_P: return Str("P");
    case DIK_PAUSE: return Str("PAUSE");
    case DIK_PERIOD: return Str("PERIOD");
    case DIK_PLAYPAUSE: return Str("PLAYPAUSE");
    case DIK_POWER: return Str("POWER");
    case DIK_PREVTRACK: return Str("PREVTRACK");
    case DIK_PRIOR: return Str("PRIOR");
    case DIK_Q: return Str("Q");
    case DIK_R: return Str("R");
    case DIK_RBRACKET: return Str("RBRACKET");
    case DIK_RCONTROL: return Str("RCONTROL");
    case DIK_RETURN: return Str("RETURN");
    case DIK_RIGHT: return Str("RIGHT");
    case DIK_RMENU: return Str("RMENU");
    case DIK_RSHIFT: return Str("RSHIFT");
    case DIK_RWIN: return Str("RWIN");
    case DIK_S: return Str("S");
    case DIK_SCROLL: return Str("SCROLL");
    case DIK_SEMICOLON: return Str("SEMICOLON");
    case DIK_SLASH: return Str("SLASH");
    case DIK_SLEEP: return Str("SLEEP");
    case DIK_SPACE: return Str("SPACE");
    case DIK_STOP: return Str("STOP");
    case DIK_SUBTRACT: return Str("SUBTRACT");
    case DIK_SYSRQ: return Str("SYSRQ");
    case DIK_T: return Str("T");
    case DIK_TAB: return Str("TAB");
    case DIK_U: return Str("U");
    case DIK_UNDERLINE: return Str("UNDERLINE");
    case DIK_UNLABELED: return Str("UNLABELED");
    case DIK_UP: return Str("UP");
    case DIK_V: return Str("V");
    case DIK_VOLUMEDOWN: return Str("VOLUMEDOWN");
    case DIK_VOLUMEUP: return Str("VOLUMEUP");
    case DIK_W: return Str("W");
    case DIK_WAKE: return Str("WAKE");
    case DIK_WEBBACK: return Str("WEBBACK");
    case DIK_WEBFAVORITES: return Str("WEBFAVORITES");
    case DIK_WEBFORWARD: return Str("WEBFORWARD");
    case DIK_WEBHOME: return Str("WEBHOME");
    case DIK_WEBREFRESH: return Str("WEBREFRESH");
    case DIK_WEBSEARCH: return Str("WEBSEARCH");
    case DIK_WEBSTOP: return Str("WEBSTOP");
    case DIK_X: return Str("X");
    case DIK_Y: return Str("Y");
    case DIK_YEN: return Str("YEN");
	case DIK_Z: return Str("Z");
	default: return Str("<unknown key>");
	}
}
