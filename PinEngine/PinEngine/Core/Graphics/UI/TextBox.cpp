#include "TextBox.h"
#include "..//Helpers//FontManager.h"

namespace PinEngine
{
	namespace UI
	{
		void TextBox::ProcessChar(wchar_t ch)
		{
			std::wstring text = label->GetText();

			switch (ch)
			{
			case 0x8: //backspace
				if (text.size() > 0) //This is rigged up, but pretty much we're not going to allow multicolored text codes with pipes in textboxes for now. Will come back to this later.
				{
					auto lastCharIndex = text.size() - 1;
					if (text[lastCharIndex] == '|')
					{
						text = text.substr(0, text.size() - 2);
					}
					else
					{
						text = text.substr(0, text.size() - 1);
					}
					label->SetText(text);
				}
				break;
			case 0x03: //Copy
				break;
			case 0x16: //Paste
				break;
			case 0x18: //Cut
				break;
			case 0x1B: //Escape
				break;
			case '|':
				text += L"||";
				label->SetText(text);
				break;
			default:
				text += ch;
				label->SetText(text);
				break;
			}
		}

		void TextBox::SetText(std::wstring text)
		{
			label->SetText(text);
		}

		std::wstring TextBox::GetText()
		{
			return label->GetText();
		}

		void TextBox::OnInitialize()
		{
			widgetType = WidgetType::TextBox;
			label = std::make_shared<Label>();
			label->Initialize(AnchorPoint::Center, this);
			label->SetFont(FontManager::GetFont(L"Times New Roman", 16));
			label->SetText(L"Temporary");
			AddChild(label);
		}

	}
}

