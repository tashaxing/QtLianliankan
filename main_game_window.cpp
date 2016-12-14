#include <QDebug>
#include <QSound>
#include <QMessageBox>
#include <QDir>
#include "main_game_window.h"
#include "ui_main_game_window.h"

// --------- 全局变量 --------- //
const int kIconSize = 36;
const int kTopMargin = 70;
const int kLeftMargin = 50;

const QString kIconReleasedStyle = "";
const QString kIconClickedStyle = "background-color: rgba(255, 255, 12, 161)";
// -------------------------- //

// 游戏主界面
MainGameWindow::MainGameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainGameWindow),
    preIcon(NULL),
    curIcon(NULL)
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
    game->startGame(MEDIUM);

    for (int i = 0; i < MAX_ROW * MAX_COL; i++)
        qDebug() << game->getGameMap()[i];

    // 添加button
    for(int i = 0; i < MAX_ROW * MAX_COL; i++)
    {
        if (game->getGameMap()[i])
        {
            // 有方块就创建button
            imageButton[i] = new IconButton(this);
            imageButton[i]->setGeometry(kLeftMargin + (i % MAX_COL) * kIconSize, kTopMargin + (i / MAX_COL) * kIconSize, kIconSize, kIconSize);
            // 设置索引
            imageButton[i]->xID = i % MAX_COL;
            imageButton[i]->yID = i / MAX_COL;
            // 设置图片
            QPixmap iconPix;
            QString fileString;
            fileString.sprintf(":/res/image/%d.png", game->getGameMap()[i]);
            iconPix.load(fileString);
            QIcon icon(iconPix);
            imageButton[i]->setIcon(icon);
            imageButton[i]->setIconSize(QSize(kIconSize, kIconSize));
            // 如果是空白的就隐藏
            imageButton[i]->show();
            connect(imageButton[i], SIGNAL(pressed()), this, SLOT(onIconButtonPressed()));

        }

    }

    // 进度条
    ui->timeBar->setMaximum(100);
    ui->timeBar->setMinimum(0);
    ui->timeBar->setValue(100);

    // 启动计时器
    gameTimer = new QTimer(this);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(gameTimerEvent()));
    gameTimer->start(300);

    // 播放背景音乐(QMediaPlayer只能播放绝对路径文件)
    audioPlayer = new QMediaPlayer(this);
    QString curDir = QCoreApplication::applicationDirPath(); // 这个api获取路径在不同系统下不一样,mac 下需要截取路径
    QStringList sections = curDir.split(QRegExp("[/]"));
    QString musicPath;

    for (int i = 0; i < sections.size() - 3; i++)
        musicPath += sections[i] + "/";

    audioPlayer->setMedia(QUrl::fromLocalFile(musicPath + "res/sound/backgrand.mp3"));
    audioPlayer->play();
}

void MainGameWindow::onIconButtonPressed()
{
    // 记录当前点击的icon
    curIcon = dynamic_cast<IconButton *>(sender());

    if(!preIcon)
    {
        // 播放音效
        QSound::play(":/res/sound/select.wav");

        // 如果单击一个icon
        curIcon->setStyleSheet(kIconClickedStyle);
        preIcon = curIcon;
    }
    else
    {
        if(curIcon != preIcon)
        {
            // 如果不是同一个button就都标记,尝试连接
            curIcon->setStyleSheet(kIconClickedStyle);
            if(game->linkTwoTiles(preIcon->xID, preIcon->yID, curIcon->xID, curIcon->yID))
            {
                // 播放音效
                QSound::play(":/res/sound/pair.wav");

                // 消除成功，隐藏掉
                preIcon->hide();
                curIcon->hide();
            }
            else
            {
                // 播放音效
                QSound::play(":/res/sound/release.wav");

                // 消除失败，恢复
                preIcon->setStyleSheet(kIconReleasedStyle);
                curIcon->setStyleSheet(kIconReleasedStyle);
            }
            preIcon = NULL;
            curIcon = NULL;

        }
        else if(curIcon == preIcon)
        {
            // 播放音效
            QSound::play(":/res/sound/release.wav");

            preIcon->setStyleSheet(kIconReleasedStyle);
            curIcon->setStyleSheet(kIconReleasedStyle);
            preIcon = NULL;
            curIcon = NULL;
        }
    }
}

void MainGameWindow::gameTimerEvent()
{
    // 进度条计时效果
    if(ui->timeBar->value() == 0)
    {
        gameTimer->stop();
//        QMessageBox::information(this, "game over", "play again>_<");
    }
    else
    {
        ui->timeBar->setValue(ui->timeBar->value() - 1);
    }


}
