
#include <string>
#include <vector>

struct ReadInput{
    std::vector<char> vecOfchrs;
    ReadInput(std::string filename);
    void getFileContent(std::string fileName);
    void printData() const;
    std::vector<char> get_input_vector() const;
};