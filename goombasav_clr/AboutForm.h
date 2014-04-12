#pragma once
#include "LicenseForm.h"

namespace goombasav_clr {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for AboutForm
	/// </summary>
	public ref class AboutForm : public System::Windows::Forms::Form
	{
	public:
		AboutForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AboutForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  lblName;
	private: System::Windows::Forms::Label^  lblVersion;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  btnLicense;
	private: System::Windows::Forms::Label^  lblCopyright;

	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(AboutForm::typeid));
			this->lblName = (gcnew System::Windows::Forms::Label());
			this->lblVersion = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->btnLicense = (gcnew System::Windows::Forms::Button());
			this->lblCopyright = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// lblName
			// 
			this->lblName->AutoSize = true;
			this->lblName->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblName->Location = System::Drawing::Point(12, 9);
			this->lblName->Name = L"lblName";
			this->lblName->Size = System::Drawing::Size(240, 25);
			this->lblName->TabIndex = 0;
			this->lblName->Text = L"Goomba Save Manager";
			// 
			// lblVersion
			// 
			this->lblVersion->AutoSize = true;
			this->lblVersion->Location = System::Drawing::Point(14, 34);
			this->lblVersion->Name = L"lblVersion";
			this->lblVersion->Size = System::Drawing::Size(82, 17);
			this->lblVersion->TabIndex = 1;
			this->lblVersion->Text = L"2014-04-11";
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label1->Location = System::Drawing::Point(12, 77);
			this->label1->Margin = System::Windows::Forms::Padding(3, 9, 3, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(408, 175);
			this->label1->TabIndex = 2;
			this->label1->Text = resources->GetString(L"label1.Text");
			// 
			// btnLicense
			// 
			this->btnLicense->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->btnLicense->AutoSize = true;
			this->btnLicense->Location = System::Drawing::Point(12, 264);
			this->btnLicense->Name = L"btnLicense";
			this->btnLicense->Size = System::Drawing::Size(79, 27);
			this->btnLicense->TabIndex = 3;
			this->btnLicense->Text = L"View GPL";
			this->btnLicense->UseVisualStyleBackColor = true;
			this->btnLicense->Click += gcnew System::EventHandler(this, &AboutForm::btnLicense_Click);
			// 
			// lblCopyright
			// 
			this->lblCopyright->AutoSize = true;
			this->lblCopyright->Location = System::Drawing::Point(14, 51);
			this->lblCopyright->Name = L"lblCopyright";
			this->lblCopyright->Size = System::Drawing::Size(137, 17);
			this->lblCopyright->TabIndex = 4;
			this->lblCopyright->Text = L"(C) 2014 libertyernie";
			// 
			// AboutForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(432, 303);
			this->Controls->Add(this->lblCopyright);
			this->Controls->Add(this->btnLicense);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->lblVersion);
			this->Controls->Add(this->lblName);
			this->Name = L"AboutForm";
			this->Text = L"About";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		Void btnLicense_Click(Object^ sender, EventArgs^ e) {
			LicenseForm l;
			l.ShowDialog();
		}
};
}
