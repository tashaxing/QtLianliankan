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

void GameModel::startGame(GameLevel level)
{
    // 用C的方式初始化数组，怀旧一下~
    gameMap = (int *)malloc(sizeof(int) * MAX_ROW * MAX_COL);
    memset(gameMap, 0, MAX_ROW * MAX_COL);
    for (int i = 0; i < MAX_ROW * MAX_COL; i++) // 必须这样赋初值，memset容易出问题
        gameMap[i] = 0;

    hintArray = (int *)malloc(sizeof(int) * 4);
    memset(hintArray, 0, 4);
    for (int i = 0; i < 4; i++)
        hintArray[i] = -1;

    gameLevel = level;

    int gameLevelNum;
    switch (gameLevel)
    {
    case BASIC:
        gameLevelNum = kBasicNum;
        break;
    case MEDIUM:
        gameLevelNum = kMediumNum;
        break;
    case HARD:
        gameLevelNum = kHardNum;
    }

    // 填充方块标号
    int iconID = 0;
    for(int i = 0; i < gameLevelNum; i += 2)
    {
        // 每次填充连着的两个，图片用尽了就循环
        gameMap[i] = iconID % MAX_ICON + 1;
        gameMap[i + 1] = iconID % MAX_ICON + 1;
        iconID++;
    }

    // 打乱方块
    srand((unsigned)time(0));
    for(int i = 0; i < MAX_ROW * MAX_COL; i++)
    {
        int randomID = rand() % (MAX_ROW * MAX_COL);
        std::swap(gameMap[i], gameMap[randomID]);
    }

}

// 重载
void GameModel::startGame()
{
    startGame(gameLevel);
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
bool GameModel::canLinkDirectly(int srcX, int srcY, int dstX, int dstY)
{
    // 竖线
    if (srcX == dstX)
    {
        if (srcY > dstY)
            std::swap(srcY, dstY);
        for (int y = srcY + 1; y < dstY; y++)
            if (gameMap[MAX_COL * y + srcX])
                return false;

        // 记录点和路线
        return true;
    }

    // 横线
    if (srcY == dstY)
    {
        if (srcX > dstX)
            std::swap(srcX, dstX);
        for (int x = srcX + 1; x < dstX; x++)
            if (gameMap[MAX_COL * srcY + x])
                return false;

        return true;
    }

    return false;
}

bool GameModel::canLinkWithOneCorner(int srcX, int srcY, int dstX, int dstY)
{
    if (srcX > dstX)
    {
        // 统一化，方便后续处理
        std::swap(srcX, dstX);
        std::swap(srcY, dstY);
    }

    // 先确定拐点，再确定线路,2种情况，4个点
    if (dstY > srcY)
    {
        if (gameMap[srcY * MAX_COL + dstX] == 0)
        {
            // 右上角

            // 判断路线
            for (int x = srcX + 1; x < dstX; x++)
            {
                if (gameMap[srcY * MAX_COL + x])
                    return false;
            }

            for (int y = srcY + 1; y < dstY; y++)
            {
                if (gameMap[y * MAX_COL + dstX])
                    return false;
            }

            return true;
        }
        else if (gameMap[dstY * MAX_COL + srcX] == 0)
        {
            // 左下角
            for (int y = srcY + 1; y < dstY; y++)
            {
                if (gameMap[y * MAX_COL + srcX])
                    return false;
            }

            for (int x = srcX + 1; x < dstX; x++)
            {
                if (gameMap[dstY * MAX_COL + x])
                    return false;
            }

            return true;
        }
    }
    else
    {
        if (gameMap[dstY * MAX_COL + srcX] == 0)
        {
            // 左上角
            for (int y = dstY + 1; y < srcY; y++)
            {
                if (gameMap[y * MAX_COL + srcX])
                    return false;
            }

            for (int x = srcX + 1; x < dstX; x++)
            {
                if (gameMap[dstY * MAX_COL + x])
                    return false;
            }

            return true;
        }
        else if (gameMap[srcY * MAX_COL + dstX] == 0)
        {
            for (int x = srcX + 1; x < dstX; x++)
            {
                if (gameMap[srcY * MAX_COL + x])
                    return false;
            }

            for (int y = dstY + 1; y < srcY; y++)
            {
                if (gameMap[y * MAX_COL + dstX])
                    return false;
            }

            return true;
        }
    }

    return false;
}

bool GameModel::canLinkWithTwoCorner(int srcX, int srcY, int dstX, int dstY)
{
    return false;
}

bool GameModel::isCanLink(int srcX, int srcY, int dstX, int dstY)
{
    // 首先判断点击的两个方块不是同一个不是空，且方块相同
    // 判断方块是否可以连，可用于实际的连接消除和提示消除
    // x表示横向索引，y表示纵向索引，从0开始
    // 分3种情况往下找，找到一种情况可以连通就返回true，并选用这种连接情况

    if (srcX == dstY && srcY == dstY)
        return false;

    if(gameMap[MAX_COL * srcY + srcX] != gameMap[MAX_COL * dstY + dstX])
        return false;

    // 情况1：横向或者竖向可以直线连通
    if (canLinkDirectly(srcX, srcY, dstX, dstY))
        return true;

    // 情况2：一次拐弯可以连通
    if (canLinkWithOneCorner(srcX, srcY, dstX, dstY))
        return true;

    // 情况3：两次拐弯可以连通
    if (canLinkWithTwoCorner(srcX, srcY, dstX, dstY))
        return true;


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
