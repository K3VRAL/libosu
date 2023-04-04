#include "unrelated/reading_line.h"

static char *ou_readingline_removecrlf(char *line, int read) {
	if (read >= 2 && *(line + read - 2) == '\r') {
		*(line + read - 2) = '\0';
	} else if (read >= 1 && *(line + read - 1) == '\n') {
		*(line + read - 1) = '\0';
	}
	return line;
}

static bool ou_readingline_iscomment(char *line) {
	return strncmp(line, "//", 2) == 0;
}

char *ou_readingline_line(FILE *fp) {
	static int state = 0;
	static char line[8192] = {0};
	switch (state) {
		case 0:
			state = 1;
			while (fgets(line, sizeof(line), fp)) {
				ou_readingline_removecrlf(line, strlen(line));
				if (strlen(line) == 0
					|| ou_readingline_iscomment(line)) {
					continue;
				}
				return line;
		case 1:;
			}
	}
	state = 0;
	return NULL;
}
