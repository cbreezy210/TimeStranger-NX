// ============================================================
// TimeStranger-NX - Reverse-Engineered Save Structure
// Title ID:     0x010062E01FE0C000
// Save layout:  /savedata/0001.bin  <- slot the GAME LOADS (primary)
//               /savedata/0000.bin  <- secondary slot (keep in sync)
//               /savedata/slot_000X.bin, system_data.bin (metadata)
//               /0000.bin (root)    <- unrelated stats container
// Yen offset:   0x7973 (u32 little-endian)
// CRITICAL:     Writes REQUIRE fsFileWrite(..., FsWriteOption_Flush)
//               AND fsFsCommit(&fs) or they silently roll back!
// Profiles:     Scan ALL users - some profiles have empty saves.
// ============================================================

#include <switch.h>
#include <stdio.h>
#include <string.h>

#define GAME_TITLE_ID 0x010062E01FE0C000ULL
#define MONEY_OFFSET 0x7973

static const char* SAVE_FILES[] = { "/savedata/0001.bin", "/savedata/0000.bin" };
#define NUM_SAVE_FILES 2

int main(int argc, char* argv[]) {
    consoleInit(NULL);
    PadState pad;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);

    fsInitialize();
    accountInitialize((AccountServiceType)0);

    printf("TimeStranger-NX v1.0.0\n");
    printf("By cbreezy210\n");
    printf("Direct NAND Save Editor\n\n");
    consoleUpdate(NULL);

    FsFileSystem saveFs;
    bool fsOpen = false;
    unsigned int yen = 0;
    bool yenRead = false;
    AccountUid usedUid = {0};

    FsSaveDataInfoReader reader;
    Result rc = fsOpenSaveDataInfoReader(&reader, FsSaveDataSpaceId_User);
    if (R_SUCCEEDED(rc)) {
        FsSaveDataInfo info;
        s64 total = 0;
        while (R_SUCCEEDED(fsSaveDataInfoReaderRead(&reader, &info, 1, &total)) && total > 0) {
            if (info.application_id != GAME_TITLE_ID) continue;

            FsSaveDataAttribute attr = {0};
            attr.application_id = info.application_id;
            attr.uid = info.uid;
            attr.save_data_index = info.save_data_index;
            attr.save_data_type = info.save_data_type;

            if (R_FAILED(fsOpenSaveDataFileSystem(&saveFs, FsSaveDataSpaceId_User, &attr))) continue;

            for (int i = 0; i < NUM_SAVE_FILES && !yenRead; i++) {
                FsFile f;
                if (R_SUCCEEDED(fsFsOpenFile(&saveFs, SAVE_FILES[i], FsOpenMode_Read, &f))) {
                    u64 rd = 0;
                    if (R_SUCCEEDED(fsFileRead(&f, MONEY_OFFSET, &yen, 4, FsReadOption_None, &rd)) && rd == 4) {
                        yenRead = true;
                        usedUid = info.uid;
                    }
                    fsFileClose(&f);
                }
            }
            if (yenRead) { fsOpen = true; break; }
            fsFsClose(&saveFs);
        }
        fsSaveDataInfoReaderClose(&reader);
    }

    if (!fsOpen || !yenRead) {
        printf("ERROR: save not found.\nFully close the game and retry.\n");
    } else {
        printf("Profile: %016llX\n", (unsigned long long)usedUid.uid[0]);
        printf("Current Yen: %u\n\n", yen);
        printf("[A] Set 999,999   [B] Set 0\n");
        printf("[X] +100,000      [Y] -100,000\n");
        printf("[+] Write to NAND & Exit\n\n");
        consoleUpdate(NULL);

        bool modified = false;
        while (appletMainLoop()) {
            padUpdate(&pad);
            u64 k = padGetButtonsDown(&pad);

            if (k & HidNpadButton_A) { yen = 999999; modified = true; }
            else if (k & HidNpadButton_B) { yen = 0; modified = true; }
            else if (k & HidNpadButton_X) { yen += 100000; modified = true; }
            else if (k & HidNpadButton_Y) { yen = (yen >= 100000) ? yen - 100000 : 0; modified = true; }

            if (modified && !(k & HidNpadButton_Plus)) {
                consoleClear();
                printf("TimeStranger-NX v1.0.0\nBy cbreezy210\n\n");
                printf("Current Yen: %u\n\n", yen);
                printf("[A] Set 999,999   [B] Set 0\n");
                printf("[X] +100,000      [Y] -100,000\n");
                printf("[+] Write to NAND & Exit\n");
                consoleUpdate(NULL);
            }

            if (k & HidNpadButton_Plus) {
                consoleClear();
                printf("TimeStranger-NX v1.0.0\nBy cbreezy210\n\n");
                if (modified) {
                    printf("Writing to NAND...\n");
                    int written = 0;
                    for (int i = 0; i < NUM_SAVE_FILES; i++) {
                        FsFile f;
                        if (R_SUCCEEDED(fsFsOpenFile(&saveFs, SAVE_FILES[i], FsOpenMode_Read | FsOpenMode_Write, &f))) {
                            Result wr = fsFileWrite(&f, MONEY_OFFSET, &yen, 4, FsWriteOption_Flush);
                            fsFileClose(&f);
                            printf("  %s : %s\n", SAVE_FILES[i], R_SUCCEEDED(wr) ? "OK" : "FAIL");
                            if (R_SUCCEEDED(wr)) written++;
                        }
                    }
                    Result c = fsFsCommit(&saveFs);
                    printf("  COMMIT: %s\n", R_SUCCEEDED(c) ? "ok" : "FAIL");
                    if (written > 0 && R_SUCCEEDED(c))
                        printf("\nSUCCESS! Yen = %u\nBoot the game to verify.\n", yen);
                    else
                        printf("\nWrite problem! See above.\n");
                } else {
                    printf("No changes made.\n");
                }
                consoleUpdate(NULL);
                break;
            }
        }
        fsFsClose(&saveFs);
    }

    printf("\nPress [+] to exit.\n");
    while (appletMainLoop()) {
        padUpdate(&pad);
        if (padGetButtonsDown(&pad) & HidNpadButton_Plus) break;
        consoleUpdate(NULL);
    }

    accountExit();
    fsExit();
    consoleExit(NULL);
    return 0;
}