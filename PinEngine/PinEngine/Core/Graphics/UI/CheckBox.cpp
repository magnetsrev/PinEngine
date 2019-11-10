#include "CheckBox.h"

using namespace PinEngine;
using namespace PinEngine::UI;
using namespace std;

void PinEngine::UI::CheckBox::RenderOverride(DirectX::FXMMATRIX cameraMatrix)
{
}

bool PinEngine::UI::CheckBox::IsChecked() const
{
	return isChecked;
}

void PinEngine::UI::CheckBox::ToggleCheck()
{
	isChecked = !isChecked;
	if (isChecked)
	{
		for (auto & fnc : OnChecked.callbacks)
		{
			fnc(this);
		}
	}
	else
	{
		for (auto & fnc : OnUnchecked.callbacks)
		{
			fnc(this);
		}
	}
}

void PinEngine::UI::CheckBox::SetText(std::wstring text)
{
	this->label->SetText(text);
}

void PinEngine::UI::CheckBox::SetTextColor(Color color)
{
	label->SetColor(color);
}

void PinEngine::UI::CheckBox::SetFont(std::shared_ptr<UIFont> font)
{
	label->SetFont(font);
	if (!HasChild(label))
		AddChild(label);
}

void PinEngine::UI::CheckBox::OnInitialize()
{
	widgetType = WidgetType::CheckBox;
	checkboxButton = make_shared<Sprite>();
	checkboxButton->Initialize(AnchorPoint::TopLeft, this, AnchorPoint::TopLeft);
	checkboxButton->SetDimensions(25, 25);
	checkboxButton->AssignTexture(L"Data/Textures/checkbox_empty.png");
	this->ToggleMouseInteraction(true);
	checkboxButton->ToggleMouseInteraction(true);
	checkboxButton->OnMouseOver += [](Widget* widget)
	{
		Sprite* sprite = (Sprite*)widget;
		CheckBox* parent = (CheckBox*)sprite->GetParent();
		if (parent->IsChecked())
		{
			sprite->AssignTexture(L"Data/Textures/checkbox_checked_highlight.png");
		}
		else
		{
			sprite->AssignTexture(L"Data/Textures/checkbox_empty_highlight.png");
		}
	};
	checkboxButton->OnMouseExit += [](Widget* widget)
	{
		Sprite* sprite = (Sprite*)widget;
		CheckBox* parent = (CheckBox*)sprite->GetParent();
		if (parent->IsChecked())
		{
			sprite->AssignTexture(L"Data/Textures/checkbox_checked.png");
		}
		else
		{
			sprite->AssignTexture(L"Data/Textures/checkbox_empty.png");
		}
	};
	checkboxButton->OnLeftClick += [](Widget* widget)
	{
		Sprite* sprite = (Sprite*)widget;
		CheckBox* parent = (CheckBox*)sprite->GetParent();
		parent->ToggleCheck();
		if (parent->IsChecked())
		{
			sprite->AssignTexture(L"Data/Textures/checkbox_checked_highlight.png");
		}
		else
		{
			sprite->AssignTexture(L"Data/Textures/checkbox_empty_highlight.png");
		}
	};
	label = make_shared<Label>();
	label->Initialize(AnchorPoint::TopLeft, checkboxButton.get(), AnchorPoint::TopRight);
	label->SetPosition(4, 0);
	AddChild(checkboxButton);
}