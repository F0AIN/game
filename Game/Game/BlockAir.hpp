#ifndef ___Class_BlockAir
#define ___Class_BlockAir

#include "Block.hpp"

/**
 * $B6u5$%V%m%C%/%/%i%9!%(B
 */
class BlockAir : public Block {
public:
	BlockAir(int width, int height);
	/**
	 * $BIA2h;~$K8F$P$l$k!%(B
	 */
	virtual void draw(void);
};

BlockAir::BlockAir(int width, int height)
	: Block(width, height)
{
	
}

void BlockAir::draw(void)
{
	glColor3d(1.0, 1.0, 0.0);
	Block::draw();
}

#endif

