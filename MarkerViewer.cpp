
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