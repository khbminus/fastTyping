#include "client/include/mainwindow.h"
#include "constGame.h"
#include <QApplication>
#include <memory>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}

//int main(int argc, char *argv[]) {
//    if (argc < 2) {
//        std::cout << "Usage: ./main <dictionary-name>" << std::endl;
//        return 0;
//    }
//
//    auto dictionaryPtr = FileDictionaryLoader().loadDictionary(argv[1]);
//    auto inputter = std::make_unique<ConstInputter>("Lorem ipsum dolor sit amet");
//    auto finishWordUseCase = FinishWordUseCase{std::move(dictionaryPtr),
//                                               std::make_unique<ConsoleOutputter>(),
//                                               std::move(inputter),
//                                               std::make_unique<SimpleParser>()};
//    finishWordUseCase.execute();
//}
