#include "Core.h"

int main(int args, char** argv) {
	Core* core = Core::getInstance();

	core->init("OpenGL", 800, 600);
	core->run();

	return 0;
}