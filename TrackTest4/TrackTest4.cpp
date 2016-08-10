/*
修改自KCFcpp，官方代码
只使用opencv3.1.0，不使用opencv_contribute
最后修改时间：2016.7.28
作者：HJL
*/
#include <iostream>
#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "kcftracker.hpp"

//#pragma comment(lib, "opencv_world310d.lib")

using namespace std;
using namespace cv;


class BoxExtractor {
public:
	Rect2d extract(Mat img);
	Rect2d extract(const std::string& windowName, Mat img, bool showCrossair = true);

	struct handlerT {
		bool isDrawing;
		Rect2d box;
		Mat image;

		// initializer list
		handlerT() : isDrawing(false) {};
	}params;

private:
	static void mouseHandler(int event, int x, int y, int flags, void *param);
	void opencv_mouse_callback(int event, int x, int y, int, void *param);
};


int main(int argc, char* argv[]) {

	//if (argc > 5) return -1;

	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = true;
	bool LAB = false;

	//for (int i = 0; i < argc; i++) {
	//	if (strcmp(argv[i], "hog") == 0)
	//		HOG = true;
	//	if (strcmp(argv[i], "fixed_window") == 0)
	//		FIXEDWINDOW = true;
	//	if (strcmp(argv[i], "singlescale") == 0)
	//		MULTISCALE = false;
	//	if (strcmp(argv[i], "show") == 0)
	//		SILENT = false;
	//	if (strcmp(argv[i], "lab") == 0) {
	//		LAB = true;
	//		HOG = true;
	//	}
	//	if (strcmp(argv[i], "gray") == 0)
	//		HOG = false;
	//}

	// Create KCFTracker object
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);

	// ROI selector
	BoxExtractor box;

	// set input video
	std::string video = "E://flip.avi";//"E://data//trackingData//tracking_clip.mp4";// "E://dudek.webm";//argv[1];
	VideoCapture cap(video);

	Mat frame;

	// get bounding box
	cap >> frame;
	Rect2d roi = box.extract("tracker", frame);

	//quit if ROI was not selected
	if (roi.width == 0 || roi.height == 0)
		return 0;

	printf("x = %f, y = %f, width = %f, height = %f;\n",roi.x,roi.y,roi.width,roi.height);/////输出长宽  从上往下框  从下往上框  区别

	tracker.init(roi, frame);
	rectangle(frame, roi, Scalar(0, 255, 255), 1, 8);
	Rect result;
	// do the tracking
	printf("Start the tracking process, press ESC to quit.\n");
	while (1) 
	{
		// get frame from the video
		cap >> frame;
		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;
		result = tracker.update(frame);


		rectangle(frame, Point(result.x, result.y), Point(result.x + result.width, result.y + result.height), Scalar(0, 255, 255), 1, 8);
		imshow("Image", frame);
		//quit on ESC button
		if (waitKey(1) == 27)break;
	}


}


void BoxExtractor::mouseHandler(int event, int x, int y, int flags, void *param) {
	BoxExtractor *self = static_cast<BoxExtractor*>(param);
	self->opencv_mouse_callback(event, x, y, flags, param);
}

void BoxExtractor::opencv_mouse_callback(int event, int x, int y, int, void *param) {
	handlerT * data = (handlerT*)param;
	switch (event) {
		// update the selected bounding box
	case EVENT_MOUSEMOVE:
		if (data->isDrawing) {
			data->box.width = x - data->box.x;
			data->box.height = y - data->box.y;
		}
		break;

		// start to select the bounding box
	case EVENT_LBUTTONDOWN:
		data->isDrawing = true;
		data->box = cvRect(x, y, 0, 0);
		break;

		// cleaning up the selected bounding box
	case EVENT_LBUTTONUP:
		data->isDrawing = false;
		if (data->box.width < 0) {
			data->box.x += data->box.width;
			data->box.width *= -1;
		}
		if (data->box.height < 0) {
			data->box.y += data->box.height;
			data->box.height *= -1;
		}
		break;
	}
}

Rect2d BoxExtractor::extract(Mat img) {
	return extract("Bounding Box Extractor", img);
}

Rect2d BoxExtractor::extract(const std::string& windowName, Mat img, bool showCrossair) {

	int key = 0;

	// show the image and give feedback to user
	imshow(windowName, img);
	printf("Select an object to track and then press SPACE/BACKSPACE/ENTER button!\n");

	// copy the data, rectangle should be drawn in the fresh image
	params.image = img.clone();

	// select the object
	setMouseCallback(windowName, mouseHandler, (void *)&params);

	// end selection process on SPACE (32) BACKSPACE (27) or ENTER (13)
	while (!(key == 32 || key == 27 || key == 13)) {
		// draw the selected object
		rectangle(
			params.image,
			params.box,
			Scalar(255, 0, 0), 2, 1
			);

		// draw cross air in the middle of bounding box
		if (showCrossair) {
			// horizontal line
			line(
				params.image,
				Point((int)params.box.x, (int)(params.box.y + params.box.height / 2)),
				Point((int)(params.box.x + params.box.width), (int)(params.box.y + params.box.height / 2)),
				Scalar(255, 0, 0), 2, 1
				);

			// vertical line
			line(
				params.image,
				Point((int)(params.box.x + params.box.width / 2), (int)params.box.y),
				Point((int)(params.box.x + params.box.width / 2), (int)(params.box.y + params.box.height)),
				Scalar(255, 0, 0), 2, 1
				);
		}

		// show the image bouding box
		imshow(windowName, params.image);

		// reset the image
		params.image = img.clone();

		//get keyboard event
		key = waitKey(1);
	}


	return params.box;
}


