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
#ifndef __OBJECT_MARKER__
#define __OBJECT_MARKER__

#include <opencv2/core/core.hpp>
#include "MarkerViewer.h"


class ObjectMarker
{
public:
	ObjectMarker();
	~ObjectMarker();

	//! �v���O�����N��
	int run(const std::string& conf_file);

	bool begin(){ return jump(0); };
	bool next(){ return jump(_image_idx + 1); };
	bool prev(){ return jump(_image_idx - 1); };
	bool reload(){ return jump(_image_idx); };
	bool jump(int idx);

	//! �w���v���\��
	static void printHelp();

	//! �X�e�[�^�X�\��
	void printStatus() const;

	//! �ݒ��t�@�C���̕ۑ�
	/*!
	\param[in] config_name �ݒ��t�@�C����
	\param[in] input_dir �摜�i�[�t�H���_��
	\param[in] outputname �o�̓e�L�X�g�t�@�C����
	\param[in] display_scale �摜�̕\���X�P�[��
	\return �t�@�C���������݂̐���
	*/
	static bool saveConfiguration(const std::string& config_name,
		const std::string& input_dir, const std::string& outputname,
		const MarkerViewer& marker_viewer
		);


	//! �ݒ��t�@�C���̓ǂݍ���
	/*!
	\param[in] config_name �ݒ��t�@�C����
	\param[out] input_dir �摜�i�[�t�H���_��
	\param[out] outputname �o�̓e�L�X�g�t�@�C����
	\param[out] display_scale �摜�̕\���X�P�[��
	\return �t�@�C���ǂݍ��݂̐���
	*/
	static bool loadConfiguration(const std::string& config_name,
		std::string& input_dir, std::string& outputname,
		MarkerViewer& marker_viewer
		);

	bool Load(const std::string& image_dir, const std::string& anno_file);
	bool LoadAnnotationFile(const std::string& anno_file);

	//! �O�̃t���[���̃}�[�J�[�����t���[���ɃR�s�[
	void CopyFormerMarkers();

	//! �A�m�e�[�V�����ŉ摜���؂������ĕۑ�
	void CropAndSaveImages(const std::string& dir_name);

	//! �A�m�e�[�V�����t�@�C���𐮌`���ďo��
	bool ExportAnnotationFile(const std::string& filename);

private:
	std::string _input_dir;	// ���̓t�H���_
	std::string _annotation_file;	// �o�̓t�@�C��

	std::vector<std::string> _file_list;	// �摜�t�@�C���ւ̃p�X
	std::vector<std::vector<cv::Rect>>	_rectlist;	// �e�摜�̃A�m�e�[�V����
	MarkerViewer _marker_viewer;	// Viewer�N���X

	int _image_idx;		// ���ݎQ�Ƃ��Ă����摜ID


	//! �A�m�e�[�V�����̕��ёւ�
	/*!
	���[�h���ꂽ�A�m�e�[�V�����ɑΉ������摜�t�@�C�������A�Q�Ɖ摜�t�@�C����