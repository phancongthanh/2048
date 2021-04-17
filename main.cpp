#include "Code.h"
#include <fstream>
#include <ctime>
#include <cstdlib>

const int WIDTH = 900;
const int HEIGHT = 627;
    SDL_Window* Window;
    SDL_Renderer* Renderer;
    SDL_Texture* Texture;
    SDL_Event e;

    Square** square;    // lưu giá trị của 16 ô square[x][y]
    int** Value;        // lưu giá trị tạm thời của square cho mục đích xử lý

    int Score = 0;

void SetData();         // khởi tạo game
void RandomValue();     // tạo ô vuông ngẫu nhiên

void UpDate();          // cập nhật khi nhấn phím
void LoadScore();       // cập nhật điểm
bool CheckEnd();        // kiểm tra điều kiện kết thúc game

void EndGame();         // xử lý kết quả

// xử lý khi nhấn phím
void KEYUP();           // phím UP
void KEYDOWN();         // phím DOWN
void KEYLEFT();         // phím LEFT
void KEYRIGHT();        // phím RIGHT

void NewGame();         // phím F1
void SaveGame();        // phím F2

// xử lý hàm main
/*
    khởi tạo game bằng SetData();
    vòng while vô hạn của game
        {
            đợi phím nhấn thì chạy Update() xong kiểm tra kết thúc game bằng CheckEnd()
            đợi 100ms = 0.1s rồi kiểm tra phím nhấn lần nữa
        }
    xong game thì chạy EndGame() xử lý kết quả thoát game
*/

int main(int argc, char* argv[]) {
    SetData();
    while (true) {
        if (SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT)) {
            if (e.type == SDL_QUIT) { SaveGame(); exit(1);} // tắt game thì lưu dữ liệu thoát game bằng exit()
            UpDate();
            if (!CheckEnd()) break;
        }
        SDL_Delay(100);
    }
    EndGame();
    return 0;
}

void SetData() {
    // khởi tạo cửa sổ SDL
    srand(time(0));
    initSDL(Window,Renderer);
    Texture = loadTexture(Renderer,"2048.bmp");
    SDL_Rect rect;
    rect.x = 0;rect.y=0;rect.w=WIDTH;rect.h=HEIGHT;
    SDL_RenderCopy(Renderer,Texture,&rect,&rect);

    // cấp phát và khởi tạo cho square và Value
    square = new Square*[4];
    Value = new int*[4];
    for (int x=0;x<4;x++) {
        square[x] = new Square[4];
        for (int y=0;y<4;y++) square[x][y].Set(x,y,Renderer,Texture);
        Value[x] = new int[4];
    }

    // load dữ liệu lần chơi trước
    int val = 0;
    std::ifstream ifile("Score.txt");
    for (int y=0;y<4;y++)
        for (int x=0;x<4;x++) {
            ifile >> val;
            square[x][y].SetValue(val);
        }
    ifile.close();

    // cập nhật lần đầu cho game
    LoadScore();
    SDL_RenderPresent(Renderer);
}

void UpDate() {
    // sao lưu square vào value để xử lý
    for (int x=0;x<4;x++) {
        for (int y=0;y<4;y++) Value[x][y] = square[x][y].GetValue();
    }

    // xử lý theo phím nhấn
    switch (e.key.keysym.sym) {
        case SDLK_F1: NewGame();
            return ;
        case SDLK_F2: SaveGame();
            return ;
        case SDLK_UP: KEYUP();
            break;
        case SDLK_DOWN: KEYDOWN();
            break;
        case SDLK_LEFT: KEYLEFT();
            break;
        case SDLK_RIGHT: KEYRIGHT();
            break;
        default: return ; // chỉ nhận 6 phím ko thì thôi
            break;
    }

    // kiểm tra phím hợp lệ (nghĩa là có sự thay đổi của Value so với square ban đầu
    bool check = false;
    for (int x=0;x<4;x++) {
        for (int y=0;y<4;y++) {
            if (Value[x][y] != square[x][y].GetValue()){
                check = true;
                square[x][y].SetValue(Value[x][y]);
            }
        }
    }

    // nếu hợp lệ chạy cập nhật
    if (check) {
        LoadScore();
        SDL_RenderPresent(Renderer);
        SDL_Delay(50);

        RandomValue();
        SDL_RenderPresent(Renderer);
    }
}

void LoadScore() {
    // gán Score là giá trị lớn nhất của square::Value
    for (int x=0;x<4;x++)
        for (int y=0;y<4;y++) if (square[x][y].GetValue() > Score) Score = square[x][y].GetValue();

    // vẽ lại Score lên renderer
    static SDL_Rect srcrect;
    static SDL_Rect dstrect;
    srcrect = GetRect(Score);
    dstrect.x = 745;
    dstrect.y = 470;
    dstrect.w = 135;
    dstrect.h = 135;
    SDL_RenderCopy(Renderer,Texture,&srcrect,&dstrect);
}

bool CheckEnd() {
    // kết thúc false khi lên 2048
    for (int x=0;x<4;x++)
        for (int y=0;y<4;y++)
            if (square[x][y].GetValue()==2048) return false;

    // chưa lên 2048 còn ô rỗng tiếp tục true
    for (int x=0;x<4;x++)
        for (int y=0;y<4;y++)
            if (square[x][y].GetValue()==0) return true;

    // chưa lên 2048, kín 16 ô
    for (int x=0;x<4;x++) {
        if (square[x][0].GetValue() == square[x][1].GetValue()) return true;
        if (square[x][1].GetValue() == square[x][2].GetValue()) return true;
        if (square[x][2].GetValue() == square[x][3].GetValue()) return true;
    }
    for (int y=0;y<4;y++) {
        if (square[0][y].GetValue() == square[1][y].GetValue()) return true;
        if (square[1][y].GetValue() == square[2][y].GetValue()) return true;
        if (square[2][y].GetValue() == square[3][y].GetValue()) return true;
    }
    return false; // trả về false thua cuộc nếu ko có ô nào tiếp tục được
}

void EndGame() {
    SDL_Rect rect,src,dst;

    // xóa 16 ô bằng hình vuông để  viết thông báo
    rect.x = 0; rect.y = 0; rect.w = 627; rect.h = 627;
    SDL_SetRenderDrawColor(Renderer,255,255,255,255);
    SDL_RenderFillRect(Renderer,&rect);

    // tải thông báo theo kết quả Score
    src.x = 0; src.y = 628; src.w = 450; src.h = 100;
    dst.x = 75; dst.y = 250; dst.w = 450; dst.h = 100;
    if (Score != 2048) src.x = 451;
    SDL_RenderCopy(Renderer,Texture,&src,&dst);

    SDL_RenderPresent(Renderer); // update từ renderer lên window

    // đợi kết thúc
    while (true) {if (SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT)) break;}
    SDL_Delay(2000);

    // giải phóng con trỏ,...
    for (int x=0;x<4;x++) {delete[] square[x]; delete[] Value[x];}
    SDL_DestroyTexture(Texture);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
}

void RandomValue() {
    int rannumber = 0;

    // đếm số lượng ô rỗng
    for (int x=0;x<4;x++)
        for (int y=0;y<4;y++) if (square[x][y].GetValue() == 0) rannumber++;

    // xác suất ô rỗng được gán giá trị ngẫu nhiên Val
    rannumber = rand() % rannumber + 1;
    int Val = 2;
    if (Score >= 32 && rand()%1024 < Score) Val = 4;
    if (Score >= 512 && rand()%10240 < Score) Val = 8;

    // cập nhật Val cho ô rỗng đã chọn
    for (int x=0;x<4;x++)
        for (int y=0;y<4;y++) {
            if (square[x][y].GetValue() == 0) rannumber--;
            if (rannumber==0) {square[x][y].SetValue(Val);return ;}
        }
}


// KEY công thức chung
/*
    xử lý trên dữ liệu ảo Value[x][y]
    xử 4 dòng tương tự nhau nên for ( =0; <4; ++)
    sắp xếp dồn 4 ô lại
    ô cạnh nhau cùng giá trị -> gộp lại 1 ô nhân đôi 1 ô về 0
    sắp xếp lại lần nữa
*/

void KEYUP() {
    for (int x=0;x<4;x++) {
        for (int i=0;i<3;i++)
            if (Value[x][i]==0)
                for (int j=i+1;j<4;j++)
                    if (Value[x][j]!=0) {
                        Value[x][i] = Value[x][j];
                        Value[x][j] = 0;
                        break;
                    }
        for (int i=0;i<3;i++)
            if (Value[x][i]==Value[x][i+1]) {
                Value[x][i] = 2*Value[x][i];
                Value[x][i+1] = 0;
            }
        for (int i=0;i<3;i++)
            if (Value[x][i]==0)
                for (int j=i+1;j<4;j++)
                    if (Value[x][j]!=0) {
                        Value[x][i] = Value[x][j];
                        Value[x][j] = 0;
                        break;
                    }
    }
}
void KEYDOWN() {
    for (int x=0;x<4;x++) {
        for (int i=3;i>0;i--)
            if (Value[x][i]==0)
                for (int j=i-1;j>=0;j--)
                    if (Value[x][j]!=0) {
                        Value[x][i] = Value[x][j];
                        Value[x][j] = 0;
                        break;
                    }
        for (int i=3;i>0;i--)
            if (Value[x][i]==Value[x][i-1]) {
                Value[x][i] = 2*Value[x][i];
                Value[x][i-1] = 0;
            }
        for (int i=3;i>0;i--)
            if (Value[x][i]==0)
                for (int j=i-1;j>=0;j--)
                    if (Value[x][j]!=0) {
                        Value[x][i] = Value[x][j];
                        Value[x][j] = 0;
                        break;
                    }
    }
}
void KEYLEFT() {
    for (int y=0;y<4;y++) {
        for (int i=0;i<3;i++)
            if (Value[i][y]==0)
                for (int j=i+1;j<4;j++)
                    if (Value[j][y]!=0) {
                        Value[i][y] = Value[j][y];
                        Value[j][y] = 0;
                        break;
                    }
        for (int i=0;i<3;i++)
            if (Value[i][y]==Value[i+1][y]) {
                Value[i][y] = 2*Value[i][y];
                Value[i+1][y] = 0;
            }
        for (int i=0;i<3;i++)
            if (Value[i][y]==0)
                for (int j=i+1;j<4;j++)
                    if (Value[j][y]!=0) {
                        Value[i][y] = Value[j][y];
                        Value[j][y] = 0;
                        break;
                    }
    }
}
void KEYRIGHT() {
    for (int y=0;y<4;y++) {
        for (int i=3;i>0;i--)
            if (Value[i][y]==0)
                for (int j=i-1;j>=0;j--)
                    if (Value[j][y]!=0) {
                        Value[i][y] = Value[j][y];
                        Value[j][y] = 0;
                        break;
                    }
        for (int i=3;i>0;i--)
            if (Value[i][y]==Value[i-1][y]) {
                Value[i][y] = 2*Value[i][y];
                Value[i-1][y] = 0;
            }
        for (int i=3;i>0;i--)
            if (Value[i][y]==0)
                for (int j=i-1;j>=0;j--)
                    if (Value[j][y]!=0) {
                        Value[i][y] = Value[j][y];
                        Value[j][y] = 0;
                        break;
                    }
    }
}

void NewGame() {
    // làm mới Score và square::Value -> chạy ngẫu nhiên ô rỗng -> update lên window
    Score = 0;
    for (int y=0;y<4;y++)
        for (int x=0;x<4;x++) square[x][y].SetValue(0);
    RandomValue();
    LoadScore();
    SDL_RenderPresent(Renderer);
}
void SaveGame() {
    // lưu giá trị toàn bộ 16 square::Value xuống file
    std::ofstream ofile("Score.txt");
    for (int y=0;y<4;y++) {
        for (int x=0;x<4;x++) ofile << square[x][y].GetValue() << " ";
        ofile << std::endl;
    }
    ofile.close();
}
