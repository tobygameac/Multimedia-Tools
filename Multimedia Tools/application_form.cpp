#include "application_form.h"

namespace MultimediaTools {

  [STAThread]
  int main(array<String^> ^args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    MultimediaTools::ApplicationForm form;
    Application::Run(%form);
    return 0;
  }

  ApplicationForm::ApplicationForm() {
    InitializeComponent();

    open_video_tool_strip_menu_item_->Click += gcnew System::EventHandler(this, &MultimediaTools::ApplicationForm::OnButtonsClick);

    edit_video_button_->Click += gcnew System::EventHandler(this, &MultimediaTools::ApplicationForm::OnButtonsClick);

    stitch_button_->Click += gcnew System::EventHandler(this, &MultimediaTools::ApplicationForm::OnButtonsClick);

    flip_check_box_->CheckedChanged += gcnew System::EventHandler(this, &MultimediaTools::ApplicationForm::OnCheckedChanged);
  }

  void ApplicationForm::OnButtonsClick(System::Object ^sender, System::EventArgs ^e) {
    if (sender == open_video_tool_strip_menu_item_) {
      OpenFileDialog ^open_video_file_dialog = gcnew OpenFileDialog();
      open_video_file_dialog->Filter = "video files|*.*";
      open_video_file_dialog->Title = "Open a video file.";

      if (open_video_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        std::string raw_file_path = msclr::interop::marshal_as<std::string>(open_video_file_dialog->FileName);
        source_video_file_directory = msclr::interop::marshal_as<std::string>(System::IO::Path::GetDirectoryName(open_video_file_dialog->FileName)) + "\\";
        source_video_file_name = msclr::interop::marshal_as<std::string>(open_video_file_dialog->SafeFileName);

        std::vector<size_t> keyframes_index;

        //keyframes_index = AutomaticExtractionOfRepresentativeKeyframesBasedOnSceneContent::AverageFramesMethod(raw_file_path);
        keyframes_index = AutomaticExtractionOfRepresentativeKeyframesBasedOnSceneContent::StatisticalMethod(raw_file_path);

        cv::VideoCapture video_capture(raw_file_path);

        cv::Mat video_frame;

        video_capture.read(video_frame);

        const cv::Rect LEFT_ROI(0, 0, video_frame.cols / 2, video_frame.rows);
        const cv::Rect RIGHT_ROI(video_frame.cols / 2, 0, video_frame.cols / 2, video_frame.rows);

        for (size_t i = 0; i < keyframes_index.size(); ++i) {
          video_capture.set(CV_CAP_PROP_POS_FRAMES, keyframes_index[i]);
          video_capture.read(video_frame);

          cv::imwrite(raw_file_path + "_L_" + std::to_string(keyframes_index[i]) + ".png", video_frame(LEFT_ROI));
          cv::imwrite(raw_file_path + "_R_" + std::to_string(keyframes_index[i]) + ".png", video_frame(RIGHT_ROI));

          std::cout << "Done : frame " << keyframes_index[i] << "\n";
        }
      }

    } else if (sender == edit_video_button_) {
      OpenFileDialog ^open_video_files_dialog = gcnew OpenFileDialog();
      open_video_files_dialog->Multiselect = true;
      open_video_files_dialog->Filter = "video files|*.*";
      open_video_files_dialog->Title = "Open a video file.";

      if (open_video_files_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        source_video_file_directory = msclr::interop::marshal_as<std::string>(System::IO::Path::GetDirectoryName(open_video_files_dialog->FileName)) + "\\";

        const size_t NEW_WIDTH = System::Decimal::ToInt32(width_numeric_up_down_->Value);
        const size_t NEW_HEIGHT = System::Decimal::ToInt32(height_numeric_up_down_->Value);
        const size_t START_FRAME = System::Decimal::ToInt32(begin_frame_numeric_up_down_->Value);
        const size_t END_FRAME = System::Decimal::ToInt32(end_frame_numeric_up_down_->Value);

        for each (System::String ^raw_file_path in open_video_files_dialog->FileNames) {
          std::string file_path = msclr::interop::marshal_as<std::string>(raw_file_path);
          cv::VideoCapture video_capture(file_path);

          cv::Mat video_frame;

          const size_t FRAME_WIDTH = video_capture.get(CV_CAP_PROP_FRAME_WIDTH);
          const size_t FRAME_HEIGHT = video_capture.get(CV_CAP_PROP_FRAME_HEIGHT);
          const size_t FRAME_COUNT = video_capture.get(CV_CAP_PROP_FRAME_COUNT);

          cv::Size new_size(NEW_WIDTH ? NEW_WIDTH : FRAME_WIDTH, NEW_HEIGHT ? NEW_HEIGHT : FRAME_HEIGHT);

          const int FLIP_CODE = System::Decimal::ToInt32(flip_code_numeric_up_down_->Value);

          if (transpose_check_box_->Checked) {
            new_size = cv::Size(new_size.height, new_size.width);
          }

          const std::string edited_video_path = file_path + "(" + std::to_string(new_size.width) + "x" + std::to_string(new_size.height) + "_" + std::to_string(START_FRAME) + "-" + std::to_string(END_FRAME ? END_FRAME : FRAME_COUNT) + ").avi";
          cv::VideoWriter edited_video_writer;

          edited_video_writer.open(edited_video_path, /*0*/video_capture.get(CV_CAP_PROP_FOURCC), video_capture.get(CV_CAP_PROP_FPS), new_size);

          size_t current_frame_count = 0;
          while (video_capture.read(video_frame)) {
            if (END_FRAME && current_frame_count >= END_FRAME) {
              break;
            }

            if (current_frame_count >= START_FRAME) {

              if (transpose_check_box_->Checked) {
                cv::transpose(video_frame, video_frame);
              }

              if (flip_check_box_->Checked) {
                cv::flip(video_frame, video_frame, FLIP_CODE);
              }

              cv::Mat edited_frame;
              cv::resize(video_frame, edited_frame, new_size);
              edited_video_writer.write(edited_frame);

            }
            ++current_frame_count;
          }

          std::cout << "Done : " << edited_video_path << "\n";
        }

        std::cout << "Everything done\n";
      }
    } else if (sender == stitch_button_) {
      OpenFileDialog ^open_image_files_dialog = gcnew OpenFileDialog();
      open_image_files_dialog->Multiselect = true;
      open_image_files_dialog->Filter = "image files|*.*";
      open_image_files_dialog->Title = "Open image files.";

      if (open_image_files_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        std::string source_images_file_directory = msclr::interop::marshal_as<std::string>(System::IO::Path::GetDirectoryName(open_image_files_dialog->FileName)) + "\\";
        //std::vector<cv::cuda::GpuMat> gpu_images;
        std::vector<cv::Mat> images;
        for each (System::String ^raw_file_path in open_image_files_dialog->FileNames) {
          std::string file_path = msclr::interop::marshal_as<std::string>(raw_file_path);
          cv::Mat image = cv::imread(file_path);
          //cv::cuda::GpuMat gpu_image;
          //gpu_image.upload(image);
          //gpu_images.push_back(gpu_image);
          images.push_back(image);
        }
        cv::Mat pano;

        //cv::cuda::GpuMat gpu_pano;
        cv::Stitcher stitcher = cv::Stitcher::createDefault(true);
        //stitcher.setWarper(new cv::StereographicWarper());
        //stitcher.setSeamEstimationResol(0.1);
        //cv::Stitcher::Status stitching_status = stitcher.estimateTransform(gpu_images);
        cv::Stitcher::Status stitching_status = stitcher.estimateTransform(images);
        if (stitching_status != cv::Stitcher::OK) {
          std::cout << "Stitch failed, error code = " << int(stitching_status) << "\n";
        }
        //stitching_status = stitcher.composePanorama(gpu_images, gpu_pano);
        stitching_status = stitcher.composePanorama(images, pano);
        if (stitching_status != cv::Stitcher::OK) {
          std::cout << "Stitch failed, error code = " << int(stitching_status) << "\n";
        }

        //gpu_pano.download(pano);
        cv::imshow("Pano", pano);
        cv::imwrite(source_images_file_directory + "Pano.jpg", pano);
      }
    }
  }

  void ApplicationForm::OnCheckedChanged(System::Object ^sender, System::EventArgs ^e) {
    if (sender == flip_check_box_) {
      flip_code_numeric_up_down_->Enabled = flip_check_box_->Checked;
    }
  }
}
