#include <QApplication>
#include "MainWindow.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    std::cout << "Application started" << std::endl;

    MainWindow window;
    window.show();

    std::cout << "Main window shown" << std::endl;

    return QApplication::exec(); 
}

