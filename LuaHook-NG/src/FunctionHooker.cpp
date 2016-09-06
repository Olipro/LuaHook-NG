#include "stdafx.h"
#include "FunctionHooker.h"

using namespace Olipro;

FunctionHooker::FunctionHooker()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
}

bool FunctionHooker::Commit()
{
	return isCommitted = DetourTransactionCommit() == NO_ERROR;
}

void FunctionHooker::Abort()
{
	DetourTransactionAbort();
	isAborted = true;
}

FunctionHooker::~FunctionHooker()
{
	if (!isCommitted && !isAborted)
		Commit();
}