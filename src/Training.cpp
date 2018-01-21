/*
* trading_indicators - Indicators for technical analysis.
*
* Copyright (c) 2018 Yaroslav Barabanov. Email: elektroyar@yandex.ru
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

#include "Training.hpp"
#include "TradingIndicators.hpp"
#include "NormalizationData.hpp"
#include "Scaling.hpp"
#include "FannFunctions.hpp"
#include <iostream>

#include "Drawing.hpp"

using namespace std;

namespace TrainingFunctions {

    void getMaxMinData(std::vector<double>& close, std::vector<double>& dataMin, std::vector<double>& dataMax) {
        Indicators::ExtremaDetector iExtrema;
        cout << "close.size() = " << close.size() << endl;
        for(int i = 0; i < (int)close.size(); i++) {
            iExtrema.updata(close[i]);
            if(iExtrema.isUpdataExtremaUp == true) {
                dataMax.push_back(iExtrema.dataMax);
            }
            if(iExtrema.isUpdataExtremaDown == true) {
                dataMin.push_back(iExtrema.dataMin);
            }
        }
        cout << "dataMax.size() = " << dataMax.size() << endl;
        cout << "dataMin.size() = " << dataMin.size() << endl;
    }

    void getWindowTrainData(std::string name, int numTrainingSamples, int nWindow, std::vector<double>& data) {
        int posSamples = 0;
        fann_type** inputTrain = FannFunctions::reserveData(numTrainingSamples, nWindow);
        fann_type** outputTrain = FannFunctions::reserveData(numTrainingSamples, 1);
        for(int i = 0; i < (int)data.size() - nWindow - 1; i++) {
            std::vector<double> vWindow;
            std::vector<double> vOutput(1);

            std::copy(data.begin() + i, data.begin() + i + nWindow, std::back_inserter(vWindow));

            double maxData = *std::max_element(vWindow.begin(), vWindow.end());
            double minData = *std::min_element(vWindow.begin(), vWindow.end());
            double& temp = data[i + nWindow];
            double out = temp < minData ? 0.0 : temp > maxData ? 1.0 : (temp - minData) / (maxData - minData);
            out = 2.0 * out - 1.0;
            vOutput[0] = out;

            Normalization::calcMinMax(vWindow, vWindow, 1);
            Normalization::checkingData(vWindow);
            Normalization::checkingData(vOutput);

            #if(1)
            std::vector<double> vNull;
            Drawing::drawOscilloscope4xBeam("vWindow", "Window", vWindow, vNull, vNull, vNull, 800, 400, 0);
            #endif

            FannFunctions::setData(vWindow, vOutput, inputTrain, outputTrain, posSamples);
            posSamples++;
            if(posSamples >= numTrainingSamples) break;
            if(posSamples % 1440 == 0) {
                cout << "[" << posSamples << ":" << numTrainingSamples << "]\r";
            }
        }

        if(posSamples < numTrainingSamples) {
            cout << endl;
            cout << "error! posSamples < numTrainingSamples" << endl;
            cout << "posSamples: " << posSamples <<  endl;
            cout << "numTrainingSamples: " << numTrainingSamples <<  endl;
            FannFunctions::deleteData(numTrainingSamples, inputTrain);
            FannFunctions::deleteData(numTrainingSamples, outputTrain);
            return;
        }
        cout << endl;
        struct fann_train_data* trainFannData;
        cout << "create_train..." << endl;
        trainFannData = fann_create_train_pointer_array(numTrainingSamples, nWindow, inputTrain, 1, outputTrain);
        cout << "shuffle..." << endl;
        fann_shuffle_train_data(trainFannData);
        cout << "save: " << name << endl;
        //setlocale(LC_ALL,"English");
        fann_save_train(trainFannData, name.c_str());
        cout << "save!" << endl;

        FannFunctions::deleteData(numTrainingSamples, inputTrain);
        FannFunctions::deleteData(numTrainingSamples, outputTrain);
    }

}
