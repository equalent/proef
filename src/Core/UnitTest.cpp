/*
 * Proef
 *
 * Copyright (c) Andrey Tsurkan
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef PF_UNIT_TEST
#include "pch.h"

static struct
{
	const __SUnitTestDesc* Desc = nullptr;
	uint NumFailures = 0;
	uint NumSuccesses = 0;
} gCurrentTestInfo{};

TArray<__SUnitTestDesc> __gUnitTests;

void RunUnitTestsAndExit()
{
	FConsole::Initialize();
	FConsole::WriteLine("Running unit tests...");
	FConsole::WriteLine();

	bool allSucceeded = true;

	for (const __SUnitTestDesc& test : __gUnitTests)
	{
		gCurrentTestInfo.Desc = &test;
		gCurrentTestInfo.NumSuccesses = 0;
		gCurrentTestInfo.NumFailures = 0;

		FConsole::WriteLine(FString::PrintF("[TEST \"%s\"]", test.TestName));
		test.TestBody();
		const bool succeeded = gCurrentTestInfo.NumFailures == 0;

		if (succeeded)
		{
			FConsole::SetTextColor(EConsoleTextColor::Green);
		}
		else
		{
			FConsole::SetTextColor(EConsoleTextColor::Red);
		}

		FConsole::Write(succeeded ? "    TEST SUCCEEDED" : "    TEST FAILED");

		FConsole::SetTextColor(EConsoleTextColor::White);

		FConsole::WriteLine(FString::PrintF(" (succeeded: %u, failed: %u, total: %u)",
		                                    gCurrentTestInfo.NumSuccesses, gCurrentTestInfo.NumFailures,
		                                    gCurrentTestInfo.NumSuccesses + gCurrentTestInfo.NumFailures));

		if (!succeeded)
		{
			allSucceeded = false;
		}

		gCurrentTestInfo.Desc = nullptr; // reset desc to prevent asserts from working
	}

	FConsole::WriteLine();
	FConsole::Write("Press any key to continue...  ");
	FConsole::WaitForKey();
	::ExitProcess(allSucceeded ? 0 : 3);
}

bool __UnitTestAssert(const bool result, const char* code, const char* message, uint line)
{
	verify(gCurrentTestInfo.Desc);

	if (!result)
	{
		gCurrentTestInfo.NumFailures++;
		FConsole::SetTextColor(EConsoleTextColor::Red);
		FConsole::WriteLine("    Failed:");

		if (message)
			FConsole::WriteLine(FString::PrintF("        %s   :   %s", code, message));
		else
			FConsole::WriteLine(FString::PrintF("        %s", code, message));
		FConsole::SetTextColor(EConsoleTextColor::White);
	}
	else
	{
		gCurrentTestInfo.NumSuccesses++;
	}

	return !result;
}
#endif
