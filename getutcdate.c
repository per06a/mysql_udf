/* Copyright (c) 2000, 2010, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */
  
/*
  @author: Philip Russell
  @link: https://github.com/per06a/mysql_udh
*/

#ifdef STANDARD
/* STANDARD is defined, don't use any mysql functions */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __WIN__
typedef unsigned __int64 ulonglong;/* Microsofts 64 bit types */
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif /*__WIN__*/
#else
#include <my_global.h>
#include <my_sys.h>
#if defined(MYSQL_SERVER)
#include <m_string.h>/* To get strmov() */
#else
/* when compiled as standalone */
#include <string.h>
#define strmov(a,b) stpcpy(a,b)
#define bzero(a,b) memset(a,0,b)
#endif
#endif
#include <mysql.h>
#include <ctype.h>

#ifdef _WIN32
/* inet_aton needs winsock library */
#pragma comment(lib, "ws2_32")
#endif

#ifdef HAVE_DLOPEN

#if !defined(HAVE_GETHOSTBYADDR_R) || !defined(HAVE_SOLARIS_STYLE_GETHOST)
static pthread_mutex_t LOCK_hostname;
#endif


// Actual header files we care about
#include <time.h>
#include <string.h>
#include <sys/time.h>


/* These must be right or mysqld will not find the symbol! */

my_bool GETUTCDATE_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void GETUTCDATE_deinit(UDF_INIT *initid);
char *GETUTCDATE(UDF_INIT *initid, UDF_ARGS *args, char *result,
		 unsigned long *length, char *is_null, char *error);

my_bool GETUTCDATE_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
  if (args->arg_count != 0)
    {
      strcpy(message,"GETUTCDATE() does not take any arguments");
      return 1;
    }
  return 0;
}

/****************************************************************************
 ** Deinit function. This should free all resources allocated by
 ** this function.
 ** Arguments:
 ** initidReturn value from xxxx_init
 ****************************************************************************/

char *GETUTCDATE(UDF_INIT *initid __attribute__((unused)),
		 UDF_ARGS *args, char *result, unsigned long *length,
		 char *is_null, char *error __attribute__((unused))) {

  struct timeval time_of_day;
  struct tm gm_time;
  char buff[*length];

  // Clear the buffer where we are going to store the result
  memset(result, '\0', *length);
  memset(buff, '\0', *length);

  gettimeofday(&time_of_day, NULL);
  gmtime_r(&(time_of_day.tv_sec), &gm_time);

  strftime(buff, *length, "%Y-%m-%d %H:%M:%S.%%06d", &gm_time);
  snprintf(result, *length, buff, time_of_day.tv_usec);

  return result;
}


void GETUTCDATE_deinit(UDF_INIT *initid __attribute__((unused))) {
  
}


#endif /* HAVE_DLOPEN */
