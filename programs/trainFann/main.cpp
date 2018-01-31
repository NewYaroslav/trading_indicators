#include <iostream>
#include <fstream>
#include "fann.h"
#include "floatfann.h"
#include "stdio.h"

using namespace std;

std::string nameTrainFile;
std::string nameNet;
float desired_error = 0.01;
unsigned int max_epochs = 100;

int FANN_API test_callback(struct fann * ann, struct fann_train_data * train,
                           unsigned int max_epochs, unsigned int epochs_between_reports,
                           float wish_error, unsigned int epochs) {
    char netName[512] = {0};
    fann_test_data(ann, train);
    sprintf(netName, "%s_%d_%.5f._bit_%d.net", nameNet.c_str(), epochs, fann_get_MSE(ann), fann_get_bit_fail(ann));
    fann_save(ann, netName);
    printf("Epochs     %8d. MSE: %.5f. Fail bit: %d\n", epochs, fann_get_MSE(ann), fann_get_bit_fail(ann));
    return 0;
}

int main()
{
    // Включаем поддержку русских символов в консоли
	setlocale(LC_ALL, "Rus");
    cout << "Запуск программы для обучения нейронной сети Fann" << endl;
    cout << "Введите имя файла для обучения" << endl;
    while(1) {
        cin >> nameTrainFile;
        // проверяем наличие файла
        std::fstream fin(nameTrainFile, std::ios::binary | std::ios::in | std::ios::out);
        if(!fin.is_open()) {
            std::cout << "Ошибка! файл не был открыт. Возможно вы ввели неверное имя файла. Попробуйте еще раз!" << std::endl;
        } else {
            fin.close();
            break;
        }
    }

    cout << "Файл найден!" << endl;
    cout << "Введите имя файла для сохранения нейросети" << endl;
    cin >> nameNet;

    cout << "Читаем данные для обучения..." << endl;
    setlocale(LC_ALL,"English");
    struct fann_train_data* trainFannData = fann_read_train_from_file((const char*)nameTrainFile.c_str());

    const unsigned int num_input = fann_num_input_train_data(trainFannData);
    const unsigned int num_output = fann_num_output_train_data(trainFannData);
    setlocale(LC_ALL, "Rus");
    cout << "Число входов: " << num_input << " число выходов: " << num_output << endl;


    cout << "Введите минимальную ошибку" << endl;
    cin >> desired_error;

    cout << "Введите максимальное число эпох" << endl;
    cin >> max_epochs;

    const unsigned int epochs_between_reports = 1;

    unsigned int _hidden_layers;
    cout << "Введите число скрытых слоев" << endl;
    cin >> _hidden_layers;

    const unsigned int hidden_layers = _hidden_layers;

    unsigned int num_layers = 2 + hidden_layers;
    if(num_layers < 3) num_layers = 3;
    if(num_layers > 5) num_layers = 5;

    unsigned int num_neurons_hidden = fann_num_input_train_data(trainFannData);
    cout << "Введите число нейронов cкрытых слоев" << endl;
    cin >> num_neurons_hidden;

    cout << "Введите тип нейросети (0 - соединения от слоя к слою, 1 - каждый слой соединен с неронами всех предыдущих слоев)" << endl;
    int netType = 0;
    cin >> netType;
    // создание нейронной сети с именем ann
    struct fann *ann = NULL;
    if(netType == 0) {
        if(num_layers == 3) {
            ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
            //ann = fann_create_shortcut(num_layers, num_input, num_neurons_hidden, num_output);
        } else
        if(num_layers == 4) {
            ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_neurons_hidden, num_output);
            //ann = fann_create_shortcut(num_layers, num_input, num_neurons_hidden, num_neurons_hidden, num_output);
        } else
        if(num_layers == 5) {
            ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_neurons_hidden, num_neurons_hidden, num_output);
            //ann = fann_create_shortcut(num_layers, num_input, num_neurons_hidden, num_neurons_hidden, num_neurons_hidden, num_output);
        }
    } else
    if(netType == 1) {
        if(num_layers == 3) {
            //ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
            ann = fann_create_shortcut(num_layers, num_input, num_neurons_hidden, num_output);
        } else
        if(num_layers == 4) {
            //ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_neurons_hidden, num_output);
            ann = fann_create_shortcut(num_layers, num_input, num_neurons_hidden, num_neurons_hidden, num_output);
        } else
        if(num_layers == 5) {
            //ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_neurons_hidden, num_neurons_hidden, num_output);
            ann = fann_create_shortcut(num_layers, num_input, num_neurons_hidden, num_neurons_hidden, num_neurons_hidden, num_output);
        }
    }


    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    fann_set_callback(ann, test_callback);

    cout << "Начали обучение" << endl;
    setlocale(LC_ALL,"English");
    //обучение
    fann_train_on_data(ann, trainFannData, max_epochs, epochs_between_reports, desired_error);
    setlocale(LC_ALL, "Rus");
    cout << "Обучение закончено" << endl;
    cout << "Сохраняем сеть..." << endl;
    //сохранение обученной сети в файл
    setlocale(LC_ALL,"English");
    char netName[512] = {0};
    sprintf(netName, "%s.net", nameNet.c_str());
    fann_save(ann, netName);
    setlocale(LC_ALL, "Rus");
    cout << "Сеть сохранена" << endl;
    fann_destroy(ann);
    return 0;
}
