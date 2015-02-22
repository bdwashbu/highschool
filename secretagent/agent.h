class agents
{
public:
	agents();
	~agents();
	void MoveRight(float scalar);
	void MoveLeft(float scalar);
	float Xmove, Ymove;
	float X, Y;
	bool CheckAhead(float lineslope, float yintercept, float linex, float line2x, float liney, float line2y);
	void MoveForward(float speed, float scalar, short direction);
	void Collision(float lineslope, float yintercept, bool noinput);
	void Move(float xoffset, float yoffset);
	void DrawCharacter(void);
private:
	void FindShortest(float *shortestx, float *shortesty, float *longestx, float *longesty, float linex, float line2x, float liney, float line2y);
	float Xmoving, Ymoving;
	float rotation;
};
