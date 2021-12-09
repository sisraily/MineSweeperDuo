#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <square.h>
#include <QTimer>
#include <player.h>
#include <QQueue>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void set_blank_squares();
    ~MainWindow();
    void adjustPoints(int count);

private slots:
    void mineTriggeredSlot(Square *s);
    void nextTurnSlot();
    void searchForNearbyMinesSlot(Square *s);
    void flagSetSlot();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *cellsScene;
    QGraphicsScene *graphScene;

    static const int scene_width_ = 525;
    static const int scene_height_ = 375;


    // Cells view will have a height of 20 cells and 10 rows of cells (200 total).
    // y coordinates
    static const int y_cells_ = ((scene_width_/15));
    // x coordinates
    static const int x_cells_ = ((scene_width_/15));

    Square * cells_[x_cells_][y_cells_];

    int neighborsX_[8] = {1,-1,0,0,1,1,-1,-1};
    int neighborsY_[8] = {0,0,1,-1,1,-1,1,-1};

    // Keeps track of whos turn it is. Odd is player1 even is player2.
    int turn_count_ = 1;

    // Used to calculate percentage of population alive in a given turn
    int total_pop_ = y_cells_ * x_cells_;

    QTimer *timer = new QTimer(this);

    // Pct square that are mines, on average. A number between 1 and 100.
    int difficulty_ = 15;

    // keeps track how many turns in the game.
    int counter_ = 0;

    // creates two players {player id, points count, turn count}
    player player1 = {1,0,0};
    player player2 = {2,0,0};

    // points for triggering a mine
    int points_mine_trigger_ = -10;

    int points_empty_square_ = 1;
    // queue of empty squares near the ones the user clicks.
    QQueue<Square *> squares_to_show_;
};


#endif // MAINWINDOW_H
