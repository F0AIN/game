#ifndef ___Class_Block
#define ___Class_Block

/**
 * $B%V%m%C%/%/%i%9!%(B
 * $BJI$d%H%i%C%W$N4p$H$J$k!%(B
 */
class Block {
private:
	int width;	/* $BI}(B */
	int height;	/* $B9b$5(B */
public:
	/**
	 * $BF)L@%V%m%C%/$+$I$&$+!%(B
	 *
	 * @return true $BF)L@$G$"$k>l9g(B
	 * @return false $BF)L@$G$J$$>l9g(B
	 */
	virtual bool isTransparent(void) const;
	/**
	 * $B%V%m%C%/$NGK2u;~$K8F$P$l$k!%>-MhE*$K%V%m%C%/%"%$%F%`$N%I%m%C%W$d%"%K%a!<%7%g%s%$%s%9%?%s%9$N@8@.$r9T$&!%(B
	 */
	virtual void destroy(void) = 0;
	/**
	 * $BIA2h;~$K8F$P$l$k!%(B
	 */
	virtual void draw(void) = 0;

	/**
	 * $B%;%C%?!<5Z$S%2%C%?!<(B
	 */
	int getWidth(void) const;
	int getHeight(void) const;
	void setWidth(int width);
	void setHeight(int height);

};

bool Block::isTransparent(void) const
{
	return false;
}

inline int Block::getWidth(void) const
{
	return width;
}

inline int Block::getHeight(void) const
{
	return height;
}

inline void Block::setWidth(int width)
{
	this->width = width;
}

inline void Block::setHeight(int height)
{
	this->height = height;
}

#endif

