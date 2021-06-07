
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

#ifndef __MARKER_VIEWER__
#define __MARKER_VIEWER__

#include <opencv2/core/core.hpp>

class MarkerViewer
{
public:
	//////////// ガイドの形状 //////////////
	const static int GUIDE_NONE = 0;
	const static int GUIDE_SQUARE = 1;
	const static int GUIDE_RECTANGLE = 2;
	const static int GUIDE_CIRCLE = 3;
	const static int GUIDE_ELLIPSE = 4;
	////////////////////////////////////////

public:
	//!< 画像とウィンドウ名のセット
	MarkerViewer();
	~MarkerViewer();

	//! 画像とウィンドウ名で起動
	void Open(const cv::Mat& image, const std::string& window_name);

	//! 窓を閉じる
	void Close();

	//! 開いているかどうか
	bool is_open(){
		return !_window_name.empty(); 
	}

	bool is_changed(){
		return _change_flag;
	}

	void reset_change(){
		_change_flag = false;
	}

	//! パラメータ読み込み
	void Read(const cv::FileNode& fn);

	//! パラメータ書き込み
	void Write(cv::FileStorage& fs, const std::string& node_name) const;

	//! ウィンドウに入力されたキーを取得
	int GetWindowKey();

	//! マーカーの取得
	const std::vector<cv::Rect> GetMarkers() const;

	//! マーカーの設定
	void SetMarkers(const std::vector<cv::Rect>& objects);

	//! マーカーを消す
	void DeleteMarker();
	
	//! マーカーの位置をずらしたり、大きさの変更を行う
	void ReshapeMarker(const cv::Rect& mv);

	//! マーカーの大きさを変更する
	void ResizeMarker(float scale);