#ifndef CODE_H_INCLUDED
#define CODE_H_INCLUDED

#include "SDL_lib.h"

SDL_Rect GetRect(int value);    // lấy vị trí ảnh của giá trị value

class Square
{
private:
    int Value = 0;          // giá trị của ô
    SDL_Rect srcrect;       // vị trí ảnh của giá trị
    SDL_Rect dstrect;       // vị trí của ô trên window để in ảnh

public:
    SDL_Renderer* Renderer;
    SDL_Texture* Texture;

    void Set(int x, int y, SDL_Renderer* renderer, SDL_Texture* texture);   // khởi tạo ô thay cho hàm khởi tạo
    void SetValue(int value);                                               // cập nhật giá trị và ảnh trên renderer
    int GetValue() {return Value;}      // trả về giá trị của ô cho mục đích xử lý
};

#endif // CODE_H_INCLUDED
