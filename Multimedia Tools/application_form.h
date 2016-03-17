#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <opencv\cv.hpp>
#include <opencv2\stitching.hpp>

#include <msclr\marshal_cppstd.h>

#include "keyframse_selector.h"

namespace MultimediaTools {

  std::string source_images_file_directory;
  std::vector<std::string> source_images_file_path;
  std::vector<std::string> source_images_file_name;
  std::vector<std::string> source_images_file_extension;

  std::string source_videos_file_directory;
  std::vector<std::string> source_videos_file_path;
  std::vector<std::string> source_videos_file_name;
  std::vector<std::string> source_videos_file_extension;

  using namespace System;
  using namespace System::ComponentModel;
  using namespace System::Collections;
  using namespace System::Windows::Forms;
  using namespace System::Data;
  using namespace System::Drawing;

  /// <summary>
  /// Summary for ApplicationForm
  /// </summary>
  public ref class ApplicationForm : public System::Windows::Forms::Form {

  public:

    ApplicationForm();

  private:

    void OnButtonsClick(System::Object ^sender, System::EventArgs ^e);

    void OnCheckedChanged(System::Object ^sender, System::EventArgs ^e);

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~ApplicationForm() {
      if (components) {
        delete components;
      }
    }

  private:

    System::Windows::Forms::MenuStrip ^menu_strip_;

    System::Windows::Forms::ToolStripMenuItem ^file_tool_strip_menu_item_;
    System::Windows::Forms::ToolStripMenuItem ^open_images_tool_strip_menu_item_;
    System::Windows::Forms::ToolStripMenuItem ^open_videos_tool_strip_menu_item_;
    System::Windows::Forms::ToolStripMenuItem ^mode_tool_strip_menu_item_;

    System::Windows::Forms::Button ^edit_videos_button_;
    System::Windows::Forms::Button ^stitch_button_;
    System::Windows::Forms::Button ^select_keyframes_button_;

    System::Windows::Forms::NumericUpDown ^width_numeric_up_down_;
    System::Windows::Forms::NumericUpDown ^height_numeric_up_down_;
    System::Windows::Forms::NumericUpDown ^begin_frame_numeric_up_down_;
    System::Windows::Forms::NumericUpDown ^end_frame_numeric_up_down_;
    System::Windows::Forms::NumericUpDown ^flip_code_numeric_up_down_;

    System::Windows::Forms::CheckBox ^flip_check_box_;
    System::Windows::Forms::CheckBox ^transpose_check_box_;


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
    void InitializeComponent(void) {
      this->menu_strip_ = (gcnew System::Windows::Forms::MenuStrip());
      this->file_tool_strip_menu_item_ = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->open_videos_tool_strip_menu_item_ = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->mode_tool_strip_menu_item_ = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->edit_videos_button_ = (gcnew System::Windows::Forms::Button());
      this->width_numeric_up_down_ = (gcnew System::Windows::Forms::NumericUpDown());
      this->height_numeric_up_down_ = (gcnew System::Windows::Forms::NumericUpDown());
      this->begin_frame_numeric_up_down_ = (gcnew System::Windows::Forms::NumericUpDown());
      this->stitch_button_ = (gcnew System::Windows::Forms::Button());
      this->end_frame_numeric_up_down_ = (gcnew System::Windows::Forms::NumericUpDown());
      this->flip_code_numeric_up_down_ = (gcnew System::Windows::Forms::NumericUpDown());
      this->flip_check_box_ = (gcnew System::Windows::Forms::CheckBox());
      this->transpose_check_box_ = (gcnew System::Windows::Forms::CheckBox());
      this->select_keyframes_button_ = (gcnew System::Windows::Forms::Button());
      this->open_images_tool_strip_menu_item_ = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->menu_strip_->SuspendLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->width_numeric_up_down_))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->height_numeric_up_down_))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->begin_frame_numeric_up_down_))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->end_frame_numeric_up_down_))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->flip_code_numeric_up_down_))->BeginInit();
      this->SuspendLayout();
      // 
      // menu_strip_
      // 
      this->menu_strip_->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
        this->file_tool_strip_menu_item_,
          this->mode_tool_strip_menu_item_
      });
      this->menu_strip_->Location = System::Drawing::Point(0, 0);
      this->menu_strip_->Name = L"menu_strip_";
      this->menu_strip_->Size = System::Drawing::Size(284, 24);
      this->menu_strip_->TabIndex = 0;
      this->menu_strip_->Text = L"Menu";
      // 
      // file_tool_strip_menu_item_
      // 
      this->file_tool_strip_menu_item_->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
        this->open_images_tool_strip_menu_item_,
          this->open_videos_tool_strip_menu_item_
      });
      this->file_tool_strip_menu_item_->Name = L"file_tool_strip_menu_item_";
      this->file_tool_strip_menu_item_->Size = System::Drawing::Size(37, 20);
      this->file_tool_strip_menu_item_->Text = L"File";
      // 
      // open_videos_tool_strip_menu_item_
      // 
      this->open_videos_tool_strip_menu_item_->Name = L"open_videos_tool_strip_menu_item_";
      this->open_videos_tool_strip_menu_item_->Size = System::Drawing::Size(152, 22);
      this->open_videos_tool_strip_menu_item_->Text = L"Open Videos";
      // 
      // mode_tool_strip_menu_item_
      // 
      this->mode_tool_strip_menu_item_->Name = L"mode_tool_strip_menu_item_";
      this->mode_tool_strip_menu_item_->Size = System::Drawing::Size(50, 20);
      this->mode_tool_strip_menu_item_->Text = L"Mode";
      // 
      // edit_videos_button_
      // 
      this->edit_videos_button_->Location = System::Drawing::Point(12, 110);
      this->edit_videos_button_->Name = L"edit_videos_button_";
      this->edit_videos_button_->Size = System::Drawing::Size(75, 23);
      this->edit_videos_button_->TabIndex = 1;
      this->edit_videos_button_->Text = L"Edit videos";
      this->edit_videos_button_->UseVisualStyleBackColor = true;
      // 
      // width_numeric_up_down_
      // 
      this->width_numeric_up_down_->Location = System::Drawing::Point(13, 58);
      this->width_numeric_up_down_->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {
        4096, 0, 0, 0
      });
      this->width_numeric_up_down_->Name = L"width_numeric_up_down_";
      this->width_numeric_up_down_->Size = System::Drawing::Size(120, 20);
      this->width_numeric_up_down_->TabIndex = 2;
      // 
      // height_numeric_up_down_
      // 
      this->height_numeric_up_down_->Location = System::Drawing::Point(139, 58);
      this->height_numeric_up_down_->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {
        4096, 0, 0, 0
      });
      this->height_numeric_up_down_->Name = L"height_numeric_up_down_";
      this->height_numeric_up_down_->Size = System::Drawing::Size(120, 20);
      this->height_numeric_up_down_->TabIndex = 3;
      // 
      // begin_frame_numeric_up_down_
      // 
      this->begin_frame_numeric_up_down_->Location = System::Drawing::Point(13, 84);
      this->begin_frame_numeric_up_down_->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {
        9999999, 0, 0, 0
      });
      this->begin_frame_numeric_up_down_->Name = L"begin_frame_numeric_up_down_";
      this->begin_frame_numeric_up_down_->Size = System::Drawing::Size(120, 20);
      this->begin_frame_numeric_up_down_->TabIndex = 4;
      // 
      // stitch_button_
      // 
      this->stitch_button_->Location = System::Drawing::Point(12, 139);
      this->stitch_button_->Name = L"stitch_button_";
      this->stitch_button_->Size = System::Drawing::Size(75, 23);
      this->stitch_button_->TabIndex = 5;
      this->stitch_button_->Text = L"Stitch";
      this->stitch_button_->UseVisualStyleBackColor = true;
      // 
      // end_frame_numeric_up_down_
      // 
      this->end_frame_numeric_up_down_->Location = System::Drawing::Point(139, 84);
      this->end_frame_numeric_up_down_->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {
        9999999, 0, 0, 0
      });
      this->end_frame_numeric_up_down_->Name = L"end_frame_numeric_up_down_";
      this->end_frame_numeric_up_down_->Size = System::Drawing::Size(120, 20);
      this->end_frame_numeric_up_down_->TabIndex = 6;
      // 
      // flip_code_numeric_up_down_
      // 
      this->flip_code_numeric_up_down_->Enabled = false;
      this->flip_code_numeric_up_down_->Location = System::Drawing::Point(223, 109);
      this->flip_code_numeric_up_down_->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {
        1, 0, 0, 0
      });
      this->flip_code_numeric_up_down_->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {
        1, 0, 0, System::Int32::MinValue
      });
      this->flip_code_numeric_up_down_->Name = L"flip_code_numeric_up_down_";
      this->flip_code_numeric_up_down_->Size = System::Drawing::Size(27, 20);
      this->flip_code_numeric_up_down_->TabIndex = 7;
      // 
      // flip_check_box_
      // 
      this->flip_check_box_->AutoSize = true;
      this->flip_check_box_->Location = System::Drawing::Point(175, 110);
      this->flip_check_box_->Name = L"flip_check_box_";
      this->flip_check_box_->Size = System::Drawing::Size(42, 17);
      this->flip_check_box_->TabIndex = 8;
      this->flip_check_box_->Text = L"Flip";
      this->flip_check_box_->UseVisualStyleBackColor = true;
      // 
      // transpose_check_box_
      // 
      this->transpose_check_box_->AutoSize = true;
      this->transpose_check_box_->Location = System::Drawing::Point(93, 110);
      this->transpose_check_box_->Name = L"transpose_check_box_";
      this->transpose_check_box_->Size = System::Drawing::Size(76, 17);
      this->transpose_check_box_->TabIndex = 9;
      this->transpose_check_box_->Text = L"Transpose";
      this->transpose_check_box_->UseVisualStyleBackColor = true;
      // 
      // select_keyframes_button_
      // 
      this->select_keyframes_button_->Location = System::Drawing::Point(13, 29);
      this->select_keyframes_button_->Name = L"select_keyframes_button_";
      this->select_keyframes_button_->Size = System::Drawing::Size(105, 23);
      this->select_keyframes_button_->TabIndex = 10;
      this->select_keyframes_button_->Text = L"Select keyframes";
      this->select_keyframes_button_->UseVisualStyleBackColor = true;
      // 
      // open_images_tool_strip_menu_item_
      // 
      this->open_images_tool_strip_menu_item_->Name = L"open_images_tool_strip_menu_item_";
      this->open_images_tool_strip_menu_item_->Size = System::Drawing::Size(152, 22);
      this->open_images_tool_strip_menu_item_->Text = L"Open Images";
      // 
      // ApplicationForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(284, 262);
      this->Controls->Add(this->select_keyframes_button_);
      this->Controls->Add(this->transpose_check_box_);
      this->Controls->Add(this->flip_check_box_);
      this->Controls->Add(this->flip_code_numeric_up_down_);
      this->Controls->Add(this->end_frame_numeric_up_down_);
      this->Controls->Add(this->stitch_button_);
      this->Controls->Add(this->begin_frame_numeric_up_down_);
      this->Controls->Add(this->height_numeric_up_down_);
      this->Controls->Add(this->width_numeric_up_down_);
      this->Controls->Add(this->edit_videos_button_);
      this->Controls->Add(this->menu_strip_);
      this->MainMenuStrip = this->menu_strip_;
      this->Name = L"ApplicationForm";
      this->Text = L"ApplicationForm";
      this->menu_strip_->ResumeLayout(false);
      this->menu_strip_->PerformLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->width_numeric_up_down_))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->height_numeric_up_down_))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->begin_frame_numeric_up_down_))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->end_frame_numeric_up_down_))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->flip_code_numeric_up_down_))->EndInit();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  };
}

