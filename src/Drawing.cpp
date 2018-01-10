#include "Drawing.hpp"

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

    void CandlesType::updateIndicator(double data, int pos) {
        indicator[pos] = data;
    }

    void CandlesType::updateArrow(char arrow) {
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

    void Window::update(double open, double high, double low, double close) {
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

    void Window::updateLast(double open, double high, double low, double close) {
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

    void Window::setIndicator(double dataInd) {
        if(data.size() > 0)
        data[data.size() - 1].setIndicator(dataInd);
    }

    void Window::updateIndicator(double dataInd, int pos) {
        if(data.size() > 0)
        data[data.size() - 1].updateIndicator(dataInd, pos);
    }

    void getCandleGraph(cv::Mat& out, int width, int height, std::vector<CandlesType>& in, cv::Scalar backgroundColor, cv::Scalar backgroundColor2, cv::Scalar lineColor) {
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
                if(i + 1 < (int)in.size() && in[i].indicator.size() == in[i + 1].indicator.size()) {
                    for(int k = 0; k < (int)in[i].indicator.size(); k++) {
                        double indic = heightDiv2 - ((in[i].indicator[k] - aver) * sacle);
                        double indic2 = heightDiv2 - ((in[i + 1].indicator[k] - aver) * sacle);
                        //int rgbNum = (0xF0F0F0 * (k + 1)) / in[i].indicator.size();
                        //int R = (rgbNum >> 16) & 0xFF; int G = (rgbNum >> 8) & 0xFF; int B = rgbNum & 0xFF;
                        cv::line(output, cv::Point(offset, indic), cv::Point(offset + step, indic2), cv::Scalar(255,0,0));
                    }
                }
            }
        } // for  i
        out = output.clone();
        output.release();
    }

    void viewCandleGraph(std::string name, std::vector<CandlesType>& in, int flag = 1) {
        cv::Mat output;
        getCandleGraph(output, 800, 200, in, cv::Scalar(0,0,0),  cv::Scalar(25,0,0),  cv::Scalar(255,0,0));
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
        getCandleGraph(output, 1000, 400, in, cv::Scalar(0,0,0),  cv::Scalar(25,0,0),  cv::Scalar(255,0,0));
        std::string imageName = name + ".jpg";
        cv::imwrite(imageName, output);
        output.release();
    }

}
