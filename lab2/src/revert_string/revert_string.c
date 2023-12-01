#include "revert_string.h"
#include <string.h>
#include <stdlib.h>

void RevertString(char *str)
{
	size_t n = strlen(str);
	char *tmp_str = malloc(sizeof(char) * (n + 1));
	strcpy(tmp_str, str);
	for (int i = 0; i < n; i++) {
		str[i] = tmp_str[n - i - 1];
	}
	free(tmp_str);
}

