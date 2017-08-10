/*
 * Copyright (c) 1999, 2015, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "jvm.h"
#include "TimeZone_md.h"
#include "jdk_util.h"
#include <VersionHelpers.h>

#define VALUE_UNKNOWN           0
#define VALUE_KEY               1
#define VALUE_MAPID             2
#define VALUE_GMTOFFSET         3

#define MAX_ZONE_CHAR           256
#define MAX_MAPID_LENGTH        32

typedef struct _TziValue {
    LONG        bias;
    LONG        stdBias;
    LONG        dstBias;
    SYSTEMTIME  stdDate;
    SYSTEMTIME  dstDate;
} TziValue;

#if _WIN32_WINNT < 0x0600 /* < _WIN32_WINNT_VISTA */
typedef struct _TIME_DYNAMIC_ZONE_INFORMATION {
    LONG        Bias;
    WCHAR       StandardName[32];
    SYSTEMTIME  StandardDate;
    LONG        StandardBias;
    WCHAR       DaylightName[32];
    SYSTEMTIME  DaylightDate;
    LONG        DaylightBias;
    WCHAR       TimeZoneKeyName[128];
    BOOLEAN     DynamicDaylightTimeDisabled;
} DYNAMIC_TIME_ZONE_INFORMATION, *PDYNAMIC_TIME_ZONE_INFORMATION;
#endif


/*
 * Produces custom name "GMT+hh:mm" from the given bias in buffer.
 */
static void customZoneName(LONG bias, char *buffer) {
    LONG gmtOffset;
    int sign;

    if (bias > 0) {
        gmtOffset = bias;
        sign = -1;
    } else {
        gmtOffset = -bias;
        sign = 1;
    }
    if (gmtOffset != 0) {
        sprintf(buffer, "GMT%c%02d:%02d",
                ((sign >= 0) ? '+' : '-'),
                gmtOffset / 60,
                gmtOffset % 60);
    } else {
        strcpy(buffer, "GMT");
    }
}

/*
 * Use NO_DYNAMIC_TIME_ZONE_INFO as the return value indicating that no
 * dynamic time zone information is available.
 */
#define NO_DYNAMIC_TIME_ZONE_INFO     (-128)

static int getDynamicTimeZoneInfo(PDYNAMIC_TIME_ZONE_INFORMATION pdtzi) {
    DWORD timeType = NO_DYNAMIC_TIME_ZONE_INFO;
    HMODULE dllHandle;

    /*
     * Dynamically load the dll to call GetDynamicTimeZoneInformation.
     */
    dllHandle = JDK_LoadSystemLibrary("Kernel32.dll");
    if (dllHandle != NULL) {
        typedef DWORD (WINAPI *GetDynamicTimezoneInfoType)(PDYNAMIC_TIME_ZONE_INFORMATION);
        GetDynamicTimezoneInfoType getDynamicTimeZoneInfoFunc =
            (GetDynamicTimezoneInfoType) GetProcAddress(dllHandle,
                                                        "GetDynamicTimeZoneInformation");

        if (getDynamicTimeZoneInfo != NULL) {
            timeType = getDynamicTimeZoneInfoFunc(pdtzi);
        }
    }
    return timeType;
}

/*
 * Gets the current time zone entry in the "Time Zones" registry.
 */
static int getWinTimeZone(char *winZoneName, char *winMapID)
{
    TIME_ZONE_INFORMATION tzi;
    HANDLE hKey = NULL, hSubKey = NULL;
    WCHAR *stdNamePtr = tzi.StandardName;
	DWORD timeType;

    // if (IsWindowsVistaOrGreater()) { // UWP assumes yes
	if (TRUE) {
        DYNAMIC_TIME_ZONE_INFORMATION dtzi;

        /*
         * Get the dynamic time zone information, if available, so that time
         * zone redirection can be supported. (see JDK-7044727)
         */
        timeType = getDynamicTimeZoneInfo(&dtzi);
        if (timeType == TIME_ZONE_ID_INVALID) {
			return VALUE_UNKNOWN;
        }

        if (timeType != NO_DYNAMIC_TIME_ZONE_INFO) {
            /*
             * Make sure TimeZoneKeyName is available from the API call. If
             * DynamicDaylightTime is disabled, return a custom time zone name
             * based on the GMT offset. Otherwise, return the TimeZoneKeyName
             * value.
             */
            if (dtzi.TimeZoneKeyName[0] != 0) {
                if (dtzi.DynamicDaylightTimeDisabled) {
                    customZoneName(dtzi.Bias, winZoneName);
                    return VALUE_GMTOFFSET;
                }
                wcstombs(winZoneName, dtzi.TimeZoneKeyName, MAX_ZONE_CHAR);
                return VALUE_KEY;
			} else {
				return VALUE_UNKNOWN;
			}
        }
    }

    /*
     * Fall back to GetTimeZoneInformation
     */
    timeType = GetTimeZoneInformation(&tzi);
    if (timeType == TIME_ZONE_ID_INVALID) {
		return VALUE_UNKNOWN;
	} else {
		return VALUE_KEY;
	}
}

/*
 * The mapping table file name.
 */
#define MAPPINGS_FILE "\\lib\\tzmappings"

/*
 * Index values for the mapping table.
 */
#define TZ_WIN_NAME     0
#define TZ_MAPID        1
#define TZ_REGION       2
#define TZ_JAVA_NAME    3

#define TZ_NITEMS       4       /* number of items (fields) */

/*
 * Looks up the mapping table (tzmappings) and returns a Java time
 * zone ID (e.g., "America/Los_Angeles") if found. Otherwise, NULL is
 * returned.
 *
 * value_type is one of the following values:
 *      VALUE_KEY for exact key matching
 *      VALUE_MAPID for MapID (this is
 *      required for the old Windows, such as NT 4.0 SP3).
 */
static char *matchJavaTZ(const char *java_home_dir, int value_type, char *tzName,
                         char *mapID)
{
    int line;
    int IDmatched = 0;
    FILE *fp;
    char *javaTZName = NULL;
    char *items[TZ_NITEMS];
    char *mapFileName;
    char lineBuffer[MAX_ZONE_CHAR * 4];
    int noMapID = *mapID == '\0';       /* no mapID on Vista and later */

    mapFileName = malloc(strlen(java_home_dir) + strlen(MAPPINGS_FILE) + 1);
    if (mapFileName == NULL) {
        return NULL;
    }
    strcpy(mapFileName, java_home_dir);
    strcat(mapFileName, MAPPINGS_FILE);

    if ((fp = fopen(mapFileName, "r")) == NULL) {
        jio_fprintf(stderr, "can't open %s.\n", mapFileName);
        free((void *) mapFileName);
        return NULL;
    }
    free((void *) mapFileName);

    line = 0;
    while (fgets(lineBuffer, sizeof(lineBuffer), fp) != NULL) {
        char *start, *idx, *endp;
        int itemIndex = 0;

        line++;
        start = idx = lineBuffer;
        endp = &lineBuffer[sizeof(lineBuffer)];

        /*
         * Ignore comment and blank lines.
         */
        if (*idx == '#' || *idx == '\n') {
            continue;
        }

        for (itemIndex = 0; itemIndex < TZ_NITEMS; itemIndex++) {
            items[itemIndex] = start;
            while (*idx && *idx != ':') {
                if (++idx >= endp) {
                    goto illegal_format;
                }
            }
            if (*idx == '\0') {
                goto illegal_format;
            }
            *idx++ = '\0';
            start = idx;
        }

        if (*idx != '\n') {
            goto illegal_format;
        }

        if (noMapID || strcmp(mapID, items[TZ_MAPID]) == 0) {
            /*
             * When there's no mapID, we need to scan items until the
             * exact match is found or the end of data is detected.
             */
            if (!noMapID) {
                IDmatched = 1;
            }
            if (strcmp(items[TZ_WIN_NAME], tzName) == 0) {
                /*
                 * Found the time zone in the mapping table.
                 */
                javaTZName = _strdup(items[TZ_JAVA_NAME]);
                break;
            }
        } else {
            if (IDmatched == 1) {
                /*
                 * No need to look up the mapping table further.
                 */
                break;
            }
        }
    }
    fclose(fp);

    return javaTZName;

 illegal_format:
    (void) fclose(fp);
    jio_fprintf(stderr, "tzmappings: Illegal format at line %d.\n", line);
    return NULL;
}

/*
 * Detects the platform time zone which maps to a Java time zone ID.
 */
char *findJavaTZ_md(const char *java_home_dir)
{
    char winZoneName[MAX_ZONE_CHAR];
    char winMapID[MAX_MAPID_LENGTH];
    char *std_timezone = NULL;
    int  result;

    winMapID[0] = 0;
    result = getWinTimeZone(winZoneName, winMapID);

    if (result != VALUE_UNKNOWN) {
        if (result == VALUE_GMTOFFSET) {
            std_timezone = _strdup(winZoneName);
        } else {
            std_timezone = matchJavaTZ(java_home_dir, result,
                                       winZoneName, winMapID);
            if (std_timezone == NULL) {
                std_timezone = getGMTOffsetID();
            }
        }
    }
    return std_timezone;
}

/**
 * Returns a GMT-offset-based time zone ID.
 */
char *
getGMTOffsetID()
{
    LONG bias = 0;
    HANDLE hKey = NULL;
    char zonename[32];

    // Obtain the current GMT offset value of ActiveTimeBias.
    // If we can't get the ActiveTimeBias value, use Bias of TimeZoneInformation.
    // Note: Bias doesn't reflect current daylight saving.
	// (On UWP, we couldn't get ActiveTimeBias Value)
    TIME_ZONE_INFORMATION tzi;
    if (GetTimeZoneInformation(&tzi) != TIME_ZONE_ID_INVALID) {
        bias = tzi.Bias;
    }

    customZoneName(bias, zonename);
    return _strdup(zonename);
}
