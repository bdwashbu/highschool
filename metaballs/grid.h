class gridclass
{
public:
	void AssignGrid(int a, int b, float xwidth, float ylength);
	void RenderGrid();
	bool point1;
	float point1color, point2color, point3color, point4color;
	int leftx, bottomy;
	float red, green, blue;
	bool beenon;
	static bool circle;
	static bool torus;
private:
};