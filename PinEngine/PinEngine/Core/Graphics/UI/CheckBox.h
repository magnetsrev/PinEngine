#pragma once
#include "Label.h"
#include "Sprite.h"

namespace PinEngine
{
	namespace UI
	{
		class CheckBox : public Widget
		{
		public:
			void SetText(std::wstring text);
			void SetTextColor(Color color);
			void SetFont(std::shared_ptr<UIFont> font);
			bool IsChecked() const;
			void ToggleCheck();
			Event<CheckBox> OnChecked;
			Event<CheckBox> OnUnchecked;
		protected:
			void OnInitialize() override;
		private:
			std::shared_ptr<Sprite> checkboxButton = nullptr;
			std::shared_ptr<Label> label = nullptr;
			bool isChecked = false;
		};
	}
}