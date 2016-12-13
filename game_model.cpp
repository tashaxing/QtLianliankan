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
        free(hintArray);
        gameMap = NULL;
    }
}

void GameModel::startGame()
{
    // 用C的方式初始化数组，怀旧一下~
    gameMap = (int *)malloc(sizeof(int) * MAX_ROW * MAX_COL);
    memset(gameMap, 0, MAX_ROW * MAX_COL);

    hintArray = (int *)malloc(sizeof(int) * 4);
    memset(hintArray, -1, 4);


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

bool GameModel::isFrozen()
{
    // 暴力法，所有方块两两判断是否可以连接
    // 每次消除后做一次判断
    // 其实在这个过程中记录提示

    for (int i = 0; i < MAX_ROW * MAX_COL - 1; i++)
        for( int j = i + 1; j < MAX_ROW * MAX_COL; j++)
        {
            int srcX = i % MAX_COL;
            int srcY = i / MAX_COL;
            int dstX = j % MAX_COL;
            int dstY = j / MAX_COL;

            // 只要能找到可以连接的就不为僵局
            if (isCanLink(srcX, srcY, dstX, dstY))
            {
                // 记录hint
                hintArray[0] = srcX;
                hintArray[1] = srcY;
                hintArray[2] = dstX;
                hintArray[3] = dstY;

                return false;
            }
        }

    return true;
}

// 最重要的判断连接算法
bool GameModel::isCanLink(int srcX, int srcY, int dstX, int dstY)
{
    // 判断方块是否可以连，可用于实际的连接消除和提示消除
    // x表示横向索引，y表示纵向索引，从0开始

    // 情况1：横向或者竖向可以直线连通
    if (srcX == dstX)
    {
        if (srcY > dstY)
            std::swap(srcY, dstY);
        for (int y = srcY + 1; y < dstY; y++)
            if (gameMap[MAX_COL * y + srcX])
                return false;
    }
    if (srcY == dstY)
    {
        if (srcX > dstX)
            std::swap(srcX, dstX);
        for (int x = srcX + 1; x < dstX; x++)
            if (gameMap[MAX_COL * srcY + x])
                return false;
    }

    // 情况2：一次拐弯可以连通



    // 情况3：两次拐弯可以连通

    if(gameMap[MAX_COL * srcY + srcX] == gameMap[MAX_COL * dstY + dstX])
    {
        return true;
    }
    return false;
}

// 点击方块进行连接操作
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
