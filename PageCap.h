
#pragma once

#include <QtWebKitWidgets>

class PageCap : public QObject
{
    Q_OBJECT

public:
    PageCap();
    void load(const QUrl &url, const QString &outputFileName);

signals:
	void finished();

private slots:
    void printProgress(int percent);
    void saveResult(bool ok);

private:
    QWebPage m_page;
    QString m_fileName;
    int m_percent;
};

