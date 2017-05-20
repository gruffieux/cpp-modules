#ifndef CENERGYBAR
#define CENERGYBAR

#include <cvisual.h>

class EnergyBar : public Visual
{
private:
	int MaxSize, frequency, type;
	float amount, MinAmount, MaxAmount;
	Clock Regulator;
	Visual Bar;
public:
	EnergyBar();
	EnergyBar(float EB_MinAmount, float EB_MaxAmount, int EB_MaxSize, int EB_frequency, int EB_type, int Item_xpos, int Item_ypos, int Item_width, int Item_height);
	EnergyBar(EnergyBar &model);
	~EnergyBar() {}
	void SetMaxSize(int EB_MaxSize) {MaxSize = EB_MaxSize;}
	void SetAmount(float EB_amount) {amount = EB_amount;}
	void SetMinAmount(float EB_MinAmount) {MinAmount = EB_MinAmount;}
	void SetMaxAmount(float EB_MaxAmount) {MaxAmount = EB_MaxAmount;}
	void restoreAni();
	void destroyAni();
	void stickToPanel(Axe CellPosition);
	bool showOnPanel(int frame, Item *Cursor);
	void GenerateBar();
	void Increase(float gain, int divider, bool regulate);
	void Decrease(float loss, int divider, bool regulate);
	float GetMinAmount() {return MinAmount;}
	float GetMaxAmount() {return MaxAmount;}
	int GetMaxSize() {return MaxSize;}
	int GetFrequency() {return frequency;}
	int GetType() {return type;}
	float GetAmount() {return amount;}
	Clock * GetRegulator() {return &Regulator;}
	Visual * GetBar() {return &Bar;}
	static EnergyBar* getEnergyBarElement(int index, List *pList) {return dynamic_cast<EnergyBar*>(pList->GetElement(index));}
};

#endif
