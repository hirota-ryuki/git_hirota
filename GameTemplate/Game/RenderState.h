/*!
 * @brief	�ǂ��g�������_�����O�X�e�[�g���܂Ƃ߂����́B
 */

#pragma once

/*!
* @brief	�[�x�X�e���V���X�e�[�g�B�B
*/
struct DepthStencilState
{
	static ID3D11DepthStencilState* disable;			//!<���ׂĖ����B
	static ID3D11DepthStencilState* SceneRender;		//!<3D���f����`�悷�鎞�̊�{�I�Ȑ[�x�X�e���V���X�e�[�g�B
														//!<�[�x�e�X�g�A�[�x�������݂Ƃ��ɗL���ȃX�e�[�g�ł��B
	static ID3D11DepthStencilState* spriteRender;		//!<2D�`�悷�鎞�̊�{�I�Ȑ[�x�X�e���V���X�e�[�g�B
														//!<�[�x�e�X�g�A�[�x�������݂Ƃ��ɖ����ȃX�e�[�g�ł��B
	static ID3D11DepthStencilState* defferedRender;		//!<�f�B�t�@�[�h�����_�����O���s���Ƃ��̐[�x�X�e���V���X�e�[�g�B
														//!<�[�x�e�X�g�����A�[�x�������ݗL���ȃX�e�[�g�ł��B
	/*!
	* @brief	�������B
	*@param[in]	ge		�O���t�B�b�N�G���W��
	*/
	static void Init(GraphicsEngine& ge);
};