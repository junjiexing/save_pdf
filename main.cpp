
#include "PageCap.h"

#include <iostream>
#include <QtWidgets>

int main(int argc, char * argv[])
{
    if (argc != 3)
	{
        std::cout << "usage: PageCap <url> <save path>" << std::endl;
        return 0;
    }

    QUrl url = QUrl::fromUserInput(QString::fromLatin1(argv[1]));
    QString fileName = QString::fromLatin1(argv[2]);

    QApplication a(argc, argv);
    PageCap capture;
    QObject::connect(&capture, SIGNAL(finished()), QApplication::instance(), SLOT(quit()));
    capture.load(url, fileName);

    return a.exec();
}

