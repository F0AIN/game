#ifndef ___Class_BlockNormalWall
#define ___Class_BlockNormalWall

#include "BlockWall.hpp"

/**
 * $BIaDL$NJI%/%i%9!%(B
 */
class BlockNormalWall : public BlockWall {
public:
	BlockNormalWall(int width, int height);
};

BlockNormalWall::BlockNormalWall(int width, int height)
	: BlockWall(width, height)
{

}

#endif

