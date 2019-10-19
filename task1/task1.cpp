#include <iostream>
#include <vector>
#include <fstream>
#include <tuple>
#include <sstream>
#include <algorithm>
#include <string>

template <size_t I, class T>
class LineReader {
public:
    void read_line(T& tuple, std::istringstream& file) {
        LineReader<I - 1, T> line_reader;
        line_reader.read_line(tuple, file);
        file >> std::get<I>(tuple);
    }
};

template <class T>
class LineReader<0, T> {
public:
    void read_line(T& tuple, std::istringstream& file) {
        file >> std::get<0>(tuple);
    }
};

template <class... U>
class Reader {
public:
    std::vector<std::tuple<U...>> ReadCSV(const std::string& file_name) {
        std::ifstream file;
        file.open(file_name, std::ifstream::in);
        std::vector<std::tuple<U...>> array;

        LineReader<sizeof ...(U) - 1, std::tuple<U...>> line_reader;
        std::tuple<U...> temp_tuple;

        std::string line;
        while (std::getline(file, line)) {
            std::replace(line.begin(), line.end(), ',', ' ');
            auto line_stream = std::istringstream (line);
            line_reader.read_line(temp_tuple, line_stream);
            array.push_back(temp_tuple);
        }

        return array;
    }
};

int main() {
    Reader<int, int, float, std::string> reader;
    auto array = reader.ReadCSV("../input.csv");

    std::cout << "File: " << std::endl;
    for (auto& tuple: array) {
        std::cout << std::get<0>(tuple) << " " <<
                std::get<1>(tuple) << " " <<
                std::get<2>(tuple) << " " <<
                std::get<3>(tuple) << std::endl;
    }

    return 0;
}