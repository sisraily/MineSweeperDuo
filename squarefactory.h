#ifndef SQUARE_FACTORY_H
#define SQUARE_FACTORY_H

#include "square.h"
#include <QString>


class SquareFactory {
    public:
        /*
         *  Factory pattern used here.
         *  Creates two different pieces based on which one the user wants.
         *  Makes are game easily modifiable, by making it easy to add new types of pieces.
         */
        Square* Create(QString squareType, int x, int y) {
            if (squareType == "mine")  return new MineSquare(x,y);
            if (squareType == "empty") return new EmptySquare(x,y);
            return 0;
        }
};

#endif // squarefactory ends here.
