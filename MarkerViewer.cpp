
/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//
// Copyright (C) 2014 Takuya MINAGAWA, 2006 Gunawan Herman, 2003 Florian Adolf.
// Third party copyrights are property of their respective owners.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//M*/

#include "MarkerViewer.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "util_cv_functions.h"
#include "util_functions.h"


MarkerViewer::MarkerViewer()
{
	Init();
}


MarkerViewer::~MarkerViewer()
{
	Close();
}


void MarkerViewer::Init()
{
	_roi_b = cv::Point2d(-1, -1);
	_roi_e = cv::Point2d(-1, -1);
	_display_scale = 1.0;	// ディスプレイ表示の原画像からの縮尺
	_FIX_MARKER_AR = false;	// マーカーの縦横比を固定するかどうか
	_aspect_ratio = 1.0; // マーカーのアスペクト比（幅/高さ）
	_ACCEPT_POINT = false;	// マーカーの点形状を認めるか否か
	_GUIDE_SHAPE = GUIDE_NONE;	// ガイドの形状
	_guide_rect = cv::Rect(0, 0, 0, 0);	// ガイドの矩形
	_guide_rect_org = cv::Rect(0, 0, 0, 0);	// ガイドの矩形(縮尺前)
	_SHOW_GUIDE = false;	// ガイドの表示有無

	_change_flag = false;
}


void MarkerViewer::Open(const cv::Mat& image, const std::string& window_name)
{
	Close();

	_window_name = window_name;
	cv::resize(image, _image, cv::Size(_display_scale * image.cols, _display_scale * image.rows));
	cv::namedWindow(_window_name);
	cv::setMouseCallback(_window_name, MarkerViewer::on_mouse, this);
	RedrawImage();
//	_change_flag = false;
}


void MarkerViewer::Close()
{
	if (!_window_name.empty()){
		cv::destroyWindow(_window_name);
		_window_name = std::string();
	}
}


int MarkerViewer::GetWindowKey(){
	return cv::waitKey(0);
};


//! マーカーの取得
const std::vector<cv::Rect> MarkerViewer::GetMarkers() const{
	std::vector<cv::Rect> objects = removeOutRangeRect(_objects, _image.size());
	std::vector<cv::Rect> objects2;
	util::RescaleRect(objects, objects2, 1.0 / _display_scale);
	return objects2;
};


//! マーカーの設定
void MarkerViewer::SetMarkers(const std::vector<cv::Rect>& objects){
	util::RescaleRect(objects, _objects, _display_scale);
	_change_flag = true;
	RedrawImage();
};


//! マーカーを消す
void MarkerViewer::DeleteMarker()
{
	if (!_objects.empty()){
		_objects.pop_back();
		_change_flag = true;
		RedrawImage();
	}
}


//! マーカーの位置をずらしたり、大きさの変更を行う
void MarkerViewer::ReshapeMarker(const cv::Rect& mv)
{
	if (!_objects.empty()){
		cv::Rect* rect = &(_objects.back());
		rect->x += mv.x;
		rect->y += mv.y;
		rect->width += mv.width;
		rect->height += mv.height;
		if (rect->width <= 0)
			rect->width = 1;
		if (rect->height <= 0)
			rect->height = 1;
		_change_flag = true;
		RedrawImage();
	}
}


//! マーカーの大きさを変更する
void MarkerViewer::ResizeMarker(float scale)
{
	if (scale <= 0)
		return;

	if (!_objects.empty()){
		cv::Rect* rect = &(_objects.back());
		int w = util::round(scale * rect->width);
		int h = util::round(scale * rect->height);
		if (_FIX_MARKER_AR)
			h = util::round((double)w / _aspect_ratio);
		if (w < 1 || h < 1)
			return;

		//rect->x += (rect->width - w) / 2;
		//rect->y += (rect->height - h) / 2;
		rect->width = w;
		rect->height = h;
		_change_flag = true;
		RedrawImage();
	}
}


//! アスペクト比固定の設定/解除
bool MarkerViewer::SwitchFixAR()
{
	_FIX_MARKER_AR = !_FIX_MARKER_AR;
	if (_FIX_MARKER_AR && !_objects.empty()){
		_aspect_ratio = (double)_objects.back().width / _objects.back().height;
	}
	return _FIX_MARKER_AR;
}


//! ガイド用矩形を設定
void MarkerViewer::SetGuideRectangle(const cv::Rect& rect)
{
	_guide_rect_org = rect;
	if (_guide_rect_org.width > 0 && _guide_rect_org.height > 0)
		util::RescaleRect(_guide_rect_org, _guide_rect, _display_scale);

}

//! ガイド用矩形を設定
void MarkerViewer::SetGuideShape(int shape){
	if (shape < 0 || shape > 4)
		return;
	_GUIDE_SHAPE = shape;
	RedrawImage();
}



//! 画像枠外の矩形を削除
std::vector<cv::Rect> MarkerViewer::removeOutRangeRect(const std::vector<cv::Rect>& objects, const cv::Size& img_size)
{
	std::vector<cv::Rect> dst_objects;
	std::vector<cv::Rect>::const_iterator rect_itr = objects.begin();
	while (rect_itr != objects.end()){
		if (util::CheckRectOverlapSize(*rect_itr, img_size))
			dst_objects.push_back(*rect_itr);
		rect_itr++;
	}
	return dst_objects;
}


//! ステータスの表示
void MarkerViewer::PrintStatus() const{
	std::cout << "マーカーの縦横比は" << (_FIX_MARKER_AR ? "固定" : "自由") << std::endl;
	if (_FIX_MARKER_AR) {
		std::cout << "アスペクト比 (width / height): " << _aspect_ratio << std::endl;
	}
	std::cout << "点マーカーを許可： " << (_ACCEPT_POINT ? "YES" : "NO") << std::endl;
	std::cout << "表示縮尺： " << _display_scale << std::endl;
}


//! パラメータ読み込み
void MarkerViewer::Read(const cv::FileNode& fn)
{
	fn["display_scale"] >> _display_scale;
	if (_display_scale <= 0)
		_display_scale = 1.0;

	int fixed = fn["fix_marker_ratio"];
	_FIX_MARKER_AR = (fixed == 1) ? true : false;
	fn["aspect_ratio"] >> _aspect_ratio;
	int point = fn["accept_point_shape"];
	_ACCEPT_POINT = (point == 1) ? true : false;

	cv::FileNode fng = fn["guide"];
	_GUIDE_SHAPE = GUIDE_NONE;
	_SHOW_GUIDE = false;

	if (fng.empty())
		return;

	if (!fng["shape"].empty())
		fng["shape"] >> _GUIDE_SHAPE;

	cv::Rect rectangle;
	fng["position"] >> rectangle;
	SetGuideRectangle(rectangle);

	int disp = fng["display"].empty() ? 0 : fng["display"];
	_SHOW_GUIDE = disp > 0;

	RedrawImage();
}


//! パラメータ書き込み
void MarkerViewer::Write(cv::FileStorage& fs, const std::string& node_name) const
{
	if (!node_name.empty())
		fs << node_name;
	fs << "{";
	fs << "display_scale" << _display_scale;
	fs << "fix_marker_ratio" << (int)(_FIX_MARKER_AR ? 1 : 0);
	fs << "aspect_ratio" << _aspect_ratio;
	fs << "accept_point_shape" << (int)(_ACCEPT_POINT ? 1 : 0);

	if (_GUIDE_SHAPE == GUIDE_NONE)
		return;

	fs << "guide" << "{";
	cvWriteComment(fs.fs, "1:SQUARE, 2:RECTANGLE, 3:CIRCLE, 4:ELLIPSE", 0);
	fs << "shape" << _GUIDE_SHAPE;
	fs << "position" << _guide_rect;
	fs << "display" << (int)(_SHOW_GUIDE ? 1 : 0);
	fs << "}";

	fs << "}";
}


void MarkerViewer::MouseButtonDown(int x, int y){
	_roi_b = _roi_e = cv::Point2d(x, y);
}

void MarkerViewer::MouseMove(int x, int y)
{
	_roi_e.x = x;

	if (!_FIX_MARKER_AR) {
		_roi_e.y = y;
	}
	else {
		_roi_e.y = _roi_b.y + ((_roi_e.x - _roi_b.x) / _aspect_ratio);
	}

	RedrawImage();
}