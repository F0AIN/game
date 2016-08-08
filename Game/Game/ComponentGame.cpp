#include <iostream>
#include <queue>
#include <random>
#include <typeinfo>

#include "ComponentGame.hpp"
#include "BlockNormalWall.hpp"
#include "BlockUnbreakableWall.hpp"
#include "BlockTrap.hpp"
#include "Vector.hpp"
#include "CircularSector.hpp"
#include "Size.hpp"
#include "Global.hpp"
#include "LayoutManager.hpp"
#include "RelativeLayout.hpp"
#include "ComponentTeapot.hpp"

#include "GL/glut.h"

const int ComponentGame::MAP_WIDTH = 45;
const int ComponentGame::MAP_HEIGHT = 45;
const int ComponentGame::DIVISION_NUMBER = 100;

void ComponentGame::allocMap(void)
{
	try {
		map = new Block**[MAP_HEIGHT];
		visibleMap = new Block**[MAP_HEIGHT];
		for (int i = 0; i < MAP_HEIGHT; i++) {
			map[i] = new Block*[MAP_WIDTH];
			visibleMap[i] = new Block*[MAP_WIDTH];
		}
	}
	catch (const bad_alloc&) {
		throw CannotAllocateException();
	}
}

void ComponentGame::clearMap(void)
{
	deleteMap(map);
	deleteMap(visibleMap);
	allocMap();
}

void ComponentGame::generateMap(void)
{
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		for (int j = 1; j < MAP_WIDTH - 1; j++) {
			map[i][j] = nullptr;
			visibleMap[i][j] = nullptr;
		}
	}
	for (int i = 0; i < MAP_HEIGHT; i++) {
		map[i][0] = new BlockUnbreakableWall(blockSize);
		visibleMap[i][0] = new BlockUnbreakableWall(blockSize);
		map[i][MAP_WIDTH - 1] = new BlockUnbreakableWall(blockSize);
		visibleMap[i][MAP_WIDTH - 1] = new BlockUnbreakableWall(blockSize);
	}
	for (int i = 0; i < MAP_WIDTH; i++) {
		map[0][i] = new BlockUnbreakableWall(blockSize);
		visibleMap[0][i] = new BlockUnbreakableWall(blockSize);
		map[MAP_HEIGHT - 1][i] = new BlockUnbreakableWall(blockSize);
		visibleMap[MAP_HEIGHT - 1][i] = new BlockUnbreakableWall(blockSize);
	}

	vector<Vector<int>> v;
	for (int i = 4; i < MAP_HEIGHT - 1; i += 4) {
		v.push_back(Vector<int>(0, i));
		v.push_back(Vector<int>(MAP_WIDTH - 1, i));
	}
	for (int i = 4; i < MAP_WIDTH - 1; i += 4) {
		v.push_back(Vector<int>(i, 0));
		v.push_back(Vector<int>(i, MAP_HEIGHT - 1));
	}

	static Vector<int> directions[] = {
		Vector<int>(1, 0),
		Vector<int>(-1, 0),
		Vector<int>(0, 1),
		Vector<int>(0, -1),
	};
	while (v.size()) {
		int idx = (int)(rnd(mt) * v.size());
		Vector<int> position = v[idx];
		vector<int> directionIDs = getValidDirections(position);
		int size = (int)directionIDs.size();
		if (!size) {
			v.erase(v.begin() + idx);
			continue;
		}
		idx = (int)(rnd(mt) * size);
		const Vector<int>& direction = directions[directionIDs[idx]];
		for (int i = 0; i < 4; i++) {
			position += direction;
			int x = position.getX();
			int y = position.getY();
			map[y][x] = new BlockNormalWall(blockSize);
			visibleMap[y][x] = new BlockNormalWall(blockSize);
			if (i == 3)
				v.push_back(position);
		}
	}
	regionSet = RegionSet();
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		for (int j = 1; j < MAP_WIDTH - 1; j++) {
			if (map[i][j] == nullptr) {
				map[i][j] = new BlockAir(blockSize);
				visibleMap[i][j] = new BlockAir(blockSize);
				regionSet += Vector<int>(j, i);
			}
		}
	}
	//test code
	cout << "num of regions : " << regionSet.getRegionNum() << endl;
}

vector<int> ComponentGame::getValidDirections(const Vector<int>& _position)
{
	static Vector<int> directions[] = {
		Vector<int>(1, 0),
		Vector<int>(-1, 0),
		Vector<int>(0, 1),
		Vector<int>(0, -1),
	};
	int n = sizeof(directions) / sizeof(directions[0]);

	vector<int> ret;
	for (int i = 0; i < n; i++) {
		Vector<int> position(_position);
		const Vector<int>& direction = directions[i];
		bool flag = false;
		for (int j = 0; j < 4; j++) {
			position += direction;
			int x = position.getX();
			int y = position.getY();
			if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
				flag = true;
				break;
			}
			if (map[y][x] != nullptr) {
				flag = true;
				break;
			}
		}
		if (!flag)
			ret.push_back(i);
	}
	return ret;
}

void ComponentGame::deleteMap(Block*** map)
{
	if (map == nullptr)
		return;
	for (int i = 0; i < MAP_HEIGHT; i++) {
		if (map[i] == nullptr)
			continue;
		for (int j = 0; j < MAP_WIDTH; j++)
			if (map[i][j] != nullptr)
				delete map[i][j];
		delete[] map[i];
	}
	delete[] map;
}

void ComponentGame::addPlayer(void)
{
	vector<Vector<int>> positions = getTransparentBlockVectors();
	int n = (int)positions.size();
	int idx = (int)(rnd(mt) * n);
	const Vector<int>& position = positions[idx];
	Keypad keypad(Key('w'), Key('s'), Key('a'), Key('d'), Key(' '), Key('j'), Key('k'), Key('l'));
	players.push_back(new Player(Vector<double>(position.getX(), position.getY()), Size<double>(0.0, 0.0), 3.0, keypad));
}

void ComponentGame::deletePlayers(void)
{
	for (auto itr = players.begin(); itr != players.end(); ++itr)
		delete *itr;
}

void ComponentGame::addEnemy(void)
{
	vector<Vector<int>> positions = getTransparentBlockVectors();
	int n = (int)positions.size();
	int idx = (int)(rnd(mt) * n);
	const Vector<int>& position = positions[idx];
	addEnemy(Vector<double>(position.getX(), position.getY()));
}

void ComponentGame::addEnemy(const Vector<double>& position)
{
	double speed;
	double viewAngle;
	double radius;
	int life;
	enemyGenerator.getParameter(speed, viewAngle, radius, life);
	enemies.push_back(new NormalEnemy(Vector<double>(position.getX(), position.getY()), blockSize, speed, viewAngle, radius, life));
}

void ComponentGame::deleteEnemies(void)
{
	for (auto itr = enemies.begin(); itr != enemies.end(); ++itr)
		delete *itr;
}

void ComponentGame::killEnemies(void)
{
	for (auto itr = enemies.begin(); itr != enemies.end(); ++itr) {
		const Enemy& enemy = **itr;
		if (enemy.getLife())
			continue;
		const Vector<double>& source = enemy.getSource();
		Vector<int> node((int)source.getX(), (int)source.getY());
		/*
		for (auto itr2 = mapTrees.begin(); itr2 != mapTrees.end(); itr2++) {
			const Tree<Vector<int>>& mapTree = *itr2;
			try {
				mapTree.searchNode(node);
				if (mapTree.getNodeNum() <= Global::KILL_ENEMY_THRESHOLD) {
					const Vector<double>& position = enemy.getPosition();
					itemBlocks.push_back(new ItemBlock(position, new BlockUnbreakableWall(blockSize)));
					delete *itr;
					itr = enemies.erase(itr);
					break;
				}
			}
			catch (...) {
			}
		}
		*/
		if (itr == enemies.end())
			break;
	}
}

void ComponentGame::drawItemBlocks(const Vector<double>& position, double distance)
{
	for (auto itr = itemBlocks.begin(); itr != itemBlocks.end(); ++itr) {
		ItemBlock& itemBlock = **itr;
		Vector<double> displacement = itemBlock.getPosition() - position;
		displacement.setX(displacement.getX() * blockSize.getWidth());
		displacement.setY(displacement.getY() * blockSize.getHeight());
		if (displacement.norm2() > distance * distance)
			continue;
		itemBlock.draw();
	}
}

void ComponentGame::drawEnemyVisibilities(const Vector<double>& position, double distance) const
{
	double blockWidth = blockSize.getWidth();
	double blockHeight = blockSize.getHeight();
	glColor3d(1.0, 1.0, 1.0);
	for (auto itr = enemies.begin(); itr != enemies.end(); ++itr) {
		Enemy& enemy = **itr;
		const Vector<double>& enemyPosition = enemy.getPosition();
		Vector<double> displacement = position - enemyPosition;
		displacement.setX(displacement.getX() * blockSize.getWidth());
		displacement.setY(displacement.getY() * blockSize.getHeight());
		if (displacement.norm2() > distance * distance)
			continue;
		double x = enemyPosition.getX();
		double y = enemyPosition.getY();
		glPushMatrix();
		glTranslated(x * blockWidth, y * blockHeight, 0.0);
		enemy.drawVisibility();
		glPopMatrix();
	}
}

void ComponentGame::drawEnemyInformations(void)
{
	for (auto itr = enemies.begin(); itr != enemies.end(); ++itr) {
		Enemy& enemy = **itr;
		enemy.drawInformations();
	}
}

void ComponentGame::moveEnemiesAI(void)
{
	for (auto itr = enemies.begin(); itr != enemies.end(); ++itr)
		(**itr).onMoveAI();
}

bool ComponentGame::isFound(const Vector<double>& rectanglePosition, const Utility::Rectangle& rectangle, const Enemy& enemy) const
{
	static Vector<double> dtable[] = {
		Vector<double>(0.0, 0.0),
		Vector<double>(1.0, 0.0),
		Vector<double>(0.0, 1.0),
		Vector<double>(1.0, 1.0),
	};
	static int n = sizeof(dtable) / sizeof(dtable[0]);
	Vector<double> enemyPosition = enemy.getPosition();
	enemyPosition += Vector<double>(0.5, 0.5);
	Vector<double> enemyWorldPosition(enemyPosition);
	const Size<double>& enemySize = enemy.getSize();
	enemyWorldPosition.setX(enemyWorldPosition.getX() * enemySize.getWidth());
	enemyWorldPosition.setY(enemyWorldPosition.getY() * enemySize.getHeight());
	CircularSector circularSector(enemyWorldPosition, enemy.getAngle(), enemy.getViewAngle(), enemy.getRadius());
	const Size<double>& rectangleSize = rectangle.getSize();
	for (int i = 0; i < n; i++) {
		const Vector<double>& v = dtable[i];
		Vector<double> position = rectanglePosition + v;
		Vector<double> worldPosition(position);
		worldPosition.setX(worldPosition.getX() * rectangleSize.getWidth());
		worldPosition.setY(worldPosition.getY() * rectangleSize.getHeight());
		if (!circularSector.isHit(worldPosition))
			continue;
		if (isBlocked(position, enemyPosition))
			continue;
		return true;
	}
	return false;
}

bool ComponentGame::isBlocked(const Vector<double>& position1, const Vector<double>& position2) const
{
	Vector<double> v = position2 - position1;
	v /= DIVISION_NUMBER;
	Vector<double> position(position1);
	for (int i = 0; i < DIVISION_NUMBER; i++) {
		position += v;
		if (isHit(position, &Block::isTransparentByEnemy) != nullptr)
			return true;
	}
	return false;
}

Block* ComponentGame::isHit(const Character& character, bool (Block::*isTransparent)() const) const
{
	Vector<int> dummy;
	return isHit(character, isTransparent, dummy);
}

Block* ComponentGame::isHit(const Character& character, bool (Block::*isTransparent)() const, Vector<int>& position) const
{
	static Vector<double> dv[] = {
		Vector<double>(0.5, 0.0),
		Vector<double>(0.5, 0.95),
		Vector<double>(0.0, 0.5),
		Vector<double>(0.95, 0.5),
	};
	int n = sizeof(dv) / sizeof(dv[0]);
	const Vector<double>& characterPosition = character.getPosition();
	for (int i = 0; i < n; i++) {
		Block* block = isHit(characterPosition + dv[i], isTransparent, position);
		if (block != nullptr)
			return block;
	}
	return nullptr;
}

bool ComponentGame::isPlaceable(const Vector<double>& position) const
{
	int row = (int)position.getY();
	int col = (int)position.getX();
	if (row < 0 || row >= MAP_HEIGHT || col < 0 || col >= MAP_WIDTH)
		return false;
	const Block& block = *map[row][col];
	if (!block.isTransparent())
		return false;
	if (!isPlaceable(position, players))
		return false;
	if (!isPlaceable(position, enemies))
		return false;
	return true;
}

void ComponentGame::breakBlock(void)
{
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (map[i][j]->getDamage() <= 0) {
				if (!map[i][j]->isTransparent()) {
					regionSet += Vector<int>(j, i);
					//test code
					cout << "num of regions : " << regionSet.getRegionNum() << endl;
				}
				map[i][j] = map[i][j]->brokenBlock();
			}
		}
	}
}

void ComponentGame::placeBlock(const vector<Player*> players)
{
	for (auto itr = players.begin(); itr != players.end(); ++itr) {
		Player& player = **itr;
		if (player.getIsMoving()) {
			player.setIsPlacing(false);
			continue;
		}
		if (!player.getIsPlacing())
			continue;
		player.setIsPlacing(false);
		const Vector<double>& directionVector = player.getDirectionVector();
		const Vector<double>& position = player.getPosition();
		Vector<double> destination = position + directionVector;
		int row = (int)destination.getY();
		int col = (int)destination.getX();
		if (!isPlaceable(destination))
			continue;
		Player::BlockType blockType = player.getPlacingBlockType();
		Block* block;
		switch (blockType) {
		case Player::WALL:
			block = new BlockNormalWall(blockSize);
			break;
		case Player::TRAP:
			block = new BlockTrap(blockSize);
			break;
		case Player::DECOY:
			block = nullptr;
			vector<double> distances = getDistances(decoys, destination);
			for (auto itr = distances.begin(); itr != distances.end(); ++itr)
				if (*itr < 0.5)
					return;
			decoys.push_back(new Decoy(destination, player.getSize()));
			break;
		}
		if (block != nullptr) {
			delete map[row][col];
			map[row][col] = block;
			regionSet -= Vector<int>(col, row);
			//test code
			cout << "num of regions : " << regionSet.getRegionNum() << endl;
		}
	}
}

vector<Vector<int>> ComponentGame::getTransparentBlockVectors(void) const
{
	vector<Vector<int>> positions;
	//Characterを配置可能な場所を取得する
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		for (int j = 1; j < MAP_WIDTH - 1; j++) {
			if (map[i][j]->isTransparent())
				positions.push_back(Vector<int>(j, i));
		}
	}
	return positions;
}

void ComponentGame::setBlockSize(void)
{
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[i][j]->setSize(blockSize);
			visibleMap[i][j]->setSize(blockSize);
		}
	}
	for (auto itr = players.begin(); itr != players.end(); ++itr) {
		Player& player = **itr;
		player.setSize(blockSize);
	}
	for (auto itr = enemies.begin(); itr != enemies.end(); ++itr) {
		Enemy& enemy = **itr;
		enemy.setSize(blockSize);
	}
}

void ComponentGame::moveEvent(void)
{
	moveCharacters(players);
	moveEnemiesAI();
	moveCharacters(enemies);
}

void ComponentGame::hitEvent(void)
{
	hitDetectCharacters(players, &Block::isTransparent);
	hitDetectCharacters(enemies, &Block::isTransparentByEnemy);
	hitDetectInformations();
}

void ComponentGame::findBlockEvent(void)
{
	static int cnt = 0;
	if (++cnt >= MAP_WIDTH)
		cnt = 0;
	for (int i = 0; i < MAP_HEIGHT; i++) {
		int j = cnt;
		Vector<double> position(j, i);
		Block& block = *map[i][j];
		if (block.isTransparent())
			continue;
		for (auto itr = enemies.begin(); itr != enemies.end(); ++itr) {
			Enemy& enemy = **itr;
			if (isFound(position, block, enemy))
				enemy.onFind(Vector<int>(j, i), block);
		}
	}
}

void ComponentGame::breakBlockEvent(void)
{
	breakBlock(players);
	breakBlock(enemies);
	breakBlock();
}

void ComponentGame::hitDetectInformations(void)
{
	for (auto itr = enemies.begin(); itr != enemies.end(); ++itr) {
		Enemy& enemy = **itr;
		hitDetectInformations(enemy.getInformations());
	}
}

void ComponentGame::hitDetectInformations(vector<Information>& informations)
{
	for (auto itr = informations.begin(); itr != informations.end(); ++itr) {
		Information& information = *itr;
		hitDetectInformation(information);
	}
}

void ComponentGame::hitDetectInformation(Information& information)
{
	for (auto itr = enemies.begin(); itr != enemies.end(); ++itr) {
		Enemy& enemy = **itr;
		int ID = enemy.getID();
		if (information.searchID(ID))
			continue;
		Vector<double> position = enemy.getPosition();
		position += Vector<double>(0.5, 0.5);
		const Size<double>& size = enemy.getSize();
		position.setX(position.getX() * size.getWidth());
		position.setY(position.getY() * size.getHeight());
		if (information.isHit(position)) {
			information.registerID(ID);
			enemy.onFind(information.getCharacter());
		}
	}
}

void ComponentGame::placeBlockEvent(void)
{
	placeBlock(players);
}

void ComponentGame::spawnEnemyEvent(void)
{
	static int cnt = 0;
	if (enemyGenerator.getSpawnedNum() == enemyGenerator.getSpawnNum())
		return;
	cnt++;
	if (cnt <= 600)
		return;
	for (auto itr = enemies.begin(); itr != enemies.end(); ++itr) {
		const Enemy& enemy1 = **itr;
		for (auto itr1 = enemies.begin(); itr1 != enemies.end(); ++itr1) {
			if (itr1 == itr)
				continue;
			const Enemy& enemy2 = **itr1;
			if (isHit(enemy1, enemy2)) {
				cnt = 0;
				const Vector<double>& position = enemy1.getSource();
				addEnemy(position);
				return;
			}
		}
	}
}

void ComponentGame::keyEvent(const Key& key, Player& player, void (Player::*funcUp)(), void (Player::*funcDown)(), void (Player::*funcLeft)(), void (Player::*funcRight)(), void (Player::*funcBreakBlock)(), void (Player::*funcPlaceWall)(), void (Player::*funcPlaceTrap)(), void (Player::*funcPlaceDecoy)())
{
	const Keypad& keypad = player.getKeypad();
	if (key == keypad.getUp())
		((&player)->*funcUp)();
	if (key == keypad.getDown())
		((&player)->*funcDown)();
	if (key == keypad.getLeft())
		((&player)->*funcLeft)();
	if (key == keypad.getRight())
		((&player)->*funcRight)();
	if (key == keypad.getBreakBlock())
		((&player)->*funcBreakBlock)();
	if (key == keypad.getPlaceWall())
		((&player)->*funcPlaceWall)();
	if (key == keypad.getPlaceTrap())
		((&player)->*funcPlaceTrap)();
	if (key == keypad.getPlaceDecoy())
		((&player)->*funcPlaceDecoy)();
}

void ComponentGame::keyEvent(const Key& key, void (Player::*funcUp)(), void (Player::*funcDown)(), void (Player::*funcLeft)(), void (Player::*funcRight)(), void (Player::*funcBreakBlock)(), void (Player::*funcPlaceWall)(), void (Player::*funcPlaceTrap)(), void (Player::*funcPlaceDecoy)())
{
	for (auto itr = players.begin(); itr != players.end(); ++itr) {
		Player& player = **itr;
		keyEvent(key, player, funcUp, funcDown, funcLeft, funcRight, funcBreakBlock, funcPlaceWall, funcPlaceTrap, funcPlaceDecoy);
	}
}

ComponentGame::ComponentGame(const Size<double>& size, const string& fpath)
	: Component(size), mt(rd()), rnd(0.0, 1.0), map(nullptr), blockSize(size.getWidth()/ MAP_WIDTH, size.getHeight() / MAP_HEIGHT), audio("data/music/BreakWall.wav"), enemyGenerator(fpath)
{
	init();
}

ComponentGame::~ComponentGame()
{
	deleteMap(map);
	deleteMap(visibleMap);
	deletePlayers();
}

void ComponentGame::setSize(const Size<double>& size)
{
	this->size = size;
	blockSize = Size<double>(size.getWidth() / MAP_WIDTH, size.getHeight() / MAP_HEIGHT);
	setBlockSize();
}

void ComponentGame::init(void)
{
	allocMap();
	generateMap();
	addPlayer();
	for (unsigned i = 0; i < enemyGenerator.getFirstSpawnNum(); i++)
		addEnemy();
}

void ComponentGame::draw(void)
{
	if (!players.size()) {
		RelativeLayout* layout = new RelativeLayout(Size<double>(Global::WORLD_WIDTH, Global::WORLD_HEIGHT));
		layout->add(new ComponentTeapot(Size<double>(0, 0)), Vector<double>(0.0, 0.0), true);
		cout << "game over" << endl;
		throw layout;
	}
	if (!enemies.size()) {
		RelativeLayout* layout = new RelativeLayout(Size<double>(Global::WORLD_WIDTH, Global::WORLD_HEIGHT));
		layout->add(new ComponentTeapot(Size<double>(0, 0)), Vector<double>(0.0, 0.0), true);
		cout << "game clear" << endl;
		throw layout;
	}
	moveEvent();
	hitEvent();
	killCharacterEvent(players);
	killCharacterEvent(decoys);
	findCharacterEvent(players);
	findCharacterEvent(decoys);
	findBlockEvent();
	breakBlockEvent();
	placeBlockEvent();
	killEnemies();
	spawnEnemyEvent();
	double blockWidth = blockSize.getWidth();
	double blockHeight = blockSize.getHeight();
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	for (int i = 0; i < MAP_HEIGHT; i++) {
		glPushMatrix();
		for (int j = 0; j < MAP_WIDTH; j++) {
			bool visible = false;
			Vector<double> position(j, i);
			for (auto itr = players.begin(); itr != players.end(); ++itr) {
				const Player& player = **itr;
				const Vector<double>& playerPosition = player.getPosition();
				Vector<double> displacement = position - playerPosition;
				displacement.setX(displacement.getX() * blockSize.getWidth());
				displacement.setY(displacement.getY() * blockSize.getHeight());
				if (displacement.norm2() < Global::PLAYER_RADIUS * Global::PLAYER_RADIUS) {
					visible = true;
					break;
				}
			}
			if (visible && (typeid(*visibleMap[i][j]) != typeid(*map[i][j]))) {
				delete visibleMap[i][j];
				visibleMap[i][j] = map[i][j]->clone();
			}
			visibleMap[i][j]->draw();
			glTranslated(blockWidth, 0.0, 0.0);
		}
		glPopMatrix();
		glTranslated(0.0, blockHeight, 0.0);
	}
	glPopMatrix();
	drawCharacters(players);
	for (auto itr = players.begin(); itr != players.end(); ++itr) {
		const Player& player = **itr;
		const Vector<double>& playerPosition = player.getPosition();
		drawCharacters(enemies, playerPosition, Global::PLAYER_RADIUS);
		drawCharacters(decoys, playerPosition, Global::PLAYER_RADIUS);
		drawItemBlocks(playerPosition, Global::PLAYER_RADIUS);
	}
	for (auto itr = decoys.begin(); itr != decoys.end(); ++itr) {
		const Decoy& decoy = **itr;
		const Vector<double>& decoyPosition = decoy.getPosition();
		drawCharacters(enemies, decoyPosition, Global::PLAYER_RADIUS);
		drawCharacters(decoys, decoyPosition, Global::PLAYER_RADIUS);
		drawItemBlocks(decoyPosition, Global::PLAYER_RADIUS);
	}
	glDisable(GL_TEXTURE_2D);
	drawCharacterVisibilities(players);
	for (auto itr = players.begin(); itr != players.end(); ++itr) {
		const Player& player = **itr;
		const Vector<double>& playerPosition = player.getPosition();
		drawEnemyVisibilities(playerPosition, Global::PLAYER_RADIUS);
	}
	drawEnemyInformations();
}

void ComponentGame::mouse(int button, int state, int x, int y)
{
	for (auto itr = players.begin(); itr != players.end(); ++itr)
		(*itr)->mouse(button, state, x, y);
}

void ComponentGame::keyboard(unsigned char key, int x, int y)
{
	keyEvent(Key(key), &Player::onUp, &Player::onDown, &Player::onLeft, &Player::onRight, &Player::onBreakBlock, &Player::onPlaceWall, &Player::onPlaceTrap, &Player::onPlaceDecoy);
}

void ComponentGame::keyboardOnce(unsigned char key, int x, int y)
{
	keyEvent(Key(key), &Player::onUpDown, &Player::onDownDown, &Player::onLeftDown, &Player::onRightDown, &Player::onBreakBlockDown, &Player::onPlaceWallDown, &Player::onPlaceTrapDown, &Player::onPlaceDecoyDown);
}

void ComponentGame::keyboardup(unsigned char key, int x, int y)
{

}

void ComponentGame::keyboardupOnce(unsigned char key, int x, int y)
{
	keyEvent(Key(key), &Player::onUpUp, &Player::onDownUp, &Player::onLeftUp, &Player::onRightUp, &Player::onBreakBlockUp, &Player::onPlaceWallDown, &Player::onPlaceTrapUp, &Player::onPlaceDecoyUp);
}

void ComponentGame::special(int key, int x, int y)
{

}

void ComponentGame::specialOnce(int key, int x, int y)
{

}

void ComponentGame::specialup(int key, int x, int y)
{

}

void ComponentGame::specialupOnce(int key, int x, int y)
{

}

