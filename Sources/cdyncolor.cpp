#include <cdyncolor.h>

DynColor::DynColor(int DC_OldColor, int DC_NewColor)
{
	OldColor = DC_OldColor;
	NewColor = DC_NewColor;
}

bool DynColor::compare(Object *pObject)
{
	DynColor *pDynColor = dynamic_cast<DynColor*>(pObject);

	if (!pDynColor) return false;

	return pDynColor->GetOldColor() == OldColor && pDynColor->GetNewColor() == NewColor;
}
