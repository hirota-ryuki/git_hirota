/*!
 * @brief	コリジョン属性
 */
#pragma once

/*!
 * @brief	コリジョン属性の大分類。
 */
enum EnCollisionAttr{
	enCollisionAttr_Ground,
	enCollisionAttr_Character,
	enCollisionAttr_User,					//以下にユーザー定義のコリジョン属性を設定する。
	enCollisionAttr_Map,					//マップチップ用。
	enCollisionAttr_RigidBody,				//剛体をモデルで作成したときに使用。
	enCollisionAttr_PassingWallCharacter	//壁を通過するキャラクタ。
};