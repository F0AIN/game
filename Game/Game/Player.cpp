#include <iostream>

#include "Player.hpp"
#include "CircularSector.hpp"

#include <GL/glut.h>

using namespace std;

void Player::init(void)
{
	loadAnimations();
}

void Player::loadAnimations(void)
{
	animation = Animation("data/animations/Player.ani");
	animations.push_back(animation);
	Character::loadAnimations();
}

void Player::startPlacing(void)
{
	isPlacing = true;
}

Player::Player()
	: Character(), isChangingDirection(false), isPlacing(false)
{
	init();
}

Player::Player(const Vector<double>& position, const Size<double>& size, const Keypad& keypad)
	: Character(position, size), keypad(keypad), isChangingDirection(false), isPlacing(false)
{
	init();
}

Player::Player(const Vector<double>& position, const Keypad& keypad)
	: Character(position), keypad(keypad), isChangingDirection(false), isPlacing(false)
{
	init();
}


Player::~Player()
{
}

void Player::onUp(void)
{
	if (isMoving)
		return;
	if (isChangingDirection)
		return;
	startMoving();
}

void Player::onDown(void)
{
	if (isMoving)
		return;
	if (isChangingDirection)
		return;
	startMoving();
}

void Player::onLeft(void)
{
	if (isMoving)
		return;
	if (isChangingDirection)
		return;
	startMoving();
}

void Player::onRight(void)
{
	if (isMoving)
		return;
	if (isChangingDirection)
		return;
	startMoving();
}

void Player::onBreakBlock(void)
{

}

void Player::onPlaceWall(void)
{

}

void Player::onPlaceTrap(void)
{

}

void Player::onPlaceDecoy(void)
{

}

void Player::onUpDown(void)
{
	if (isMoving)
		return;
	if (direction != NORTH) {
		direction = NORTH;
		isChangingDirection = true;
		return;
	}
}

void Player::onDownDown(void)
{
	if (isMoving)
		return;
	if (direction != SOUTH) {
		direction = SOUTH;
		isChangingDirection = true;
		return;
	}
}

void Player::onLeftDown(void)
{
	if (isMoving)
		return;
	if (direction != WEST) {
		direction = WEST;
		isChangingDirection = true;
		return;
	}
}

void Player::onRightDown(void)
{
	if (isMoving)
		return;
	if (direction != EAST) {
		direction = EAST;
		isChangingDirection = true;
		return;
	}
}

void Player::onBreakBlockDown(void)
{
	startBreaking();
}

void Player::onPlaceWallDown(void)
{
	startPlacing();
}

void Player::onPlaceTrapDown(void)
{

}

void Player::onPlaceDecoyDown(void)
{

}

void Player::onUpUp(void)
{
	isChangingDirection = false;
}

void Player::onDownUp(void)
{
	isChangingDirection = false;
}

void Player::onLeftUp(void)
{
	isChangingDirection = false;
}

void Player::onRightUp(void)
{
	isChangingDirection = false;
}

void Player::onBreakBlockUp(void)
{

}

void Player::onPlaceWallUp(void)
{

}

void Player::onPlaceTrapUp(void)
{

}

void Player::onPlaceDecoyUp(void)
{

}

void Player::changeColor(void) const
{
	glColor3d(1.0, 1.0, 1.0);
}

void Player::draw(void)
{
	Character::draw();
	Size<double> size = getSize();
	double width = size.getWidth();
	double height = size.getHeight();
	glBegin(GL_LINE_LOOP);
	glVertex2d(0.0, 0.0);
	glVertex2d(width, 0.0);
	glVertex2d(width, height);
	glVertex2d(0.0, height);
	glEnd();
	//プレイヤーの向きを表示
	double angle = getAngle();
	double viewAngle = 0.0;
	double radius = width;
	CircularSector circularSector(Vector<double>(width / 2, height / 2), angle, viewAngle, radius);
	circularSector.draw();
}

Keypad Player::getKeypad(void) const
{
	return keypad;
}

