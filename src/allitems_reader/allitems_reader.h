#ifndef ALLITEMS_READER_H
#define ALLITEMS_READER_H

#define ITEM_ID_OFFSET 0
#define LOCATION_ID_OFFSET 4
#define INFO_OFFSET 8
#define MULTIPLIER_OFFSET 12
#define DIFF_MODE_ALLITEMS_OFFSET 16

typedef struct
{
    unsigned int item_id;
    unsigned int location_id;
    unsigned int info;
    unsigned int multiplier;
    unsigned int diff_mode;
} item_line;

int get_item_id_by_location_id(int loc_id);

#endif /* !ALLITEMS_READER_H */