
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
#include "ObjectMarker.h"
#include "util_functions.h"
#include "util_cv_functions.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem/path.hpp>
#include <iostream>

using namespace std;

ObjectMarker::ObjectMarker(){
}


ObjectMarker::~ObjectMarker(void){
}



bool ObjectMarker::saveConfiguration(const std::string& config_name,
	const std::string& input_dir, const std::string& outputname,
	const MarkerViewer& marker_viewer)
{
	cv::FileStorage fs(config_name, cv::FileStorage::WRITE);

	if (!fs.isOpened())
		return false;

	fs << "image_folder" << input_dir;
	fs << "output_file" << outputname;
	marker_viewer.Write(fs, "Viewer");

	return true;
}


bool ObjectMarker::loadConfiguration(const std::string& config_name,
	std::string& input_dir, std::string& outputname,
	MarkerViewer& marker_viewer)
{
	cv::FileStorage fs(config_name, cv::FileStorage::READ);
	if (!fs.isOpened())
		return false;

	fs["image_folder"] >> input_dir;
	fs["output_file"] >> outputname;
	marker_viewer.Read(fs["Viewer"]);
	return true;
}


void ObjectMarker::printHelp() {
	printf("\nObject Marker: \n\t �摜���̃I�u�W�F�N�g�ʒu���L�^���邽�߂̃c�[��\n");
	printf("\tGunawan Herman, April 2006\n");
	printf("\tAdapted from ObjectMarker.cpp by A.Florian\n");
	printf("\tModified and localized by Takuya MINAGAWA in 2014\n");
	printf("\n");
	printf("------------------------------------------------------------\n");
	printf("|  btn  |               function                           |\n");
	printf("|-------|--------------------------------------------------|\n");
	printf("|Enter  | ���݂̃}�[�J�[���t�@�C���ɋL�q���A               |\n");
	printf("|       | ���̉摜��ǂݍ���                               |\n");
	printf("|Space  | Enter�Ɠ���                                      |\n");
	printf("|BS     | �ЂƂO�̉摜�ɖ߂�                             |\n");
	printf("|ESC    | �v���O�������I��                                 |\n");
	printf("|d      | ��ԐV�����}�[�J�[������                         |\n");
	printf("|r      | �O�t���[���̃}�[�J�[���P�Ăяo��               |\n");
	printf("|8      | ��ԐV�����}�[�J�[��1 px��֓�����               |\n");
	printf("|9      | ��ԐV�����}�[�J�[��10px��֓�����               |\n");
	printf("|2      | ��ԐV�����}�[�J�[��1 px���֓�����               |\n");
	printf("|3      | ��ԐV�����}�[�J�[��10px���֓�����               |\n");
	printf("|4      | ��ԐV�����}�[�J�[��1 px���֓�����               |\n");
	printf("|5      | ��ԐV�����}�[�J�[��10px���֓�����               |\n");
	printf("|6      | ��ԐV�����}�[�J�[��1 px�E�֓�����               |\n");
	printf("|7      | ��ԐV�����}�[�J�[��10px�E�֓�����               |\n");
	printf("|w      | ��ԐV�����}�[�J�[�̕���1 px�g�傷��             |\n");
	printf("|W      | ��ԐV�����}�[�J�[�̕���1 px�k������             |\n");
	printf("|h      | ��ԐV�����}�[�J�[�̍�����1 px�g�傷��           |\n");
	printf("|H      | ��ԐV�����}�[�J�[�̍�����1 px�k������           |\n");
	printf("|z      | ��ԐV�����}�[�J�[�̂�傫����2%%�g�傷��         |\n");
	printf("|Z      | ��ԐV�����}�[�J�[�̂�傫����2%%�k������         |\n");
	printf("|m      | �}�[�J�[�̏c������Œ�^�Œ��������             |\n");
	printf("|a      | �}�[�J�[�̏c������w�肷��                       |\n");
	printf("|s      | �摜�̕\���T�C�Y��ύX����                       |\n");
	printf("|c      | �}�[�J�[�̈�̉摜��؂����ăt�@�C���ۑ�       |\n");
	printf("|p      | �}�[�J�[����`�����łȂ��_��������             |\n");
	printf("|e      | ���t���[���ɂ����}�[�J�[���W����ʏo��         |\n");
	printf("|g      | �K�C�h�̕\��/��\��                              |\n");
	printf("|G      | �K�C�h�̐ݒ�                                     |\n");
	printf("|f      | �摜�̃t�H���_����ύX                           |\n");
	printf("|o      | �o�̓t�@�C����ύX                               |\n");
	printf("|O      | �o�̓t�@�C���𐬌`���ĐV���ɍ쐬                 |\n");
	printf("|j      | �w��ԍ��̉摜�փW�����v                         |\n");
	printf("|t      | ���̃w���v��\��                                 |\n");
	printf("------------------------------------------------------------\n");
	printf("�I�u�W�F�N�g���E�N���b�N�őI��\n");
	printf("\n");
}

void ObjectMarker::printStatus() const
{
	std::cout << "���̃v���O�����͓��͉摜��'" << _input_dir << "'�t�H���_�̒�����T���܂�\n";
	std::cout << "�I�u�W�F�N�g�̈ʒu�̓t�@�C��'" << _annotation_file << "'�Ƀe�L�X�g�o�͂���܂�\n";
	_marker_viewer.PrintStatus();
}


//! �A�m�e�[�V�����̕��ёւ�
/*!
���[�h���ꂽ�A�m�e�[�V�����ɑΉ�����摜�t�@�C�������A�Q�Ɖ摜�t�@�C�����ƑΉ�����悤�ɕ��ёւ�
\param[in] loaded_img_list ���͉摜���X�g
\param[in] loaded_annotation �ǂݍ��܂ꂽ�A�m�e�[�V����
\param[in] ref_img_list �Q�ƃA�m�e�[�V����
\return �������ꂽ�A�m�e�[�V����
*/
std::vector<std::vector<cv::Rect>> ObjectMarker::reorderAnnotation(
	const std::vector<std::string>& loaded_img_list,
	const std::vector<std::vector<cv::Rect>>& loaded_annotation,
	const std::vector<std::string>& ref_img_list)
{
	assert(loaded_img_list.size() == loaded_annotation.size());
	using namespace boost::filesystem;

	std::vector<std::vector<cv::Rect>> annotation(ref_img_list.size());

	int num_loaded = loaded_img_list.size();
	int num_ref = ref_img_list.size();

	int i,j;
	for (i = 0; i < num_loaded; i++){
		path loaded_img_path = path(loaded_img_list[i]);
		for (j = 0; j < num_ref; j++){
			if (loaded_img_path.compare(ref_img_list[j]) == 0)
				break;
		}

		if (j < num_ref)
			annotation[j] = loaded_annotation[i];
	}

	return annotation;
}


bool ObjectMarker::Load(const std::string& image_dir, const std::string& anno_file)
{
	// �t�H���_����摜�ꗗ���擾
	std::string input_dir = image_dir;
	_file_list.clear();
	while (!util::ReadImageFilesInDirectory(input_dir, _file_list)){
		std::cout << "no appropriate input files in directory " << input_dir << std::endl;
		std::string flag = util::AskQuestionGetString("Quit?(1:Yes, 0:No): ");
		if (flag == "1")
			return false;
		input_dir = util::AskQuestionGetString("New Image Directory Name: ");
	}

	_image_idx = 0;

	_input_dir = input_dir;

	return LoadAnnotationFile(anno_file);
}


bool ObjectMarker::LoadAnnotationFile(const std::string& anno_file)
{
	// �A�m�e�[�V�����t�@�C����ǂݍ���
	std::vector<std::string> anno_file_list;
	std::vector<std::vector<cv::Rect>> anno_rect_list;
	util::LoadAnnotationFile(anno_file, anno_file_list, anno_rect_list);

	// �t�H���_�̉摜�ꗗ�ƃA�m�e�[�V�����t�@�C����R�Â�
	_rectlist = reorderAnnotation(anno_file_list, anno_rect_list, _file_list);
	anno_file_list.clear();
	anno_rect_list.clear();

	_annotation_file = anno_file;

	// �w�b�_�̏�������