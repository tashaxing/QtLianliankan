#ifndef MAIN_GAME_WINDOW_H
#define MAIN_GAME_WINDOW_H

#include <QMainWindow>

namespace Ui {
class MainGameWindow;
}

class MainGameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainGameWindow(QWidget *parent = 0);
    ~MainGameWindow();

private:
    Ui::MainGameWindow *ui;
};

#endif // MAIN_GAME_WINDOW_H
