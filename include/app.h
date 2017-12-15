#pragma once

class Application
{
public:
	Application();
	~Application();
	
	int run();
private:
	int last_key_pressed;

	void initialize();
	void cleanup();
	
	void update();
	void renderFrame();
	
	void mainloop();
};
