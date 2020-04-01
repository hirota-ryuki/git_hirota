#include "stdafx.h"
#include "Navimesh.h"
#include <thread>
#include "Floor.h"

/*!
	*@brief	ストップウォッチクラス。
	*@details
	* C#のStopwatchのように使える。</br>
	*@code
	サンプルコード
	Stopwatch sw;
	sw.Start();		//計測開始。
	for(int i = 0; i < 100; i++ ){
	}
	sw.Stop();		//計測終了
	printf("経過時間 = %lf(単位：秒)\n", sw.GetElapsed());
	printf("経過時間 = %lf(単位：ミリ)\n", sw.GetElapsedMillisecond());
	*@endcode
	*
	*/
class CStopwatch {
public:
	/*!
		*@brief	コンストラクタ。
		*/
	CStopwatch()
	{
		freq = 0;
		end = 0;
		begin = 0;
		elapsed = 0.0;
		elapsedMill = 0.0;
		elapsedMicro = 0.0;
	}
	/*!
		*@brief	デストラクタ。
		*/
	~CStopwatch()
	{
	}
	/*!
		*@brief	計測開始。
		*/
	void Start()
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&begin);
	}
	/*!
		*@brief	計測終了
		*/
	void Stop()
	{
		::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		::QueryPerformanceCounter((LARGE_INTEGER*)&end);
		elapsed = double(end - begin) / freq;
		elapsedMill = elapsed * 1000.0;
		elapsedMicro = elapsedMill * 1000.0;
	}
	/*!
		*@brief	経過時間を取得(単位:秒)
		*/
	double GetElapsed() const
	{
		return elapsed;
	}
	/*!
		*@brief	経過時間を取得(単位:ミリ秒)
		*/
	double GetElapsedMillisecond() const
	{
		return elapsedMill;
	}
	/*!
		*@brief	経過時間を取得(単位:マイクロ秒)
		*/
	double GetElapsedMicrosecond() const
	{
		return elapsedMicro;
	}
private:
	LONGLONG freq;
	LONGLONG end;
	LONGLONG begin;
	double elapsed;			//経過時間(単位：秒)
	double elapsedMill;		//経過時間(単位：ミリ秒)
	double elapsedMicro;	//経過時間(単位：マイクロ秒)

};

//#define REBUILD_NAVIMESH_DATA		//これを定義するとナビメッシュのデータが再作成される。

#ifdef _DEBUG
	//#define USE_NAVIMESH_DEBUG	//これを定義すると、ナビメッシュのデバッグ表示が有効になる。
#else
	//#define USE_NAVIMESH_DEBUG	//これを定義すると、ナビメッシュのデバッグ表示が有効になる。
#endif
struct CallBack : public btCollisionWorld::ConvexResultCallback
{
	/// <summary>
	/// 備考
		//CallBackクラスのコンストラクタ
		//CallBack()
		//初期化リスト
		//{}の前に初期化を行う
		//namespace　btCollisionWorld
		//構造体　ClosestConvexResultCallback
		//ClosestConvexResultCallbackはコンストラクタに引数があるので
		//初期化しなくてはならない
		//{}
	/// </summary>
	
	//障害物があるかないか判定
	bool isHit = false;
	//衝突したら勝手に呼んでくれる
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//当たった
		isHit = true;
		return 0;
	}
};

Navimesh::Navimesh()
{
}

Navimesh::~Navimesh()
{
}

void Navimesh::Init(Floor* floor)
{
	//セルの当たり判定用のカプセルコライダー
	//初期化
	//半径が1であるのは後々に三角形の一番長い重心頂点間に拡大するため
//	m_collider.Create(1.f, 30.f);
#ifdef REBUILD_NAVIMESH_DATA
	Create(floor->GetSkinModelRender()->GetModel());
#else
	//アセットフォルダからナビメッシュのデータをロードする。
	FILE* fp = fopen("stage.hnv", "rb");
	if (fp == nullptr) {
		//ファイルオープンに失敗
		MessageBox(nullptr, "エラー", "stage.hnvが開けませんでした。", MB_OK);
	}
	int numCell;
	fread(&numCell, sizeof(int), 1, fp);
	//CellBinaryの配列を動的に確保
	CellBinary* cellBinarys = new CellBinary[numCell];
	fread(cellBinarys, sizeof(CellBinary) * numCell, 1, fp);

	//CellBinaryからCellの情報を構築する。
	for (int i = 0; i < numCell; i++) {
		//セルの生成
		Cell* cell = new Cell;

		for (int j = 0; j < 3; j++) {
			cell->vertexPos[j] = cellBinarys[i].vertexPos[j];
		}
		cell->centerPos = cellBinarys[i].centerPos;
		m_cells.push_back(cell);
	}

	for (int i = 0; i < numCell; i++) {
		for (int j = 0; j < 3; j++) {
			if (cellBinarys[i].linkCellNo[j] != -1) {
				m_cells[i]->linkCells[j] = m_cells[(cellBinarys[i].linkCellNo[j])];
			}
			else {
				m_cells[i]->linkCells[j] = NULL;
			}
		}
	}
	delete[] cellBinarys;
	fclose(fp);
#endif
#ifdef USE_NAVIMESH_DEBUG
	for (auto &all : m_cells)
	{
		m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
		m_model->Init(L"modelData/debug/debugbox.cmo");
		m_model->SetPos(all->centerPos);
		int No = 0;
		if (all->linkCells[No] != NULL)
		{
			//float dot = all->centerPos.Dot(all->linkCells[2]->centerPos);
			float dot = all->centerPos.x*all->linkCells[No]->centerPos.x +
				all->centerPos.y*all->linkCells[No]->centerPos.y +
				all->centerPos.z*all->linkCells[No]->centerPos.z;
			float k = atan(dot);
			CQuaternion g;
			//g.SetRotationDeg(CVector3::AxisX(), 45.0f);
			g.SetRotation(CVector3::AxisY(), k);
			m_model->SetRot(g);
		}
	}
#endif
}

void Navimesh::BuildLinkCellInfo(int startCellNo, int endCellNo)
{
	//登録されたセルの数だけ回す
	for (int cellNo = startCellNo; cellNo < endCellNo && cellNo < m_cells.size(); cellNo++ )
	{
		auto all = m_cells[cellNo];

		//同じセルと比較しないようにするためのループ
		for (auto &currentCell : m_cells)
		{

			//同じセルなら処理を除外する
			if (all != currentCell)
			{
				//座標が一致した頂点をカウント
				int countSameVertex = 0;
				//allの頂点の要素を進める
				for (int i = 0; i < 3; i++)
				{
					//allの頂点を格納
					CVector3 allVPos;
					allVPos = all->vertexPos[i];

					//currentCellの頂点の要素を進める
					for (int j = 0; j < 3; j++)
					{
						//allVPosとcurrentVPosの距離
						CVector3 diff;
						diff = allVPos - currentCell->vertexPos[j];
						//もし頂点どうしがほぼ同じ距離にいたら
						if (diff.Length() <= 0.01f)
						{
							//一致した頂点数をカウント
							countSameVertex++;
						}
					}
				}
				//２頂点が重複していたら、隣接しているとする。
				if (countSameVertex == 2) {
					all->linkCells[all->linkMax] = currentCell;
					all->linkMax++;

				}

			}
		}
	}
}

void Navimesh::Create(SkinModel& model)
{
	CMatrix mBias = model.GetWorldMatrix();
	
	//コールバック関数
	model.FindMesh([&](const auto& mesh) {
		ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
		//頂点バッファを作成。
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			HRESULT hr = deviceContext->Map(mesh->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
			if (FAILED(hr)) {
				return;
			}
			D3D11_BUFFER_DESC bufferDesc;
			mesh->vertexBuffer->GetDesc(&bufferDesc);
			int vertexCount = bufferDesc.ByteWidth / mesh->vertexStride;
			char* pData = reinterpret_cast<char*>(subresource.pData);
			VertexBufferPtr vertexBuffer = std::make_unique<VertexBuffer>();
			CVector3 pos;
			for (int i = 0; i < vertexCount; i++) {
				pos = *reinterpret_cast<CVector3*>(pData);
				//バイアスをかける。
				mBias.Mul(pos);
				vertexBuffer->push_back(pos);
				//次の頂点へ。
				pData += mesh->vertexStride;
			}
			//頂点バッファをアンロック
			deviceContext->Unmap(mesh->vertexBuffer.Get(), 0);
			m_vertexBufferArray.push_back(std::move(vertexBuffer));
		}
		//インデックスバッファを作成。
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			//インデックスバッファをロック。
			HRESULT hr = deviceContext->Map(mesh->indexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
			if (FAILED(hr)) {
				return;
			}
			D3D11_BUFFER_DESC bufferDesc;
			mesh->indexBuffer->GetDesc(&bufferDesc);
			//@todo cmoファイルはインデックスバッファのサイズは2byte固定。
			IndexBufferPtr indexBuffer = std::make_unique<IndexBuffer>();
			int stride = 2;
			int indexCount = bufferDesc.ByteWidth / stride;
			unsigned short* pIndex = reinterpret_cast<unsigned short*>(subresource.pData);
			for (int i = 0; i < indexCount; i++) {
				indexBuffer->push_back(pIndex[i]);
			}
			//インデックスバッファをアンロック。
			deviceContext->Unmap(mesh->indexBuffer.Get(), 0);
			m_indexBufferArray.push_back(std::move(indexBuffer));
		}
		}
	);

	//セルに頂点3つと重心を設定し、セルに登録
	//m_vertexBufferArrayとm_indexBufferArrayの要素数は同じ
	for (int i = 0; i < m_indexBufferArray.size(); i++) {
		//インデックスバッファの配列
		//頂点の場所
		auto& indexBuffer = m_indexBufferArray[i];
		//頂点バッファの配列
		//一つの頂点バッファにつき1パーツ
		auto& vertexBuffer = m_vertexBufferArray[i];
		
		//ポリゴンの数
		//ポリゴンは頂点3つで形成されるので3で割っている
		int numPoly = indexBuffer->size() / 3;
		//ポリゴンの数だけ回す
		for (int polyNo = 0; polyNo < numPoly; polyNo++) {
			//セルの生成
			Cell* cell = new Cell;

			//頂点の格納
			{
				//ポリゴン一個に対する3か所の頂点の場所を割り出す
				int vertNo_0 = indexBuffer->at(polyNo * 3);
				int vertNo_1 = indexBuffer->at(polyNo * 3 + 1);
				int vertNo_2 = indexBuffer->at(polyNo * 3 + 2);
				//セルに頂点の場所を格納
				cell->vertexPos[0] = vertexBuffer->at(vertNo_0);
				cell->vertexPos[1] = vertexBuffer->at(vertNo_1);
				cell->vertexPos[2] = vertexBuffer->at(vertNo_2);
			}

			//centerPosの登録
			//三角形の重心
			//合計なので初期化
			cell->centerPos.Zero();
			cell->centerPos = cell->vertexPos[0] + cell->vertexPos[1] + cell->vertexPos[2];
			cell->centerPos /= 3;

			//カプセルコライダーの半径を
			//三角形の一番長い重心頂点間の長さに拡大
			
			//重心から頂点までの長さを求める
			//これがカプセルコライダーの半径になる
			CVector3 radius[3];
			for (int i = 0; i < 3; i++)
			{
				radius[i] = cell->centerPos - cell->vertexPos[i];
			}
			//1番長い距離を比較で求める
			CVector3 maxLength;
			maxLength = radius[0];
			for (int i = 1; i < 3; i++)
			{
				if (maxLength.Length() < radius[i].Length())
				{
					maxLength = radius[i];
				}
			}
			//カプセルコライダーの半径を拡大
			CVector3 halfSize;
			halfSize.x = maxLength.Length() * 2.0f;
			halfSize.z = maxLength.Length() * 2.0f;
			halfSize.y = 30.0f;
			m_collider.Create(halfSize);

			//コリジョンの移動の始点と終点の設定
			//btTransformは座標、回転の情報を持っている
			//情報の初期化を行わなければならない
			btTransform start, end;
			{
				//回転の設定
				start.setIdentity();
				end.setIdentity();
				//座標の設定
				//Origin 意味：原点
				start.setOrigin(btVector3(cell->centerPos.x, cell->centerPos.y + 20.f, cell->centerPos.z));
				end.setOrigin(btVector3(cell->centerPos.x, cell->centerPos.y + 30.f, cell->centerPos.z));
			}

			CallBack callback;
			//startからendまでコリジョンを移動させて当たり判定を取る
			g_physics.ConvexSweepTest((btConvexShape*)m_collider.GetBody(), start, end, callback);
			
			if (callback.isHit == false) {
				//セルに登録
				m_cells.push_back(cell);
			}
		}
	}

	for (auto &all : m_cells)
	{
		for (int i = 0; i < 3; i++)
		{
			all->linkCells[i] = nullptr;
			all->costFromStart = 0.0f;
			all->costToGoal = 0.0f;
			all->parentCell = nullptr;
		}
	}
	//一つのスレッドで調べるセルの数を計算。
	CStopwatch sw;
	sw.Start();
	int numCellOneThread = m_cells.size() / 3;
#if 1
	//4スレッドに分担して隣接セル情報を構築する。
	//並列。
	std::thread buildLinkCellThread00([&] {
		BuildLinkCellInfo(0, numCellOneThread);
	});

	std::thread buildLinkCellThread01([&] {
		BuildLinkCellInfo(numCellOneThread, numCellOneThread*2);
	});

	std::thread buildLinkCellThread02([&] {
		BuildLinkCellInfo(numCellOneThread*2, numCellOneThread*3);
	});
	std::thread buildLinkCellThread03([&] {
		BuildLinkCellInfo(numCellOneThread*3, numCellOneThread*4);
	});

	buildLinkCellThread00.join();
	buildLinkCellThread01.join();
	buildLinkCellThread02.join();
	buildLinkCellThread03.join();
#else
	//直列。
	BuildLinkCellInfo(0, numCellOneThread);
	BuildLinkCellInfo(numCellOneThread, numCellOneThread * 2);
	BuildLinkCellInfo(numCellOneThread * 2, numCellOneThread * 3);
	BuildLinkCellInfo(numCellOneThread * 3, numCellOneThread * 4);
#endif
#ifdef _DEBUG
	//時間計測。
	sw.Stop();
	char text[256];
	sprintf(text, "build time = %f\n", sw.GetElapsed());
	OutputDebugString(text);
#endif
#ifdef USE_NAVIMESH_DEBUG

	for (auto &all : m_cells)
	{
		m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
		m_model->Init(L"modelData/debug/debugbox.cmo");
		m_model->SetPos(all->centerPos);
		int No = 0;
		if (all->linkCells[No] != NULL)
		{
			//float dot = all->centerPos.Dot(all->linkCells[2]->centerPos);
			float dot = all->centerPos.x*all->linkCells[No]->centerPos.x +
				all->centerPos.y*all->linkCells[No]->centerPos.y +
				all->centerPos.z*all->linkCells[No]->centerPos.z;
			float k = atan(dot);
			CQuaternion g;
			//g.SetRotationDeg(CVector3::AxisX(), 45.f);
			g.SetRotation(CVector3::AxisY(), k);
			m_model->SetRot(g);
		}
		/*for (int i = 0; i < 3; i++) {
			if (all->linkCells[i] != nullptr) {
				auto dir = all->linkCells[i]->centerPos - all->centerPos;
				dir.Normalize();
				auto hoge = NewGO<SkinModelRender>(GOPrio_Defalut);
				m_model->Init(L"modelData/zombie/zombie.cmo");
				hoge->SetPos(all->centerPos);
				CQuaternion rot;
				rot.SetRotation({ 0.0f, 1.0f, 0.0f }, dir);
				hoge->SetRot(rot);
			}
		}*/
	}
#endif //

	FILE* fp = fopen( "stage.hnv", "wb");
	if (fp == nullptr) {
		//ファイルオープンに失敗
		MessageBox(nullptr, "エラー", "stage.hnvが開けませんでした。", MB_OK);
	}
	int cellNum = m_cells.size();
	fwrite(&cellNum, sizeof(int), 1, fp);
	for (auto &cell : m_cells)
	{
		CellBinary cellBinary;
		//for (int i = 0; i < 3; i++) {
		//	auto itr = std::find(m_cells.begin(), m_cells.end(), cell->linkCells[i]);
		//	//size_t index = std::distance(m_cells.begin(), itr);
		//	/*if (index != m_cells.size()) {
		//		cellBinary.linkCellNo[i] = index;
		//	}
		//	else {
		//		cellBinary.linkCellNo[i] = -1;
		//	}*/
		//}

		//頂点。
		for (int i = 0; i < 3; i++) {
			cellBinary.vertexPos[i] = cell->vertexPos[i];
		}
		//隣接セル。
		for (int i = 0; i < 3; i++) {
			int count = 0;
			if (cell->linkCells[i] == nullptr) {
				cellBinary.linkCellNo[i] = -1;
			}
			else {
				for (auto itr = m_cells.begin(); itr != m_cells.end(); ++itr) {
					if (cell->linkCells[i] == *itr) {
						cellBinary.linkCellNo[i] = count;
						break;
					}
					else if (itr == m_cells.end()) {
						cellBinary.linkCellNo[i] = -1;
					}
					count++;
				}
			}
		}

		//中心
		cellBinary.centerPos = cell->centerPos;

		fwrite(&cellBinary, sizeof(cellBinary), 1, fp);
	}
	fclose(fp);
}

bool Navimesh::Start()
{
	return true;
}