/*
 * Copyright (C) 2008,2009  OMRON SOFTWARE Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * approxPatternEnToUpper
 */
static const PREDEF_APPROX_PATTERN approxPatternEnToUpper = {
	/* size */
	26,
	/* from */
	(const NJ_UINT8* )
	"\x00\x61\x00\x62\x00\x63\x00\x64\x00\x65\x00\x66\x00\x67\x00\x68"
	"\x00\x69\x00\x6a\x00\x6b\x00\x6c\x00\x6d\x00\x6e\x00\x6f\x00\x70"
	"\x00\x71\x00\x72\x00\x73\x00\x74\x00\x75\x00\x76\x00\x77\x00\x78"
	"\x00\x79\x00\x7a",
	/* to */
	(const NJ_UINT8* )
	"\x00\x41\x00\x42\x00\x43\x00\x44\x00\x45\x00\x46\x00\x47\x00\x48"
	"\x00\x49\x00\x4a\x00\x4b\x00\x4c\x00\x4d\x00\x4e\x00\x4f\x00\x50"
	"\x00\x51\x00\x52\x00\x53\x00\x54\x00\x55\x00\x56\x00\x57\x00\x58"
	"\x00\x59\x00\x5a"
};

/**
 * approxPatternEnToLower
 */
static const PREDEF_APPROX_PATTERN approxPatternEnToLower = {
	/* size */
	26,
	/* from */
	(const NJ_UINT8* )
	"\x00\x41\x00\x42\x00\x43\x00\x44\x00\x45\x00\x46\x00\x47\x00\x48"
	"\x00\x49\x00\x4a\x00\x4b\x00\x4c\x00\x4d\x00\x4e\x00\x4f\x00\x50"
	"\x00\x51\x00\x52\x00\x53\x00\x54\x00\x55\x00\x56\x00\x57\x00\x58"
	"\x00\x59\x00\x5a",
	/* to */
	(const NJ_UINT8* )
	"\x00\x61\x00\x62\x00\x63\x00\x64\x00\x65\x00\x66\x00\x67\x00\x68"
	"\x00\x69\x00\x6a\x00\x6b\x00\x6c\x00\x6d\x00\x6e\x00\x6f\x00\x70"
	"\x00\x71\x00\x72\x00\x73\x00\x74\x00\x75\x00\x76\x00\x77\x00\x78"
	"\x00\x79\x00\x7a"
};

/**
 * approxPatternEnQwertyNear
 */
static const PREDEF_APPROX_PATTERN approxPatternEnQwertyNear = {
	/* size */
	110,
	/* from */
	(const NJ_UINT8* )
	"\x00\x61\x00\x61\x00\x61\x00\x61\x00\x62\x00\x62\x00\x62\x00\x62"
	"\x00\x63\x00\x63\x00\x63\x00\x63\x00\x64\x00\x64\x00\x64\x00\x64"
	"\x00\x64\x00\x64\x00\x65\x00\x65\x00\x65\x00\x65\x00\x66\x00\x66"
	"\x00\x66\x00\x66\x00\x66\x00\x66\x00\x67\x00\x67\x00\x67\x00\x67"
	"\x00\x67\x00\x67\x00\x68\x00\x68\x00\x68\x00\x68\x00\x68\x00\x68"
	"\x00\x69\x00\x69\x00\x69\x00\x69\x00\x6a\x00\x6a\x00\x6a\x00\x6a"
	"\x00\x6a\x00\x6a\x00\x6b\x00\x6b\x00\x6b\x00\x6b\x00\x6b\x00\x6c"
	"\x00\x6c\x00\x6c\x00\x6d\x00\x6d\x00\x6d\x00\x6e\x00\x6e\x00\x6e"
	"\x00\x6e\x00\x6f\x00\x6f\x00\x6f\x00\x6f\x00\x70\x00\x70\x00\x71"
	"\x00\x71\x00\x72\x00\x72\x00\x72\x00\x72\x00\x73\x00\x73\x00\x73"
	"\x00\x73\x00\x73\x00\x73\x00\x74\x00\x74\x00\x74\x00\x74\x00\x75"
	"\x00\x75\x00\x75\x00\x75\x00\x76\x00\x76\x00\x76\x00\x76\x00\x77"
	"\x00\x77\x00\x77\x00\x77\x00\x78\x00\x78\x00\x78\x00\x78\x00\x79"
	"\x00\x79\x00\x79\x00\x79\x00\x7a\x00\x7a\x00\x7a",
	/* to */
	(const NJ_UINT8* )
	"\x00\x71\x00\x77\x00\x73\x00\x7a\x00\x76\x00\x67\x00\x68\x00\x6e"
	"\x00\x78\x00\x64\x00\x66\x00\x76\x00\x65\x00\x72\x00\x66\x00\x63"
	"\x00\x78\x00\x73\x00\x72\x00\x64\x00\x73\x00\x77\x00\x72\x00\x74"
	"\x00\x67\x00\x76\x00\x63\x00\x64\x00\x74\x00\x79\x00\x68\x00\x62"
	"\x00\x76\x00\x66\x00\x79\x00\x75\x00\x6a\x00\x6e\x00\x62\x00\x67"
	"\x00\x6f\x00\x6b\x00\x6a\x00\x75\x00\x75\x00\x69\x00\x6b\x00\x6d"
	"\x00\x6e\x00\x68\x00\x69\x00\x6f\x00\x6c\x00\x6d\x00\x6a\x00\x6b"
	"\x00\x6f\x00\x70\x00\x6e\x00\x6a\x00\x6b\x00\x62\x00\x68\x00\x6a"
	"\x00\x6d\x00\x70\x00\x6c\x00\x6b\x00\x69\x00\x6c\x00\x6f\x00\x77"
	"\x00\x61\x00\x74\x00\x66\x00\x64\x00\x65\x00\x65\x00\x64\x00\x78"
	"\x00\x7a\x00\x61\x00\x77\x00\x79\x00\x67\x00\x66\x00\x72\x00\x69"
	"\x00\x6a\x00\x68\x00\x79\x00\x63\x00\x66\x00\x67\x00\x62\x00\x65"
	"\x00\x73\x00\x61\x00\x71\x00\x7a\x00\x73\x00\x64\x00\x63\x00\x75"
	"\x00\x68\x00\x67\x00\x74\x00\x61\x00\x73\x00\x78"
};

/**
 * approxPatternEnQwertyNearUpper
 */
static const PREDEF_APPROX_PATTERN approxPatternEnQwertyNearUpper = {
	/* size */
	110,
	/* from */
	(const NJ_UINT8* )
	"\x00\x61\x00\x61\x00\x61\x00\x61\x00\x62\x00\x62\x00\x62\x00\x62"
	"\x00\x63\x00\x63\x00\x63\x00\x63\x00\x64\x00\x64\x00\x64\x00\x64"
	"\x00\x64\x00\x64\x00\x65\x00\x65\x00\x65\x00\x65\x00\x66\x00\x66"
	"\x00\x66\x00\x66\x00\x66\x00\x66\x00\x67\x00\x67\x00\x67\x00\x67"
	"\x00\x67\x00\x67\x00\x68\x00\x68\x00\x68\x00\x68\x00\x68\x00\x68"
	"\x00\x69\x00\x69\x00\x69\x00\x69\x00\x6a\x00\x6a\x00\x6a\x00\x6a"
	"\x00\x6a\x00\x6a\x00\x6b\x00\x6b\x00\x6b\x00\x6b\x00\x6b\x00\x6c"
	"\x00\x6c\x00\x6c\x00\x6d\x00\x6d\x00\x6d\x00\x6e\x00\x6e\x00\x6e"
	"\x00\x6e\x00\x6f\x00\x6f\x00\x6f\x00\x6f\x00\x70\x00\x70\x00\x71"
	"\x00\x71\x00\x72\x00\x72\x00\x72\x00\x72\x00\x73\x00\x73\x00\x73"
	"\x00\x73\x00\x73\x00\x73\x00\x74\x00\x74\x00\x74\x00\x74\x00\x75"
	"\x00\x75\x00\x75\x00\x75\x00\x76\x00\x76\x00\x76\x00\x76\x00\x77"
	"\x00\x77\x00\x77\x00\x77\x00\x78\x00\x78\x00\x78\x00\x78\x00\x79"
	"\x00\x79\x00\x79\x00\x79\x00\x7a\x00\x7a\x00\x7a",
	/* to */
	(const NJ_UINT8* )
	"\x00\x51\x00\x57\x00\x53\x00\x5a\x00\x56\x00\x47\x00\x48\x00\x4e"
	"\x00\x58\x00\x44\x00\x46\x00\x56\x00\x45\x00\x52\x00\x46\x00\x43"
	"\x00\x58\x00\x53\x00\x52\x00\x44\x00\x53\x00\x57\x00\x52\x00\x54"
	"\x00\x47\x00\x56\x00\x43\x00\x44\x00\x54\x00\x59\x00\x48\x00\x42"
	"\x00\x56\x00\x46\x00\x59\x00\x55\x00\x4a\x00\x4e\x00\x42\x00\x47"
	"\x00\x4f\x00\x4b\x00\x4a\x00\x55\x00\x55\x00\x49\x00\x4b\x00\x4d"
	"\x00\x4e\x00\x48\x00\x49\x00\x4f\x00\x4c\x00\x4d\x00\x4a\x00\x4b"
	"\x00\x4f\x00\x50\x00\x4e\x00\x4a\x00\x4b\x00\x42\x00\x48\x00\x4a"
	"\x00\x4d\x00\x50\x00\x4c\x00\x4b\x00\x49\x00\x4c\x00\x4f\x00\x57"
	"\x00\x41\x00\x54\x00\x46\x00\x44\x00\x45\x00\x45\x00\x44\x00\x58"
	"\x00\x5a\x00\x41\x00\x57\x00\x59\x00\x47\x00\x46\x00\x52\x00\x49"
	"\x00\x4a\x00\x48\x00\x59\x00\x43\x00\x46\x00\x47\x00\x42\x00\x45"
	"\x00\x53\x00\x41\x00\x51\x00\x5a\x00\x53\x00\x44\x00\x43\x00\x55"
	"\x00\x48\x00\x47\x00\x54\x00\x41\x00\x53\x00\x58"
};

/**
 * approxPatternJAJP12keyNormal
 */
static const PREDEF_APPROX_PATTERN approxPatternJAJP12keyNormal = {
	/* size */
	37,
	/* from */
	(const NJ_UINT8* )
	"\x30\x42\x30\x44\x30\x46\x30\x48\x30\x4a\x30\x4b\x30\x4d\x30\x4f"
	"\x30\x51\x30\x53\x30\x55\x30\x57\x30\x59\x30\x5b\x30\x5d\x30\x5f"
	"\x30\x61\x30\x64\x30\x64\x30\x66\x30\x68\x30\x6f\x30\x72\x30\x75"
	"\x30\x78\x30\x7b\x30\x6f\x30\x72\x30\x75\x30\x78\x30\x7b\x30\x84"
	"\x30\x86\x30\x88\x30\x8f\x30\x9b\x30\x9c",
	/* to */
	(const NJ_UINT8* )
	"\x30\x41\x30\x43\x30\x45\x30\x47\x30\x49\x30\x4c\x30\x4e\x30\x50"
	"\x30\x52\x30\x54\x30\x56\x30\x58\x30\x5a\x30\x5c\x30\x5e\x30\x60"
	"\x30\x62\x30\x63\x30\x65\x30\x67\x30\x69\x30\x70\x30\x73\x30\x76"
	"\x30\x79\x30\x7c\x30\x71\x30\x74\x30\x77\x30\x7a\x30\x7d\x30\x83"
	"\x30\x85\x30\x87\x30\x8e\x30\x9c\x30\x9b"
};

/**
 * tables
 */
static const PREDEF_APPROX_PATTERN* predefinedApproxPatterns[] = {
	&approxPatternEnToUpper, &approxPatternEnToLower, &approxPatternEnQwertyNear, &approxPatternEnQwertyNearUpper, &approxPatternJAJP12keyNormal, NULL
};
