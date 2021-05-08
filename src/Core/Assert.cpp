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
#include "../resource.h"

static thread_local bool gIgnoreAsserts = false;

enum class EAssertResponse
{
	Ignore,
	IgnoreAll,
	Break,
	Abort
};

struct SDialogInitParam
{
	const wchar_t* Code;
	const wchar_t* Filename;
	uint Line;
};

INT_PTR WINAPI AssertDlgProc(HWND dialogWindow, const UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			SDialogInitParam& initParam = *(SDialogInitParam*)lParam;

			::SetDlgItemTextW(dialogWindow, IDC_ASSERT_FILENAME, initParam.Filename);

			wchar_t lineBuffer[8];
			_ultow(initParam.Line, lineBuffer, 10);
			::SetDlgItemTextW(dialogWindow, IDC_ASSERT_LINE, lineBuffer);
			::SetDlgItemTextW(dialogWindow, IDC_ASSERT_CODE, initParam.Code);
			return TRUE;
		}
	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			default:
				return FALSE;
			case IDABORT:
				::EndDialog(dialogWindow, (INT_PTR)EAssertResponse::Abort);
				return TRUE;
			case IDIGNORE:
				::EndDialog(dialogWindow, (INT_PTR)EAssertResponse::Ignore);
				return TRUE;
			case IDIGNOREALL:
				::EndDialog(dialogWindow, (INT_PTR)EAssertResponse::IgnoreAll);
				return TRUE;
			case IDBREAK:
				::EndDialog(dialogWindow, (INT_PTR)EAssertResponse::Break);
				return TRUE;
			}
		}
	case WM_CLOSE:
		::EndDialog(dialogWindow, (INT_PTR)EAssertResponse::Abort);
		return TRUE;
	default:
		return FALSE;
	}
}

bool AssertInternal::Assert(const char* code, const char* filename, const uint line)
{
	if (gIgnoreAsserts)return false;

	EAssertResponse response = EAssertResponse::Abort;

	const size_t codeLen = strlen(code) + 1;
	const size_t filenameLen = strlen(filename) + 1;

	wchar_t* codeBuffer = new wchar_t[codeLen];
	wchar_t* filenameBuffer = new wchar_t[filenameLen];

	if (FStringConv::ToUtf16(code, (uint)codeLen, codeBuffer))
	{
		if (FStringConv::ToUtf16(filename, (uint)filenameLen, filenameBuffer))
		{
			SDialogInitParam dlgInitParam = {codeBuffer, filenameBuffer, line};
			response = (EAssertResponse)::DialogBoxParamW(NULL, MAKEINTRESOURCEW(IDD_DIALOG_ASSERT), NULL,
			                                              &AssertDlgProc,
			                                              (LPARAM)&dlgInitParam);
		}
	}

	delete[] codeBuffer;
	delete[] filenameBuffer;

	switch (response)
	{
	case EAssertResponse::Abort:
		::ExitProcess(2);
	case EAssertResponse::Break:
		return true;
	case EAssertResponse::Ignore:
		break;
	case EAssertResponse::IgnoreAll:
		gIgnoreAsserts = true;
		break;
	}

	return false;
}
