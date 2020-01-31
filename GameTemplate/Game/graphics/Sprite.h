#pragma once

#include "graphics/shader.h"

struct SVertex {
	float position[4];		//頂点座標。4要素なのは今は気にしない。
	float uv[2];			//UV座標。これがテクスチャ座標
};

class Sprite {
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Sprite::Sprite();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	Sprite::~Sprite();
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="textureFilePath">テクスチャのファイルパス。</param>
	void Init( const wchar_t* textureFilePath, float w, float h );
	void Init(ID3D11ShaderResourceView * srv, float w, float h);
	void InitCommon(float w, float h);
	void InitVertexBuffer(float w, float h);
	void InitIndexBuffer();
	/// <summary>
	/// ワールド行列を更新。
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	void UpdateWorldMatrix( CVector3 pos, CQuaternion rot, CVector3 scale);
	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="mView">カメラ行列</param>
	/// /// <param name="mView">プロジェクション行列</param>
	void Draw( CMatrix mView, CMatrix mProj );
	void Draw();
	void InitSamplerState();
	void InitConstantBuffer();
	/// <summary>
	/// アルファ値をセット。
	/// </summary>
	/// <param name="m_alpha">アルファ値</param>
	void SetAlpha(float alpha)
	{
		m_alpha = alpha;
	}
private:
	/// <summary>
	/// シェーダーをロード。
	/// </summary>
	void LoadShader();
	/// <summary>
	/// 頂点バッファを作成。
	/// </summary>
	void CreateVertexBuffer(float w, float h);
	/// <summary>
	/// インデックスバッファを作成。
	/// </summary>
	void CreateIndexBuffer();
	/// <summary>
	/// サンプラステートを作成。
	/// </summary>
	void CreateSamplerState();
	/// <summary>
	/// 定数バッファの作成。
	/// </summary>
	void CreateConstantBuffer();
	/// <summary>
	/// テクスチャをロ―ド。
	/// </summary>
	/// <param name="textureFIlePath">ロードするテクスチャのファイルパス。</param>
	void LoadTexture(const wchar_t* textureFIlePath);
private:
	struct ConstantBuffer {
		CMatrix WVP;		//ワールドビュープロジェクション行列。
		float alpha;		//α値。
	};
	Shader	m_vs;											//頂点シェーダー。
	Shader	m_ps;											//ピクセルシェーダー。
	ID3D11Buffer*	m_vertexBuffer = nullptr;	//VRAM上の頂点バッファにアクセスするためのインターフェース。
	ID3D11Buffer*	m_indexBuffer = nullptr;		//VRAM上のインデックスバッファにアクセスするためのインターフェース。
	ID3D11Buffer*	m_cbGPU = nullptr;			//GPU側の定数バッファにアクセスするためのインターフェース。
	ID3D11ShaderResourceView* m_texture = nullptr;	//テクスチャにアクセスするためのインターフェース。
	ID3D11SamplerState* m_samplerState = nullptr;	//サンプラステート。
	CMatrix m_world = CMatrix::Identity();					//ワールド行列。

	CVector2					m_size = CVector2::Zero();				//画像のサイズ。
	ID3D11Buffer*				m_cb = nullptr;							//定数バッファ。
	float						m_alpha = 1.0f;							//スプライトのα値。
};