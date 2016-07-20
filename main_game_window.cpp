#include <QDebug>
#include "main_game_window.h"
#include "ui_main_game_window.h"

// --------- 全局变量 --------- //
const int kIconSize = 36;
const int kTopMargin = 70;
const int kLeftMargin = 50;
const int kIconMargin = 5;
// -------------------------- //

// 游戏主界面
MainGameWindow::MainGameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainGameWindow)
{
    ui->setupUi(this);
//    setFixedSize(kLeftMargin * 2 + (kIconMargin + kIconSize) * MAX_COL, kTopMargin + (kIconMargin + kIconSize) * MAX_ROW);
    // 初始化游戏
    initGame();
}

MainGameWindow::~MainGameWindow()
{
    delete game;
    delete ui;
}

void MainGameWindow::initGame()
{
    // 启动游戏
    game = new GameModel;
    game->startGame();

    // 添加button
    for(int i = 0; i < MAX_ROW * MAX_COL; i++)
    {
        imageButton[i] = new QPushButton(this);
        imageButton[i]->setGeometry(kLeftMargin + (i % MAX_COL) * kIconSize, kTopMargin + (i / MAX_COL) * kIconSize, kIconSize, kIconSize);
        QPixmap iconPix;
        QString fileString;
        fileString.sprintf(":/res/image/%d.png", game->getGameMap()[i]);
        iconPix.load(fileString);
        QIcon icon(iconPix);
        imageButton[i]->setIcon(icon);
        imageButton[i]->setIconSize(QSize(kIconSize, kIconSize));
        imageButton[i]->show();
//        imageButton[i]->setPalette(QPalette(Qt::lightGray));
        connect(imageButton[i], SIGNAL(pressed()), this, SLOT(onIconButtonPressed()));
    }

    // 进度条
    ui->timeBar->setMaximum(100);
    ui->timeBar->setMinimum(0);
    ui->timeBar->setValue(100);

    // 启动计时器
    gameTimer = new QTimer(this);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(gameTimerEvent()));
    gameTimer->start(300);
}

void MainGameWindow::onIconButtonPressed()
{
    QPushButton *iconButton = dynamic_cast<QPushButton *>(sender());
    iconButton->setStyleSheet("background-color: rgba(255, 255, 12, 161)");

    qDebug() << "icon pressed";
}

void MainGameWindow::gameTimerEvent()
{
    if(ui->timeBar->value() == 0)
    {
        qDebug() << "game over";
        gameTimer->stop();
    }
    else
    {
        ui->timeBar->setValue(ui->timeBar->value() - 1);
    }


}
