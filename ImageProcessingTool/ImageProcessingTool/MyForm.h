/*
Author: Menelaos Kotsollaris
Email: mkotsollari@gmail.com

All rights reserved 2016.
Image Processing & Computer Vision
Dr. Yun Zhang, UNB - GGE
*/
#pragma once
#include <cstdlib>
namespace ImageProcessingTool {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;


	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
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
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  InitialImageButton;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  PSFImageButton;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Button^  ResultButton;
	private: System::Windows::Forms::Button^  ComputeButton;
	private: System::Windows::Forms::Button^  Exit;
	private: System::Windows::Forms::ComboBox^  ComboBox;

	private: void NotchFilter(const char* _initial_image, const char* _psf_image,
		const char* _output_path);
	private: void ImageProcessingTool::MyForm::UnBlur(const char* _initial_image, const char* _psf_image,
		const char* _output_path);

	/*Returns true if all inputs have been inserted; false on the other case*/
	private: bool CheckUIInputs()
	{
		String^ str1 = InitialImageButton->Text;
		String^ str2 = PSFImageButton->Text;
		String^ str3 = ResultButton->Text;
		String^ str4 = ComboBox->Text;
		return(str1 != "..." && str2 != "..." && str3 != "..." && str4 != "");
	};

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->InitialImageButton = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->PSFImageButton = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->ResultButton = (gcnew System::Windows::Forms::Button());
			this->ComputeButton = (gcnew System::Windows::Forms::Button());
			this->ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->Exit = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 25);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(66, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Load Image:";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// InitialImageButton
			// 
			this->InitialImageButton->Location = System::Drawing::Point(224, 20);
			this->InitialImageButton->Name = L"InitialImageButton";
			this->InitialImageButton->Size = System::Drawing::Size(120, 23);
			this->InitialImageButton->TabIndex = 1;
			this->InitialImageButton->Text = L"...";
			this->InitialImageButton->UseVisualStyleBackColor = true;
			this->InitialImageButton->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 187);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(40, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Action:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(12, 77);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(89, 13);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Load PSF Image:";
			// 
			// PSFImageButton
			// 
			this->PSFImageButton->Location = System::Drawing::Point(224, 72);
			this->PSFImageButton->Name = L"PSFImageButton";
			this->PSFImageButton->Size = System::Drawing::Size(120, 23);
			this->PSFImageButton->TabIndex = 5;
			this->PSFImageButton->Text = L"...";
			this->PSFImageButton->UseVisualStyleBackColor = true;
			this->PSFImageButton->Click += gcnew System::EventHandler(this, &MyForm::PSFImageButton_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(12, 134);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(93, 13);
			this->label4->TabIndex = 6;
			this->label4->Text = L"Save Result Path:";
			// 
			// ResultButton
			// 
			this->ResultButton->Location = System::Drawing::Point(224, 129);
			this->ResultButton->Name = L"ResultButton";
			this->ResultButton->Size = System::Drawing::Size(120, 23);
			this->ResultButton->TabIndex = 7;
			this->ResultButton->Text = L"...";
			this->ResultButton->UseVisualStyleBackColor = true;
			this->ResultButton->Click += gcnew System::EventHandler(this, &MyForm::ResultButton_Click);
			// 
			// ComputeButton
			// 
			this->ComputeButton->Location = System::Drawing::Point(15, 230);
			this->ComputeButton->Name = L"ComputeButton";
			this->ComputeButton->Size = System::Drawing::Size(329, 41);
			this->ComputeButton->TabIndex = 8;
			this->ComputeButton->Text = L"Compute";
			this->ComputeButton->UseVisualStyleBackColor = true;
			this->ComputeButton->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// ComboBox
			// 
			this->ComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->ComboBox->FormattingEnabled = true;
			this->ComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Blur & Unblur", L"Notch Filter" });
			this->ComboBox->Location = System::Drawing::Point(223, 184);
			this->ComboBox->Name = L"ComboBox";
			this->ComboBox->Size = System::Drawing::Size(121, 21);
			this->ComboBox->TabIndex = 9;
			this->ComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::ComboBox_SelectedIndexChanged);
			this->ComboBox->SelectedIndex = 0;
			// 
			// Exit
			// 
			this->Exit->Location = System::Drawing::Point(15, 277);
			this->Exit->Name = L"Exit";
			this->Exit->Size = System::Drawing::Size(329, 41);
			this->Exit->TabIndex = 10;
			this->Exit->Text = L"Exit";
			this->Exit->UseVisualStyleBackColor = true;
			this->Exit->Click += gcnew System::EventHandler(this, &MyForm::Exit_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(356, 325);
			this->Controls->Add(this->Exit);
			this->Controls->Add(this->ComboBox);
			this->Controls->Add(this->ComputeButton);
			this->Controls->Add(this->ResultButton);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->PSFImageButton);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->InitialImageButton);
			this->Controls->Add(this->label1);
			this->HelpButton = true;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"MyForm";
			this->Text = L"Image Processing Tool";
			this->HelpButtonClicked += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::MyForm_HelpButtonClicked);
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		OpenFileDialog^ openFileDialog;
		openFileDialog = gcnew OpenFileDialog;

		openFileDialog->InitialDirectory = "Documents\\";
		openFileDialog->Filter = "Images (*.pgm,*.pbm)|*.pgm;*.pbm;";
		openFileDialog->FilterIndex = 2;
		openFileDialog->RestoreDirectory = true;

		if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			array<String^>^ files = openFileDialog->FileNames;
			String^ _file_name = files[0];
			InitialImageButton->Text = _file_name;
		}
	}
	private: System::Void MyForm_HelpButtonClicked(System::Object^ sender, System::ComponentModel::CancelEventArgs^  e) 
	{
		MessageBox::Show("Image Processing Tool Version 0.0.1. " +
			"Developed By Menelaos Kotsollaris. Supervisors: Dr. Yun Zhang.\nAll rights reserved © 2016.",
			"Information", MessageBoxButtons::OK, MessageBoxIcon::Information);
	}
private: System::Void ShowError(String^ _title_string, String^ _message_string)
{
	MessageBox::Show(_message_string, _title_string,
		MessageBoxButtons::OK, MessageBoxIcon::Error);
}
private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) 
{
}
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) 
{
	const char* str1 = (const char*)(void*)
		Marshal::StringToHGlobalAnsi(InitialImageButton->Text);
	const char* str2 = (const char*)(void*)
		Marshal::StringToHGlobalAnsi(PSFImageButton->Text);
	const char* str3 = (const char*)(void*)
		Marshal::StringToHGlobalAnsi(ResultButton->Text);
	const char* str4 = (const char*)(void*)
		Marshal::StringToHGlobalAnsi(ComboBox->Text);
	int _selected_index = ComboBox->SelectedIndex;
	if(_selected_index == 0)
	{
		UnBlur(str1, str2, str3);
	}
	else if(_selected_index == 1)
	{
		NotchFilter(str1, str2, str3);
	}
	/*MessageBox::Show(System::Convert::ToString(ComboBox->SelectedIndex),
		"Information", MessageBoxButtons::OK, MessageBoxIcon::Information);*/
	bool _check_inputs = CheckUIInputs();
	//Calculate(str1, str2, str3, str1);
}
private: System::Void ResultButton_Click(System::Object^ sender, System::EventArgs^  e) 
{
	FolderBrowserDialog^ folderBrowserDialog;
	folderBrowserDialog = gcnew FolderBrowserDialog;
	folderBrowserDialog->Description = "Select the Directory that you want to save the image";
	folderBrowserDialog->ShowNewFolderButton = false;
	if (folderBrowserDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		String^ _selected_path = folderBrowserDialog->SelectedPath;
		ResultButton->Text = _selected_path;
	}
}
private: System::Void PSFImageButton_Click(System::Object^  sender, System::EventArgs^  e) 
{
	OpenFileDialog^ openFileDialog;
	openFileDialog = gcnew OpenFileDialog;

	openFileDialog->InitialDirectory = "Documents\\";
	openFileDialog->Filter = "Images (*.pgm)|*.pgm;";
	openFileDialog->FilterIndex = 2;
	openFileDialog->RestoreDirectory = true;

	if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		array<String^>^ files = openFileDialog->FileNames;
		String^ _file_name = files[0];
		PSFImageButton->Text = _file_name;
	}
}
private: System::Void ComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
}


private: System::Void Exit_Click(System::Object^  sender, System::EventArgs^  e) 
{
	std::exit(0);
}
};
}