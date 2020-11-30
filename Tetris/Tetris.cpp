#include "Tetris.h"
#include <QDebug>

Tetris::Tetris(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    width = ui.gameWidget->width();
    height = ui.gameWidget->height();
    step = 40;
    beginX = 240;
    pix = new QPixmap(width, height);

	for (int i = 0; i < 4; i++)
	{
		blockLabel[i] = new QLabel(ui.gameWidget);
		blockLabel[i]->setFixedSize(step, step);
        blockLabel[i]->setFocus();
	}
    shp = ONE;
    generateBlocks(shp, BLUE);
    drawPix();
    startTimer(1000);
}

void Tetris::drawPix()
{
    pix->fill(Qt::black);
    QPainter* painter = new QPainter;
    QPen pen(Qt::DotLine);
    for (int i = step; i < width; i += step)
    {
        painter->begin(pix);
        pen.setColor(Qt::yellow);
        painter->setPen(pen);
        painter->drawLine(QPoint(i, 0), QPoint(i, height));
        painter->end();
    }
    for (int i = step; i < height; i += step)
    {
        painter->begin(pix);
		pen.setColor(Qt::yellow);
        painter->setPen(pen);
        painter->drawLine(QPoint(0, i), QPoint(width, i));
        painter->end();
    }
}

void Tetris::drawSingleBlock(QLabel* label, QString tip)
{
    QPainter* painter = new QPainter;
    QPixmap* tmpPix = new QPixmap(step, step);

    if (tip == "red")
		tmpPix->fill(Qt::red);
    else if (tip == "blue")
		tmpPix->fill(Qt::blue);
    else if (tip == "yellow")
		tmpPix->fill(Qt::yellow);
    else if (tip == "green")
		tmpPix->fill(Qt::green);
    else if (tip == "gray")
		tmpPix->fill(Qt::gray);

    QPen pen;
    pen.setColor(Qt::red);
    painter->setPen(pen);
    int x, y;
    x = label->x();
    y = label->y();
    painter->begin(pix);
    painter->drawPixmap(x, y, *tmpPix);
    painter->end();
    update();
}

void Tetris::generateBlocks(Blocks shape, Colors c)
{
    shp = shape;
    switch (shape)
    {
    case ONE:    // ██
        blockLabel[0]->move(beginX, 0);
        blockLabel[1]->move(beginX + step, 0);
		blockLabel[2]->move(beginX, step);
		blockLabel[3]->move(beginX + step, step);
        break;
    case TWO:    // ■■■■
		blockLabel[0]->move(beginX, 0);
		blockLabel[1]->move(beginX + step, 0);
		blockLabel[2]->move(beginX + step + step, 0);
		blockLabel[3]->move(beginX + step + step + step, 0);
        break;
    case THREE:  // ▜▘
		blockLabel[0]->move(beginX, 0);
		blockLabel[1]->move(beginX - step, step);
		blockLabel[2]->move(beginX, step);
		blockLabel[3]->move(beginX + step, step);
        break;
    case FOUR:   // ▞
		blockLabel[0]->move(beginX, 0);
		blockLabel[1]->move(beginX + step, 0);
		blockLabel[2]->move(beginX + step, step);
		blockLabel[3]->move(beginX + step + step, step);
        break;
    case FIVE:   // ▙
		blockLabel[0]->move(beginX, 0);
		blockLabel[1]->move(beginX, step);
		blockLabel[2]->move(beginX + step, step);
		blockLabel[3]->move(beginX + step + step, step);
        break;
    }

	switch (c)
	{
	case RED:    
        for (int i = 0; i < 4; i++)
        {
            blockLabel[i]->setStyleSheet("QLabel{background-color:red;}");
            blockLabel[i]->setToolTip("red");
        }
		break;
	case GREEN:    
        for (int i = 0; i < 4; i++)
        {
            blockLabel[i]->setStyleSheet("QLabel{background-color:green;}");
            blockLabel[i]->setToolTip("green");
        }
		break;
	case BLUE:  
        for (int i = 0; i < 4; i++)
        {
            blockLabel[i]->setStyleSheet("QLabel{background-color:blue;}");
            blockLabel[i]->setToolTip("blue");
        }
		break;
	case YELLOW:  
        for (int i = 0; i < 4; i++)
        {
            blockLabel[i]->setStyleSheet("QLabel{background-color:yellow;}");
            blockLabel[i]->setToolTip("yellow");
        }
		break;
	case GRAY:   
        for (int i = 0; i < 4; i++)
        {
            blockLabel[i]->setStyleSheet("QLabel{background-color:gray;}");
            blockLabel[i]->setToolTip("gray");
        }
		break;
	}
}

void Tetris::timerEvent(QTimerEvent* event)
{
    int maxY = qMax(qMax(blockLabel[0]->y(), blockLabel[1]->y()), 
                            qMax(blockLabel[2]->y(), blockLabel[3]->y()));
    if (blockArr[(blockLabel[0]->y() + step) / step][(blockLabel[0]->x()) / step] == 1 ||
        blockArr[(blockLabel[1]->y() + step) / step][(blockLabel[1]->x()) / step] == 1 || 
        blockArr[(blockLabel[2]->y() + step) / step][(blockLabel[2]->x()) / step] == 1 || 
        blockArr[(blockLabel[3]->y() + step) / step][(blockLabel[3]->x()) / step] == 1 || 
        maxY == height - step)
    {
        for (int i = 0; i < 4; i++)
        {
            drawSingleBlock(blockLabel[i], blockLabel[i]->toolTip());
            blockArr[(blockLabel[i]->y()) / step][blockLabel[i]->x() / step] = 1;
            flag[i] = 0;
        }

		int shape = qrand() % 5 + 1;
        int c = qrand() % 5 + 1;
		generateBlocks(Blocks(shape), Colors(c));
        return;
    }

	for (int i = 0; i < 4; i++)
        blockLabel[i]->move(blockLabel[i]->x(), blockLabel[i]->y() + step);
}

void Tetris::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Left)
    {
		int minX = qMin(qMin(blockLabel[0]->x(), blockLabel[1]->x()),
            qMin(blockLabel[2]->x(), blockLabel[3]->x()));
        if (minX > 0 && 
		        (blockArr[(blockLabel[0]->y()) / step][(blockLabel[0]->x() - step) / step] != 1 &&
			     blockArr[(blockLabel[1]->y()) / step][(blockLabel[1]->x() - step) / step] != 1 &&
			     blockArr[(blockLabel[2]->y()) / step][(blockLabel[2]->x() - step) / step] != 1 &&
			     blockArr[(blockLabel[3]->y()) / step][(blockLabel[3]->x() - step) / step] != 1
                )
            )
        {
            for (int i = 0; i < 4; i++)
                blockLabel[i]->move(blockLabel[i]->x() - step, blockLabel[i]->y());
        }
    }
    else if (event->key() == Qt::Key_Right)
    {
		int maxX = qMax(qMax(blockLabel[0]->x(), blockLabel[1]->x()),
            qMax(blockLabel[2]->x(), blockLabel[3]->x()));
		if ((maxX < width - step) &&
			   (blockArr[(blockLabel[0]->y()) / step][(blockLabel[0]->x() + step) / step] != 1 &&
				blockArr[(blockLabel[1]->y()) / step][(blockLabel[1]->x() + step) / step] != 1 &&
				blockArr[(blockLabel[2]->y()) / step][(blockLabel[2]->x() + step) / step] != 1 &&
				blockArr[(blockLabel[3]->y()) / step][(blockLabel[3]->x() + step) / step] != 1
				)
            )
        {
            for (int i = 0; i < 4; i++)
                blockLabel[i]->move(blockLabel[i]->x() + step, blockLabel[i]->y());
        }
    }
	else if (event->key() == Qt::Key_Up)
	{
        switch (shp)
        {
        case ONE:   // ██
            break;
        case TWO:   // ■■■■
            if (flag[0] == 0)
            {
				blockLabel[0]->move(blockLabel[0]->x() + step, blockLabel[0]->y() - step);
				blockLabel[2]->move(blockLabel[2]->x() - step, blockLabel[2]->y() + step);
				blockLabel[3]->move(blockLabel[3]->x() - step * 2, blockLabel[3]->y() + step * 2);
                flag[0] = 1;
            }
            else if (flag[0] == 1)
            {
				blockLabel[0]->move(blockLabel[0]->x() - step, blockLabel[0]->y() + step);
				blockLabel[2]->move(blockLabel[2]->x() + step, blockLabel[2]->y() - step);
				blockLabel[3]->move(blockLabel[3]->x() + step * 2, blockLabel[3]->y() - step * 2);
                flag[0] = 0;
            }
            break;
        case THREE: // ▜▘
			if (flag[1] == 0)
			{
				blockLabel[3]->move(blockLabel[3]->x() - step, blockLabel[3]->y() + step);
				flag[1] = 1;
			}
			else if (flag[1] == 1)
			{
				blockLabel[0]->move(blockLabel[0]->x() + step, blockLabel[0]->y() + step);
				flag[1] = 2;
			}
            else if (flag[1] == 2)
            {
                blockLabel[1]->move(blockLabel[1]->x() + step, blockLabel[1]->y() - step);
                flag[1] = 3;
            }
            else if (flag[1] == 3)
            {
                blockLabel[0]->move(blockLabel[0]->x() - step, blockLabel[0]->y() - step);
                blockLabel[1]->move(blockLabel[1]->x() - step, blockLabel[1]->y() + step);
                blockLabel[3]->move(blockLabel[3]->x() + step, blockLabel[3]->y() - step);
                flag[1] = 0;
            }
            break;
        case FOUR:  // ▞
			if (flag[2] == 0)
			{
				blockLabel[0]->move(blockLabel[0]->x() + step, blockLabel[0]->y() - step);
				blockLabel[1]->move(blockLabel[1]->x(), blockLabel[1]->y());
                blockLabel[2]->move(blockLabel[2]->x() - step, blockLabel[2]->y() - step);
				blockLabel[3]->move(blockLabel[3]->x() - step * 2, blockLabel[3]->y());
				flag[2] = 1;
			}
			else if (flag[2] == 1)
			{
				blockLabel[0]->move(blockLabel[0]->x() - step, blockLabel[0]->y() + step);
				blockLabel[2]->move(blockLabel[2]->x() + step, blockLabel[2]->y() + step);
				blockLabel[3]->move(blockLabel[3]->x() + step * 2, blockLabel[3]->y());
				flag[2] = 0;
			}
            break;
        case FIVE:  // ▙
			if (flag[3] == 0)
			{
				blockLabel[0]->move(blockLabel[0]->x() + step, blockLabel[0]->y());
				blockLabel[1]->move(blockLabel[1]->x(), blockLabel[1]->y() - step);
                blockLabel[2]->move(blockLabel[2]->x() - step, blockLabel[2]->y());
				blockLabel[3]->move(blockLabel[3]->x() - step * 2, blockLabel[3]->y() + step);			
                flag[3] = 1;
			}
			else if (flag[3] == 1)
			{
				blockLabel[0]->move(blockLabel[0]->x(), blockLabel[0]->y() + step * 2);
				blockLabel[1]->move(blockLabel[1]->x() + step, blockLabel[1]->y() + step);
				blockLabel[3]->move(blockLabel[3]->x() - step, blockLabel[3]->y() - step);			
                flag[3] = 2;
			}
			else if (flag[3] == 2)
			{
				blockLabel[0]->move(blockLabel[0]->x() - step, blockLabel[0]->y());
				blockLabel[1]->move(blockLabel[1]->x(), blockLabel[1]->y() + step);
				blockLabel[2]->move(blockLabel[2]->x() + step, blockLabel[2]->y());
				blockLabel[3]->move(blockLabel[3]->x() + step * 2, blockLabel[3]->y() - step);	
                flag[3] = 3;
			}
			else if (flag[3] == 3)
			{
				blockLabel[0]->move(blockLabel[0]->x(), blockLabel[0]->y() - step);
				blockLabel[1]->move(blockLabel[1]->x() - step, blockLabel[1]->y());
				blockLabel[2]->move(blockLabel[2]->x(), blockLabel[2]->y() + step);
				blockLabel[3]->move(blockLabel[3]->x() + step, blockLabel[3]->y() + step * 2);
				flag[3] = 0;
			}
            break;
        }
	}
	else if (event->key() == Qt::Key_Down)
	{
        for (;;)
        {
            int maxY = qMax(qMax(blockLabel[0]->y(), blockLabel[1]->y()),
                qMax(blockLabel[2]->y(), blockLabel[3]->y()));
            if (blockArr[(blockLabel[0]->y() + step) / step][(blockLabel[0]->x()) / step] == 1 ||
                blockArr[(blockLabel[1]->y() + step) / step][(blockLabel[1]->x()) / step] == 1 ||
                blockArr[(blockLabel[2]->y() + step) / step][(blockLabel[2]->x()) / step] == 1 ||
				blockArr[(blockLabel[3]->y() + step) / step][(blockLabel[3]->x()) / step] == 1 ||
				maxY == height - step)
            {
                for (int i = 0; i < 4; i++)
                {
                    drawSingleBlock(blockLabel[i], blockLabel[i]->toolTip());
                    blockArr[(blockLabel[i]->y()) / step][blockLabel[i]->x() / step] = 1;
                    flag[i] = 0;
                }
                return;
            }

            for (int i = 0; i < 4; i++)
                blockLabel[i]->move(blockLabel[i]->x(), blockLabel[i]->y() + step);
        }
	}
}

void Tetris::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(QPoint(ui.gameWidget->x(), ui.gameWidget->y()), *pix);
    painter.end();
}
