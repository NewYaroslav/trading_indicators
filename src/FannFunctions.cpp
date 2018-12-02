/*
* trading_indicators - Indicators for technical analysis.
*
* Copyright (c) 2018 Elektro Yar. Email: git.electroyar@gmail.com
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "FannFunctions.hpp"
#include <iostream>

namespace FannFunctions {

    fann_type** reserveData(int x, int y) {
        //std::cout << "reserveData: x: " << x << " y: " << y << std::endl;
        fann_type** data = new fann_type*[x];
        for(int i = 0; i < x; i++) {
            data[i] = new fann_type[y]; // Кол-во нейроннов в слое
        }
        return data;
    }

    void deleteData(int x, fann_type** data) {
        for(int i = 0; i < x; i++) {
            delete[] data[i];
        }
        delete[] data;
    }

    BaseNet::BaseNet() {isInit = false;};

    BaseNet::BaseNet(std::string path) {
        BaseNet::path = path;
        ann = fann_create_from_file(path.c_str());
        nInput = fann_get_num_input(ann);
        nOutput = fann_get_num_output(ann);
        input = (fann_type*)malloc(nInput * sizeof(fann_type));
        std::cout << "BaseNet, num input: " << nInput << " num output: " << nOutput << std::endl;
        isInit = true;
    }

    BaseNet::~BaseNet() {
        if(isInit) {
            fann_destroy(ann);
            free(input);
        }
        std::cout << "~BaseNet" << std::endl;
    };

    void BaseNet::updata(std::vector<double>& in, std::vector<double>& out) {
        if(isInit == false) {
            std::cout << "error: neural network not initialized" << std::endl;
            return;
        }
        if((int)in.size() != nInput) {
            std::cout << "error: in.size() != nInput" << std::endl;
            return;
        }
        conversion(in, input);
        fann_type* calcOut = fann_run(ann, input);

        out.resize(nOutput);
        for(int i = 0; i < nOutput; i++) {
            out[i] = calcOut[i];
        }
    }

    int BaseNet::updata(std::vector<double>& in) {
        if(isInit == false) {
            std::cout << "error: neural network not initialized" << std::endl;
            return -1;
        }
        if((int)in.size() != nInput) {
            std::cout << "error: in.size() != nInput" << std::endl;
            return -1;
        }
        conversion(in, input);
        fann_type* calcOut = fann_run(ann, input);
        fann_type maxData = -2;
        int pos = -1;
        //printf("out: ");
        for(int i = 0; i < nOutput; i++) {
            //printf("%f ", calcOut[i]);
            if(calcOut[i] > maxData) {
                maxData = calcOut[i];
                pos = i;
            }
        }
        //printf("\n");
        return pos;
    }

}
