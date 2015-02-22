#include "headclass.h"
#include "particle.h"
#include "lightmap.h"
#include <windows.h>		
#include <gl\gl.h>	

blockparticleheadclass::blockparticleheadclass()
{
	last = &first;
}

void blockparticleheadclass::Insert(int f, int y)
{
	//blockparticle *temp = &first;
	//while (temp->next != 0) {
	//	temp=temp->next;
	//}
	last->next = new blockparticle(f+rand()%24-12.0f, y+rand()%24-12.0f, 1.0f, 1.0f, 1.0f, float(rand()%720)-360.0f, float(rand()%720)-360.0f);
	last = last->next;
}

void blockparticleheadclass::DeleteAll()
{
	while (first.next != 0) {
	blockparticle *temp = &first;
	blockparticle *temp2 = 0;
	while (temp->next != 0) {
		temp2 = temp;
		temp=temp->next;
	}
	delete temp;
	temp = 0;
	temp2->next = 0;
	}
	last = &first;
}

void blockparticleheadclass::Check()
{
	blockparticle *previousclass = &first;
	for (blockparticle *temp = first.next; temp != 0; temp = temp->next) {
		if (temp->Alive == false) {
			previousclass->next = temp->next;
			if (temp == last)
				last = previousclass;
			delete temp; temp = previousclass;
		}
		else
			previousclass = temp;
	}
}

snowparticleheadclass::snowparticleheadclass()
{
	last = &first;
}

void snowparticleheadclass::DeleteAll()
{
	while (first.next != 0) {
	snow *temp = &first;
	snow *temp2 = 0;
	while (temp->next != 0) {
		temp2 = temp;
		temp=temp->next;
	}
	delete temp;
	temp = 0;
	temp2->next = 0;
	}
	last = &first;
}

void snowparticleheadclass::Insert()
{
	//snow *temp = &first;
	//while (temp->next != 0) {
	//	temp=temp->next;
	//}
	last->next = new snow(float(rand()%630+5), 481.0f, 1.0f, 1.0f, 1.0f, float(rand()%40)-20, -float(rand()%30+30));
	last = last->next;
}

void snowparticleheadclass::Check()
{
	snow *previousclass = &first;
	for (snow *temp = first.next; temp != 0; temp = temp->next) {
		if (temp->Alive == false) {
			previousclass->next = temp->next;
			if (temp == last)
				last = previousclass;
			delete temp; temp = previousclass;
		}
		else
			previousclass = temp;
	}
}

explodeparticleheadclass::explodeparticleheadclass()
{
	last = &first;
}

void explodeparticleheadclass::DeleteAll()
{
	while (first.next != 0) {
	explodeparticle *temp = &first;
	explodeparticle *temp2 = 0;
	while (temp->next != 0) {
		temp2 = temp;
		temp=temp->next;
	}
	delete temp;
	temp = 0;
	temp2->next = 0;
	}
	last = &first;
}

void explodeparticleheadclass::Insert(int leftmost, int bottommost, int width, int height)
{
	//explodeparticle *temp = &first;
	//while (temp->next != 0) {
	//	temp=temp->next;
	//}
	last->next = new explodeparticle(float(rand()%width+leftmost), float(rand()%height+bottommost), 1.0f, 1.0f, 1.0f, float(rand()%1000)-500, float(rand()%1000)-500);
	last = last->next;
}

void explodeparticleheadclass::Check()
{
	explodeparticle *previousclass = &first;
	for (explodeparticle *temp = first.next; temp != 0; temp = temp->next) {
		if (temp->Alive == false) {
			previousclass->next = temp->next;
			if (temp == last)
				last = previousclass;
			delete temp; temp = previousclass;
		}
		else
			previousclass = temp;
	}
}

lightheadclass::lightheadclass()
{
	last = &first;
}

void lightheadclass::DeleteAll()
{
	while (first.next != 0) {
	lightmapclass *temp = &first;
	lightmapclass *temp2 = 0;
	while (temp->next != 0) {
		temp2 = temp;
		temp=temp->next;
	}
	delete temp;
	temp = 0;
	temp2->next = 0;
	}
	last = &first;
}

void lightheadclass::Insert(float x, float y)
{
	//lightmapclass *temp = &first;
	//while (temp->next != 0) {
	//	temp=temp->next;
	//}
	last->next = new lightmapclass(x, y);
	last = last->next;
}

void lightheadclass::Check()
{
	lightmapclass *previousclass = &first;
	for (lightmapclass *temp = first.next; temp != 0; temp = temp->next) {
		if (temp->Alive == false) {
			previousclass->next = temp->next;
			if (temp == last)
				last = previousclass;
			delete temp; temp = previousclass;
		}
		else
			previousclass = temp;
	}
}



fireheadclass::fireheadclass()
{
	last = &first;
}

void fireheadclass::DeleteAll()
{
	while (first.next != 0) {
	fire *temp = &first;
	fire *temp2 = 0;
	while (temp->next != 0) {
		temp2 = temp;
		temp=temp->next;
	}
	delete temp;
	temp = 0;
	temp2->next = 0;
	}
	last = &first;
}

void fireheadclass::Insert(int leftmost, int bottommost, int width, int height)
{
	//fire *temp = &first;
	//while (temp->next != 0) {
	//	temp=temp->next;
	//}
	last->next = new fire(float(rand()%width+leftmost), float(rand()%height+bottommost), 1.0f, 1.0f, 1.0f, 0, (rand()%100+150)/10.0f);
	last = last->next;
}

void fireheadclass::Check()
{
	fire *previousclass = &first;
	for (fire *temp = first.next; temp != 0; temp = temp->next) {
		if (temp->Alive == false) {
			previousclass->next = temp->next;
			if (temp == last)
				last = previousclass;
			delete temp; temp = previousclass;
		}
		else
			previousclass = temp;
	}
}

waterheadclass::waterheadclass()
{
	last = &first;
}

void waterheadclass::DeleteAll()
{
	while (first.next != 0) {
	water *temp = &first;
	water *temp2 = 0;
	while (temp->next != 0) {
		temp2 = temp;
		temp=temp->next;
	}
	delete temp;
	temp = 0;
	temp2->next = 0;
	}
	last = &first;
}

void waterheadclass::Insert()
{
	//fire *temp = &first;
	//while (temp->next != 0) {
	//	temp=temp->next;
	//}
	last->next = new water(rand()%40+10.0f, rand()%460+10.0f, 1.0f, 1.0f, 1.0f, rand()%300-150.0f, rand()%400-200.0f);
	last = last->next;
}

void waterheadclass::Check()
{
	water *previousclass = &first;
	for (water *temp = first.next; temp != 0; temp = temp->next) {
		if (temp->Alive == false) {
			previousclass->next = temp->next;
			if (temp == last)
				last = previousclass;
			delete temp; temp = previousclass;
		}
		else
			previousclass = temp;
	}
}