#include <string>
#include <SDL.h>
using namespace std;
class TileSet {
public:
    int pixelWidth;
    int pixelHeight;
    int tileSetColumn;// số cột 
    int tileSetRow;// số hàng 

    SDL_Texture* tileSetTexture;
    string tsxFilePath;
    string imgFilePath;

    int firstTileSetID;
    int lastTileSetID;
    TileSet(int tsColumn, int tsRow, SDL_Texture* tsTexture, string tsxPath, string imgPath, int fID, int lsID);
    SDL_Rect getSrcRect(int pixelID);

};