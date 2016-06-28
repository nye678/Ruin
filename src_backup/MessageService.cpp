#include "MessageService.h"

using namespace std;
using namespace Ruin;

MessageService::MessageService()
{}

void MessageService::DisplayNativeDialog(const string &title, const string &text)
{
#ifdef _WIN32
	MessageBox(nullptr, (LPCSTR)text.c_str(), (LPCSTR)title.c_str(), MB_ICONWARNING | MB_OK | MB_DEFBUTTON1);
#elif __APPLE__
	// TODO: Do something for Apple.
#elif __linux
	// TODO: Do something for Linux.
#endif
}
