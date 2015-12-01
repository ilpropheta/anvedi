#include "Signal.h"

Signal Signal::Create(QString name)
{
	return{ std::move(name) };
}
