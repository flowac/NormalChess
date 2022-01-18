#ifndef _MAIN_LIB_H_
#define _MAIN_LIB_H_

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdint>

#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QPalette>
#include <QPushButton>
#include <QSizePolicy>
#include <QTabBar>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QIODevice>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageLogger>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QTextStream>
#include <QTimer>

#define BUF_80	80
#define BUF_200	200
#define FONT_SZ	24
#define LOG_SZ	18
#define TAB_H	(FONT_SZ + 4)
#define SPACE	5

typedef enum { NONE, SOLDIER, CANNON, CAR, HORSE, MINISTER, ADVISOR, GENERAL } PieceType;
typedef enum { MOKAY, MERROR, MCHECK, P1WON, P2WON, PDRAW } MoveStatus;
typedef struct {
	bool isRed;
	PieceType val;
} BoardInfo;
typedef struct {
	double div, h, r, w;
} SizeParam;

extern QLabel *turnPrompt;
extern QTextEdit *logArea;

void LOGF(QString str, int level = 0);

class Web : public QObject
{
  Q_OBJECT
  public:
	~Web();
	void get(QNetworkRequest req);
	void post(QNetworkRequest req, QByteArray data);
  signals:
	void read_ok(QString str);
  private slots: void _read(QNetworkReply *ret);
  private:
	QNetworkAccessManager *mgr = 0;
};

class Chess : public QGraphicsView
{
  Q_OBJECT
  public:
	Chess(QWidget *p);
	void draw();
	void reset(bool isRed);
  private:
	void addText(double xpos, double ypos, QString str, QColor col = Qt::white);
	void addText(double xpos, double ypos, const char *str, QColor col = Qt::white);
	void gameStatus(MoveStatus ms = MOKAY);
	void grid();
	MoveStatus mateCheck();
	void mousePressEvent(QMouseEvent *evt);
	void putPiece(PieceType val, int xpos, int ypos, QPen qp, QBrush qb);
	bool selectPiece(QPoint pos);
	MoveStatus validMove(int x1, int y1, int x2, int y2);
	bool inGame, isSelValid, p1Red, p1Turn;
	int p1Gen[2], p2Gen[2];
	BoardInfo binfo[9][10];
	QBrush bred, bgray, bblack, bwhite;
	QGraphicsScene scn;
	QPen pred, pwhite, pgray, pgreen, pblack;
	SizeParam sp;
};

class App : public QWidget
{
  Q_OBJECT
  public:
	App();
	void resizeEvent(QResizeEvent *evt);
  private slots: void _new_game();
	void _quit();
	void _time();
  private:
	Chess *cview;
	QLabel *syst, *tick;
	QTextEdit *clog;
};
#endif
