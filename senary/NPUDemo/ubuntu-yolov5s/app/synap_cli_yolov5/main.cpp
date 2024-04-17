/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2013-2020 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner
 * unless Synaptics has otherwise provided express, written
 * permission.
 *
 * Use of the materials may require a license of intellectual property
 * from a third party or from Synaptics. This file conveys no express
 * or implied licenses to any intellectual property rights belonging
 * to Synaptics.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS", AND
 * SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 * INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
 * BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
 * COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */
///
/// Synap command line application for Object Detection.
///

#include "synap/arg_parser.hpp"
#include "synap/input_data.hpp"
#include "synap/preprocessor.hpp"
#include "synap/network.hpp"
#include "synap/detector.hpp"
#include "synap/timer.hpp"
#include "synap/label_info.hpp"
#include "synap/file_utils.hpp"
#include <iomanip>
#include <iostream>

#include<opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>




using namespace std;
using namespace synaptics::synap;


int main(int argc, char* argv[])
{
    ArgParser args(argc, argv, "Object detection on image files", "[options] file(s)");
    string nbg = args.get("--nb", "<file> Network binary model file", "model.nb");
    string meta = args.get("--meta", "<file> Network meta file (json)", "model.json");
    float score_threshold = stof(args.get("--score-threshold", "<thr> Min confidence", "0.5"));
    int n_max = stoi(args.get("--max-detections", "<n> Max number of detections [0: all]", "0"));
    bool nms = !args.has("--disable-nms", "Disable Non-Max-Suppression algorithm");
    float iou_threshold = stof(args.get("--iou-threshold", "<thr> IOU threashold for NMS", "0.5"));
    bool iou_with_min = args.has("--iou-with-min", "Use min area instead of union to compute IOU");
    args.check_help("--help", "Show help");

    cout << " synap_cli_yolov5	 "<< endl; 
    Preprocessor preprocessor;
    Network network;
    Detector detector(score_threshold, n_max, nms, iou_threshold, iou_with_min);
   // LabelInfo info(filename_path(nbg) + "../../info.json");
    cout << "Loading network: " << nbg << endl;
    if (!network.load_model(nbg, meta)) {
        cerr << "Failed to load model" << endl;
        return 1;
    }
    if (!detector.init(network.outputs)) {
        cerr << "Failed to initialize detector" << endl;
        return 1;
    }

   
    std::string classNames[]={ "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
        "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
        "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
        "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
        "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
        "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
        "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
        "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
        "hair drier", "toothbrush"};    
    string input_filename;
    while ((input_filename = args.get()) != "") {
        // Load input data
        cout << " " << endl;

	cv::Mat img=cv::imread(input_filename);
        InputData image(input_filename);
        if (image.empty()) {
            cerr << "Error, unable to read data from file: " << input_filename << endl;
            return 1;
        }

        Rect assigned_rect;
        if (!preprocessor.assign(network.inputs, image, 0, &assigned_rect)) {
            cerr << "Error assigning input to tensor." << endl;
            return 1;
        }
        Dimensions dim = image.dimensions();
        cout << "Input image: " << input_filename << " (w = " << dim.w
             << ", h = " << dim.h << ", c = " << dim.c << ")" << endl;

        // Execute inference
        Timer tmr;
        if (!network.predict()) {
            cerr << "Inference failed" << endl;
            return 1;
        }

        // Postprocess network outputs
        Detector::Result result = detector.process(network.outputs, assigned_rect);
        cout << "Detection time: " << tmr << endl;
        if (!result.success) {
            cout << "Detection failed" << endl;
            return 1;
        }

        if (output_redirected()) {
            // Generate output in json so that it can be easily parsed by other tools
            cout << to_json_str(result) << endl;
        }
        else {
            cout << "#   Score  Class   Position        Size  Description     Landmarks" << endl;
            int j = 0;
            for (const auto& item : result.items) {
                const auto& bb = item.bounding_box;
                cout << setw(3) << left << j++ << right << "  " << item.confidence << " " << setw(6)
                     << item.class_index << "  " << setw(4) << right << bb.origin.x << "," << setw(4)
                     << right << bb.origin.y << "   " << setw(4) << bb.size.x << "," << setw(4)
                     << right << bb.size.y <<  "  " << setw(16) << classNames[item.class_index] << endl;
                
                int endx = bb.origin.x + bb.size.x;
		int endy = bb.origin.y +  bb.size.y;

		//cout << " endx  =  " << endx  << "   endy =   " << endy << endl;
	       	cv::rectangle(img,cv::Point(bb.origin.x, bb.origin.y),
                cv::Point(endx,  endy),cv::Scalar(255,0,0),4);
                cv::putText(img,classNames[item.class_index], cv::Point(bb.origin.x, bb.origin.y), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255),1);
 

                for (const auto lm: item.landmarks) {
                    cout << lm << " ";
                }
                cout << endl;
            }
	
	cv::imwrite("result_yolov5.jpg",img);
        }
    }

    return 0;
}
