#ifndef MAIN_GAME_WINDOW_H
#define MAIN_GAME_WINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QTimer>
#include "game_model.h"

namespace Ui {
class MainGameWindow;
}

// 继承自button，存储坐标值
struct IconButton : QPushButton
{
public:
    IconButton(QWidget *parent = Q_NULLPTR) :
        QPushButton(parent),
        xID(-1),
        yID(-1)
    {
    }
    int xID;
    int yID;
};

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
    IconButton *imageButton[MAX_ROW * MAX_COL]; // 图片button数组
    QTimer *gameTimer; // 游戏计时器
    IconButton *preIcon, *curIcon; // 记录点击的icon

private slots:
    void onIconButtonPressed(); // icon点击到响应
    void gameTimerEvent();
};

#endif // MAIN_GAME_WINDOW_H
