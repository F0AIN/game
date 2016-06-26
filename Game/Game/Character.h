enum Direction { NORTH = 0, SOUTH = 1, EAST = 2, WEST = 3 };
class Character {
private:
	int *texture_id;	//�e�N�X�`��ID�̔z��
	int x;				//x���W
	int y;				//y���W
	Direction direction;//�L�����N�^�[�̌���
	
	/**
	 * �L�����N�^�[�̌������w�肷��֐�
	 * @param d ��������
	 */
	void face(Direction d);

	/**
	 * �����Ă������1���ړ�����֐�
	 */
	virtual void move(void);

	/**
	 * �ǂ��󂷊֐�
	 */
	virtual void breakWall(void);

public:
	Character() {}
	/**
	 * �R���X�g���N�^
	 * @param x		Character��x���W
	 * @param y		Character��y���W
	 * @param size	�e�N�X�`��ID�̔z��̃T�C�Y
	 */
	Character(int x, int y, int size);
	~Character();
	
	/**
	 * �������֐�
	 * @param x		Character��x���W
	 * @param y		Character��y���W
	 * @param size	�e�N�X�`��ID�̔z��̃T�C�Y
	 */
	virtual void init(int x, int y, int size);

	/**
	 * display�ɑΉ�����֐�
	 */
	virtual void drawDisplay(void) = 0;
	
	/**
	 * visibility�ɑΉ�����֐�
	 *
	 * @param state [�悭�킩��Ȃ�]
	 */
	virtual void visibility(int state) = 0;
	
	/**
	 * resize�ɑΉ�����֐�
	 * 
	 * @param w ���T�C�Y��̃E�B���h�E�̕�
	 * @param h ���T�C�Y��̃E�B���h�E�̍���
	 */
	virtual void resize(int w, int h) = 0;
	
	/**
	* mouse�ɑΉ�����֐�,�}�E�X����̓��͂�^����
	* 
	* @param button �����ꂽ�{�^���̎��
	* @param state  �{�^���̉������
	* @param x      �{�^���̉�������x���W
	* @param x      �{�^���̉�������y���W
	*/
	virtual void inputMouse(int button, int state, int x, int y) = 0;

	/**
	 * activeMotion�ɑΉ�����֐�,�}�E�X�̃{�^�����������̍��W��^����
	 * 
	 * @param x �{�^���������̃}�E�Xx���W
	 * @param x �{�^���������̃}�E�Xy���W
	 */
	virtual void inputActiveMotion(int x, int y) = 0;
	
	/**
	 * passiveMotion�ɑΉ�����֐�
	 * �}�E�X�̃{�^������������Ă��Ȃ����̍��W��^����
	 * 
	 * @param x �Ăяo�����̃}�E�Xx���W
	 * @param x �Ăяo�����̃}�E�Xy���W
	 */
	virtual void inputPassiveMotion(int x, int y) = 0;
	
	/**
	 * keyboard�ɑΉ�����֐��C�L�[�{�[�h�������̓��͂�^����
	 * 
	 * @param key �������ꂽ�L�[�̎��
	 * @param x   �L�[�{�[�h�������̃}�E�Xx���W
	 * @param x   �L�[�{�[�h�������̃}�E�Xy���W
	 */
	virtual void inputKeyboard(unsigned char key, int x, int y) = 0;

	/**
	 * keyboardup�ɑΉ�����֐��C�L�[�������ꂽ���̓��͂�^����
	 *
	 * @param key �����ꂽ�L�[�̎��
	 * @param x   �L�[�������ꂽ���̃}�E�Xx���W
	 * @param x   �L�[�������ꂽ���̃}�E�Xy���W
	 */
	virtual void inputKeyboardUp(unsigned char key, int x, int y) = 0;
	
	/**
	 * special�ɑΉ�����֐��C����L�[�������̓��͂�^����
	 *
	 * @param key �������ꂽ����L�[�̎��
	 * @param x   ����L�[�������̃}�E�Xx���W
	 * @param x   ����L�[�������̃}�E�Xy���W
	 */
	virtual void inputSpecialKey(unsigned char key, int x, int y) = 0;
	
	/**
 	 * specialup�ɑΉ�����֐��C����L�[�������ꂽ���̓��͂�^����
	 *
	 * @param key �����ꂽ����L�[�̎��
	 * @param x   ����L�[�������ꂽ���̃}�E�Xx���W
	 * @param x   ����L�[�������ꂽ���̃}�E�Xy���W
	 */
	virtual void inputSpecialKeyUp(unsigned char key, int x, int y) = 0;
	
	/**
	 * idle�ɑΉ�����֐�
	 */
	virtual void idle(void) = 0;
};
