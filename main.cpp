#include "utils.h"
#include "cpp_tpl.h"

#include <wx/wx.h>

#include <sstream>
#include <fstream>
#include <locale>
#include <codecvt>

using std::ofstream;
using std::wstring_convert;
using std::codecvt_utf8_utf16;

class MyApp: public wxApp
{
public:
	bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

class MyFrame: public wxFrame
{
public:
	MyFrame(): wxFrame(nullptr, wxID_ANY, "neIDE")
	{
		SetSize(400, 400);
		Center();
		auto *main_sizer = new wxBoxSizer(wxVERTICAL);
		const auto main_window_size = this->GetSize();
		const auto input_area_size = wxSize{ main_window_size.GetWidth(),
			main_window_size.GetHeight() * 3 / 4 };
		const auto output_area_size = wxSize{ main_window_size.GetWidth(),
			main_window_size.GetHeight() / 4 };
		input_area_ = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition,
				input_area_size, wxTE_MULTILINE | wxSTATIC_BORDER,
				wxDefaultValidator, wxTextCtrlNameStr);
		output_area_ =
				new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition,
						output_area_size, wxTE_MULTILINE | wxSTATIC_BORDER,
						wxDefaultValidator, wxTextCtrlNameStr);

		auto *lbl_run = new wxStaticText(this, wxID_ANY, "Run");
		const int padding_px = 10;
		main_sizer->Add(lbl_run, 0, wxLEFT, padding_px);
		main_sizer->Add(input_area_, 3, wxEXPAND | wxALL, padding_px);
		main_sizer->Add(output_area_, 1, wxEXPAND | wxALL, padding_px);
		main_sizer->SetSizeHints(this);
		SetSizer(main_sizer); // activates padding for wxAll

		lbl_run->Bind(wxEVT_LEFT_DOWN, &MyFrame::Run, this);
	}

	wxTextCtrl *input_area_{};
	wxTextCtrl *output_area_{};

	void Run(wxMouseEvent & /*event*/);
};

void MyFrame::Run(wxMouseEvent & /*event*/)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;

	const char *memory_tmp_dir{ "/dev/shm/" };
	const char *local_tmp_dir{ "./" };

	string dir_tmp_files =
			file_exists(memory_tmp_dir) ? memory_tmp_dir : local_tmp_dir;

	const string tmp_cpp_file = dir_tmp_files + "tmp.cpp";
	const string tmp_bin_file = dir_tmp_files + "a.out";

	string clean_cmd{ "rm -f" };
	clean_cmd.append(tmp_cpp_file);
	clean_cmd.append(" ");
	clean_cmd.append(tmp_bin_file);
	run_cmd(move(clean_cmd));

	ofstream file(tmp_cpp_file);
	file << input_area_->GetValue();
	file.close();

	string cmpl_cmd{ "g++ --std=c++17 -Wall " };
	cmpl_cmd.append(tmp_cpp_file);
	cmpl_cmd.append(" -o ");
	cmpl_cmd.append(tmp_bin_file);
	const auto [text_cmpl, status_cmpl] = run_cmd(move(cmpl_cmd));
	output_area_->SetValue(converter.from_bytes(text_cmpl));
	if (status_cmpl != 0) {
		return;
	}

	const auto [text_run, status_run] = run_cmd(tmp_bin_file);
	output_area_->AppendText(converter.from_bytes(text_run));
}

bool MyApp::OnInit()
{
	auto *frame = new MyFrame();
	frame->Show(true);

	frame->input_area_->SetValue(tpl::cpp_tpl_hello);

	return true;
}
