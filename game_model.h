#ifndef GAMEMODEL_H
#define GAMEMODEL_H

// ------------ 全局变量 ------------ //
// 最大行和列数
static const int MAX_ROW = 20;
static const int MAX_COL = 20;
// 游戏可玩图片数量
static const int MAX_ICON = 25;
// 游戏状态
enum GameStatus
{
    PLAYING,
    PAUSE,
    WIN,
    OVER
};
// 游戏难度
enum GameLevel
{
    BASIC,
    MEDIUM,
    HARD
};

// -------------------------------- //

class GameModel
{
public:
    GameModel();
    virtual ~GameModel();
public:
    void startGame(); // 开始游戏
    int *gatGameMap();    // 获得地图
    GameStatus checkGameStatus(); // 判断获得游戏状态，是否输赢
    bool linkTwoTiles(int srcX, int srcY, int dstX,int dstY); // 连接起点和终点方块，连接是否成功
private:
    // 游戏地图，存储方块，0表示消失，1-其他数字表示图片标号
    int *gameMap;
    // 游戏状态和难度
    GameStatus gameStatus;
    GameLevel gameLevel;
    // 判断起点到终点的方块是否可以连接消除
    bool isCanLink(int srcX, int srcY, int dstX, int dstY);
};

#endif // GAMEMODEL_H
