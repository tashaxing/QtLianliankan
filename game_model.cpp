#include <string.h>
#include <stdlib.h>
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

    // 打乱方块

}

bool GameModel::isCanLink(int srcX, int srcY, int dstX, int dstY)
{
    return false;
}

bool GameModel::linkTwoTiles(int srcX, int srcY, int dstX, int dstY)
{
    return false;
}
