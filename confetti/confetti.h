class confetticlass
{
public:
	confetticlass();
	void DrawConfetti(float scalar, float lightX, float lightY, float lightZ, bool moving);
	bool CheckCollison(void);
	void MoveConfetti(float scalar, bool moving);
	static void MoveSpitPoint(bool moving);
	static void TossConfetti(confetticlass *confetti, float newtime, bool trick, bool moving);
	void InitilizeConfetti(int trick);
	float X, Y, Z;
	bool Rotating;
	static float angle2;
	bool Active;
private:
//	float V1, V2;
	float XRotation,YRotation,ZRotation;
	float RotatingXAmount, RotatingYAmount, RotatingZAmount;
	float MoveX, MoveY, MoveZ;
	float Red, Green, Blue;
//	float Size;
	float RotationXOffset, RotationYOffset, RotationZOffset;
	float Life;
	float Gravity;
	int Shape;
	static float ConfettiColors;
	static bool ConfettiColorsUp;
	static float SpitpointX, SpitpointY, SpitpointZ;
	static float fart;
};