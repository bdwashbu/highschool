#ifndef _STRING_H_
#define _STRING_H_

#include "particle.h"

class StringClass
{
public:
	StringClass();
	ParticleClass *Head;                
	double SpringConstant;    
	double DampingConstant;   
	double RestLength;        
private:
};

#endif