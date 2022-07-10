#include <osu.h>
#include "global.h"

#define osu_init_file_EXCEPTION (1)
void test_osu_file_init();

int main(void) {
    TRY {
        test_osu_file_init();

        puts("Test Succeeded! No Exception was caught.");
    } CATCH (osu_init_file_EXCEPTION) {
        puts("Test Failed! An Exception has occurred.");
    } ETRY;
}

void test_osu_file_init() {
    Osu osu = osu_init();

    osu.structure = osus_init("./test/files/Go Ichinose - Mizuumi (K 3 V R A L) [test].osu");
    if (osu.structure == NULL) {
        THROW;
    }
    free(structure);
}