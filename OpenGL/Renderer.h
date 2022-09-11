#pragma once

class Renderer {
private:
	static Renderer* instance;
	Renderer();
public:
	static Renderer* getInstance();
	void render();
};

