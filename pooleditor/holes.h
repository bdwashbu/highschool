#ifndef _HOLES_H_
#define _HOLES_H_

class holeclass
{
	public:
		static float *SetPointer(holeclass *ball, float mousex, float mousey, int count);
		void DrawHole(void);
		void WriteFile(char *file);
		float *SetxRightPointer(float mousex, float mousey, float *templinex, float *templiney, int choice);
		float X;
		float Y;
	private:
};

#endif
