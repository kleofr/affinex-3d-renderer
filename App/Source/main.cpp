#include "Engine/Engine.h"

int main() {

	AffineX::Engine engine;
	engine.Init({ "AffineX Application", 1280, 720, true });
	engine.Run();

}