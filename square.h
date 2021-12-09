#ifndef SQUARE_H
#define SQUARE_H

#include <QColor>
#include <QGraphicsItem>
#include <QString>


class Square : public QObject, public QGraphicsItem {

    // For emitting signals
    Q_OBJECT

    public:
        Square(const int x,const int y);
        bool isMine(){return this->is_mine_;}
        void setMine(){this->is_mine_ = 1;}
        void setFlag(){
            this->is_flag_ = true;
            this->is_pressed_ = true;
        }
        void removeFlag(){
            this->is_flag_ = false;
            this->is_pressed_ = false;
        }
        int get_x() const { return (x_); }  // inline member function
        int get_y() const { return (y_); }  // inline member function
        void showCount();
        void incNearbyMineCount(){this->nearby_mines_+=1;}
        int getNearbytMineCount() const {return nearby_mines_;}
        bool getIsPressed(){return is_pressed_;}
        bool getIsFlag(){return is_flag_;}
        // sets the square as a blank square, meaning there are no nearby mines.
        void setBlank(){this->is_blank_ = true; }
        bool isBlank(){return this->is_blank_;}
        void showBlank(){
            this->square_type_ = ":/images/emptypressed.png";
            this->is_pressed_ = true;
            update();
        }
        QRectF boundingRect() const override;
        QPainterPath shape() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    signals:
        void triggerMine(Square *s);
        void searchForNearbyMines(Square *s);
        void nextTurn();
        void flagSet();

    private:
        // width and height of each square is stored here.
        int width_ = 15;
        int height_ = 15;

        int x_;
        int y_;
        // 0 no mine, 1 mine.
        bool is_mine_ = 0;

        // If none of the nearby squares are mines, the square is blank.
        bool is_blank_ = false;

        // Sets the square as pressed.
        bool is_pressed_ = false;

        bool is_flag_ = false;

        // Mines that are nearby the current square.
        int nearby_mines_ = 0;
        QColor color_;
        QString square_type_ = ":/images/emptySquare.png";

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};


//class MineSquare: public Square{
//    void setMine(){this->is_mine_ = 1;}
//private:
//    bool is_mine_ = 1;
//};



/*
class EmptySquare: public Square{
    public:
        MineSquare(int x, int y)
        {
            Super(x,y);
            this->setMine();
        }
     Private:
        // Calcules, sets, and displays nearby mines when user clicks a value.
        void nearbyMines();
};
*/

#endif // SQUARE_H
