
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
	//////////// �K�C�h�̌`�� //////////////
	const static int GUIDE_NONE = 0;
	const static int GUIDE_SQUARE = 1;
	const static int GUIDE_RECTANGLE = 2;
	const static int GUIDE_CIRCLE = 3;
	const static int GUIDE_ELLIPSE = 4;
	////////////////////////////////////////

public:
	//!< �摜�ƃE�B���h�E���̃Z�b�g
	MarkerViewer();
	~MarkerViewer();

	//! �摜�ƃE�B���h�E���ŋN��
	void Open(const cv::Mat& image, const std::string& window_name);

	//! �������
	void Close();

	//! �J���Ă��邩�ǂ���
	bool is_open(){
		return !_window_name.empty(); 
	}

	bool is_changed(){
		return _change_flag;
	}

	void reset_change(){
		_change_flag = false;
	}

	//! �p�����[�^�ǂݍ���
	void Read(const cv::FileNode& fn);

	//! �p�����[�^��������
	void Write(cv::FileStorage& fs, const std::string& node_name) const;

	//! �E�B���h�E�ɓ��͂��ꂽ�L�[���擾
	int GetWindowKey();

	//! �}�[�J�[�̎擾
	const std::vector<cv::Rect> GetMarkers() const;

	//! �}�[�J�[�̐ݒ�
	void SetMarkers(const std::vector<cv::Rect>& objects);

	//! �}�[�J�[������
	void DeleteMarker();
	
	//! �}�[�J�[�̈ʒu�����炵����A�傫���̕ύX���s��
	void ReshapeMarker(const cv::Rect& mv);

	//! �}�[�J�[�̑傫����ύX����
	void ResizeMarker(float scale);

	//! �\���X�P�[�����Z�b�g����
	void SetDisplayScale(double scale){
		_display_scale = scale;
	};

	//! �A�X�y�N�g��Œ�̐ݒ�/����
	bool SwitchFixAR();

	//! �A�X�y�N�g��̐ݒ�
	void SetAspectRatio(double ratio){
		if (ratio > 0)
			_aspect_ratio = ratio;
	}

	// �_�`��̋���/�s����
	bool SwitchAcceptPointShape(){
		_ACCEPT_POINT = !_ACCEPT_POINT;
		return _ACCEPT_POINT;
	}

	// �K�C�h�̕\��/��\���؂�ւ�
	bool SwitchShowGuide(){
		_SHOW_GUIDE = !_SHOW_GUIDE;
		RedrawImage();
		return _SHOW_GUIDE;
	}

	// �K�C�h�̕\��
	void ShowGuide(){
		_SHOW_GUIDE = true;
		RedrawImage();
	}

	//! �K�C�h�p��`��ݒ�
	void SetGuideRectangle(const cv::Rect& rect);

	//! �K�C�h�p��`��ݒ�
	void SetGuideShape(int shape);

	//! �}�E�X�̃{�^���������ꂽ���̃A�N�V����
	void MouseButtonDown(int x, int y);

	//! �}�E�X�̃{�^���������ꂽ�܂ܓ����������̃A�N�V����
	void MouseMove(int x, int y);

	//! �}�E�X�̃{�^�����グ�����̃A�N�V����
	void MouseButtonUp();

	//! �}�E�X�̉E�{�^�����グ�����̃A�N�V����
	void MouseRButtonUp(int x, int y);

	//! �X�e�[�^�X�̕\��
	void PrintStatus() const;

private:
	bool _change_flag;	//! �`�擙�̕ύX�����������ǂ���