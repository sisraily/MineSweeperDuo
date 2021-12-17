#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <qtimer.h>
#include <QDebug>
#include <QTime>
#include <random>
#include <square.h>
#include <QFont>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // UI status when game is first opened is set here.
    ui->forfeitButton->setDisabled(1);
    ui->groupBox2->setStyleSheet("border: 3px solid darkMagenta");
    ui->groupBox1->setStyleSheet("border: 3px solid darkCyan");

    // Starts the game.
    startNewGame();
}

/*
 * Generates a new game by creating new grid of cells composed of mines and empty squares.
 * also resets players scores.
*/
void MainWindow::startNewGame(){

    // Create a scene in our ui, where are game is played.
    QGraphicsView * player_1_view = ui->player_1_view;
    QGraphicsScene * player_1_scene = new QGraphicsScene;

    player_1_scene->setSceneRect(0,0,this->scene_width_,this->scene_height_);
    player_1_view->setScene(player_1_scene);

    // Generate empty squares on the board, at start.
    for (int i = 0; i<x_cells_; i++){
        for (int j = 0; j<y_cells_; j++){
            this->cells_[i][j] = square_factory->Create(QString("empty"),i,j);
            connect(cells_[i][j],&Square::triggerMine,this,&MainWindow::mineTriggeredSlot);
            connect(cells_[i][j],&Square::nextTurn,this,&MainWindow::nextTurnSlot);
            connect(cells_[i][j],&Square::searchForNearbyMines,this,&MainWindow::searchForNearbyMinesSlot);
            connect(cells_[i][j],&Square::flagSet,this,&MainWindow::flagSetSlot);
            player_1_scene->addItem(cells_[i][j]);
        }
    }


    // Generate mines randomly on board based on difficulty.
    for (int i = 0; i<x_cells_; i++){
        for (int j = 0; j<y_cells_; j++){

            // Rnadom number generator for placing mines in random squares.
            int randNum = getRandomNumber();

            // ***Add mine to square, based on difficulty.***
            if (randNum < this->difficulty_){
                // Sets a mine in square area.
                this->cells_[i][j]->setMine();

                // increase the count of nearby squares.
                for(int k=0;k<8;k++)
                {
                    int neighbor_x = i + neighborsX_[k];
                    int neighbor_y = j + neighborsY_[k];

                  /* if not edge cases */
                    if (!((neighbor_x < 0)||(neighbor_x == x_cells_)|| (neighbor_y < 0) || (neighbor_y == y_cells_))){
                        this->cells_[neighbor_x][neighbor_y]->incNearbyMineCount();
                   }

                }

            }

        }
    }
    this->set_blank_squares();
}


/*
 * Generates and returns a random number.
 * @return number integer between 0 and 100
*/
int MainWindow::getRandomNumber(){
    std::random_device rd;
    std::mt19937 mt(rd());
    const int min = 0;
    const int max = 100;
    std::uniform_int_distribution<int> ds(min, max);
    return ds(mt);
}


/*
 * Sets square that don't have mines nearby to blank
*/
void MainWindow::set_blank_squares(){
    for (int i = 0; i < x_cells_; i++){
        for (int j = 0; j < y_cells_; j++){
            if (this->cells_[i][j]->getNearbytMineCount() == 0){
                cells_[i][j]->setBlank();
            }

        }
    }
}


/*
 * Gets triggered when a mine square is cliked, and removes a certain amount of points
 * from the player who clicked it.
*/
void MainWindow::mineTriggeredSlot(Square *s){
    // Negative 10 points for triggering a mine.
    this->adjustPoints(this->points_mine_trigger_);
}



/*
 * Gets triggered when the next players turn is up.
 * Updates the counter, and updates the ui to highlight
 * which player's turn it is.
*/
void MainWindow::nextTurnSlot(){
    this->counter_ += 1;

     if (this->counter_%2 == 0){

         ui->groupBox1->setStyleSheet("border: 3px solid darkCyan");
         ui->groupBox2->setStyleSheet("border-color: gray;");

     }
     else {

         ui->groupBox2->setStyleSheet("border: 3px solid darkMagenta");
         ui->groupBox1->setStyleSheet("border-color: gray;");
     }

     update();
}


/*
 * Resets players' scores, and well as the turn counter to zero.
 * This function is called at the end of a game, to reset the score.
*/
void MainWindow::resetScores(){
    this->counter_ = 0;
    this->player1.points_counter = 0;
    this->player1.turn_count = 0;

    this->player2.points_counter = 0;
    this->player2.turn_count = 0;

    resetPointsLabels();
}


/*
 * Resets the point labels to show however many points the players currently have.
*/
void MainWindow::resetPointsLabels(){
    std::string p = "Total Points: " + std::to_string(player1.points_counter);
    QString qs(p.c_str());
    ui->pointsLabel1->setText(qs);

    std::string p2 = "Total Points: " + std::to_string(player2.points_counter);
    QString qs2(p2.c_str());
    ui->pointsLabel2->setText(qs2);
}


/*
 * Searches for all nearby squares that don't have mines.
 * Used to display the nearby mines when the user clicks on a blank square.
 *
 * @param Sqaure blank square the user clicked on during their turn.
*/
void MainWindow::searchForNearbyMinesSlot(Square *s){

        this->adjustPoints(this->points_empty_square_);


        while (true){
            int x_coord = s->get_x()/15;
            int y_coord = s->get_y()/15;

            //qDebug() << x_coord << y_coord;

            for(int k=0;k<8;k++)
            {
              int neighbor_x = x_coord + neighborsX_[k];
              int neighbor_y = y_coord + neighborsY_[k];

              // Avoid edge cases
              if (!((neighbor_x < 0)||(neighbor_x == x_cells_)|| (neighbor_y < 0) || (neighbor_y == y_cells_))){
                  if (cells_[neighbor_x][neighbor_y]->isBlank() && (!cells_[neighbor_x][neighbor_y]->getIsPressed())){
                        squares_to_show_.enqueue(cells_[neighbor_x][neighbor_y]);

                        cells_[neighbor_x][neighbor_y]->showBlank();
                        //searchForNearbyMinesSlot(cells_[neighbor_x][neighbor_y]);
                  }
                  else if ((!cells_[neighbor_x][neighbor_y]->isMine()) && (!cells_[neighbor_x][neighbor_y]->getIsPressed())){
                      cells_[neighbor_x][neighbor_y]->showCount();
                  }
              }
            }

            // continue iterating as long as the there is somthing in the queue.
            if(squares_to_show_.isEmpty()){
                break;
            }

            s = squares_to_show_.dequeue();
        }

}


/*
 * Called when flag is set on a mine.
 * Player will earn 10 points when they set a flag on a mined square.
*/
void MainWindow::flagSetSlot(){
    this->adjustPoints(10);
}


/*
 * Adjust users' points based on quantity.
 *
 * @param count the number of points to adjust a player's score by.
*/
void MainWindow::adjustPoints(int count){
    // Check whos turn it is.
    if (this->counter_%2 == 0){
        this->player1.points_counter+=count;
    }
    else {
        this->player2.points_counter+=count;  
    }

    resetPointsLabels();
}



/*
 * Slot to generate a new game, when new game button is clicked.
 *
 * Forfeit button is enabled, and new game button disabled, until a user forfeits or
 * the game ends.
*/
void MainWindow::on_newGameButton_clicked()
{
    ui->label_logo->hide();
    ui->groupBox2->setStyleSheet("border-color: gray;");
    ui->newGameButton->setDisabled(1);
    ui->forfeitButton->setDisabled(0);
    ui->winner_label->hide();
}


/*
 * Slot to generate new action based on a user forfeiting.
 *
 * The winner is calculated and displayed on the winner label.
 * The new game button is renabled, and the forfiet button is disabled, until a
 * new game has started.
*/
void MainWindow::on_forfeitButton_clicked()
{
    ui->label_logo->show();
    ui->newGameButton->setDisabled(0);
    ui->forfeitButton->setDisabled(1);


    std::string win = "Winner: ";
    if (counter_%2 != 0){
        win += "Player 1 ";
    }
    else {
        win += "Player 2 ";
    }

    QString qs(win.c_str());
    ui->winner_label->setText(qs);
    ui->winner_label->show();

    this->resetScores();
    this->startNewGame();
}


MainWindow::~MainWindow()
{
    delete ui;
}
