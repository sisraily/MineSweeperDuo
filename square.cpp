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

  // set initial color:
  QColor c(128, 128, 128);
  this->x_ = x*width_;
  this->y_ = y*height_;
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
        if (!this->is_pressed_){
            this->square_type_ = ":/images/flag2.png";
            this->setFlag();

            if (this->isMine()){
                emit flagSet();
                this->squareClicked();
            }
        }
        else if (this->getIsFlag() && !this->isMine()){
            this->square_type_ = ":/images/emptySquare.png";
            this->removeFlag();
        }

    }
    else if (event->button() == Qt::LeftButton){

        // check if mine present.
        if (!this->is_pressed_){
            if (this->isMine()){

                this->squareClicked();

                // Currently the first turn can land on a mine, to change this, check if its first turn here.

                // Decrease points by 10 here.
                // emit steped on mine.
                this->square_type_ = ":/images/mine3.png";
                emit triggerMine(this);
            }
            else {
                this->square_type_ = ":/images/emptypressed.png";
                this->squareClicked();
                emit searchForNearbyMines(this);
            }
        }
    }
    update();


    // emits a signal to tell slots that the next turn is up.
    emit nextTurn();
}


/*
 * Adds a flag to a square.
 */
void Square::setFlag(){
    this->is_flag_ = true;
    this->squareClicked();
}


/*
 * Removes a flag the user placed that was not on a mine.
 */
void Square::removeFlag(){
    this->is_flag_ = false;
    this->is_pressed_ = false;
    emit flagRemoved();
}


/*
 * Shows a pressed button, when the user clicks an empty button.
 */
void Square::showBlank(){
    this->square_type_ = ":/images/emptypressed.png";
    this->is_pressed_ = true;
    update();
}


/*
 *
 */
void Square::squareClicked(){
    this->is_pressed_ = true;
    //this->total_squares_clicked_ += 1;
}
