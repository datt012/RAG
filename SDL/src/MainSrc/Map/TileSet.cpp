#include "TileSet.h"
#include "Define.h"
// Constructor implementation
TileSet::TileSet(int tsWidth, int tsHeight, SDL_Texture* tsTexture, string tsxPath, string imgPath, int fID, int lsID) {

    pixelWidth = PIXEL_WIDTH;
    pixelHeight = PIXEL_HEIGHT;

    tileSetWidth = tsWidth;
    tileSetHeight = tsHeight;
    tileSetTexture = tsTexture;
    tsxFilePath = tsxPath;
    imgFilePath = imgPath;
    firstTileSetID = fID;
    lastTileSetID = lsID;

   
}


SDL_Rect TileSet::getSrcRect(int pixelID) {
    SDL_Rect srcRect = { 0, 0, pixelWidth, pixelHeight };
    int localID = pixelID - firstTileSetID;
    if (localID >= 0 && localID <= (lastTileSetID - firstTileSetID)) {
        int column = localID % tileSetWidth;
        int row = localID / tileSetWidth;
        srcRect.x = column * pixelWidth;
        srcRect.y = row * pixelHeight;
    }

    return srcRect;
}