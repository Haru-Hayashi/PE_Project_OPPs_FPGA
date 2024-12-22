#ifndef TABLE_REFERENCE
#define TABLE_REFERENCE
#include "UserDefined.h"

void Table_Reference(uint8_t Array[][MAXIMUM_TABLE_SIZE], float *Tc, uint8_t *numlines, uint8_t table_number, uint8_t Slot_Num, uint8_t *step);
void Init_Table(uint8_t Array[][MAXIMUM_TABLE_SIZE]);

#endif