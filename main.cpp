#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

struct Dictionary {
    std::vector<std::string> words;
    std::size_t currentPosition = 0;
    [[nodiscard]] std::string getCurrentWord() const {
        return words.at(currentPosition);
    }

    void nextWord() {
        currentPosition++;
    }
};

struct DictionaryLoader {
    virtual Dictionary loadDictionary(const std::string &name) = 0;
};

struct Outputter {
    virtual void printSymbol(char) = 0;
    virtual void highlightError(std::size_t position) = 0;
};

struct Inputter {
    virtual char getSymbol() = 0;
    virtual std::string getBuffer() = 0;
    virtual void clearBuffer() = 0;
    virtual bool endOfInput() const = 0;
};


// 1. Загрузка словаря
// 2. получить символ, получить по весь буфер
// 3. проверить, что слово корректно
// 4. вывести

struct Parser {           // В последствии + статистика по правильным буквам
    Dictionary dictionary;// Dictionary
    virtual bool isCorrect(const std::string &inputWord) const;
};


struct ConstInputter : Inputter {
    std::string data;
    std::string buffer;
    std::size_t cur = 0;
    ConstInputter(std::string d) : data(std::move(d)) {}

    char getSymbol() override {
        if (data[cur] != ' ') {
            buffer.push_back(data[cur]);
        }
        return data[cur++];
    }

    [[nodiscard]] bool endOfInput() const override {
        return cur == data.length();
    }

    std::string getBuffer() override {
        return buffer;
    }

    void clearBuffer() override {
        buffer.clear();
    }
};

struct SimpleParser {

    bool isCorrect(const std::string &inputWord, const std::string &dictionaryWord) const {
        return dictionaryWord == inputWord;
    }
};

struct FileDictionaryLoader : DictionaryLoader {
    Dictionary loadDictionary(const std::string &name) override {
        std::fstream dictionaryFile(name);
        std::string word;
        std::vector<std::string> dictionary;
        while (dictionaryFile >> word)
            dictionary.push_back(word);
        return {dictionary};
    }
};

struct ConsoleOutputter : Outputter {
    inline static const std::string ESCAPE_CODE = "\x1b";
    ConsoleOutputter() {
        std::cout << "\n"
                  << ESCAPE_CODE << "[1A";
    }
    std::size_t currentPosition = 0;
    void printSymbol(char i) override {
        std::cout << i;
        currentPosition++;
    }
    void highlightError(std::size_t position) override {
        moveCursorLeft(currentPosition - position);
        std::cout << ESCAPE_CODE << "[1B" << 'W';
        std::cout << ESCAPE_CODE << "[1A";
        moveCursorLeft(1);
        moveCursorRight(currentPosition - position);
    }

    void highlightPositions(std::size_t howMany) {
        moveCursorLeft(howMany);
        std::cout << ESCAPE_CODE << "[1B";
        for (int i = 0; i < howMany; i++)
            std::cout << 'W';
        std::cout << ESCAPE_CODE << "[1A";
    }
    ~ConsoleOutputter() {
        std::cout << ESCAPE_CODE << "[1B";
    }

private:
    void moveCursorLeft(std::size_t x) {
        std::cout << ESCAPE_CODE + "[" << x << "D";
    }
    void moveCursorRight(std::size_t x) const {
        std::cout << ESCAPE_CODE + "[" << x << "C";
    }
};


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./main <dictionary-name>" << std::endl;
        return 0;
    }

    Dictionary dictionary = FileDictionaryLoader().loadDictionary(argv[1]);
    ConstInputter inputter("Lorem ipsum dolor sit amet");
    ConsoleOutputter outputter;
    SimpleParser simpleParser;

    while (true) {
        char currentSymbol = inputter.getSymbol();
        if (currentSymbol == ' ' || inputter.endOfInput()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if (!simpleParser.isCorrect(inputter.getBuffer(), dictionary.getCurrentWord())) {
                outputter.highlightPositions(inputter.getBuffer().size());
            }
            dictionary.nextWord();
            inputter.clearBuffer();
        }
        outputter.printSymbol(currentSymbol);
        std::cout.flush();
        if (inputter.endOfInput()) {
            break;
        }
    }
}
