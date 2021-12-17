#ifndef SQUARE_H
#define SQUARE_H

#include <QColor>
#include <QGraphicsItem>
#include <QString>


class Square : public QObject, public QGraphicsItem {
    Q_OBJECT

    public:
        explicit Square(const int x,const int y);
        bool isMine(){return this->is_mine_;} // inline member function
        void setMine(){this->is_mine_ = 1;} // inline member function
        void incNearbyMineCount(){this->nearby_mines_+=1;}
        int getNearbytMineCount() const {return nearby_mines_;}
        bool getIsPressed(){return is_pressed_;}
        bool getIsFlag(){return is_flag_;}
        int get_x() const { return (x_); }  // inline member function
        int get_y() const { return (y_); }  // inline member function

        // sets the square as a blank square, meaning there are no nearby mines.
        void setBlank(){this->is_blank_ = true; }

        bool isBlank(){return this->is_blank_;}
        void showBlank();
        void showCount();
        void setFlag();
        void removeFlag();

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

        // coordinates of the square
        int x_;
        int y_;

        // 0 no mine, 1 mine.
        bool is_mine_ = 0;

        // If none of the nearby squares are mines, the square is blank.
        bool is_blank_ = false;

        // Sets the square as pressed, when a user clicks on it.
        bool is_pressed_ = false;

        bool is_flag_ = false;

        // Count of mines that are nearby this square.
        int nearby_mines_ = 0;

        QColor color_;

        // Name of image file for the square type.
        QString square_type_ = ":/images/emptySquare.png";

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};


/*
 * Inheritance applied here. Blueprint for a square thats a mine.
 */
class MineSquare: public Square{
    public:
        explicit MineSquare(const int x,const int y) : Square(x,y){}
        void setMine(){this->is_mine_ = 1;}
    private:
        bool is_mine_ = 1;
        QString hidden_square_type_ = ":/images/mine3.png";

};


/*
 * Inheritance applied here. Blueprint for an empty square, one that is not a mine.
 */
class EmptySquare: public Square{
    public:
        explicit EmptySquare(const int x,const int y) : Square(x,y){}
    private:
        bool is_mine_ = 0;
         QString hidden_square_type_ = ":/images/emptypressed.png";
};


#endif // SQUARE_H
