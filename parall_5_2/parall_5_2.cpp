// parall_5_2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//﻿
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <omp.h>

using namespace std;

int main() {
    setlocale(LC_ALL, "RUSSIAN");
    string filename = "text.txt";
    ifstream file(filename);
    string line;
    int n = 0;
    while (std::getline(file, line)) {
        ++n;
    }
    file.clear();
    file.seekg(0);
    int numThreads = omp_get_max_threads();
    std::cout << "Количество строк в файле: " << n << std::endl;
    std::cout << "Количество потоков для обработки: " << numThreads << std::endl;

    vector<string> lines(n);
#pragma omp parallel for num_threads(numThreads)
    for (int i = 0; i < n; ++i) {
        getline(file, lines[i]);
    }

    map<string, int> wordFrequency;
#pragma omp parallel for num_threads(numThreads) shared(wordFrequency) private(line)
    for (int i = 0; i < n; ++i) {
        line = lines[i];
        istringstream iss(line);
        string word;
        while (iss >> word) {
            word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
            if (!word.empty()) {
#pragma omp atomic
                ++wordFrequency[word];
            }
        }
    }
    for (const auto& pair : wordFrequency) {
        if (pair.second > 100)
            std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}



// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
