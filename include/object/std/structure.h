#ifndef OOS_STRUCTURE_H
#define OOS_STRUCTURE_H

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#structure
typedef struct Structure {
    int version;
} Structure;

Structure oos_structure_init(void);
void oos_structure_free(Structure);

#endif