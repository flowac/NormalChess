#include "lib.h"

QLabel *turnPrompt = NULL;
QTextEdit *logArea = NULL;

App::App()
{
	QBrush brush(QColor(0, 0, 0), Qt::SolidPattern);
	QPalette cback = QPalette(QPalette::Window, Qt::black);
	QSizePolicy szpol1, szpol1e, szpol2;
	QString base, fontsize, logsize;
	QString css1, css2, css3, css4;
	QTimer *systimer = new QTimer(this);

	QPushButton *but1, *but2;
	QWidget *hbox1, *vbox1;
	QHBoxLayout *hlay1, *hlay2;
	QVBoxLayout *vlay1;

	base = "red";
	fontsize = QString::number(FONT_SZ);
	logsize = QString::number(LOG_SZ);
	css1 = "background: black; border:1px solid " + base + "; color: " + base + "; font-size: " + fontsize + "px;";     //button
	css2 = "background: black; border:1px solid purple; color: " + base + "; font-size: " + fontsize + "px;";           //line edit
	css3 = "background: black; border:0px; color: " + base + "; font-size: " + fontsize + "px;";                        //label
	css4 = "background: black; border:0px; color: " + base + "; font-family: monospace; font-size: " + logsize + "px;"; //text area
	szpol1.setHorizontalPolicy(QSizePolicy::Expanding);
	szpol1e.setHorizontalPolicy(QSizePolicy::Expanding);
	szpol2.setHorizontalPolicy(QSizePolicy::Expanding);
	szpol1e.setVerticalPolicy(QSizePolicy::Expanding);
	szpol2.setVerticalPolicy(QSizePolicy::Expanding);

	hlay2 = new QHBoxLayout(this);
	vbox1 = new QWidget(this);
	vlay1 = new QVBoxLayout(vbox1);
	hbox1 = new QWidget(vbox1);
	hlay1 = new QHBoxLayout(hbox1);
	syst = new QLabel(QDateTime::currentDateTime().toString(), vbox1);
	tick = turnPrompt = new QLabel(vbox1);
	but1 = new QPushButton("Exit", hbox1);
	but2 = new QPushButton("New Game", hbox1);
	clog = logArea = new QTextEdit(vbox1);
	cview = new Chess(this);

	hlay1->setContentsMargins(0, 0, 0, 0);
	hlay2->setContentsMargins(0, 0, 0, 0);
	vlay1->setContentsMargins(0, 0, 0, 0);
	hlay1->setSpacing(SPACE);
	hlay2->setSpacing(0);
	vlay1->setSpacing(0);

//  hbox1->setFixedHeight(TAB_H);
//  but1->setFixedHeight();
//  but2->setFixedHeight();
//  syst->setFixedHeight(FONT_SZ + 2);
//  tick->setFixedHeight(FONT_SZ + 2);
	syst->setAlignment(Qt::AlignLeft);
	tick->setAlignment(Qt::AlignLeft);

	but1->setSizePolicy(szpol1);
	but2->setSizePolicy(szpol1);
	clog->setSizePolicy(szpol1e);
	cview->setSizePolicy(szpol2);
	syst->setSizePolicy(szpol1);
	tick->setSizePolicy(szpol1);

	but1->setStyleSheet(css1);
	but2->setStyleSheet(css1);
	clog->setStyleSheet(css4);
	cview->setStyleSheet(css3);
	syst->setStyleSheet(css3);
	tick->setStyleSheet(css3);

	hlay1->addWidget(but1);
	hlay1->addWidget(but2);
	hbox1->setLayout(hlay1);

	vlay1->addWidget(syst);
	vlay1->addWidget(tick);
	vlay1->addWidget(hbox1);
	vlay1->addWidget(clog);
	vbox1->setLayout(vlay1);

	hlay2->addWidget(vbox1);
	hlay2->addWidget(cview);

	connect(but1, SIGNAL(released()), this, SLOT(_quit()));
	connect(but2, SIGNAL(released()), this, SLOT(_new_game()));
	connect(systimer, SIGNAL(timeout()), this, SLOT(_time()));
	systimer->start(1000);

	this->setPalette(cback);
	this->setWindowFlag(Qt::WindowMaximizeButtonHint);
	this->setLayout(hlay2);
	this->show();
	this->resize(960, 540);
}

void App::_new_game()
{
	bool isRed = true;

	clog->clear();
	cview->reset(isRed);
}

void App::_quit()
{
	LOGF(QString("quit"), -1);
	QApplication::quit();
}

void App::_time()
{
	syst->setText(QDateTime::currentDateTime().toString());
}

void App::resizeEvent(QResizeEvent * evt)
{
	cview->draw();
	QWidget::resizeEvent(evt);
}
