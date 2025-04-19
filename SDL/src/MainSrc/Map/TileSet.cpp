#include "TileSet.h"
#include "Define.h"

TileSet::TileSet(int tsColumn, int tsRow, SDL_Texture* tsTexture, string tsxPath, string imgPath, int fID, int lsID) {

    pixelWidth = PIXEL_WIDTH;
    pixelHeight = PIXEL_HEIGHT;

    tileSetColumn = tsColumn;
    tileSetRow = tsRow;
    tileSetTexture = tsTexture;
    tsxFilePath = tsxPath;
    imgFilePath = imgPath;
    firstTileSetID = fID;
    lastTileSetID = lsID;


}


SDL_Rect TileSet::getSrcRect(int pixelID) {
    
    SDL_Rect srcRect = { 0, 0, pixelWidth, pixelHeight };
    int localID = pixelID - firstTileSetID ;


    int column = localID % tileSetColumn;
    int row = localID / tileSetColumn;
    srcRect.x = column * pixelWidth;
    srcRect.y = row * pixelHeight;


    return srcRect;
}