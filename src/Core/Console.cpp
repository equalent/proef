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

#include "pch.h"

static bool gConsoleInitialized = false;
static HANDLE gConsoleOutput;
static HANDLE gConsoleInput;

void FConsole::Initialize()
{
	if(gConsoleInitialized){return;}
	::AllocConsole();

	::SetConsoleTitleA("Proef Output");
	::SetConsoleOutputCP(CP_UTF8);
	gConsoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
	gConsoleInput = ::GetStdHandle(STD_INPUT_HANDLE);
	::SetConsoleMode(gConsoleInput, ENABLE_ECHO_INPUT);
	::SetConsoleTextAttribute(gConsoleOutput, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	const char* introString = "[FConsole] Output console initialized!\r\n";
	::WriteConsoleA(gConsoleOutput, introString, (DWORD)strlen(introString), nullptr, nullptr);

	::ShowWindow(::GetConsoleWindow(), SW_SHOW);

	gConsoleInitialized = true;
}

void FConsole::Write(const FString& text)
{
	TArray<wchar_t> buffer16;
	FStringConv::ToUtf16(text.GetData(), buffer16);
	::WriteConsoleW(gConsoleOutput, buffer16.GetData(), (DWORD)(buffer16.GetCount() - 1), nullptr, nullptr);
}

void FConsole::WriteLine()
{
	::WriteConsoleA(gConsoleOutput, "\r\n", 2, nullptr, nullptr);
}

void FConsole::WriteLine(const FString& line)
{
	Write(line);
	::WriteConsoleA(gConsoleOutput, "\r\n", 2, nullptr, nullptr);
}

void FConsole::WaitForKey()
{
	wchar_t buf1;
	DWORD buf1count;
	::ReadConsoleW(gConsoleInput, &buf1, 1, &buf1count, nullptr);
}

void FConsole::SetTextColor(const EConsoleTextColor color)
{
	WORD attr;

	switch(color)
	{
	default:
	case EConsoleTextColor::White:
		attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		break;
	case EConsoleTextColor::Red:
		attr = FOREGROUND_RED;
		break;
	case EConsoleTextColor::Green:
		attr = FOREGROUND_GREEN;
		break;
	}
	
	::SetConsoleTextAttribute(gConsoleOutput, attr);
}
