#pragma once
#include "Character.hpp"
class Player : public Character {
protected:
	int x_width;	//�`�ʂ���ۂ�x�������̕�
	int y_width;	//�`�ʂ���ۂ�y�������̕�

	/**
	 * �����Ă�����ֈړ�����֐�
	 */
	void move(void);

	/**
	 * �ǂ��󂷊֐�
	 */
	void breakWall(void);
public:
	Player();
	/**
	* �R���X�g���N�^
	* @param x		Character��x���W
	* @param y		Character��y���W
	* @param size	�e�N�X�`��ID�̔z��̃T�C�Y
	* @param x_w	x�������̕�
	* @param y_w	y�������̕�
	*/
	Player(int x, int y, int size, int x_w, int y_w);
	~Player();

	/**
	* display�ɑΉ�����֐�
	*/
	void drawDisplay(void);

	/**
	* visibility�ɑΉ�����֐�
	*
	* @param state [�悭�킩��Ȃ�]
	*/
	void visibility(int state);

	/**
	* resize�ɑΉ�����֐�
	*
	* @param w ���T�C�Y��̃E�B���h�E�̕�
	* @param h ���T�C�Y��̃E�B���h�E�̍���
	*/
	void resize(int w, int h);

	/**
	* mouse�ɑΉ�����֐�,�}�E�X����̓��͂�^����
	*
	* @param button �����ꂽ�{�^���̎��
	* @param state  �{�^���̉������
	* @param x      �{�^���̉�������x���W
	* @param x      �{�^���̉�������y���W
	*/
	void inputMouse(int button, int state, int x, int y);

	/**
	* activeMotion�ɑΉ�����֐�,�}�E�X�̃{�^�����������̍��W��^����
	*
	* @param x �{�^���������̃}�E�Xx���W
	* @param x �{�^���������̃}�E�Xy���W
	*/
	void inputActiveMotion(int x, int y);

	/**
	* passiveMotion�ɑΉ�����֐�
	* �}�E�X�̃{�^������������Ă��Ȃ����̍��W��^����
	*
	* @param x �Ăяo�����̃}�E�Xx���W
	* @param x �Ăяo�����̃}�E�Xy���W
	*/
	void inputPassiveMotion(int x, int y);

	/**
	* keyboard�ɑΉ�����֐��C�L�[�{�[�h�������̓��͂�^����
	*
	* @param key �������ꂽ�L�[�̎��
	* @param x   �L�[�{�[�h�������̃}�E�Xx���W
	* @param x   �L�[�{�[�h�������̃}�E�Xy���W
	*/
	void inputKeyboard(unsigned char key, int x, int y);

	/**
	* keyboardup�ɑΉ�����֐��C�L�[�������ꂽ���̓��͂�^����
	*
	* @param key �����ꂽ�L�[�̎��
	* @param x   �L�[�������ꂽ���̃}�E�Xx���W
	* @param x   �L�[�������ꂽ���̃}�E�Xy���W
	*/
	void inputKeyboardUp(unsigned char key, int x, int y);

	/**
	* special�ɑΉ�����֐��C����L�[�������̓��͂�^����
	*
	* @param key �������ꂽ����L�[�̎��
	* @param x   ����L�[�������̃}�E�Xx���W
	* @param x   ����L�[�������̃}�E�Xy���W
	*/
	void inputSpecialKey(unsigned char key, int x, int y);

	/**
	* specialup�ɑΉ�����֐��C����L�[�������ꂽ���̓��͂�^����
	*
	* @param key �����ꂽ����L�[�̎��
	* @param x   ����L�[�������ꂽ���̃}�E�Xx���W
	* @param x   ����L�[�������ꂽ���̃}�E�Xy���W
	*/
	void inputSpecialKeyUp(unsigned char key, int x, int y);

	/**
	* idle�ɑΉ�����֐�
	*/
	void idle(void);
};

