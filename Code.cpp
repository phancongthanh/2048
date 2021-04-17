#include "Code.h"

void Square::Set(int x, int y, SDL_Renderer* renderer, SDL_Texture* texture) {
    Renderer = renderer;
    Texture = texture;

    srcrect = GetRect(Value);

    dstrect.x = 18 + x * 152;
    dstrect.y = 18 + y * 152;
    dstrect.w = 135;
    dstrect.h = 135;

    SDL_RenderCopy(Renderer,Texture,&srcrect,&dstrect);
}

void Square::SetValue(int value) {
    if (Value == value) return ;
    Value = value;
    srcrect = GetRect(Value);
    SDL_RenderCopy(Renderer,Texture,&srcrect,&dstrect);
}

SDL_Rect GetRect(int value) {
    SDL_Rect rect;
    rect.w = 135;
    rect.h = 135;
    switch (value) {
    case 0: rect.x = 18; rect.y = 18;
        break;
    case 2: rect.x = 170; rect.y = 18;
        break;
    case 4: rect.x = 322; rect.y = 18;
        break;
    case 8: rect.x = 474; rect.y = 18;
        break;
    case 16: rect.x = 18; rect.y = 170;
        break;
    case 32: rect.x = 170; rect.y = 170;
        break;
    case 64: rect.x = 322; rect.y = 170;
        break;
    case 128: rect.x = 474; rect.y = 170;
        break;
    case 256: rect.x = 18; rect.y = 322;
        break;
    case 512: rect.x = 170; rect.y = 322;
        break;
    case 1024: rect.x = 322; rect.y = 322;
        break;
    case 2048: rect.x = 474; rect.y = 322;
        break;
    }
    return rect;
}
