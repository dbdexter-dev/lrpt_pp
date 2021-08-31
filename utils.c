#include <stdio.h>
#include "utils.h"

#ifndef VERSION
#define VERSION "0.0"
#endif

void
usage(const char *pname)
{
	fprintf(stderr, "Usage: %s [options]\n", pname);
#ifdef ENABLE_GTK
	fprintf(stderr, "Omitting options will start up the GUI\n");
#endif
	fprintf(stderr,
			"   -e, --enhance <name>    Apply enhancement <name>\n"
			"   -h, --help              Print this help screen\n"
			"   -v, --version           Print version info\n"
		   );

	fprintf(stderr,
			"\nAvailable enhancements:\n"
			"   none - no enhancement\n"
			"   rgb122 - natural-looking clouds and terrain (requires sensors 1 and 2)\n"
			"   rgb211 - highlights vegetation in red (requires sensors 1 and 2)\n"
			"   vegetation - highlights vegetation in green (requires sensors 1 and 2)\n"
			"   thermal - colors image based on temperature (requires sensor5/6)\n"
			"   hvc - colors clouds and terrain based on temperature (requires sensors 2 and 5/6)\n"
			"   hvc_precip - same as hvc, but also shows precipitation areas (requires sensors 2 and 5/6)\n"
		   );
}

void
version()
{
#ifdef ENABLE_GTK
	fprintf(stderr, "lrpt_pp v" VERSION ", with GUI\n");
#else
	fprintf(stderr, "lrpt_pp v" VERSION "\n");
#endif
}
