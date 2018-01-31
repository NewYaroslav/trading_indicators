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
#include "ReadingData.hpp"
#include <iostream>

#include "Drawing.hpp"

using namespace std;

namespace TrainingFunctions {

    void getData(std::vector<std::string>& path, std::vector<std::vector<double>>& data, double start, double stop) {
        data.resize(path.size());
        int allDataSize = 0;
        for(int n = 0; n < (int)path.size(); n++) {
            CurrencyQuote TestData(path[n], CurrencyQuote::DTYYYYMMDD_TIME_OPEN_HIGH_LOW_CLOSE_VOL);
            int startPos = TestData.close.size() * start;
            int stopPos = TestData.close.size() * stop;
            std::copy(TestData.close.begin() + startPos, TestData.close.begin() + stopPos, std::back_inserter(data[n]));
            allDataSize += (int)data[n].size();
            cout << "Source: " << path[n] << " data size: " << data[n].size() << endl;
        }
        cout << "All data: " << allDataSize << endl;
    }

    void getExtremaDetectorTrainData(std::string name, std::vector<std::vector<double>>& vRaw, int numTrainingSamples, int nWindow) {
        int posSamples = 0;
        int nOutput = 2;

        fann_type** inputTrain = FannFunctions::reserveData(numTrainingSamples, nWindow);
        fann_type** outputTrain = FannFunctions::reserveData(numTrainingSamples, nOutput);

        int nUp = 0, nNeutral = 0;

        for(int n = 0; n < vRaw.size(); n++) {
            cout << "num raw data: " << n << endl;
            Indicators::LastExtrema iExtrema(nWindow - 1, 10);
            Indicators::LastExtrema iExtrema2(nWindow - 1, 10);
            std::vector<double> vPik(vRaw[n].size());
            vPik[0] = 0.0;
            for(int i = 1; i < (int)vRaw[n].size(); i++) {
                double& close = vRaw[n][i];
                iExtrema.updata(close);
                if(iExtrema.isUpdataExtremaUp == true) {
                    vPik[i - 1] = 1.0;
                } else
                if(iExtrema.isUpdataExtremaDown == true) {
                    vPik[i - 1] = -1.0;
                } else {
                    vPik[i - 1] = 0.0;
                }
            } // for

            for(int i = 0; i < (int)vRaw[n].size(); i++) {
                double& close = vRaw[n][i];
                iExtrema2.updata(close);

                if((int)iExtrema2.vExtrema.size() == nWindow - 1) {
                    std::vector<double> vInput = iExtrema2.vExtrema;
                    std::vector<double> vOutput(nOutput);
                    vInput.push_back(close);

                    Normalization::calcMinMax(vInput, vInput, 0);

                    int isUp = 0, isNeutral = 0;
                    if(vPik[i] == 1) {
                        vOutput[0] = 1.0; vOutput[1] = 0.0; //vOutput[2] = 0.0;
                        isUp = 1;
                    } else
                    if(vPik[i] == -1) {
                        vOutput[0] = 0.0; vOutput[1] = 1.0; //vOutput[2] = 1.0;
                        isNeutral = 1;
                    } else {
                        vOutput[0] = 0.0; vOutput[1] = 1.0; //vOutput[2] = 0.0;
                        isNeutral = 1;
                    }

                    if((nUp >= nNeutral && isNeutral == 1) || (nUp <= nNeutral && isUp == 1)) {
                        FannFunctions::setData(vInput, vOutput, inputTrain, outputTrain, posSamples);
                        nUp += isUp; nNeutral += isNeutral;
                        posSamples++;
                        if(posSamples >= numTrainingSamples) break;
                        if(posSamples % 1440 == 0) {
                            cout << "[" << posSamples << ":" << numTrainingSamples << "]\r";
                        }
                    }
                }
            }

            if(posSamples >= numTrainingSamples) break;
        } // for
        cout << endl;
        cout << "nUp: " << nUp << " nNeutral: " << nNeutral << " all: " << nNeutral + nUp << endl;

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
        trainFannData = fann_create_train_pointer_array(numTrainingSamples, nWindow, inputTrain, nOutput, outputTrain);
        cout << "shuffle..." << endl;
        fann_shuffle_train_data(trainFannData);
        cout << "save: " << name << endl;
        //setlocale(LC_ALL,"English");
        fann_save_train(trainFannData, name.c_str());
        cout << "save!" << endl;

        FannFunctions::deleteData(numTrainingSamples, inputTrain);
        FannFunctions::deleteData(numTrainingSamples, outputTrain);
    }

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

    void getNormBbRsiTrain(std::string name,
                           int numTrainingSamples,
                           int nMa,
                           int nBb, double d,
                           int nRsi,
                           int nWindow,
                           std::vector<double>& data) {

        Indicators::WRSI iRsi(nRsi);
        Indicators::NoLagMa iFilter(nMa);
        Normalization::BollingerBandsNorm iBbNorm(nBb , d);
        Indicators::Window iNeuroWindow(nWindow);
        Indicators::Window iNeuroWindow2(nWindow);

        int posSamples = 0;

        fann_type** inputTrain = FannFunctions::reserveData(numTrainingSamples, nWindow*2);
        fann_type** outputTrain = FannFunctions::reserveData(numTrainingSamples, 3);

        int startPos = 100;

        for(int i = 0; i < (int)data.size() - nWindow - 5; i++) {
            double& close = data[i];
            double& closeFutureA = data[i + 1];
            double& closeFutureB = data[i + 2];
            double& closeFutureC = data[i + 3];
            double& closeFutureD = data[i + 4];
            double& closeFutureE = data[i + 5];

            double filterData = iFilter.updata(close);
            double rsiData5 = 2.0 * (iRsi.updata(filterData) / 100.0) - 1.0;
            double normData = iBbNorm.updata(filterData);


            iNeuroWindow.updata(normData);
            iNeuroWindow2.updata(rsiData5);

            int state;
            if(close > closeFutureA &&
               close > closeFutureB &&
               close > closeFutureC &&
               close > closeFutureD &&
               close > closeFutureE) {
                state = -1;
            } else
            if(close < closeFutureA &&
               close < closeFutureB &&
               close < closeFutureC &&
               close < closeFutureD &&
               close < closeFutureE) {
                state = 1;
            } else {
                state = 0;
            }

            if(i > startPos &&
                iNeuroWindow.data.size() == iNeuroWindow.getPeriod()) {
                std::vector<double> vInputData;
                std::vector<double> vOutput(3);
                std::copy(iNeuroWindow.data.begin(), iNeuroWindow.data.end(), std::back_inserter(vInputData));
                std::copy(iNeuroWindow2.data.begin(), iNeuroWindow2.data.end(), std::back_inserter(vInputData));

                if(vInputData.size() != nWindow*2) {
                    cout << "error: vInputData.size() != nInput; vInputData.size() = " << vInputData.size() << " nInput: " << nWindow*2 << endl;
                    return;
                }

                switch(state) {
                    case 0:
                    vOutput[0] = 0; vOutput[1] = 1; vOutput[2] = 0;
                    break;
                    case 1:
                    vOutput[0] = 1; vOutput[1] = 0; vOutput[2] = 0;
                    break;
                    case -1:
                    vOutput[0] = 0; vOutput[1] = 0; vOutput[2] = 1;
                    break;
                }

                Normalization::checkingData(vInputData);
                Normalization::checkingData(vOutput);
                FannFunctions::setData(vInputData, vOutput, inputTrain, outputTrain, posSamples);

                posSamples++;
                if(posSamples >= numTrainingSamples) break;
                if(posSamples % 1440 == 0) {
                    cout << "[" << posSamples << ":" << numTrainingSamples << "]\r";
                }

            }

        } // for

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
        trainFannData = fann_create_train_pointer_array(numTrainingSamples, nWindow*2, inputTrain, 3, outputTrain);
        cout << "shuffle..." << endl;
        fann_shuffle_train_data(trainFannData);
        cout << "save: " << name << endl;
        //setlocale(LC_ALL,"English");
        fann_save_train(trainFannData, name.c_str());
        cout << "save!" << endl;

        FannFunctions::deleteData(numTrainingSamples, inputTrain);
        FannFunctions::deleteData(numTrainingSamples, outputTrain);

    }

    void getGoodTimeEmaRsiTrainData(std::string name, int numTrainingSamples, std::vector<double>& data) {
        Indicators::EMA iEMA(10);
        Indicators::RSI iRSI(5);

    }

    void getBbTrainData(std::string name, int numTrainingSamples, int nWindow, double d, std::vector<double>& data) {

        Indicators::BollingerBands iBollingerBands(nWindow, d);
        Indicators::Window iNeuroWindow(nWindow);

        int posSamples = 0;

        fann_type** inputTrain = FannFunctions::reserveData(numTrainingSamples, nWindow);
        fann_type** outputTrain = FannFunctions::reserveData(numTrainingSamples, 5);

        int startPos = 100;

        for(int i = 0; i < (int)data.size() - nWindow - 5; i++) {
            double& close = data[i];
            double& closeFutureA = data[i + 1];
            double& closeFutureB = data[i + 2];
            double& closeFutureC = data[i + 3];
            //double& closeFutureD = data[i + 4];
            //double& closeFutureE = data[i + 5];

            iBollingerBands.updata(close);
            iNeuroWindow.updata(close);

            int state;
            if(close > closeFutureA &&
               close > closeFutureB &&
               close > closeFutureC
               ) {
                state = -1;
            } else
            if(close < closeFutureA &&
               close < closeFutureB &&
               close < closeFutureC
               ) {
                state = 1;
            } else {
                state = 0;
            }

            int state2 = 0;
            if(close >= iBollingerBands.tl) {
                state2 = -1;
            } else
            if(close <= iBollingerBands.bl) {
                state2 = 1;
            }

            if(i > startPos &&
                (int)iNeuroWindow.data.size() == iNeuroWindow.getPeriod()) {
                std::vector<double> vInputData = iNeuroWindow.data;
                std::vector<double> vOutput(5);

                Normalization::calcZscore(vInputData, vInputData, 4.0);

                if(vInputData.size() != nWindow) {
                    cout << "error: vInputData.size() != nInput; vInputData.size() = " << vInputData.size() << " nInput: " << nWindow*2 << endl;
                    return;
                }

                if(state == 1 && state2 == 1) {
                    vOutput[0] = 1; vOutput[1] = 0; vOutput[2] = 0; vOutput[3] = 0; vOutput[4] = 0;
                } else
                if(state == -1 && state2 == 1) {
                    vOutput[0] = 0; vOutput[1] = 1; vOutput[2] = 0; vOutput[3] = 0; vOutput[4] = 0;
                } else
                if(state == -1 && state2 == -1) {
                    vOutput[0] = 0; vOutput[1] = 0; vOutput[2] = 0; vOutput[3] = 0; vOutput[4] = 1;
                } else
                if(state == 1 && state2 == -1) {
                    vOutput[0] = 0; vOutput[1] = 0; vOutput[2] = 0; vOutput[3] = 1; vOutput[4] = 0;
                } else {
                    vOutput[0] = 0; vOutput[1] = 0; vOutput[2] = 1; vOutput[3] = 0; vOutput[4] = 0;
                }

                Normalization::checkingData(vInputData);
                Normalization::checkingData(vOutput);
                FannFunctions::setData(vInputData, vOutput, inputTrain, outputTrain, posSamples);

                posSamples++;
                if(posSamples >= numTrainingSamples) break;
                if(posSamples % 1440 == 0) {
                    cout << "[" << posSamples << ":" << numTrainingSamples << "]\r";
                }

            }

        } // for

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
        trainFannData = fann_create_train_pointer_array(numTrainingSamples, nWindow, inputTrain, 5, outputTrain);
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
