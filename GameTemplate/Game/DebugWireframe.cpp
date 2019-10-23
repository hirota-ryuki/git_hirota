#include "stdafx.h"
#include "DebugWireframe.h"


DebugWireframe::DebugWireframe()
{
}


DebugWireframe::~DebugWireframe()
{
}

void DebugWireframe::Prepare()
{
	////頂点バッファ////
	//構造体
	//Description 意味:説明
	D3D11_BUFFER_DESC desc{};
	//使い方　読み方:ゆぅさぁじ
	desc.Usage = D3D11_USAGE_DEFAULT;
	//サイズ 読み方:バイトウィズ
	desc.ByteWidth = sizeof(Vertex) * 2;
	//頂点用にする
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//0はCPUからアクセスできない。
	//0なら書かなくって良い。
	desc.CPUAccessFlags = 0;
	//仕上げ
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&desc, nullptr, &m_vertexBuffer);
	////頂点終わりッッッ！！！////
	
	////定数「バッファ////」
	//ビュー行列とプロジェクション行列を送る用
	//サイズ 読み方:バイトウィズ
	desc.ByteWidth = sizeof(CMatrix);
	//定数（CONSTANT）用にする
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//仕上げ
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&desc, nullptr, &m_constantBuffer);
	////定数終わりッッッ！！！////
	
	//シェーダーの設定
	//wchar_tじゃないからLはいらない
	m_Vshader.Load("shader/DebugMode.fx", "VSMain", Shader::EnType::VS);
	m_Pshader.Load("shader/DebugMode.fx", "PSMain", Shader::EnType::PS);
}



void DebugWireframe::Context()
{
	//デバイスコンテキストを取得
	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	
	//頂点シェーダーをデバイスコンテキストに設定
	//Shaderは先生のラッパークラス
	//実際に使うGetBodyで取得したm_shaderはvoid*なので
	//ID3D11VertexShader*にキャストする必要がある
	d3dDeviceContext->VSSetShader((ID3D11VertexShader*) m_Vshader.GetBody(), nullptr, 0);

	//ピクセルシェーダーをデバイスコンテキストに設定
	//頂点シェーダーと同様
	d3dDeviceContext->PSSetShader((ID3D11PixelShader*) m_Pshader.GetBody(), nullptr, 0);

	//定数バッファの更新
	//mVPの更新
	//ビューとプロジェクションの掛け算
	mVP.Mul(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	//定数バッファをデバイスコンテキストに設定
	d3dDeviceContext->UpdateSubresource(m_constantBuffer, 0, nullptr, &mVP, 0, 0);

	//頂点バッファの更新

}

//1フレーム内にdrawLineは線の数だけ行う
void DebugWireframe::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{

}