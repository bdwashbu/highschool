#ifndef _PARTICLE_H_
#define _PARTICLE_H_

class ParticleClass
{
public:
	ParticleClass(int Ypos);
	void Draw();
	void Move();
	ParticleClass *Next;
	ParticleClass *Previous;
	double Mass;
	double XPosition;
	double YPosition;
	double XVelocity;
	double YVelocity;
	double XForce;
	double YForce;
private:
};

#endif