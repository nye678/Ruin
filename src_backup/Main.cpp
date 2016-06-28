#include "Engine.h"

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <libpng\png.h>

#include "PngParser.h";

using namespace Ruin;

int main(int argc, char* argv[])
{
	Engine engine;

	bool engineInitialized = engine.initialize();
	assert(engineInitialized);

	engine.run();

	engine.shutdown();
}