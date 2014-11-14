
#include "PageCap.h"
#include <QSvgGenerator>
#include <QPrinter>
#include <iostream>

PageCap::PageCap(): QObject(), m_percent(0)
{
    connect(&m_page, SIGNAL(loadProgress(int)), this, SLOT(printProgress(int)));
    connect(&m_page, SIGNAL(loadFinished(bool)), this, SLOT(saveResult(bool)));
}

void PageCap::load(const QUrl &url, const QString &outputFileName)
{
    std::cout << "Loading " << qPrintable(url.toString()) << std::endl;
    m_percent = 0;
    int index = outputFileName.lastIndexOf('.');
    m_fileName = (index == -1) ? outputFileName + ".pdf" : outputFileName;
    m_page.mainFrame()->load(url);
    m_page.mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    m_page.mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    m_page.setViewportSize(QSize(1024, 768));
}

void PageCap::printProgress(int percent)
{
    if (m_percent >= percent)
        return;

    while (m_percent++ < percent)
        std::cout << "#" << std::flush;
}

void PageCap::saveResult(bool ok)
{
    std::cout << std::endl;

    if (!ok)
	{
        std::cerr << "Failed loading " << qPrintable(m_page.mainFrame()->url().toString()) << std::endl;
        emit finished();
        return;
    }

	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(m_fileName);


	auto frame = m_page.mainFrame();
	QWebElement art_ele;
	foreach (QWebElement element, frame->documentElement().findAll("div"))
	{
		if (element.attribute("class") == "span8")
		{
			art_ele = element;
			break;
		}
	}

	//开始将整个网页的内容打印到pdf
	int frame_width = frame->contentsSize().width();
	int frame_height = frame->contentsSize().height();
	int pdf_width = printer.width();
	int pdf_height = printer.height() -200;

	if (frame_width == 0)
	{
		return;
	}
	qreal scale = printer.width() / frame_width;

	QPainter painter(&printer);
	painter.scale(scale,scale);
	for (int i = 0; i < frame_height * scale / pdf_height; ++i)
	{
		frame->documentElement().render(&painter,QRect(0,pdf_height / scale * i, pdf_height / scale * (i + 1),frame_width));
		printer.newPage();
	}

	//将文章单独摘出来再打印一遍
	//懒得写了，不管分页和是否居中了
	QRect geo = art_ele.geometry();
	art_ele.render(&painter);

	painter.end();

    emit finished();
}
