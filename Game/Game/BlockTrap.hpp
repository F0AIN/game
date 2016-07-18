#ifndef ___Class_BlockTrap
#define ___Class_BlockTrap

#include "Block.hpp"

class BlockTrap : public Block {
public:
	BlockTrap();
	BlockTrap(const Size<double>& size);

	void loadAnimations(void);
	std::string getAnimationPath(void) const;
	bool isTransparent(void) const;
	bool isTransparentByEnemy(void) const;
	void draw(void);
	Block* clone(void) const;
};

#endif

