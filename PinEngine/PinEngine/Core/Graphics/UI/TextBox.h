#pragma once
#include "Label.h"

namespace PinEngine
{
	namespace UI
	{
		class TextBox : public Widget
		{
		public:
			void ProcessChar(wchar_t ch) override;
			void SetText(std::wstring text);
			std::wstring GetText();
		private:
			void OnInitialize() override;
			std::shared_ptr<Label> label = nullptr;
		};
	}
}