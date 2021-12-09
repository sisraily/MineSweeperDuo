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

    // Add both scenes to both views (player 1 and player 2).
    QGraphicsView * player_1_view = ui->player_1_view;
    QGraphicsScene * player_1_scene = new QGraphicsScene;
    // sets the scene to the view (0,0,width,height)
    player_1_scene->setSceneRect(0,0,this->scene_width_,this->scene_height_);
    player_1_view->setScene(player_1_scene);

    //QGraphicsView * player_2_view = ui->player_2_view;
    QGraphicsScene * player_2_scene = new QGraphicsScene;
    // sets the scene to the view (0,0,width,height)
    player_2_scene->setSceneRect(0,0,this->scene_width_,this->scene_height_);

    //player_2_view->setScene(player_2_scene);

    // Generate empty squares on the board, at start.
    for (int i = 0; i<x_cells_; i++){
        for (int j = 0; j<y_cells_; j++){
            Square * s = new Square(i, j);
            this->cells_[i][j] = s;
            connect(s,&Square::triggerMine,this,&MainWindow::mineTriggeredSlot);
            connect(s,&Square::nextTurn,this,&MainWindow::nextTurnSlot);
            connect(s,&Square::searchForNearbyMines,this,&MainWindow::searchForNearbyMinesSlot);
            connect(s,&Square::flagSet,this,&MainWindow::flagSetSlot);
            player_1_scene->addItem(s);
        }
    }


    // Generate mines randomly on board based on difficulty.
    for (int i = 0; i<x_cells_; i++){
        for (int j = 0; j<y_cells_; j++){
            // Rnadom number generator for placing mines in random squares.
            std::random_device rd;
            std::mt19937 mt(rd());
            const int min = 0;
            const int max = 100;
            std::uniform_int_distribution<int> ds(min, max);
            int result = ds(mt);

            // ***Add mine to square, based on difficulty.***
            if (result < this->difficulty_){
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

                //qDebug() << i << j;
            }

        }
    }
    this->set_blank_squares();



}


void MainWindow::set_blank_squares(){
    for (int i = 0; i < x_cells_; i++){
        for (int j = 0; j < y_cells_; j++){
            if (this->cells_[i][j]->getNearbytMineCount() == 0){
                cells_[i][j]->setBlank();
            }

        }
    }
}


void MainWindow::mineTriggeredSlot(Square *s){
    // Negative 10 points for triggering a mine.
    this->adjustPoints(this->points_mine_trigger_);
}



void MainWindow::nextTurnSlot(){
    this->counter_ += 1;

     if (this->counter_%2 == 0){

         ui->groupBox1->setStyleSheet("border: 1px solid blue");
         ui->groupBox2->setStyleSheet("border-color: gray;");

     }
     else {

         ui->groupBox2->setStyleSheet("border: 1px solid red");
         ui->groupBox1->setStyleSheet("border-color: gray;");
     }
     update();
}



void MainWindow::searchForNearbyMinesSlot(Square *s){

        this->adjustPoints(this->points_empty_square_);


        while (true){
            int x_coord = s->get_x()/15;
            int y_coord = s->get_y()/15;

            qDebug() << x_coord << y_coord;

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

void MainWindow::flagSetSlot(){
    this->adjustPoints(10);
}

void MainWindow::adjustPoints(int count){
    if (this->counter_%2 == 0){
        // count nearby mines for player1.
        this->player1.points_counter+=count;
        std::string p = "Total Points: " + std::to_string(player1.points_counter);
        QString qs(p.c_str());
        ui->pointsLabel1->setText(qs);
    }
    else {
        this->player2.points_counter+=count;
        std::string p = "Total Points: " + std::to_string(player2.points_counter);
        QString qs(p.c_str());
        ui->pointsLabel2->setText(qs);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

