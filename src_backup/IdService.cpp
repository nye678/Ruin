#include "IdService.h"

using namespace std;
using namespace Ruin;

IdService::IdService()
	: _idSource(FreeRangeLowerBound)
{}

Id IdService::createId()
{
	return Id(_idSource++);
}