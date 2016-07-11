#include "main_game_window.h"
#include "ui_main_game_window.h"
// 游戏主界面
MainGameWindow::MainGameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainGameWindow)
{
    ui->setupUi(this);
}

MainGameWindow::~MainGameWindow()
{
    delete ui;
}
