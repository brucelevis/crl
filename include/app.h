/*
 * inputsystem.h
 *
 *  Created on: Dec 15, 2017
 *      Author: joshua
 */

#ifndef INCLUDE_APP_H_
#define INCLUDE_APP_H_

class Application
{
public:
	Application();
	~Application();
	
	int run();
	void cleanup();
private:
	int last_key_pressed;

	void initialize();
	
	void update();
	void renderFrame();
	
	void mainloop();
};

#endif /* INCLUDE_APP_H_ */
