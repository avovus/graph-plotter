#include <iostream>
#include <iomanip>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <SDL2/SDL_ttf.h>
#include <cmath>
//#include <boost/spirit/include/qi.hpp>
using namespace std;

// Задаем параметры окна
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
TTF_Font* font = NULL;
vector <float> odds {1,0,0,0,0};
SDL_DisplayMode mode;


SDL_Texture* createTextTexture(SDL_Renderer* renderer, const std::string& text, SDL_Color color)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (textSurface == NULL)
    {
        std::cout << "Unable to render text surface. TTF Error: " << TTF_GetError() << std::endl;
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (texture == NULL)
    {
        std::cout << "Unable to create texture from rendered text. SDL Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return NULL;
    }

    SDL_FreeSurface(textSurface);

    return texture;
}

struct MenuTexture{
	SDL_Texture* mTexture;
	int w, h;
};

MenuTexture tf(TTF_Font* font, string item, SDL_Renderer* gRenderer,
	SDL_Color textColor){
		//Render text
		SDL_Surface* textSurface =
			TTF_RenderUTF8_Solid( font, item.c_str(), textColor );
		if( textSurface == NULL )
		{
			printf(
				"Unable to render text surface! SDL_ttf Error: %s\n",
				TTF_GetError() );
		}

		//Create texture from surface pixels
		SDL_Texture* mTexture =
			SDL_CreateTextureFromSurface( gRenderer, textSurface );
		MenuTexture mt{mTexture,textSurface->w,textSurface->h};
		SDL_FreeSurface( textSurface );
		if( mTexture == NULL )
		{
			printf(
			"Unable to create texture from rendered text! SDL Error: %s\n",
			SDL_GetError() );
		}

		return mt;
}

void renderText(SDL_Renderer* renderer, SDL_Texture* texture, int xPos, int yPos)
{
    SDL_Rect renderQuad = { xPos, yPos, 0, 0 };

    SDL_QueryTexture(texture, NULL, NULL, &renderQuad.w, &renderQuad.h);
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}

string removeZero(double num){
    string ans = to_string(num);
    while(ans.back() == '0'){
        ans.pop_back();
    }
    if(ans.back() == '.'){
        ans.pop_back();
    }
    return ans;
}

// Функция для рисования графика
void drawGraph(SDL_Renderer* renderer, float(*function)(float,int), float endX, float step, int scale,int type, int offsetX = WINDOW_WIDTH / 2, int offsetY = WINDOW_HEIGHT / 2)
{
    
    //расчитываем смещение
    // Очищаем экран
//    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
//    SDL_RenderClear(renderer);

    // Задаем цвет линии графика
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Рисуем график
    float x, y,xp,yp;
    int r = 1;
    
    xp = -500; yp = -500; 

    //float p = abs(function(endX,type));

    if(abs(function(endX,type))>500){
        endX = 13;
        xp = -endX;
        yp = -function(xp,type);
    }

    if(type < 11){

    for (x = -endX; x <= endX; x += step) {
        y = -function(x,type);

        if((abs(x-xp)<endX && abs(y-yp)<endX)){
            SDL_RenderDrawLine(renderer,static_cast<int>(xp*scale) + offsetX,static_cast<int>(yp*scale) + offsetY,static_cast<int>(x*scale) + offsetX,static_cast<int>(y*scale) + offsetY);
            //SDL_RenderDrawPoint(renderer, static_cast<int>(x*scale) + offsetX, static_cast<int>(y*scale)+ offsetY);
        }
        yp = y;
        xp = x;
    }

    if(type == 10){
        for (x = -endX; x <= endX; x += step) {
            y = -(acos(1-abs(x)) - 3.141592653589793);

            if((abs(x-xp)<endX && abs(y-yp)<endX)){
                SDL_RenderDrawLine(renderer,static_cast<int>(xp*scale) + offsetX,static_cast<int>(yp*scale) + offsetY,static_cast<int>(x*scale) + offsetX,static_cast<int>(y*scale) + offsetY);
            }
            //SDL_RenderDrawPoint(renderer, static_cast<int>(x*scale) + offsetX, static_cast<int>(y*scale)+ offsetY);
            yp = y;
            xp = x;
        }
    }
    }
    if(type == 11){
        r = 10;
        yp = r*pow(sin(0),3); xp = r*pow(cos(0),3);
        cout << "in type 11" << endl;
        for(double t = 0; t<=6.28; t+=0.01){
            cout <<t << endl;
            x = r*pow(cos(t),3);
            y = r*pow(sin(t),3);
            SDL_RenderDrawPoint(renderer, static_cast<int>(x*scale) + offsetX, static_cast<int>(y*scale)+ offsetY);
            SDL_RenderDrawLine(renderer,static_cast<int>(xp*scale) + offsetX,static_cast<int>(yp*scale) + offsetY,static_cast<int>(x*scale) + offsetX,static_cast<int>(y*scale) + offsetY);
            yp = y;
            xp = x;

        }
    }

    if(type == 12){
        r = 2;
        double R = 0;
        yp = 0; xp = 0;
        cout << "in type 11" << endl;
        for(double t = 0; t<=15; t+=0.01){
            R = r*t;
            cout <<t << endl;
            x = R*cos(t);
            y = R*sin(t);
            SDL_RenderDrawPoint(renderer, static_cast<int>(x*scale) + offsetX, static_cast<int>(y*scale)+ offsetY);
            SDL_RenderDrawLine(renderer,static_cast<int>(xp*scale) + offsetX,static_cast<int>(yp*scale) + offsetY,static_cast<int>(x*scale) + offsetX,static_cast<int>(y*scale) + offsetY);
            yp = y;
            xp = x;

        }
    }

}

void drawCL(SDL_Renderer* renderer, float endX, int scale, int offsetX = WINDOW_WIDTH / 2, int offsetY = WINDOW_HEIGHT / 2)
{
    float step = 1.0;
    //расчитываем смещение

    // Задаем цвет линии графика
    
    SDL_SetRenderDrawColor(renderer, 25, 50, 75, 0);

    // Рисуем координатную прямую
    float x, y;

    SDL_RenderDrawLine(renderer,static_cast<int>(0*scale) + offsetX,static_cast<int>(endX*scale) + offsetY,static_cast<int>(0*scale) + offsetX,static_cast<int>(-endX*scale) + offsetY);
    SDL_RenderDrawLine(renderer,static_cast<int>(endX*scale) + offsetX,static_cast<int>(0*scale) + offsetY,static_cast<int>(-endX*scale) + offsetX,static_cast<int>(0*scale) + offsetY);
    
    SDL_SetRenderDrawColor(renderer, 100, 150, 205, 255);

    //точки еденичных отрезков
    for (x = -endX; x <= endX; x += step) {   
        y = 0;

        if(x != 0){
            SDL_SetRenderDrawColor(renderer, 50, 60, 70, 0);
            SDL_RenderDrawLine(renderer,static_cast<int>(x*scale) + offsetX,static_cast<int>(-endX*scale) + offsetY,static_cast<int>(x*scale) + offsetX,static_cast<int>(endX*scale) + offsetY);
            SDL_SetRenderDrawColor(renderer, 100, 150, 205, 255);
        }

        SDL_Rect pointRect = {static_cast<int>(x*scale)-2 + offsetX, static_cast<int>(y*scale)-2 + offsetY, 4, 4};
        SDL_RenderDrawRect(renderer, &pointRect);
        //SDL_RenderFillRect(renderer, &pointRect)    
    }

    for (y = -endX; y <= endX; y += step) {
        x = 0;

        if(y != 0){
            SDL_SetRenderDrawColor(renderer, 50, 60, 70, 0);
            SDL_RenderDrawLine(renderer,static_cast<int>(-endX*scale) + offsetX,static_cast<int>(y*scale) + offsetY,static_cast<int>(endX*scale) + offsetX,static_cast<int>(y*scale) + offsetY);
            SDL_SetRenderDrawColor(renderer, 100, 150, 205, 255);
        }

        SDL_Rect pointRect = {static_cast<int>(x*scale)-2 + offsetX, static_cast<int>(y*scale)-2 + offsetY, 4, 4};
        SDL_RenderDrawRect(renderer, &pointRect);
        //SDL_RenderFillRect(renderer, &pointRect);
    }
}

void drawConsole(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 0);
    SDL_Rect terminalRect = {0,WINDOW_HEIGHT-WINDOW_HEIGHT/6, WINDOW_WIDTH, WINDOW_HEIGHT/6};
    SDL_RenderDrawRect(renderer, &terminalRect);
    SDL_RenderFillRect(renderer, &terminalRect);
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 0);
    SDL_Rect upperRect = {0,WINDOW_HEIGHT-WINDOW_HEIGHT/6, WINDOW_WIDTH, WINDOW_HEIGHT/24};
    SDL_RenderDrawRect(renderer, &upperRect);
    SDL_RenderFillRect(renderer, &upperRect);

    SDL_SetRenderDrawColor(renderer, 45, 45, 60, 0);
    SDL_RenderDrawLine(renderer,0,WINDOW_HEIGHT-WINDOW_HEIGHT/6,WINDOW_WIDTH,WINDOW_HEIGHT-WINDOW_HEIGHT/6);

    SDL_SetRenderDrawColor(renderer, 40, 40, 50, 0);
    SDL_RenderDrawLine(renderer,0,WINDOW_HEIGHT-WINDOW_HEIGHT/6+WINDOW_HEIGHT/24,WINDOW_WIDTH,WINDOW_HEIGHT-WINDOW_HEIGHT/6+WINDOW_HEIGHT/24);

}

float customFunction(float x, int t)
{
    switch(t){
        case 1:
            return x*odds[0] + odds[1];
            break;
        case 2:
            return odds[0]* abs(x+odds[1]) + odds[2];
            break;
        case 3:
            return odds[0]*pow(x,odds[1]) + odds[2]*x + odds[3];
            break;
        case 4:
            return odds[0]*pow(odds[1],x) + odds[2];
            break;
        case 5:
            return odds[0]* 1/(x + odds[1]) + odds[2];
            break;
        case 6:
            return odds[0]*log2(odds[1]*x + odds[2]) + odds[3];
            break;
        case 7:
            return (odds[0]*sin(x + odds[1])) + odds[2];
            break;
        case 8:
            return odds[0]*tan(x*odds[1] + odds[2]) + odds[3];
            break;
        case 9:
            return abs(odds[0] * pow(abs(x) + odds[1], odds[2]) + odds[3]);
            break;
        case 10:
            return pow(1-pow(abs(x)-1,2),0.5);
            break;

        case 11:
            //return acos(1-abs(x)) - 3.141592653589793;            
            break;

        case 12:
            //return sqrt(1 - 3 * abs(x));            
            break;

        default:
            return 1;
            break;

    }
    //return abs(3*abs(x)-2);
    //return abs(pow(abs(x) - 1,2)-1);
    //return x*x;
    //return sqrt(x);
    //return abs(1/(2*x+1));
    //return 1/(x);
    //return sqrt(1 - 3 * abs(x));
    //return abs(2 * abs(x) -1);
    //return log2(x);
    //return sin(x);
    //return tan(x);

}

int main(int argc, char* args[])
{
    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    //изменение масштабов окна, в зависимости от разрешения экрана
    if (SDL_GetCurrentDisplayMode(0, &mode) == 0){
        WINDOW_WIDTH = mode.w/2 + 2*(mode.w/10);
        WINDOW_HEIGHT = mode.h/2 + 2*(mode.h/10);
    }
    else{
        SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
        return -1;
    }

    // Создание окна
    SDL_Window* window = SDL_CreateWindow("Graph Plotter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Создание рендерера
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() == -1)
    {
        std::cout << "SDL_ttf initialization failed. TTF Error: " << TTF_GetError() << std::endl;
        return false;
    }

    
    font = TTF_OpenFont("Stengazeta-Regular_5.ttf", 30);
    if (font == NULL)
    {
        std::cout << "Failed to load font. TTF Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // Основной цикл программы
    float scale = 50;
    float num = 1;
    int type = 1;
    int maxOffset = 70;
    bool needUpdate = false;
    bool leftMouseButtonDown = false;
    int n = 3;
    int xoffset = WINDOW_WIDTH/2; int yoffset = WINDOW_HEIGHT/2;

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_Delay(10);
    //отчищаем экран
    SDL_RenderClear(renderer);
    SDL_Color textColor = { 255, 255, 255 };
    SDL_Color textModeColor = { 200, 200, 200 };

    bool enterMode = false;
    string text = "нажимайте от 1 до 9 для выбора типа графика";
    string textMode = "нажмите ENTER для выбора коэффицента";
    string inputText = " ";
    //SDL_Texture* textTexture = createTextTexture(renderer,text, textColor);
    MenuTexture textTexture = tf(font, text, renderer, textColor);
    renderText(renderer,textTexture.mTexture, 40,40);

    SDL_Texture* inputTextTexture = createTextTexture(renderer,inputText, textColor);
    //MenuTexture inputTextTexture = tf(font, inputText, renderer, textColor);
    //renderText(renderer,inputTextTexture.mTexture, WINDOW_WIDTH - 200,WINDOW_HEIGHT - 100);

    drawCL(renderer, 400.0,scale);
    drawGraph(renderer, customFunction, 500.0, 0.01,scale,type);
    SDL_RenderPresent(renderer);
    bool isRunning = true;
    while (isRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            if(event.type == SDL_TEXTINPUT && enterMode){
                inputText += event.text.text;
                //textModeColor = { 200, 200, 200 };
                //textMode = "режим ввода";
                //cout << inputText << endl;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN){
                // Проверяем, что нажата ЛКМ
                if (event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                }
            }

            if(event.type == SDL_MOUSEBUTTONUP){
                // Проверяем, что отпущена ЛКМ
                if (event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
                }
            }
            if(event.type == SDL_MOUSEMOTION){
                if(leftMouseButtonDown){
                    if((event.motion.xrel < 0 && xoffset <= (-2*maxOffset+100)*scale) || (event.motion.xrel > 0 && xoffset >= (2*maxOffset)*scale) || (event.motion.yrel < 0 && yoffset <= (-2*maxOffset+50)*scale) || (event.motion.yrel > 0 && yoffset >= (2*maxOffset-50)*scale)){

                    }
                    else{
                        xoffset+=event.motion.xrel;
                        yoffset += event.motion.yrel;
                    }
                }
            }
            if( event.type == SDL_MOUSEWHEEL){
                if ((event.wheel.y > 0 && scale <= 200) || (event.wheel.y < 0 && scale >= 15)) {
                    scale += event.wheel.y;
                }
                //} else if (event.wheel.y < 0) {
                    //std::cout << "Колесико прокручено вниз" << std::endl;
                //}            
            }
            if (event.type == SDL_KEYDOWN) {
                //cout << scale << endl;
                needUpdate = true;
                switch (event.key.keysym.sym) {
                    /*
                    case SDLK_UP:{
                        if(scale >= 100){
                            num = 2;
                        }
                        else if(scale <= 50){
                            num = 0.5;
                        }

                        else{
                            num = 1;
                        }
                        // Увеличение масштаба
                        if(scale < 200){
                            scale += num;
                        }
                        break;
                    }
                    case SDLK_DOWN:{
                        if(scale >= 100){
                            num = 2;
                        }
                        else if(scale <= 50){
                            num = 0.5;
                        }

                        else{
                            num = 1;
                        }

                        // Уменьшение масштаба
                        if(scale > 10){
                            scale -= num;
                        }
                        break;
                    }
                    case SDLK_RIGHT:{
                        xoffset -= 10;
                        break;
                    }
                    case SDLK_LEFT:{
                        xoffset += 10;
                        break;
                    }
                    */
                    case SDLK_RETURN:
                    {if(enterMode){
                            stringstream s1;
                            int u; double p;
                            s1 << inputText;
                            s1 >> u >> p;
                            inputText = " ";

                            /*
                            int u; cin >> u;                        
                            float p; cin >> p;
                            */
                            if(odds.size() >= u){
                                odds[u-1] = p;
                                textModeColor = { 200, 200, 200 };
                                textMode = "нажмите ENTER для выбора коэффицента";
                            }
                            else{
                                //textModeColor = { 155, 0, 0 };
                                textMode = "нажмите ENTER для выбора коэффицента";
                            }
                            needUpdate = true;
                            //inputText = "tap ENTER for choose coefficient";
                        }
                        else{
                            //textModeColor = { 200, 200, 200 };
                            textMode = "режим ввода";
                        }
                        enterMode = !enterMode;
                        break;
                    }
                   case SDLK_BACKSPACE:
                    {if(enterMode){
                        if(inputText.length()>0){
                            inputText.pop_back();
                        }
                    cout << inputText << endl;
                    }
                    }
                    case SDLK_1:{
                        if(!enterMode){
                            type = 1;
                            odds = {1,0,0,0,0};
                        }              
                        break;
                    }

                    case SDLK_2:
                    {
                        if(!enterMode){
                            type = 2;
                            odds = {1,0,0,0,0};
                        }
                        break;
                    }
                    case SDLK_3:
                    {
                        if(!enterMode){
                            odds = {1,2,0,0,0};
                            type = 3;
                        }
                        break;
                    }
                    case SDLK_4:
                    {
                        if(!enterMode){
                            odds = {1,2,0,0,0};
                            type = 4;
                        }
                        break;
                    }
                    case SDLK_5:
                    {
                        if(!enterMode){
                            odds = {1,0,0,0,0};
                            type = 5;
                        }
                        break;
                    }
                    case SDLK_6:
                    {
                        if(!enterMode){
                            odds = {1,1,0,0,0};
                            type = 6;
                        }
                        break;
                    }
                    case SDLK_7:
                    {
                        if(!enterMode){
                            odds = {1,0,0,0,0};
                            type = 7;
                        }
                        break;
                    }
                    case SDLK_8:
                    {
                        if(!enterMode){
                            odds = {1,1,0,0,0};
                            type = 8;
                        }
                        break;
                    }

                    case SDLK_9:
                    {
                        if(!enterMode){
                            odds = {1,0,1,0,0};
                            type = 9;
                        }
                        break;
                    }
                    //**********************************************

                    case SDLK_0:
                    {
                        if(!enterMode){
                            type = 10;
                            text = "y = |2 * |x| -1|";
                        }
                        break;
                    }
                    case SDLK_MINUS:
                     {
                        if(!enterMode){
                            type = 11;
                            text = "y = |(|x| - 1)^2)-1|";
                        }
                        break;
                    }
                   case SDLK_EQUALS:
                    {
                        if(!enterMode){
                            type = 12;
                            text = "y = sqrt(1 - 3 * |x|)";
                        }
                        break;
                    }
                }
            }
            if(needUpdate){
                switch(type){
                    case 1:{
                        //stringstream textbuff;
                        //textbuff.precision(2);
                        //textbuff << fixed << "y = " << odds[0] << "x + " << odds[1];
                        //textbuff << "y = " << odds[0] << "x + " << odds[1];
                        //text = textbuff.str();
                        text = "y =" + removeZero(odds[0]) + "x + " + removeZero(odds[1]);
                        break;
                    }
                    case 2:{
                        text = "y = " + removeZero(odds[0]) + "|x + " + removeZero(odds[1]) +"| + " +removeZero(odds[2]);
                        break;
                    }
                    case 3:{
                        text = "y = " + removeZero(odds[0]) + "x^" + removeZero(odds[1]) + " + " + removeZero(odds[2]) + "x" + " + " + removeZero(odds[3]);
                        break;
                    }
                    case 4:{
                        text = "y =" + removeZero(odds[0]) + "* "+removeZero(odds[1]) + "^x + " + removeZero(odds[2]);
                        break;
                    }
                    case 5:{
                        text = "y =" + removeZero(odds[0]) + " * 1/(x + " + removeZero(odds[1]) + ") + " + removeZero(odds[2]);
                        break;
                    }
                    case 6:{
                        text = "y =" + removeZero(odds[0]) + " * log2("+removeZero(odds[1])+ "x + " + removeZero(odds[2]) + ") + " + removeZero(odds[3]);
                        break;
                    }
                    case 7:{
                        text = "y =" + removeZero(odds[0]) + " * Sin(x + "+ removeZero(odds[1]) + ") + " + removeZero(odds[2]);
                        break;
                    }
                    case 8:{
                        text = "y =" + removeZero(odds[0]) + " * Tan(" + removeZero(odds[1]) + "x + "+ removeZero(odds[2]) + ") + " + removeZero(odds[3]);   
                        break;
                    }
                    case 9:{
                        text = "y = |" + removeZero(odds[0]) + " * (|x| + " + removeZero(odds[1]) + ")^" + removeZero(odds[2]) + " + " + removeZero(odds[3]) + "|";
                        break;
                    }
                    //************************************************************************
                    case 10:{
                        text = "♥сердце♥ y = (1-((|x|-1)^2)^0.5); y = (acos(1-|x|) - л)";
                        cout << "heart \n (1-((|x|-1)^2)^0.5) \n " << endl;
                        break;
                    }
                    case 11:{
                        text = "◊астроида◊ x = r*cos(t)^3; y = r*sin(t)^3, где t - параметр, r - радиус";
                        break;
                    }
                    case 12:{
                        text = "Ҩспираль АрхимедаҨ x = r*cos(t); y = r*sin(t), где t - параметр, r - радиус";
                        break;
                    }

                }
            }

            // Рисуем график по запросу пользователя
            //if(n >= 3|| needUpdate){
                drawCL(renderer, 2*maxOffset+50,scale, xoffset, yoffset);
                drawGraph(renderer, customFunction, 2*maxOffset+20, 0.05,scale,type, xoffset, yoffset);//работает даже со step = 0.001 без лагов благодаря оптимизации!
                drawConsole(renderer);
                //
                MenuTexture textTexture = tf(font, text, renderer, textColor);
                renderText(renderer,textTexture.mTexture, 40,40);
                MenuTexture textTextureMode = tf(font, textMode, renderer, textModeColor);
                renderText(renderer,textTextureMode.mTexture, WINDOW_WIDTH-textTextureMode.w-20,WINDOW_HEIGHT-WINDOW_HEIGHT/6+10);
                if(enterMode){
                    inputTextTexture = createTextTexture(renderer,inputText, textColor);
                    //inputTextTexture = tf(font, inputText, renderer, textColor);
                    renderText(renderer,inputTextTexture, 0+20, WINDOW_HEIGHT-WINDOW_HEIGHT/6+WINDOW_HEIGHT/24+25);
                }
                
                /*
                MenuTexture txt1 = tf(font, "РУССКИЙ текст", renderer, textColor);
                
                SDL_Rect qd1 = {
				WINDOW_WIDTH/2-txt1.w/2,
				250+txt1.h,
				txt1.w,
				txt1.h};

                renderText(renderer,txt1.mTexture, WINDOW_WIDTH/2,WINDOW_HEIGHT/2);

                SDL_RenderCopyEx( renderer, txt1.mTexture, NULL, &qd1, 0, NULL, SDL_FLIP_NONE );
                */

                SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);                
                SDL_RenderPresent(renderer);
                n = 0;
            //}
            n++;
            SDL_Delay(1);
            //отчищаем экран
            SDL_RenderClear(renderer);
            needUpdate = false;
        }
    }

    // Освобождение ресурсов
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
