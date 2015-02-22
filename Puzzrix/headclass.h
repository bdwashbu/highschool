#ifndef _HEADCLASS_H_
#define _HEADCLASS_H_

#include "particle.h"
#include "lightmap.h"

class particleheadclass
{
public:
	virtual void Insert() = 0;
	virtual void DeleteAll() = 0;
	virtual void Check() = 0;
};

class blockparticleheadclass
{
public:
	blockparticleheadclass();
	blockparticle first;
	blockparticle *last;
	void Insert(int f, int y);
	void DeleteAll();
	void Check();
};

class snowparticleheadclass
{
public:
	snowparticleheadclass();
	snow first;
	snow *last;
	void Insert();
	void DeleteAll();
	void Check();
};

class explodeparticleheadclass
{
public:
	explodeparticleheadclass();
	explodeparticle first;
	explodeparticle *last;
	void Insert(int leftmost, int bottommost, int width, int height);
	void DeleteAll();
	void Check();
};

class lightheadclass
{
public:
	lightheadclass();
	lightmapclass first;
	lightmapclass *last;
	void Insert(float x, float y);
	void DeleteAll();
	void Check();
};

class fireheadclass
{
public:
	fireheadclass();
	fire first;
	fire *last;
	void Insert(int leftmost, int bottommost, int width, int height);
	void DeleteAll();
	void Check();
};

class waterheadclass
{
public:
	waterheadclass();
	water first;
	water *last;
	void Insert();
	void DeleteAll();
	void Check();
};

#endif