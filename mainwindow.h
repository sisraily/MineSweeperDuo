#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <square.h>
#include <QTimer>
#include <player.h>
#include <QQueue>
#include <squarefactory.h>

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
    int getRandomNumber();
    void startNewGame();
    void resetScores();

private slots:
    void mineTriggeredSlot(Square *s);
    void nextTurnSlot();
    void searchForNearbyMinesSlot(Square *s);
    void flagSetSlot();
    void resetPointsLabels();
    void on_newGameButton_clicked();
    void on_forfeitButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *cellsScene;
    QGraphicsScene *graphScene;

    static const int scene_width_ = 525;
    static const int scene_height_ = 375;


    static const int y_cells_ = ((scene_width_/15));
    static const int x_cells_ = ((scene_width_/15));

    Square * cells_[x_cells_][y_cells_];

    int neighborsX_[8] = {1,-1,0,0,1,1,-1,-1};
    int neighborsY_[8] = {0,0,1,-1,1,-1,1,-1};

    // Keeps track of whos turn it is. Odd is player1 even is player2.
    int turn_count_ = 1;

    // Used to calculate percentage of population alive in a given turn
    int total_pop_ = y_cells_ * x_cells_;

    // Pct square that are mines, on average. A number between 1 and 100.
    int difficulty_ = 15;

    // keeps track how many turns in the game.
    int counter_ = 0;

    // creates two players {player id, points count, turn count}
    player player1 = {1,0,0};
    player player2 = {2,0,0};

    /** Points for various game options stored here **/
    int points_mine_trigger_ = -10;
    int points_empty_square_ = 1;

    // A queue used to display nearby squares to an empty square, on user click.
    QQueue<Square *> squares_to_show_;

    // Factory that generates new type of squares.
    SquareFactory * square_factory = new SquareFactory();
};
#endif // MAINWINDOW_H
