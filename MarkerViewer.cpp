
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
	_display_scale = 1.0;	// �f�B�X�v���C�\���̌��摜����̏k��
	_FIX_MARKER_AR = false;	// �}�[�J�[�̏c������Œ肷�邩�ǂ���
	_aspect_ratio = 1.0; // �}�[�J�[�̃A�X�y�N�g��i��/�����j
	_ACCEPT_POINT = false;	// �}�[�J�[�̓_�`���F�߂邩�ۂ�
	_GUIDE_SHAPE = GUIDE_NONE;	// �K�C�h�̌`��
	_guide_rect = cv::Rect(0, 0, 0, 0);	// �K�C�h�̋�`
	_guide_rect_org = cv::Rect(0, 0, 0, 0);	// �K�C�h�̋�`(�k�ڑO)
	_SHOW_GUIDE = false;	// �K�C�h�̕\���L��

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


//! �}�[�J�[�̎擾
const std::vector<cv::Rect> MarkerViewer::GetMarkers() const{
	std::vector<cv::Rect> objects = removeOutRangeRect(_objects, _image.size());
	std::vector<cv::Rect> objects2;
	util::RescaleRect(objects, objects2, 1.0 / _display_scale);
	return objects2;
};


//! �}�[�J�[�̐ݒ�
void MarkerViewer::SetMarkers(const std::vector<cv::Rect>& objects){
	util::RescaleRect(objects, _objects, _display_scale);
	_change_flag = true;
	RedrawImage();
};


//! �}�[�J�[������
void MarkerViewer::DeleteMarker()
{
	if (!_objects.empty()){
		_objects.pop_back();
		_change_flag = true;
		RedrawImage();
	}
}


//! �}�[�J�[�̈ʒu�����炵����A�傫���̕ύX���s��
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


//! �}�[�J�[�̑傫����ύX����
void MarkerViewer::ResizeMarker(float scale)
{
	if (scale <= 0)