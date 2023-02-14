#include "../header/Text.h"
#include "../header/Resources.h"
#include "../header/Camera.h"
#include "../header/Sprite.h"

Text::Text(GameObject& associated, const char* fontFile, int fontSize,
           TextStyle style, const char* text, SDL_Color color, 
           unsigned wrappingLength, float timeToBlink) : Component(associated)
{
    this->fontFile = fontFile;
    this->fontSize = fontSize;
    this->style = style;
    this->text = text;
    this->color = color;
    this->timeToBlink = timeToBlink;
    this->showText = true;
    this->texture = nullptr;
    this->font = nullptr;
    this->wrappingLength = wrappingLength;
    RemakeTexture();
}

Text::~Text()
{
    if (texture != nullptr)
        SDL_DestroyTexture(texture);
}

void Text::Update(float dt)
{
    if (timeToBlink > 0)
    {
        timer.Update(dt);
        if (timer.Get() >= timeToBlink)
        {
            showText = !showText;
            timer.Restart();
        }
    }
}

void Text::Render()
{
    if (!showText)
        return;

    SDL_Rect clipRect;
    clipRect.x = 0;
    clipRect.y = 0;
    clipRect.w = associated.box.w;
    clipRect.h = associated.box.h;

    SDL_Rect dstRect;
    dstRect.x = associated.box.x - Camera::pos.x;
    dstRect.y = associated.box.y - Camera::pos.y;
    dstRect.w = clipRect.w;
    dstRect.h = clipRect.h;

    if (SDL_RenderCopyEx(
        Game::GetInstance().GetRenderer(),
        texture,
        &clipRect,
        &dstRect,
        associated.angleDeg * DEG, // 0° to 360° clockwise
        nullptr,
        SDL_FLIP_NONE) < 0)
    {
        cout << "Error rendering text" << endl;
        cout << SDL_GetError() << endl;
        exit(1);
    }
}

bool Text::Is(const char* type)
{
    string str_type = type;
    return str_type == "Text";
}

void Text::SetText(const char* text)
{
    this->text = text;
    RemakeTexture();
}

void Text::SetColor(SDL_Color color)
{
    this->color = color;
    RemakeTexture();
}

void Text::SetStyle(TextStyle style)
{
    this->style = style;
    RemakeTexture();
}

void Text::SetFontFile(const char* fontFile)
{
    this->fontFile = fontFile;
    RemakeTexture();
}

void Text::SetFontSize(int fontSize)
{
    this->fontSize = fontSize;
    RemakeTexture();
}

void Text::SetWrappingLength(unsigned int length)
{
    this->wrappingLength = length;
    RemakeTexture();
}

void Text::RemakeTexture()
{
    if (texture != nullptr)
        SDL_DestroyTexture(texture);

    font = Resources::GetFont(fontFile, fontSize);
    
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    
    switch (style)
    {
        case SOLID:
            surface = TTF_RenderUTF8_Solid_Wrapped(font, text, color, wrappingLength);
            break;
        case SHADED: // No support for shaded text
            // surface = TTF_RenderText_Shaded(font, text, color);
            break;
        case BLENDED:
            surface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, wrappingLength);
            break;
        default:
            break;
    }
    
    texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surface);
    SDL_FreeSurface(surface);

    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    associated.box.w = width;
    associated.box.h = height;
}
