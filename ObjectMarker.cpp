
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
	printf("\nObject Marker: \n\t 画像中のオブジェクト位置を記録するためのツール\n");
	printf("\tGunawan Herman, April 2006\n");
	printf("\tAdapted from ObjectMarker.cpp by A.Florian\n");
	printf("\tModified and localized by Takuya MINAGAWA in 2014\n");
	printf("\n");
	printf("------------------------------------------------------------\n");
	printf("|  btn  |               function                           |\n");
	printf("|-------|--------------------------------------------------|\n");
	printf("|Enter  | 現在のマーカーをファイルに記述し、               |\n");
	printf("|       | 次の画像を読み込む                               |\n");
	printf("|Space  | Enterと同じ                                      |\n");
	printf("|BS     | ひとつ前の画像に戻る                             |\n");
	printf("|ESC    | プログラムを終了                                 |\n");
	printf("|d      | 一番新しいマーカーを消去                         |\n");
	printf("|r      | 前フレームのマーカーを１つ呼び出す               |\n");
	printf("|8      | 一番新しいマーカーを1 px上へ動かす               |\n");
	printf("|9      | 一番新しいマーカーを10px上へ動かす               |\n");
	printf("|2      | 一番新しいマーカーを1 px下へ動かす               |\n");
	printf("|3      | 一番新しいマーカーを10px下へ動かす               |\n");
	printf("|4      | 一番新しいマーカーを1 px左へ動かす               |\n");
	printf("|5      | 一番新しいマーカーを10px左へ動かす               |\n");
	printf("|6      | 一番新しいマーカーを1 px右へ動かす               |\n");
	printf("|7      | 一番新しいマーカーを10px右へ動かす               |\n");
	printf("|w      | 一番新しいマーカーの幅を1 px拡大する             |\n");
	printf("|W      | 一番新しいマーカーの幅を1 px縮小する             |\n");
	printf("|h      | 一番新しいマーカーの高さを1 px拡大する           |\n");
	printf("|H      | 一番新しいマーカーの高さを1 px縮小する           |\n");
	printf("|z      | 一番新しいマーカーのを大きさを2%%拡大する         |\n");
	printf("|Z      | 一番新しいマーカーのを大きさを2%%縮小する         |\n");
	printf("|m      | マーカーの縦横比を固定／固定解除する             |\n");
	printf("|a      | マーカーの縦横比を指定する                       |\n");
	printf("|s      | 画像の表示サイズを変更する                       |\n");
	printf("|c      | マーカー領域の画像を切り取ってファイル保存       |\n");
	printf("|p      | マーカーを矩形だけでなく点を許可する             |\n");
	printf("|e      | 現フレームにつけたマーカー座標を画面出力         |\n");
	printf("|g      | ガイドの表示/非表示                              |\n");
	printf("|G      | ガイドの設定                                     |\n");
	printf("|f      | 画像のフォルダ名を変更                           |\n");
	printf("|o      | 出力ファイルを変更                               |\n");
	printf("|O      | 出力ファイルを成形して新たに作成                 |\n");
	printf("|j      | 指定番号の画像へジャンプ                         |\n");
	printf("|t      | このヘルプを表示                                 |\n");
	printf("------------------------------------------------------------\n");
	printf("オブジェクトを右クリックで選択\n");
	printf("\n");
}

void ObjectMarker::printStatus() const
{
	std::cout << "このプログラムは入力画像を'" << _input_dir << "'フォルダの中から探します\n";
	std::cout << "オブジェクトの位置はファイル'" << _annotation_file << "'にテキスト出力されます\n";
	_marker_viewer.PrintStatus();
}


//! アノテーションの並び替え
/*!
ロードされたアノテーションに対応する画像ファイル名が、参照画像ファイル名と対応するように並び替え
\param[in] loaded_img_list 入力画像リスト
\param[in] loaded_annotation 読み込まれたアノテーション
\param[in] ref_img_list 参照アノテーション
\return 整理されたアノテーション
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