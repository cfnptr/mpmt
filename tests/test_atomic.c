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
	atomic_int32* value32 = calloc(1, sizeof(atomic_int32));
	atomic_int64* value64 = calloc(1, sizeof(atomic_int64));
	atomicStore32(value32, 123); atomicStore64(value64, 456);
	bool result = atomicLoad32(value32) == 123 && atomicLoad64(value64) == 456;
	result &= atomicLoad32(value32) == 123 && atomicLoad64(value64) == 456;
	result &= atomicFetchAdd32(value32, 1) == 123 && atomicFetchAdd64(value64, 1) == 456;
	result &= atomicLoad32(value32) == 124 && atomicLoad64(value64) == 457;
	result &= atomicExchange32(value32, 0) == 124 && atomicExchange64(value64, 0) == 457;
	atomicFetchOr32(value32, 111); atomicFetchOr64(value64, 2222);
	result &= atomicLoad32(value32) == 111 && atomicLoad64(value64) == 2222;
	atomicFetchAnd32(value32, 1); atomicFetchAnd64(value64, 0);
	result &= atomicLoad32(value32) == 1 && atomicLoad64(value64) == 0;
	return result ? EXIT_SUCCESS : EXIT_FAILURE;
}