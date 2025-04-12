/**
 * Лабораторная работа 3
 *
 * Вариант 14
 * Написать программу, которая будет считывать данные из предварительно
 * созданного программистом текстового файла. Файл не должен быть пустым.
 * Сделать отчет по программе.
 *
 * Написать программу, которая, игнорируя исходное деление файла на строки,
 * переформатирует его, разбивая строки так, чтобы каждая строка оканчивалась
 * точкой, либо содержала ровно 60 литер, если среди них нет точки. Результат
 * записать в другой файл.
 */

#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <string_view>

using namespace std;

constexpr const char *RED = "\033[31m";
constexpr const char *GREEN = "\033[32m";
constexpr const char *BLUE = "\033[34m";
constexpr const char *CYAN = "\033[36m";
constexpr const char *RESET = "\033[0m";

constexpr int MAX_LINE_LENGTH = 60;
constexpr string_view INPUT_FILE_NAME_DEFAULT = "input.txt";
constexpr string_view OUTPUT_FILE_NAME_DEFAULT = "output.txt";

class FileProcessing {
   private:
    ifstream _inputFile;
    ofstream _outputFile;
    string inputFileName;
    string outputFileName;
    filesystem::path currPath;

   public:
    FileProcessing(const string &_inputFileName, const string &_outputFileName,
                   const filesystem::path &_currPath = "") {
        inputFileName = _inputFileName;
        outputFileName = _outputFileName;

        currPath = _currPath.empty() ? filesystem::current_path() : _currPath;

        filesystem::path inputFileFullPath = currPath / inputFileName;
        filesystem::path outputFileFullPath = currPath / outputFileName;

        _inputFile.open(inputFileFullPath);
        _outputFile.open(outputFileFullPath);

        if (!_inputFile) {
            cerr << "Не удалось открыть входной файл " << inputFileName;
            system("pause");
            exit(1);
        }

        if (!_outputFile) {
            cerr << "Не удалось открыть выходной файл " << outputFileName;
            system("pause");
            exit(1);
        }
    }

    void run() {
        string sourceLine, filteredLine, outputLine;

        while (!_inputFile.eof()) {
            getline(_inputFile, sourceLine);

            for (char c : sourceLine) {
                if (c != '\n' && c != '\r') {
                    filteredLine.push_back(c);
                }
            }
        }
        for (char c : filteredLine) {
            outputLine.push_back(c);

            if (c == '.' || outputLine.size() == MAX_LINE_LENGTH) {
                _outputFile << outputLine << "\n";
                outputLine.clear();
            }
        }

        if (!outputLine.empty()) {
            _outputFile << outputLine << "\n";
            outputLine.clear();
        }
    }

    ~FileProcessing() {
        if (_inputFile.is_open()) {
            _inputFile.close();
        }

        if (_outputFile.is_open()) {
            _outputFile.close();
        }
    };
};

int main(void) {
    setlocale(LC_ALL, "Russian");

    string inputFileName, outputFileName, currPath;

    cout << "Введите абсолютный путь директории с файлами [Enter чтобы пропустить]: ";
    getline(cin, currPath);

    cout << "Введите название входного файла: ";
    getline(cin, inputFileName);

    cout << "Введите название выходного файла: ";
    getline(cin, outputFileName);

    if (!inputFileName.length()) {
        inputFileName = INPUT_FILE_NAME_DEFAULT;
    }

    if (!outputFileName.length()) {
        outputFileName = OUTPUT_FILE_NAME_DEFAULT;
    }

    FileProcessing fp(inputFileName, outputFileName, currPath);
    fp.run();

    cout << GREEN << "Переформатирование завершено\n" << RESET;

    return 0;
}
