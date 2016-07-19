#ifndef MAIN_GAME_WINDOW_H
#define MAIN_GAME_WINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include "game_model.h"

namespace Ui {
class MainGameWindow;
}

class MainGameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainGameWindow(QWidget *parent = 0);
    virtual ~MainGameWindow();

public:
    void initGame(); // 初始化游戏
private:
    Ui::MainGameWindow *ui;
    GameModel *game; // 游戏模型
    QPushButton *imageButton[MAX_ROW * MAX_COL]; // 图片button数组
    QTimer *gameTimer;

private slots:
    void onIconButtonClicked(); // icon点击到响应
    void gameTimerEvent();
};

#endif // MAIN_GAME_WINDOW_H
