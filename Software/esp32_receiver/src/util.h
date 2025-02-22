#include <stddef.h>
#include <stdint.h>

typedef struct {
	size_t length;
	char *data;
} rxdata_t;

union crc_value {
	uint16_t intVal;
	char strVal[2];
};

