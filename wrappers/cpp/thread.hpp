// Copyright 2020-2024 Nikita Fediuchin. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/***********************************************************************************************************************
 * @file
 * @brief Common thread functions.
 * @details See the @ref thread.h
 **********************************************************************************************************************/

#pragma once
#include <string>

extern "C"
{
#include "mpmt/thread.h"
}

namespace mpmt
{

using namespace std;

/**
 * @brief Common thread functions.
 * @details See the @ref thread.h
 */
class Thread final
{
public:
	/**
	 * @brief Sets current thread as main.
	 * @details See the @ref setMainThread().
	 */
	static void setMain() noexcept
	{
		setMainThread();
	}

	/**
	 * @brief Returns true if current thread is main.
	 * @details See the @ref isCurrentThreadMain().
	 */
	static bool isCurrentThreadMain() noexcept
	{
		return isCurrentThreadMain();
	}

	/**
	 * @brief Returns current thread name.
	 * @details See the @ref getThreadName().
	 * @param[out] name pointer to the thread name string
	 * @param size maximal name string capacity (including '\\0')
	 */
	static void getName(char* name, size_t size) noexcept
	{
		getThreadName(name, size);
	}

	/**
	 * @brief Sets current thread name.
	 * @details See the @ref setThreadName().
	 * @param[in] name thread name string
	 */
	static void setName(const string& name) noexcept
	{
		setThreadName(name.c_str());
	}

	/**
	 * @brief Sets current thread priority to foreground.
	 * @details See the @ref setThreadForegroundPriority().
	 */
	static void setForegroundPriority() noexcept
	{
		setThreadForegroundPriority();
	}

	/**
	 * @brief Sets current thread priority to background.
	 * @details See the @ref setThreadBackgroundPriority().
	 */
	static void setBackgroundPriority() noexcept
	{
		setThreadBackgroundPriority();
	}

	// Note: for other functions use C++ STD thread.
};

}; // mpmt