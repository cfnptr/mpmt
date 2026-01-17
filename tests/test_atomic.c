// Copyright 2020-2026 Nikita Fediuchin. All rights reserved.
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

#include "mpmt/atomic.h"
#include <stdbool.h>
#include <stdlib.h>

int main()
{
	atomic_int32 value32 = 0; atomic_int64 value64 = 0;
	atomicStore32(&value32, 123); atomicStore64(&value64, 456);
	bool result = value32 == 123 && value64 == 456;
	result &= atomicLoad32(&value32) == 123 && atomicLoad64(&value64) == 456;
	result &= atomicFetchAdd32(&value32, 1) == 123 && atomicFetchAdd64(&value64, 1) == 456;
	result &= value32 == 124 && value64 == 457;
	result &= atomicExchange32(&value32, 0) == 124 && atomicExchange64(&value64, 0) == 457;
	result &= atomicOrFetch32(&value32, 111) == 111 && atomicOrFetch64(&value64, 2222) == 2222;
	result &= atomicAndFetch32(&value32, 1) == 1 && atomicAndFetch64(&value64, 0) == 0;
	return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
