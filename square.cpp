#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>
#include "square.h"
#include <QPixmap>

/**
  Creates a new square object at these x, and y coordinates
  @param x int x coordinate
  @param y int y coordinate
*/
Square::Square(const int x,const int y) {
  x_ = x*width_;
  y_ = y*height_;


  // set initial color:
  QColor c(128, 128, 128);
  this->color_ = c;
}


// Location of the square. A rectangle.
QRectF Square::boundingRect() const
{
    return QRectF(x_, y_, width_, height_);
}

// Defines the shape of the cell.
QPainterPath Square::shape() const
{
    QPainterPath path;
    path.addRect(x_, y_, width_, height_);
    return path;
}


// Displays the cell
void Square::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();

    // square icon image.
    QPixmap pm(square_type_);
    painter->drawPixmap(this->x_,this->y_,this->width_,this->height_,pm);
    painter->setBrush(b);
}



void Square::showCount(){

    QString mc = QString::number(this->getNearbytMineCount());
    this->square_type_ = ":/images/" + mc + ".png";
    this->is_pressed_  = true;
    update();
}


void Square::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button() == Qt::RightButton){
        // set flags
        qDebug() << "Sets flags";
        if (!this->is_pressed_){
            this->square_type_ = ":/images/flag2.png";
            this->setFlag();

            if (this->isMine()){
               flagSet();
            }
        }
        else if (this->getIsFlag() && !this->isMine()){
            this->square_type_ = ":/images/emptySquare.png";
            this->removeFlag();
        }

    }
    else if (event->button() == Qt::LeftButton){
        // check if its first turn
        // check if mine present.
        qDebug() << "Mine present: ";
        if (!this->is_pressed_){
            if (this->isMine()){
                this->is_pressed_ = true;
                qDebug() << "Yes!";
                // Decrease points by 10 here.
                // emit steped on mine.
                this->square_type_ = ":/images/mine3.png";
                triggerMine(this);
            }
            else {
                qDebug() << "No!";
                this->square_type_ = ":/images/emptypressed.png";
                this->is_pressed_ = true;
                searchForNearbyMines(this);
                // Increase points by 1 here.
                // emit continue.
            }
        }

    }
    update();

    // emits a signal to tell slots that the next turn is up.
    nextTurn();
}
