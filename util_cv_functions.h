
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

#ifndef __UTIL_CV_FUNCTIONS__
#define __UTIL_CV_FUNCTIONS__

#include <opencv2/core/core.hpp>

namespace util{
	
	//! �A�m�e�[�V�����t�@�C���̓ǂݍ���
	/*!
	opencv_createsamles.exe�Ɠ��`���̃A�m�e�[�V�����t�@�C���ǂݏ���
	\param[in] gt_file �A�m�e�[�V�����t�@�C����
	\param[out] imgpathlist �摜�t�@�C���ւ̃p�X
	\param[out] rectlist �e�摜�ɂ���ꂽ�A�m�e�[�V�����̃��X�g
	\return �ǂݍ��݂̐���
	*/
	bool LoadAnnotationFile(const std::string& gt_file, std::vector<std::string>& imgpathlist, std::vector<std::vector<cv::Rect>>& rectlist);

	//! �A�m�e�[�V�����t�@�C���̕ۑ�
	/*!
	opencv_createsamles.exe�Ɠ��`���̃A�m�e�[�V�����t�@�C���ǂݏ���
	\param[in] anno_file �A�m�e�[�V�����t�@�C����
	\param[in] img_files �摜�t�@�C���ւ̃p�X
	\param[in] obj_rects �e�摜�ɂ���ꂽ�A�m�e�[�V�����̃��X�g
	\return �ۑ��̐���
	*/
	bool SaveAnnotationFile(const std::string& anno_file, const std::vector<std::string>& img_files, const std::vector<std::vector<cv::Rect>>& obj_rects, const std::string& sep = " ");

	//! �A�m�e�[�V�����t�@�C���ւP�s�ǋL
	/*!
	opencv_createsamles.exe�Ɠ��`���̃A�m�e�[�V�����t�@�C���ǂݏ���
	\param[in] anno_file �A�m�e�[�V�����t�@�C����
	\param[in] img_file �摜�t�@�C���ւ̃p�X
	\param[in] obj_rects �摜�ɂ���ꂽ�A�m�e�[�V����
	\return �ۑ��̐���
	*/
	bool AddAnnotationLine(const std::string& anno_file, const std::string& img_file, const std::vector<cv::Rect>& obj_rects, const std::string& sep = " ");

	//! �A�m�e�[�V�����t�@�C���փw�b�_����������
	/*!
	\param[in] anno_file �A�m�e�[�V�����t�@�C����
	\return �ۑ��̐���
	*/
	bool AddHeaderLine(const std::string& anno_file);

	//! �A�m�e�[�V����������ꂽ�̈��؂����ĉ摜�Ƃ��ĕۑ�
	/*!
	\param[in] dir_path �ۑ���f�B���N�g��
	\param[in] imgpathlist �摜�t�@�C���̃��X�g
	\param[in] rectlist �A�m�e�[�V�����̃��X�g
	*/
	void CropAnnotatedImageRegions(const std::string& dir_path, const std::vector<std::string>& imgpathlist, const std::vector<std::vector<cv::Rect>>& rectlist);

	//! ��`�����X�P�[��
	void RescaleRect(const cv::Rect& rect, cv::Rect& dst_rect, double scale);

	//! ��`�����X�P�[��
	void RescaleRect(const std::vector<cv::Rect>& rects, std::vector<cv::Rect>& dst_rects, double scale);


	//! ��`���摜�̈�Ƃ��Ԃ��Ă��邩�m�F
	inline bool CheckRectOverlapSize(const cv::Rect& rect, const cv::Size& size){
		return (rect.x < size.width && rect.y < size.height && rect.x + rect.width > 0 && rect.y + rect.height > 0);
	};
}

#endif