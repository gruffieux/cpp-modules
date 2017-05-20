#include <cenergybar.h>

EnergyBar::EnergyBar()
{
	MaxSize = frequency = type = 0;
	MinAmount = MaxAmount = amount = 0;
}

EnergyBar::EnergyBar(float EB_MinAmount, float EB_MaxAmount, int EB_MaxSize, int EB_frequency, int EB_type, int Item_xpos, int Item_ypos, int Item_width, int Item_height) :
Item(Item_xpos, Item_ypos, Item_width, Item_height, "Barre")
{
	MinAmount = EB_MinAmount;
	MaxAmount = EB_MaxAmount;
	MaxSize = EB_MaxSize;
	frequency = EB_frequency;
	type = EB_type;
	amount = 0;
}

EnergyBar::EnergyBar(EnergyBar &model)
{
	MinAmount = model.GetMinAmount();
	MaxAmount = model.GetMaxAmount();
	MaxSize = model.GetMaxSize();
	frequency = model.GetFrequency();
	type = model.GetType();
	amount = model.GetAmount();
	Regulator = *model.GetRegulator();
	Bar = *model.GetBar();
}

void EnergyBar::restoreAni()
{
	Visual::restoreAni();
	Bar.restoreAni();
}

void EnergyBar::destroyAni()
{
	Visual::destroyAni();
	Bar.destroyAni();
}

void EnergyBar::stickToPanel(Axe cellPosition)
{
	Visual::stickToPanel(cellPosition);
	*Bar.getPosition() = cellPosition;
}

bool EnergyBar::showOnPanel(int frame, Item *cursor)
{
	Visual::showOnPanel(frame, cursor);

	if (Bar.getDimension()->x >= 1)
	{
		Bar.setVisibleZone();
		return Bar.runAni(0, type);
	}

	return true;
}

void EnergyBar::GenerateBar()
{
	int size = 0;
	float BarSize = 0;

	if (MaxSize)
	{
		size = MaxSize;
		if (MaxAmount)
			BarSize = amount / MaxAmount * MaxSize;
	}
	else
	{
		size = (int)MaxAmount;
		BarSize = amount;
	}

	dimension.x = size;
	*Bar.getDimension() = Axe((int)BarSize, dimension.y);
}

void EnergyBar::Increase(float gain, int divider, bool regulate)
{
	if (regulate)
		Regulator.goCounter(frequency, 0, 1, true);

	if (amount < MaxAmount)
		if (regulate)
		{
			if (!Regulator.isGoing())
				amount += gain / divider;
		}
		else
			amount += gain / divider;

	if (amount > MaxAmount)
		amount = MaxAmount;
}

void EnergyBar::Decrease(float loss, int divider, bool regulate)
{
	if (loss < 0)
		loss = -loss;

	if (regulate)
		Regulator.goCounter(frequency, 0, 1, true);

	if (amount > MinAmount)
		if (regulate)
		{
			if (!Regulator.isGoing())
				amount -= loss / divider;
		}
		else
			amount -= loss / divider;

	if (amount < MinAmount)
		amount = MinAmount;
}
