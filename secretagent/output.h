class output2
{
public:
	output2();
	float tempx;
	char savefile[30];
	char loadfile[30];
	char tempfile[30];
	void saving(void);
	bool SaveButton(float mousex, float mousey, int lowx, int highx, int lowy, int highy);
	void save(char *file);
	void showsave(float x);
	void load(void);
private:
};