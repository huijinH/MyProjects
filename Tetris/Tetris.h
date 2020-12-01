#pragma once

#include <QtWidgets/QWidget>
#include "ui_Tetris.h"
#include <QKeyEvent>
#include <QPaintEvent>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QLabel>
#include <QImage>

class Tetris : public QWidget
{
    Q_OBJECT

public:
	enum Blocks
	{
		ONE  = 1,  // ██
		TWO,      // ■■■■
		THREE,    // ▜▘ 
		FOUR,     // ▞
		FIVE      // ▙
	};
	enum Colors
	{
		RED = 1,  
		GREEN,     
		BLUE,    
		YELLOW,  
		GRAY
	};

    Tetris(QWidget *parent = Q_NULLPTR);
    void drawPix();
    void drawSingleBlock(QLabel* label, QString tip);
    void generateBlocks(Blocks shape, Colors c);
	void ifFull();

    void keyPressEvent(QKeyEvent* event);
    void paintEvent(QPaintEvent* event);

public slots:
	void timerSlot();
	void startBtnSlot();

private:
    Ui::TetrisClass ui;
	QTimer* timer;
    QPixmap* pix;
    QLabel* blockLabel[4];
	Blocks shp;
    int width, height, step, beginX;
    int blockArr[19][15] = { 0 };
	int flag[4] = { 0 };
};
