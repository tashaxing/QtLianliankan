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

bool GameModel::isCanLink(int srcX, int srcY, int dstX, int dstY)
{
    return false;
}

bool GameModel::linkTwoTiles(int srcX, int srcY, int dstX, int dstY)
{
    return false;
}
