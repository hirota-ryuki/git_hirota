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


bool hoge = false;
void DebugWireframe::Context()
{
	//デバイスコンテキストにモデルクラス等の設定が
	//残っているため上書きする（更新）
	hoge = true;
	
}

//1フレーム内にdrawLineは線の数だけ行う
void DebugWireframe::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{
	if (hoge == false) {
		return;
	}
//	hoge = false;
	//デバイスコンテキストを取得
	ID3D11DeviceContext* dc = g_graphicsEngine->GetD3DDeviceContext();
	
	//頂点シェーダーをデバイスコンテキストに設定
	//Shaderは先生のラッパークラス
	//実際に使うGetBodyで取得したm_shaderはvoid*なので
	//ID3D11VertexShader*にキャストする必要がある
	dc->VSSetShader((ID3D11VertexShader*) m_Vshader.GetBody(), nullptr, 0);

	//ピクセルシェーダーをデバイスコンテキストに設定
	//頂点シェーダーと同様
	dc->PSSetShader((ID3D11PixelShader*) m_Pshader.GetBody(), nullptr, 0);

	//定数バッファの設定
	//mVPの更新
	CMatrix VP;		//ビュー行列とプロジェクション行列
	//ビューとプロジェクションの掛け算
	auto v = g_camera3D.GetViewMatrix();
	auto p = g_camera3D.GetProjectionMatrix();
	VP.Mul(v, p);
	//定数バッファに渡したい変数を格納(m_constantBufferの内容を上書き)
	dc->UpdateSubresource(m_constantBuffer, 0, nullptr, &VP, 0, 0);
	//定数バッファをデバイスコンテキストに設定
	dc->VSSetConstantBuffers(
		0,//レジスタの場所
		1,//配列の要素数
		&m_constantBuffer//定数バッファのポインタ
	);

	//頂点バッファの設定
	//引数のストライドとオフセット用に変数をつくる
	//引数がポインタのため
	UINT ver = sizeof(Vertex);
	UINT offset = 0;
	//定数バッファをデバイスコンテキストに設定
	dc->IASetVertexBuffers(
		0,					//レジスタの場所
		1,					//配列の要素数
		&m_vertexBuffer,	//定数バッファのポインタ
		&ver,				//頂点のサイズ,ストライド
		&offset				//頂点の配列のスタート地点,オフセット
	);

	//頂点の情報がどんな感じに入っているのかがわかる
	dc->IASetInputLayout(m_Vshader.GetInputLayout());

	//頂点をどんな感じに描画するのか
	//今回は頂点二つの間に線を描く設定
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//頂点を書き換えてドローする関数
	Vertex vers[2];
	vers[0].pos = from;
	vers[0].color = color;
	vers[1].pos = to;
	vers[1].color = color;

	//引数verはver[0]のアドレス
	//Prepare関数のByteWidthでサイズを*2にすることで
	//verの要素数が二つであることがわかる
	dc->UpdateSubresource(m_vertexBuffer, 0, nullptr, vers, 0, 0);

	//描画
	//2は頂点数、0はオフセット
	dc->Draw(2, 0);
}