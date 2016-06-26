#ifndef ___Class_BlockWall
#define ___Class_BlockWall

#include "Block.hpp"

/**
 * $BJI%/%i%9!%(B
 */
class BlockWall : public Block {
public:
	BlockWall(int width, int height);
	/**
	 * $BIA2h;~$K8F$P$l$k!%(B
	 */
	virtual void draw(void);
};

BlockWall::BlockWall(int width, int height)
	: Block(width, height)
{

}

void BlockWall::draw(void)
{
	glColor3d(0.0, 0.0, 1.0);
	Block::draw();
}

#endif

