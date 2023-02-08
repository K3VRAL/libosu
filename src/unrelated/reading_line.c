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

#ifdef _WIN32
static int getline_win(char **line, size_t *len, FILE *fp) {
	char *end = *line + *len - 1;
	char *dst = *line;
	int c;
	while ((c = fgetc(fp)) != EOF && c != '\n' && dst < end) {
		*dst++ = c;
	}
	*dst = '\0';
	return (c == EOF && dst == *line) ? EOF : dst - *line;
}
#endif

char *ou_readingline_line(FILE *fp) {
	static int state = 0;
	static char *line = NULL;
	static size_t len;
	static int read;
	switch (state) {
		case 0:
			state = 1;

#ifdef _WIN32
                       while ((read = getline_win(&line, &len, fp)) != -1) {
#endif
#ifdef __unix__
			while ((read = getline(&line, &len, fp)) != -1) {
#endif
				ou_readingline_removecrlf(line, read);
				if (strlen(line) == 0
					|| ou_readingline_iscomment(line)) {
					continue;
				}
				return line;
		case 1:;
			}
	}
	state = 0;
	if (line != NULL) {
		free(line);
		line = NULL;
	}
	return NULL;
}