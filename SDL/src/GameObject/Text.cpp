#include"Text.h"

#include "TextureManager.h"
#include "ResourceManagers.h"
Text::Text( std::string text, TTF_Font* font, SDL_Color color) :m_text(text), m_font(font), m_color(color)
{
	Init();
}

void Text::Init()
{
	m_position = Vector3(-1.0f, 1.0f, 1.0f);
	m_pTexture = std::make_shared<TextureManager>();
	GetTexture();
}

void Text::Draw(SDL_Renderer* renderer, SDL_Rect* clip)
{
	if (m_pTexture != nullptr)
	{
		SDL_Rect dstRect = { m_position.x  ,m_position.y , m_iWidth, m_iHeight };
		if (clip != NULL)
		{
			dstRect.w = clip->w;
			dstRect.h = clip->h;
		}
		SDL_RenderCopy(Renderer::GetInstance()->GetRenderer(), m_pTexture->GetTextureObj(), clip, &dstRect);//, m_angle, nullptr, m_flip);
	}
}

void Text::Update(float deltatime)
{

}

void Text::GetTexture()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->LoadTextureText(m_font, m_color, m_text);
	}
	else
	{
		printf("Error!!!Can not get texture!!!!");
	}
}

void Text::SetText(std::string text)
{
	m_text = text;
	GetTexture();
}

void Text::SetFont(TTF_Font* font)
{
	m_font = font;
	GetTexture();
}

void Text::SetColor(SDL_Color color)
{
	m_color = color;
	GetTexture();
}


