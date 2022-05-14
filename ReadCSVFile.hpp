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

#include <string>
#include <vector>
#include <fstream>

namespace util{

	template <typename T>
	T TransString(const std::string& str){
		T val;
		if (typeid(T) == typeid(int) || typeid(T) == typeid(long) ||
			typeid(T) == typeid(unsigned int) || typeid(T) == typeid(unsigned long)){
			val = atoi(str.c_str());
		}
		else if (typeid(T) == typeid(float) || typeid(T) == typeid(double)){
			val = atof(str.c_str());
		}
		else if (typeid(T) == typeid(std::string)){
			val = str;
		}
		return val;
	};


	// input_string��separater�ŕ���
	template <typename T>
	std::vector<T> TokenizeString(const std::string& input_string, const std::vector<std::string>& separater_vec)
	{
		std::vector<std::string>::const_iterator separater_itr;
		std::vector<std::string::size_type>	index_vec;
		std::string::size_type	index;
		for (separater_itr = separater_vec.begin(); separater_itr != separater_vec.end(); separater_itr++){
			index = 0;
			while (true){
				index = input_string.find(*separater_itr, index);
				if (index == std::string::npos){
					break;
				}
				else{
					index_vec.push_back(index);
					index++;
				}
			}
		}
		sort(index_vec.begin(), index_vec.end())