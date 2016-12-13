#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include "game_model.h"

// 游戏逻辑模型，与界面分离
GameModel::GameModel() :
    gameStatus(PLAYING),
    gameLevel(BASIC)
{

}

GameModel::~GameModel()
{
    if(gameMap)
    {
        free(gameMap);
        gameMap = NULL;
    }
}

void GameModel::startGame()
{
    // 用C的方式初始化数组，怀旧一下~
    gameMap = (int *)malloc(sizeof(int) * MAX_ROW * MAX_COL);
    memset(gameMap, 0, MAX_ROW * MAX_COL);

    // 填充方块标号
    int iconID = 0;
    for(int i = 0; i < MAX_ROW * MAX_COL; i += 2)
    {
        // 每次填充连着的两个，图片用尽了就循环
        gameMap[i] = iconID % MAX_ICON + 1;
        gameMap[i + 1] = iconID % MAX_ICON + 1;
        iconID++;
    }

    // 打乱方块
    srand((unsigned)time(0));
    for(int i = 0; i< MAX_ROW * MAX_COL; i++)
    {
        int randomID = rand() % (MAX_ROW * MAX_COL);
        std::swap(gameMap[i], gameMap[randomID]);
    }
}

int *GameModel::getGameMap()
{

    return gameMap;
}

// 最重要的判断连接算法
bool GameModel::isCanLink(int srcX, int srcY, int dstX, int dstY)
{
    // 判断方块是否可以连，可用于实际的连接消除和提示消除
    // x表示横向索引，y表示纵向索引，从0开始

    // 情况1：横向或者竖向可以直线连通

    // 情况2：一次拐弯可以连通

    // 情况3：两次拐弯可以连通

    if(gameMap[MAX_COL * srcY + srcX] == gameMap[MAX_COL * dstY + dstX])
    {
        return true;
    }
    return false;
}

bool GameModel::linkTwoTiles(int srcX, int srcY, int dstX, int dstY)
{
    // 成功连接就返回true否则false用于GUI里面判断
    if(isCanLink(srcX, srcY, dstX, dstY))
    {
        gameMap[MAX_COL * srcY + srcX] = 0;
        gameMap[MAX_COL * dstY + dstX] = 0;

        return true;
    }
    return false;
}
