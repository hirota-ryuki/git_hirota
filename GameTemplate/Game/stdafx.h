#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <D3Dcompiler.h>
#include <Windows.h>
#include <d3d11.h>
#include <map>
#include <functional>
#include <algorithm>

#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

//DirectXTKのインクルードファイル。
#include "DirectXTK/Inc/Model.h"
#include "DirectXTK/Inc/Effects.h"	
#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteFont.h"
#include "DirectXTK/Inc/DDSTextureLoader.h"

#include "math/Vector.h"

#include "physics/Physics.h"

#include "HID/Pad.h"

#include "math/Matrix.h"
#include "graphics/GraphicsEngine.h"

#include "graphics/SkinModel.h"
#include "graphics/Skeleton.h"

#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/Camera.h"
#include "graphics/Sprite.h"
#include "graphics/font/Font.h"

#include <xaudio2.h>
#include <x3daudio.h>
#include <xaudio2fx.h>
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"


const float FRAME_BUFFER_W = 1280.0f;				//フレームバッファの幅。
const float FRAME_BUFFER_H = 720.0f;				//フレームバッファの高さ。

#include "ShadowMap.h"
#include "IGameObject.h"
#include "GameObjectManager.h"
#include "SkinModelRender.h"
#include "SpriteRender.h"
#include "FontRender.h"
#include "RenderTarget.h"
#include "Inventory.h"
#include "Serif.h"
#include "IItem.h"
#include "Game.h"

static const int MAX_BONE = 512;	//!<ボーンの最大数。
