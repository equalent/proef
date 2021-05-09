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

#pragma once

#ifdef PF_UNIT_TEST
struct __SUnitTestDesc
{
	const char* TestName;
	const char* TestFilename;
	void(*TestBody)();
};

extern TArray<__SUnitTestDesc, TRawAllocator<__SUnitTestDesc, EAllocationPurpose::InternalDynamicInit>> __gUnitTests;

struct __SUnitTestFactory
{
	__SUnitTestFactory(const char* testName, const char* testFilename, void(*testBody)())
	{
		__gUnitTests.Add(__SUnitTestDesc{ testName, testFilename, testBody });
	}
};

#define UnitTest(name) void __UnitTest_##name(); __SUnitTestFactory __gUnitTestFactory_##name(#name, __FILE__, &__UnitTest_##name); void __UnitTest_##name ()

[[noreturn]] void RunUnitTestsAndExit();
bool __UnitTestAssert(bool result, const char* code, const char* message, uint line);

#define tcheck(code) __UnitTestAssert((code), #code, nullptr, __LINE__)
#define tcheckm(code, message) __UnitTestAssert((code), #code, message, __LINE__)

#define tverify(code) if(__UnitTestAssert((code), #code, nullptr, __LINE__))return
#define tverifym(code, message) if(__UnitTestAssert((code), #code, message, __LINE__))return

#else
#define UnitTest(name) void __unused__UnitTest_##name ()
#define tcheck(code)
#define tcheckm(code)
#define tverify(code)
#define tverifym(code)
#endif