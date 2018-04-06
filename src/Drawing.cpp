#include "Drawing.hpp"
#include <limits>

namespace Drawing {

    CandlesType::CandlesType() {};

    CandlesType::CandlesType(double _open, double _close, double _high, double _low) {
        open = _open;
        close = _close;
        high = _high;
        low = _low;
    }

    void CandlesType::setIndicator(double data) {
        indicator.push_back(data);
    }

    void CandlesType::setArrowUp() {
        isArrow = 1;
    }

    void CandlesType::setArrowDown() {
        isArrow = -1;
    }

    void CandlesType::setArrow(char arrow) {
        isArrow = arrow;
    }

    void CandlesType::setCircle(int state) {
        isCirc = state;
    }


    void CandlesType::updataIndicator(double data, int pos) {
        indicator[pos] = data;
    }

    void CandlesType::updataArrow(char arrow) {
        setArrow(arrow);
    }


    Window::Window() {
        data.clear();
        Window::N = 15;
    };

    Window::Window(int n) {
        data.clear();
        Window::N = n;
    }

    void Window::updata(double open, double high, double low, double close) {
        CandlesType typeCC(open, close, high, low);
        if((int)data.size() < N) {
            data.push_back(typeCC);
        } else {
            for(int i = 0; i < N - 1; i++) {
                data[i] = data[i + 1];
            }
            data[N - 1] = typeCC;
        }
    }

    void Window::updataLast(double open, double high, double low, double close) {
        //CandlesType typeCC(open, close, high, low);
        //data[N - 1] = typeCC;
        data[N - 1].open = open;
        data[N - 1].high = high;
        data[N - 1].low = low;
        data[N - 1].close =close;
    }

    void Window::getCandlesType(std::vector<CandlesType>& out) {
        out = data;
    }

    std::vector<CandlesType> Window::getCandlesType() {
        return data;
    }

    void Window::setArrow(char arrow) {
        if(data.size() > 0)
        data[data.size() - 1].setArrow(arrow);
    }

    void Window::setCircle(int state, int pos) {
        data[data.size() - 1 - pos].setCircle(state);
    }

    void Window::setIndicator(double dataInd) {
        if(data.size() > 0)
        data[data.size() - 1].setIndicator(dataInd);
    }

    void Window::updateIndicator(double dataInd, int pos) {
        if(data.size() > 0)
        data[data.size() - 1].updataIndicator(dataInd, pos);
    }

    void getCandleGraph(cv::Mat& out, int width, int height, std::vector<CandlesType>& in, cv::Scalar backgroundColor, cv::Scalar backgroundColor2, std::vector<cv::Scalar>& lineColor) {
        const double windowSize = 0.8;
        // создадим картинку
        cv::Mat output(cv::Size(width, height), CV_8UC3);
        output.setTo(backgroundColor);
        double maxData = -999999999.0, minData = 999999999.0;
        for(int i = 0; i < (int)in.size(); i++) {
            if (maxData < in[i].high) {
                maxData = in[i].high;
            }
            if (minData > in[i].low) {
                minData = in[i].low;
            }
            if(in[i].indicator.size() > 0) {
                for(int k = 0; k < (int)in[i].indicator.size(); k++) {
                    if (maxData < in[i].indicator[k]) {
                        maxData = in[i].indicator[k];
                    }
                    if (minData > in[i].indicator[k]) {
                        minData = in[i].indicator[k];
                    }
                }
            }
        }
        //printf("min max: %f %f\n", minData, maxData);
        double amplitudeData = maxData - minData;
        double aver = (amplitudeData / 2.0) + minData; // среднее значение
        double param = std::max(std::abs(maxData - aver),std::abs(aver - minData));// (std::abs(maxData - aver) > std::abs(aver - minData)) ? std::abs(maxData - aver)  : std::abs(aver - minData);
        double sacle = 0.7 * ((double)((double)height/2.0)/(double)param);

        double step = ((double) width * windowSize) / (double) in.size();
        //int numCandles = 0;
        double offset = 0;

        for (int i = 0; i < (int)in.size(); i++) {
            offset+= step;
            double heightDiv2 = (height/2);
            double close = heightDiv2 - ((in[i].close - aver) * sacle);
            double open = heightDiv2 - ((in[i].open - aver) * sacle);
            double high = heightDiv2 - ((in[i].high - aver) * sacle);
            double low = heightDiv2 - ((in[i].low - aver) * sacle);

            if (in[i].open < in[i].close) {
                cv::line(output, cv::Point(offset, 0), cv::Point(offset, height- 1), backgroundColor2);
                cv::line(output, cv::Point(offset, high), cv::Point(offset, low), cv::Scalar(0,255,0));
                //cv::line(output, cv::Point(offset, open), cv::Point(offset, close), cv::Scalar(0,255,0), 4.0, 4); // step/4.0
                cv::rectangle(output, cv::Point(offset - step/4.0, close), cv::Point(offset + step/4.0, open), cv::Scalar(0,255,0), CV_FILLED);
            } else
            if (in[i].open > in[i].close) {
                cv::line(output, cv::Point(offset, 0), cv::Point(offset, height- 1), backgroundColor2);
                cv::line(output, cv::Point(offset, high), cv::Point(offset, low), cv::Scalar(0,0,255));
                //cv::line(output, cv::Point(offset, open), cv::Point(offset, close), cv::Scalar(0,0,255), 4.0, 4);
                cv::rectangle(output, cv::Point(offset - step/4.0, open), cv::Point(offset + step/4.0, close), cv::Scalar(0,0,255), CV_FILLED);
            } else {
                cv::line(output, cv::Point(offset, 0), cv::Point(offset, height- 1), backgroundColor2);
                cv::line(output, cv::Point(offset, high), cv::Point(offset, low), cv::Scalar(255,255,255));
                //cv::line(output, cv::Point(offset, open), cv::Point(offset, close), cv::Scalar(255,255,255), 4.0, 4);
                cv::rectangle(output, cv::Point(offset - step/4.0, open), cv::Point(offset + step/4.0, close), cv::Scalar(255,255,255), CV_FILLED);
            }

            if (in[i].isArrow == 1) {
                cv::arrowedLine(output, cv::Point(offset, low + 0.15*height), cv::Point(offset, low + 0.05*height), cv::Scalar(0,255,0), 1, 8, 0, 0.4);
            } else if (in[i].isArrow == -1) {
                cv::arrowedLine(output, cv::Point(offset, high - 0.15*height), cv::Point(offset, high - 0.05*height), cv::Scalar(0,0,255), 1, 8, 0, 0.4);
            }

            //const int RGB_TABLE[] = {0x000000FF,0x000000FF}

            if(in[i].indicator.size() > 0) {
                if(i + 1 < (int)in.size()) {
                    for(int k = 0; k < (int)in[i].indicator.size(); k++) {
                        if(k >= (int)in[i + 1].indicator.size()) break;
                        double indic = heightDiv2 - ((in[i].indicator[k] - aver) * sacle);
                        double indic2 = heightDiv2 - ((in[i + 1].indicator[k] - aver) * sacle);
                        if(k < (int)lineColor.size()) {
                            cv::line(output, cv::Point(offset, indic), cv::Point(offset + step, indic2), lineColor[k]);
                        } else {
                            cv::line(output, cv::Point(offset, indic), cv::Point(offset + step, indic2), cv::Scalar(255,0,0));
                        }
                    }
                }
            }
        } // for  i
        out = output.clone();
        output.release();
    }

    void viewCandleGraph(std::string name, std::vector<CandlesType>& in, int flag = 1) {
        cv::Mat output;
        std::vector<cv::Scalar> vLineColoer(10);
        vLineColoer[0] = cv::Scalar(255,0,0); vLineColoer[1] = cv::Scalar(255,150,0);
        vLineColoer[2] = cv::Scalar(150,255,0); vLineColoer[3] = cv::Scalar(0,255,0);
        vLineColoer[4] = cv::Scalar(0,255,150); vLineColoer[5] = cv::Scalar(0,150,255);
        vLineColoer[6] = cv::Scalar(0,0,255); vLineColoer[7] = cv::Scalar(50,50,255);
        vLineColoer[8] = cv::Scalar(150,150,255); vLineColoer[9] = cv::Scalar(255,255,255);
        getCandleGraph(output, 1000, 400, in, cv::Scalar(0,0,0),  cv::Scalar(25,0,0),  vLineColoer);
        for(int i = 0; i < 10; i++) {
            cv::putText(output, std::to_string(i + 1), cv::Point(10 + i * 8 , 10), CV_FONT_HERSHEY_PLAIN, 1.0, vLineColoer[i], 1, 8, 0);
        }
        cv::imshow(name, output);
        output.release();
        if (flag == 0) {
            cv::waitKey(5);
        } else
        while(1) {
            char symbol = cv::waitKey(10);
            if (symbol == 'P' || symbol == 'p') {
                break;
            }
        }
    }

    void saveCandleGraph(std::string name, std::vector<CandlesType>& in) {
        cv::Mat output;
        std::vector<cv::Scalar> vLineColoer(10);
        vLineColoer[0] = cv::Scalar(255,0,0); vLineColoer[1] = cv::Scalar(255,150,0);
        vLineColoer[2] = cv::Scalar(150,255,0); vLineColoer[3] = cv::Scalar(0,255,0);
        vLineColoer[4] = cv::Scalar(0,255,150); vLineColoer[5] = cv::Scalar(0,150,255);
        vLineColoer[6] = cv::Scalar(0,0,255); vLineColoer[7] = cv::Scalar(50,50,255);
        vLineColoer[8] = cv::Scalar(150,150,255); vLineColoer[9] = cv::Scalar(255,255,255);
        getCandleGraph(output, 1000, 400, in, cv::Scalar(0,0,0),  cv::Scalar(25,0,0),  vLineColoer);
        for(int i = 0; i < 10; i++) {
            cv::putText(output, std::to_string(i + 1), cv::Point(10 + i * 8 , 10), CV_FONT_HERSHEY_PLAIN, 1.0, vLineColoer[i], 1, 8, 0);
        }
        std::string imageName = name + ".jpg";
        cv::imwrite(imageName, output);
        output.release();
    }

    char drawOscilloscope4xBeam(
        std::string name, std::string text,
        std::vector<double> in1, std::vector<double> in2, std::vector<double> in3, std::vector<double> in4,
        int width, int height, int mask) {
        std::vector<int> out1(width);
        std::vector<int> out2(width);
        std::vector<int> out3(width);
        std::vector<int> out4(width);
        int stop1 = 0, stop2 = 0, stop3 = 0, stop4 = 0;
        std::vector<int> arraysize(4);
        arraysize[0] = in1.size(); arraysize[1] = in2.size();
        arraysize[2] = in3.size(); arraysize[3] = in4.size();
        int maxArraySize = *std::max_element(arraysize.begin(), arraysize.end());
        if (maxArraySize < 2) return 0;
        // создадим картинку
        cv::Mat output(cv::Size(width, height), CV_8UC3);
        output = cv::Mat::zeros(output.size(),CV_8UC3);
        std::vector<double> all;
        std::copy(in1.begin(), in1.end(), std::back_inserter(all));
        std::copy(in2.begin(), in2.end(), std::back_inserter(all));
        std::copy(in3.begin(), in3.end(), std::back_inserter(all));
        std::copy(in4.begin(), in4.end(), std::back_inserter(all));
        double minData = *std::min_element(all.begin(), all.end());
        double maxData = *std::max_element(all.begin(), all.end());
        double stepData =  (double)maxArraySize / (double)width; // шаг
        //double amplitudeData = maxData - minData;
        double aver = ((double)(maxData - minData) / 2.0) + minData; // среднее значение
        double param = (std::abs(maxData - aver) > std::abs(aver - minData)) ? std::abs(maxData - aver)  : std::abs(aver - minData);
        double sacle = 0.8 * ((double)((double)height/2.0)/(double)param);
        double offset = 0.0;
        int prePos = 0;
        for (int i = 0; i < width; i ++) {
            int pos = offset;
            if (prePos != pos) {
                if ((pos + 1) < maxArraySize) {
                    cv::line(output, cv::Point(i, 0), cv::Point(i, height- 1), cv::Scalar(55,25,0));
                }
                prePos = pos;
            }
            offset += stepData;
        }
        offset = 0.0; prePos = 0;
        for (int i = 0; i < width; i ++) {
            int pos = offset;
            if (pos < (int)in1.size()) {
                out1[i] = (height/2) - ((in1[pos] - aver) * sacle);
                stop1 = i;
                if (i % (width/4) == 0 && mask & 0x01) {
                    std::string value = std::to_string(in1[pos]);
                    cv::putText(output, value, cv::Point(i,out1[i] - 20.0), CV_FONT_HERSHEY_PLAIN, 1.0,cv::Scalar(255,255,255), 1, 8, 0);
                }
            }
            if (pos < (int)in2.size()) {
                out2[i] = (height/2) - ((in2[pos] - aver) * sacle);
                stop2 = i;
                if (i % (width/4) == 0 && mask & 0x02) {
                    std::string value = std::to_string(in2[pos]);
                    cv::putText(output, value, cv::Point(i,out2[i] + 20.0), CV_FONT_HERSHEY_PLAIN, 1.0,cv::Scalar(255,255,255), 1, 8, 0);
                }
            }
            if (pos < (int)in3.size()) {
                out3[i] = (height/2) - ((in3[pos] - aver) * sacle);
                stop3 = i;
                if (i % (width/4) == 0 && mask & 0x04) {
                    std::string value = std::to_string(in3[pos]);
                    cv::putText(output, value, cv::Point(i,out3[i] - 40.0), CV_FONT_HERSHEY_PLAIN, 1.0,cv::Scalar(255,255,255), 1, 8, 0);
                }
            }
            if (pos < (int)in4.size()) {
                out4[i] = (height/2) - ((in4[pos] - aver) * sacle);
                stop4 = i;
                if (i % (width/4) == 0 && mask & 0x08) {
                    std::string value = std::to_string(in4[pos]);
                    cv::putText(output, value, cv::Point(i,out4[i] + 40.0), CV_FONT_HERSHEY_PLAIN, 1.0,cv::Scalar(255,255,255), 1, 8, 0);
                }
            }
            offset += stepData;
        }
        if (stop1 > 0)
        for (int i = 0; i < stop1; i ++) {
            cv::line(output, cv::Point(i, out1[i]), cv::Point(i + 1, out1[i + 1]), cv::Scalar(255,125,0));
        }
        for (int i = 0; i < stop2; i ++) {
            cv::line(output, cv::Point(i, out2[i]), cv::Point(i + 1, out2[i + 1]), cv::Scalar(255,255,0));
        }
        for (int i = 0; i < stop3; i ++) {
            cv::line(output, cv::Point(i, out3[i]), cv::Point(i + 1, out3[i + 1]), cv::Scalar(125,255,0));
        }
        for (int i = 0; i < stop4; i ++) {
            cv::line(output, cv::Point(i, out4[i]), cv::Point(i + 1, out4[i + 1]), cv::Scalar(0,255,255));
        }
        std::string textMax = "Max level = "; textMax += std::to_string(maxData);
        std::string textMin = "Min level = "; textMin += std::to_string(minData);
        cv::putText(output, textMax, cv::Point(8, 20), CV_FONT_HERSHEY_PLAIN, 1.0,cv::Scalar(0,0,255), 1, 8, 0);
        cv::putText(output, textMin, cv::Point(8, height - 10), CV_FONT_HERSHEY_PLAIN, 1.0,cv::Scalar(255,0,0), 1, 8, 0);

        cv::putText(output, "1", cv::Point(width/4, 10), CV_FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255,125,0), 1, 8, 0);
        cv::putText(output, "2", cv::Point(width/4 + 10, 10), CV_FONT_HERSHEY_PLAIN, 1.0,cv::Scalar(255,255,0), 1, 8, 0);
        cv::putText(output, "3", cv::Point(width/4 + 20, 10), CV_FONT_HERSHEY_PLAIN, 1.0,cv::Scalar(125,255,0), 1, 8, 0);
        cv::putText(output, "4", cv::Point(width/4 + 30, 10), CV_FONT_HERSHEY_PLAIN, 1.0,cv::Scalar(0,255,255), 1, 8, 0);
        cv::putText(output, text, cv::Point(8, 40), CV_FONT_HERSHEY_PLAIN, 1.0,cv::Scalar(125,255,0), 1, 8, 0);

        if(mask & 0x10) {
            std::string imageName = "img//" + name + ".jpg";
            cv::imwrite(imageName, output);
        }

        cv::imshow(name, output);
        cv::waitKey(5);

        output.release();
        return 0;
    }


    void drawCandleGraph(cv::Mat& output, int x, int y, int width, int height, std::vector<CandlesType>& in, cv::Scalar backgroundColor, std::vector<cv::Scalar>& lineColor, int isAutoCircl) {
        const double windowSize = 0.8;

        double maxData = std::numeric_limits<double>::min(), minData = std::numeric_limits<double>::max();
        for(int i = 0; i < (int)in.size(); i++) {
            if (maxData < in[i].high) { maxData = in[i].high;}
            if (minData > in[i].low) {minData = in[i].low;}
            if(in[i].indicator.size() > 0) {
                for(int k = 0; k < (int)in[i].indicator.size(); k++) {
                    if (maxData < in[i].indicator[k]) {maxData = in[i].indicator[k];}
                    if (minData > in[i].indicator[k]) {minData = in[i].indicator[k];}
                }
            }
        }
        //printf("min max: %f %f\n", minData, maxData);
        double amplitudeData = maxData - minData;
        double aver = (amplitudeData / 2.0) + minData; // среднее значение
        double param = std::max(std::abs(maxData - aver),std::abs(aver - minData));// (std::abs(maxData - aver) > std::abs(aver - minData)) ? std::abs(maxData - aver)  : std::abs(aver - minData);
        double sacle = 0.7 * ((double)((double)height/2.0)/(double)param);

        double step = ((double) width * windowSize) / (double) in.size();
        //int numCandles = 0;
        double offset = 0;
        double heightDiv2 = (height/2);

        for(int i = 0; i < (int)in.size(); i++) {
            offset+= step;

            double close = heightDiv2 - ((in[i].close - aver) * sacle);
            double open = heightDiv2 - ((in[i].open - aver) * sacle);
            double high = heightDiv2 - ((in[i].high - aver) * sacle);
            double low = heightDiv2 - ((in[i].low - aver) * sacle);

            if (in[i].open < in[i].close) {
                cv::line(output, cv::Point(x + offset, y + 0), cv::Point(x + offset, y + height- 1), backgroundColor);
                cv::line(output, cv::Point(x + offset, y + high), cv::Point(x + offset, y + low), cv::Scalar(0,255,0));
                //cv::line(output, cv::Point(offset, open), cv::Point(offset, close), cv::Scalar(0,255,0), 4.0, 4); // step/4.0
                cv::rectangle(output, cv::Point(offset - step/4.0, close), cv::Point(offset + step/4.0, open), cv::Scalar(0,255,0), CV_FILLED);
            } else
            if (in[i].open > in[i].close) {
                cv::line(output, cv::Point(x + offset, y + 0), cv::Point(x + offset, y + height- 1), backgroundColor);
                cv::line(output, cv::Point(x + offset, y + high), cv::Point(x + offset, y + low), cv::Scalar(0,0,255));
                //cv::line(output, cv::Point(offset, open), cv::Point(offset, close), cv::Scalar(0,0,255), 4.0, 4);
                cv::rectangle(output, cv::Point(offset - step/4.0, open), cv::Point(offset + step/4.0, close), cv::Scalar(0,0,255), CV_FILLED);
            } else {
                cv::line(output, cv::Point(x + offset, y + 0), cv::Point(x + offset, y +  height- 1), backgroundColor);
                cv::line(output, cv::Point(x + offset, y + high), cv::Point(x + offset, y + low), cv::Scalar(255,255,255));
                //cv::line(output, cv::Point(offset, open), cv::Point(offset, close), cv::Scalar(255,255,255), 4.0, 4);
                cv::rectangle(output, cv::Point(x + offset - step/4.0, y + open), cv::Point(x + offset + step/4.0, y + close), cv::Scalar(255,255,255), CV_FILLED);
            }

        } // for  i

        offset = 0;

        for(int i = 0; i < (int)in.size(); i++) {
            offset+= step;
            //double high = heightDiv2 - ((in[i].high - aver) * sacle);
            //double low = heightDiv2 - ((in[i].low - aver) * sacle);
            if(in[i].indicator.size() > 0) {
                if(i + 1 < (int)in.size()) {
                    for(int k = 0; k < (int)in[i].indicator.size(); k++) {
                        if(k >= (int)in[i + 1].indicator.size()) break;
                        double indic = heightDiv2 - ((in[i].indicator[k] - aver) * sacle);
                        double indic2 = heightDiv2 - ((in[i + 1].indicator[k] - aver) * sacle);
                        if(k < (int)lineColor.size()) {
                            cv::line(output, cv::Point(x + offset, y + indic), cv::Point(x + offset + step, y + indic2), lineColor[k]);
                        } else {
                            cv::line(output, cv::Point(x + offset, y + indic), cv::Point(x + offset + step, y + indic2), cv::Scalar(255,0,0));
                        }
                    }
                }
            }
        }

        offset = 0;

        for(int i = 0; i < (int)in.size(); i++) {
            offset+= step;
            double high = heightDiv2 - ((in[i].high - aver) * sacle);
            double low = heightDiv2 - ((in[i].low - aver) * sacle);
            if (in[i].isArrow == 1) {
                cv::arrowedLine(output, cv::Point(x + offset, y + low + 0.15*height), cv::Point(x + offset, y + low + 0.05*height), cv::Scalar(0,255,0), 1, 8, 0, 0.4);
            } else if (in[i].isArrow == -1) {
                cv::arrowedLine(output, cv::Point(x + offset, y + high - 0.15*height), cv::Point(x + offset, y + high - 0.05*height), cv::Scalar(0,0,255), 1, 8, 0, 0.4);
            }
        }

        const double CIRC_R = 0.06;
        offset = 0;
        if(isAutoCircl == 0) {
            for(int i = 0; i < (int)in.size(); i++) {
                offset+= step;
                double high = heightDiv2 - ((in[i].high - aver) * sacle);
                double low = heightDiv2 - ((in[i].low - aver) * sacle);
                if (in[i].isCirc == 1) {
                    cv::circle(output,cv::Point(x + offset, y + low + 0.1*height),CIRC_R*height,cv::Scalar(0,255,0),2);
                } else
                if (in[i].isCirc == 2) {
                    cv::circle(output,cv::Point(x + offset, y + low + 0.1*height),CIRC_R*height,cv::Scalar(0,0,255),2);
                } else
                if (in[i].isCirc == -1) {
                    cv::circle(output,cv::Point(x + offset, y + high - 0.1*height),CIRC_R*height,cv::Scalar(0,255,0),2);
                } else
                if (in[i].isCirc == -2) {
                    cv::circle(output,cv::Point(x + offset, y + high - 0.1*height),CIRC_R*height,cv::Scalar(0,0,255),2);
                }
            }
        } else {
            for(int i = 0; i < (int)in.size(); i++) {
                offset+= step;
                double high = heightDiv2 - ((in[i].high - aver) * sacle);
                double low = heightDiv2 - ((in[i].low - aver) * sacle);
                if(i < (int)in.size() - isAutoCircl)
                if (in[i].isArrow == 1 && in[i].close < in[i + isAutoCircl].close) {
                    cv::circle(output,cv::Point(x + offset, y + low + 0.1*height),CIRC_R*height,cv::Scalar(0,255,0),2);
                } else
                if (in[i].isArrow == 1 && in[i].close > in[i + isAutoCircl].close) {
                    cv::circle(output,cv::Point(x + offset, y + low + 0.1*height),CIRC_R*height,cv::Scalar(0,0,255),2);
                } else
                if (in[i].isArrow == -1 && in[i].close > in[i + isAutoCircl].close) {
                    cv::circle(output,cv::Point(x + offset, y + high - 0.1*height),CIRC_R*height,cv::Scalar(0,255,0),2);
                } else
                if (in[i].isArrow == -1 && in[i].close < in[i + isAutoCircl].close) {
                    cv::circle(output,cv::Point(x + offset, y + high - 0.1*height),CIRC_R*height,cv::Scalar(0,0,255),2);
                }
            }
        }
    }

    void viewCandleGraph2(std::string name, Window& iWind, int width, int height) {
        cv::Mat output(cv::Size(width, height), CV_8UC3);
        output.setTo(cv::Scalar(0,0,0));
        std::vector<CandlesType> iCT = iWind.getCandlesType();
        std::vector<cv::Scalar> vLineColoer(10);
        vLineColoer[0] = cv::Scalar(255,0,0); vLineColoer[1] = cv::Scalar(255,150,0);
        vLineColoer[2] = cv::Scalar(150,255,0); vLineColoer[3] = cv::Scalar(0,255,0);
        vLineColoer[4] = cv::Scalar(0,255,150); vLineColoer[5] = cv::Scalar(0,150,255);
        vLineColoer[6] = cv::Scalar(0,0,255); vLineColoer[7] = cv::Scalar(50,50,255);
        vLineColoer[8] = cv::Scalar(150,150,255); vLineColoer[9] = cv::Scalar(255,255,255);
        drawCandleGraph(output,0,0,width, height,iCT,cv::Scalar(10,10,10),vLineColoer,3);

        cv::imshow(name, output);
        cv::waitKey(10);
        output.release();
    }

    void drawCandleGraphV2(cv::Mat& output, int x, int y, int width, int height, std::vector<CandlesType>& in, cv::Scalar backgroundColor, std::vector<cv::Scalar>& lineColor, int isAutoCircl) {
        const double windowSize = 0.7;

        double maxData = std::numeric_limits<double>::min(), minData = std::numeric_limits<double>::max();
        for(int i = 0; i < (int)in.size(); i++) {
            if (maxData < in[i].high) { maxData = in[i].high;}
            if (minData > in[i].low) {minData = in[i].low;}
            if(in[i].indicator.size() > 0) {
                for(int k = 0; k < (int)in[i].indicator.size(); k++) {
                    if (maxData < in[i].indicator[k]) {maxData = in[i].indicator[k];}
                    if (minData > in[i].indicator[k]) {minData = in[i].indicator[k];}
                }
            }
        }
        //printf("min max: %f %f\n", minData, maxData);
        double amplitudeData = maxData - minData;
        double aver = (amplitudeData / 2.0) + minData; // среднее значение
        double param = std::max(std::abs(maxData - aver),std::abs(aver - minData));// (std::abs(maxData - aver) > std::abs(aver - minData)) ? std::abs(maxData - aver)  : std::abs(aver - minData);
        double sacle = 0.6 * ((double)((double)height/2.0)/(double)param);

        double step = ((double) width * windowSize) / (double) in.size();
        //int numCandles = 0;
        double offset = 0;
        double heightDiv2 = (height/2);

        for(int i = 0; i < (int)in.size(); i++) {
            offset+= step;

            double close = heightDiv2 - ((in[i].close - aver) * sacle);
            double open = heightDiv2 - ((in[i].open - aver) * sacle);
            double high = heightDiv2 - ((in[i].high - aver) * sacle);
            double low = heightDiv2 - ((in[i].low - aver) * sacle);

            if (in[i].open < in[i].close) {
                cv::line(output, cv::Point(x + offset, y + 0), cv::Point(x + offset, y + height- 1), backgroundColor);
                cv::line(output, cv::Point(x + offset, y + high), cv::Point(x + offset, y + low), cv::Scalar(0,255,0));
                //cv::line(output, cv::Point(offset, open), cv::Point(offset, close), cv::Scalar(0,255,0), 4.0, 4); // step/4.0
                cv::rectangle(output, cv::Point(x + offset - step/4.0, y + close), cv::Point(x + offset + step/4.0, y + open), cv::Scalar(0,255,0), CV_FILLED);
            } else
            if (in[i].open > in[i].close) {
                cv::line(output, cv::Point(x + offset, y + 0), cv::Point(x + offset, y + height- 1), backgroundColor);
                cv::line(output, cv::Point(x + offset, y + high), cv::Point(x + offset, y + low), cv::Scalar(0,0,255));
                //cv::line(output, cv::Point(offset, open), cv::Point(offset, close), cv::Scalar(0,0,255), 4.0, 4);
                cv::rectangle(output, cv::Point(x + offset - step/4.0, y + open), cv::Point(x + offset + step/4.0, y + close), cv::Scalar(0,0,255), CV_FILLED);
            } else {
                cv::line(output, cv::Point(x + offset, y + 0), cv::Point(x + offset, y +  height- 1), backgroundColor);
                cv::line(output, cv::Point(x + offset, y + high), cv::Point(x + offset, y + low), cv::Scalar(255,255,255));
                //cv::line(output, cv::Point(offset, open), cv::Point(offset, close), cv::Scalar(255,255,255), 4.0, 4);
                cv::rectangle(output, cv::Point(x + offset - step/4.0, y + open), cv::Point(x + offset + step/4.0, y + close), cv::Scalar(255,255,255), CV_FILLED);
            }

        } // for  i

        offset = 0;

        for(int i = 0; i < (int)in.size(); i++) {
            offset+= step;
            double high = heightDiv2 - ((in[i].high - aver) * sacle);
            double low = heightDiv2 - ((in[i].low - aver) * sacle);
            if(in[i].indicator.size() > 0) {
                if(i + 1 < (int)in.size()) {
                    for(int k = 0; k < (int)in[i].indicator.size(); k++) {
                        if(k >= (int)in[i + 1].indicator.size()) break;
                        double indic = heightDiv2 - ((in[i].indicator[k] - aver) * sacle);
                        double indic2 = heightDiv2 - ((in[i + 1].indicator[k] - aver) * sacle);
                        if(k < (int)lineColor.size()) {
                            cv::line(output, cv::Point(x + offset, y + indic), cv::Point(x + offset + step, y + indic2), lineColor[k]);
                        } else {
                            cv::line(output, cv::Point(x + offset, y + indic), cv::Point(x + offset + step, y + indic2), cv::Scalar(255,0,0));
                        }
                    }
                }
            }
        }

        offset = 0;

        for(int i = 0; i < (int)in.size(); i++) {
            offset+= step;
            double high = heightDiv2 - ((in[i].high - aver) * sacle);
            double low = heightDiv2 - ((in[i].low - aver) * sacle);
            if (in[i].isArrow == 1) {
                //cv::arrowedLine(output, cv::Point(x + offset, y + low + 0.15*height), cv::Point(x + offset + 0.1*height, y + low + 0.05*height), cv::Scalar(0,255,0), 1, 8, 0, 0.4);
                //cv::arrowedLine(output, cv::Point(x + offset, y + low + 0.15*height), cv::Point(x + offset + 0.05*height, y + low + 0.1*height), cv::Scalar(0,255,0), 1, 8, 0, 0.5);
                std::vector<cv::Point> point;
                    point.push_back(cv::Point(x + offset,y + low + 0.05*height));  //point1
                    point.push_back(cv::Point(x + offset + 0.025*height, y + low + 0.1*height));  //point2
                    point.push_back(cv::Point(x + offset - 0.025*height, y + low + 0.1*height));  //point3
                cv::fillConvexPoly(output, point, cv::Scalar(0,255,0), CV_AA, 0);
            } else if (in[i].isArrow == -1) {
                //cv::arrowedLine(output, cv::Point(x + offset, y + high - 0.15*height), cv::Point(x + offset + 0.1*height, y + high - 0.05*height), cv::Scalar(0,0,255), 1, 8, 0, 0.4);
                //cv::arrowedLine(output, cv::Point(x + offset, y + high - 0.15*height), cv::Point(x + offset + 0.05*height, y + high - 0.1*height), cv::Scalar(0,0,255), 1, 8, 0, 0.5);
                std::vector<cv::Point> point;
                    point.push_back(cv::Point(x + offset,y + high - 0.05*height));  //point1
                    point.push_back(cv::Point(x + offset + 0.025*height, y + high - 0.1*height));  //point2
                    point.push_back(cv::Point(x + offset - 0.025*height, y + high - 0.1*height));  //point3
                cv::fillConvexPoly(output, point, cv::Scalar(0,0,255), CV_AA, 0);
            }
        }

        const double CIRC_R = 0.07;
        offset = 0;
        if(isAutoCircl == 0) {
            for(int i = 0; i < (int)in.size(); i++) {
                offset+= step;
                double high = heightDiv2 - ((in[i].high - aver) * sacle);
                double low = heightDiv2 - ((in[i].low - aver) * sacle);
                if (in[i].isCirc == 1) {
                    cv::circle(output,cv::Point(x + offset, y + low + 0.1*height),CIRC_R*height,cv::Scalar(0,255,0),1);
                } else
                if (in[i].isCirc == 2) {
                    cv::circle(output,cv::Point(x + offset, y + low + 0.1*height),CIRC_R*height,cv::Scalar(0,0,255),1);
                } else
                if (in[i].isCirc == -1) {
                    cv::circle(output,cv::Point(x + offset, y + high - 0.1*height),CIRC_R*height,cv::Scalar(0,255,0),1);
                } else
                if (in[i].isCirc == -2) {
                    cv::circle(output,cv::Point(x + offset, y + high - 0.1*height),CIRC_R*height,cv::Scalar(0,0,255),1);
                }
            }
        } else {
            for(int i = 0; i < (int)in.size(); i++) {
                offset+= step;
                double high = heightDiv2 - ((in[i].high - aver) * sacle);
                double low = heightDiv2 - ((in[i].low - aver) * sacle);
                if(i < (int)in.size() - isAutoCircl)
                if (in[i].isArrow == 1 && in[i].close < in[i + isAutoCircl].close) {
                    //cv::circle(output,cv::Point(x + offset, y + low + 0.1*height),CIRC_R*height,cv::Scalar(0,255,0),1);
                    cv::line(output, cv::Point(x + offset - 0.03*height, y + low + 0.12*height), cv::Point(x + offset, y + low + 0.18*height), cv::Scalar(0,255,0));
                    cv::line(output, cv::Point(x + offset + 0.03*height, y + low + 0.14*height), cv::Point(x + offset, y + low + 0.18*height), cv::Scalar(0,255,0));
                } else
                if (in[i].isArrow == 1 && in[i].close > in[i + isAutoCircl].close) {
                    //cv::circle(output,cv::Point(x + offset, y + low + 0.1*height),CIRC_R*height,cv::Scalar(0,0,255),1);
                    cv::line(output, cv::Point(x + offset - 0.03*height, y + low + 0.12*height), cv::Point(x + offset + 0.03*height, y + low + 0.18*height), cv::Scalar(0,0,255));
                    cv::line(output, cv::Point(x + offset + 0.03*height, y + low + 0.12*height), cv::Point(x + offset - 0.03*height, y + low + 0.18*height), cv::Scalar(0,0,255));
                } else
                if (in[i].isArrow == -1 && in[i].close > in[i + isAutoCircl].close) {
                    //cv::circle(output,cv::Point(x + offset, y + high - 0.1*height),CIRC_R*height,cv::Scalar(0,255,0),1);
                    cv::line(output, cv::Point(x + offset - 0.03*height, y + high - 0.18*height), cv::Point(x + offset, y + high - 0.12*height), cv::Scalar(0,255,0));
                    cv::line(output, cv::Point(x + offset + 0.03*height, y + high - 0.16*height), cv::Point(x + offset, y + high - 0.12*height), cv::Scalar(0,255,0));
                } else
                if (in[i].isArrow == -1 && in[i].close < in[i + isAutoCircl].close) {
                    //cv::circle(output,cv::Point(x + offset, y + high - 0.1*height),CIRC_R*height,cv::Scalar(0,0,255),1);
                    cv::line(output, cv::Point(x + offset - 0.03*height, y + high - 0.12*height), cv::Point(x + offset + 0.03*height, y + high - 0.18*height), cv::Scalar(0,0,255));
                    cv::line(output, cv::Point(x + offset + 0.03*height, y + high - 0.12*height), cv::Point(x + offset - 0.03*height, y + high - 0.18*height), cv::Scalar(0,0,255));
                }
            }
        }
    }

    void viewCandleGraphV2(std::string name, Window& iWind, int width, int height) {
        cv::Mat output(cv::Size(width, height), CV_8UC3);
        output.setTo(cv::Scalar(0,0,0));
        std::vector<CandlesType> iCT = iWind.getCandlesType();
        std::vector<cv::Scalar> vLineColoer(10);
        vLineColoer[0] = cv::Scalar(255,0,0); vLineColoer[1] = cv::Scalar(255,150,0);
        vLineColoer[2] = cv::Scalar(150,255,0); vLineColoer[3] = cv::Scalar(0,255,0);
        vLineColoer[4] = cv::Scalar(0,255,150); vLineColoer[5] = cv::Scalar(0,150,255);
        vLineColoer[6] = cv::Scalar(0,0,255); vLineColoer[7] = cv::Scalar(50,50,255);
        vLineColoer[8] = cv::Scalar(150,150,255); vLineColoer[9] = cv::Scalar(255,255,255);
        drawCandleGraphV2(output,0,0,width, height,iCT,cv::Scalar(10,10,10),vLineColoer,3);

        cv::imshow(name, output);
        cv::waitKey(10);
        output.release();
    }

    void drawDepositGraphV2(cv::Mat& output, int x, int y, int width, int height, std::vector<double>& in, cv::Scalar backgroundColor, cv::Scalar lineColor, double th) {
        const double windowSize = 0.95;

        cv::rectangle(output,
                        cv::Point(x, y),
                        cv::Point(x + width - 1, y + height - 1),
                        backgroundColor, CV_FILLED);



        double maxData = *std::max_element(in.begin(), in.end());
        double minData = *std::min_element(in.begin(), in.end());

        double amplitudeData = maxData - minData;
        double aver = (amplitudeData / 2.0) + minData; // среднее значение
        double param = std::max(std::abs(maxData - aver),std::abs(aver - minData));
        double sacle = 0.7 * ((double)((double)height/2.0)/(double)param);

        double step = ((double) width * windowSize) / (double) in.size();

        double heightDiv2 = (height/2);
        double offset = 0;
        /*
        for(int i = 0; i < (int)in.size(); i++) {
            offset+= step;
            if(i > 1 && vMonth[i] != vMonth[i - 1]) {
                cv::line(output, cv::Point(x + offset, y + 1), cv::Point(x + offset, y + height - 1), DEF_BACK_COLOR, 1.0);
            }
        }

        offset = 0;
        */
        for(int i = 0; i < (int)in.size(); i++) {
            offset+= step;
            if(i + 1 < (int)in.size()) {
                double indic = heightDiv2 - ((in[i] - aver) * sacle);
                double indic2 = heightDiv2 - ((in[i + 1] - aver) * sacle);
                cv::line(output, cv::Point(x + offset, y + indic), cv::Point(x + offset + step, y + indic2), lineColor,th);
            }
        }
        /*
        cv::Rect bbTestWindow(x + 1,y + 1,((double) width * windowSize),height-1);
        if(checkBB(bbTestWindow) != 0) {
            cv::line(output, cv::Point(pcMouse.movePosition.x, y + 1), cv::Point(pcMouse.movePosition.x, y + height - 1), cv::Scalar(255,255,255), 1.0);
            int indexArray = (int)((double)(pcMouse.movePosition.x - x) / (double)step);
            double indic = heightDiv2 - ((in[indexArray] - aver) * sacle);
            cv::line(output, cv::Point(x + 1, y + indic), cv::Point(x + width - 1, y + indic), cv::Scalar(255,255,255), 1.0);

            if(pcMouse.movePosition.x > 128) {
                printTextWithBackYL(std::to_string((unsigned long long)in[indexArray]),cv::Rect(pcMouse.movePosition.x - 1, y + indic - 21,128,20),CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, cv::Scalar(255,255,255));
                char timeStr[64]; memset(timeStr, 0, 64); sprintf(timeStr,"%.2d.%.2d.%.2d",vYear[indexArray],vMonth[indexArray],vDay[indexArray]);
                printTextWithBackYL(timeStr,cv::Rect(pcMouse.movePosition.x - 1, y + indic - 1,128,20),CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, cv::Scalar(255,255,255));
            } else {
                printTextWithBackYR(std::to_string((unsigned long long)in[indexArray]),cv::Rect(pcMouse.movePosition.x + 1, y + indic - 21,128,20),CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, cv::Scalar(255,255,255));
                char timeStr[64]; memset(timeStr, 0, 64); sprintf(timeStr,"%.2d.%.2d.%.2d",vYear[indexArray],vMonth[indexArray],vDay[indexArray]);
                printTextWithBackYR(timeStr,cv::Rect(pcMouse.movePosition.x - 1, y + indic - 1,128,20),CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, cv::Scalar(255,255,255));
            }
        }

        */

        cv::putText(output, "MIN: " + std::to_string((int)minData),
                                cv::Point(x + 10, y + height - 10),
                                CV_FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255,255,255), 1.0, 8, 0);
        cv::putText(output, "MAX: " + std::to_string((int)maxData),
                                cv::Point(x + 10, y + 20),
                                CV_FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255,255,255), 1.0, 8, 0);

    }
}
