#pragma once
#include "Character.hpp"
class Enemy : public Character {
protected:
	int x_width;	//�`�ʂ���ۂ�x�������̕�
	int y_width;	//�`�ʂ���ۂ�y�������̕�

	int player_x;	//�v���C���[�̍��W
	int player_y;	//�v���C���[�̍��W

	/**
	 * �����Ă�����ֈړ�����֐�
	 */
	void move(void);

	/**
	 * �ǂ��󂷊֐�
	 */
	void breakWall(void);

	/**
	 * ���`�B���s���֐�
	 */
	virtual void message(void);

public:
	static int enemy_number; //�G�̐�
	
	Enemy();

	/**
	 * �R���X�g���N�^
	 * @param x		Character��x���W
	 * @param y		Character��y���W
	 * @param size	�e�N�X�`��ID�̔z��̃T�C�Y
	 * @param x_w	x�������̕�
	 * @param y_w	y�������̕�
	 */
	Enemy(int x, int y, int size, int x_w, int y_w);
	~Enemy();

	/**
	* display�ɑΉ�����֐�
	*/
	virtual void drawDisplay(void);

	/**
	* visibility�ɑΉ�����֐�
	*
	* @param state [�悭�킩��Ȃ�]
	*/
	virtual void visibility(int state);

	/**
	* resize�ɑΉ�����֐�
	*
	* @param w ���T�C�Y��̃E�B���h�E�̕�
	* @param h ���T�C�Y��̃E�B���h�E�̍���
	*/
	virtual void resize(int w, int h);

	/**
	* mouse�ɑΉ�����֐�,�}�E�X����̓��͂�^����
	*
	* @param button �����ꂽ�{�^���̎��
	* @param state  �{�^���̉������
	* @param x      �{�^���̉�������x���W
	* @param x      �{�^���̉�������y���W
	*/
	virtual void inputMouse(int button, int state, int x, int y);

	/**
	* activeMotion�ɑΉ�����֐�,�}�E�X�̃{�^�����������̍��W��^����
	*
	* @param x �{�^���������̃}�E�Xx���W
	* @param x �{�^���������̃}�E�Xy���W
	*/
	virtual void inputActiveMotion(int x, int y);

	/**
	* passiveMotion�ɑΉ�����֐�
	* �}�E�X�̃{�^������������Ă��Ȃ����̍��W��^����
	*
	* @param x �Ăяo�����̃}�E�Xx���W
	* @param x �Ăяo�����̃}�E�Xy���W
	*/
	virtual void inputPassiveMotion(int x, int y);

	/**
	* keyboard�ɑΉ�����֐��C�L�[�{�[�h�������̓��͂�^����
	*
	* @param key �������ꂽ�L�[�̎��
	* @param x   �L�[�{�[�h�������̃}�E�Xx���W
	* @param x   �L�[�{�[�h�������̃}�E�Xy���W
	*/
	virtual void inputKeyboard(unsigned char key, int x, int y);

	/**
	* keyboardup�ɑΉ�����֐��C�L�[�������ꂽ���̓��͂�^����
	*
	* @param key �����ꂽ�L�[�̎��
	* @param x   �L�[�������ꂽ���̃}�E�Xx���W
	* @param x   �L�[�������ꂽ���̃}�E�Xy���W
	*/
	virtual void inputKeyboardUp(unsigned char key, int x, int y);

	/**
	* special�ɑΉ�����֐��C����L�[�������̓��͂�^����
	*
	* @param key �������ꂽ����L�[�̎��
	* @param x   ����L�[�������̃}�E�Xx���W
	* @param x   ����L�[�������̃}�E�Xy���W
	*/
	virtual void inputSpecialKey(unsigned char key, int x, int y);

	/**
	* specialup�ɑΉ�����֐��C����L�[�������ꂽ���̓��͂�^����
	*
	* @param key �����ꂽ����L�[�̎��
	* @param x   ����L�[�������ꂽ���̃}�E�Xx���W
	* @param x   ����L�[�������ꂽ���̃}�E�Xy���W
	*/
	virtual void inputSpecialKeyUp(unsigned char key, int x, int y);

	/**
	* idle�ɑΉ�����֐�
	*/
	virtual void idle(void);
};

int Enemy::enemy_number = 0;