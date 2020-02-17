/*!
 * @brief	フォント。
 */
#include "stdafx.h"
#include "graphics/font/Font.h"

Font::Font()
{
	m_spriteBatch = GraphicsEngine().GetSpriteBatch();
	m_spriteFont = GraphicsEngine().GetSpriteFont();
	auto& ge = GraphicsEngine();
	m_scaleMat.MakeScaling(
		{ 
			ge.GetFrameBufferWidth() / static_cast<float>(ge.Get2DSpaceScreenWidth()), 
			ge.GetFrameBufferHeight() / static_cast<float>(ge.Get2DSpaceScreenHeight()), 
			1.0f 
		}
	);
}
Font::~Font()
{
}

void Font::Begin()
{
	//レンダリングステートを退避させる。
	//問題が起きたら実装してね。
	//auto* deviceContext = g_graphicsEngine->GetD3DDeviceContext();

	m_spriteBatch->Begin(
		DirectX::SpriteSortMode_Deferred,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		m_scaleMat
	);
}
void Font::End()
{
	m_spriteBatch->End();
	//レンダリングステートを復活させる。
	//問題が起きたら実装してね。
}
void Font::Draw(
	wchar_t const* text,
	const CVector2& position,
	const CVector4& color,
	float rotation,
	float scale,
	CVector2 pivot)
{		
	if (text == nullptr) {
		return;
	}
	pivot.y = 1.0f - pivot.y;
	DirectX::XMFLOAT2 tkFloat2Zero(0, 0);
	//座標系をスプライトと合わせる。
	CVector2 pos = position;
	float frameBufferHalfWidth = GraphicsEngine().Get2DSpaceScreenWidth() * 0.5f;
	float frameBufferHalfHeight = GraphicsEngine().Get2DSpaceScreenHeight() * 0.5f;
	pos.x += frameBufferHalfWidth;
	pos.y = -pos.y + frameBufferHalfHeight;

		
	//if (m_isDrawShadow) {
	//	//影を書く。
	//	CVector2 offsetTbl[] = {
	//		{ m_shadowOffset , 0.0f},
	//		{ -m_shadowOffset , 0.0f },
	//		{ 0.0f , m_shadowOffset },
	//		{ 0.0f , -m_shadowOffset },

	//		{ m_shadowOffset ,  m_shadowOffset },
	//		{ m_shadowOffset ,  -m_shadowOffset },
	//		{ -m_shadowOffset , m_shadowOffset },
	//		{ -m_shadowOffset , -m_shadowOffset },
	//	};
	//	for (auto offset : offsetTbl) {

	//		CVector2 sPos = pos;
	//		sPos.x += offset.x;
	//		sPos.y += offset.y;
	//		m_spriteFont->DrawString(
	//			m_spriteBatch,
	//			text,
	//			sPos.vec,
	//			m_shadowColor,
	//			rotation,
	//			DirectX::XMFLOAT2(pivot.x, pivot.y),
	//			scale
	//		);
	//	}

	//}
	m_spriteFont->DrawString(
		m_spriteBatch,
		text,
		pos.vec,
		color,
		rotation,
		DirectX::XMFLOAT2(pivot.x, pivot.y),
		scale
	);
}
