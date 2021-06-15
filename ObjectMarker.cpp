
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