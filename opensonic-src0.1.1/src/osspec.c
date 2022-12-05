/*
 * osspec.c - OS Specific Routines
 * Copyright (C) 2009  Alexandre Martins < alemartf [.a.t.] gmail [.d.o.t.] com >
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <allegro.h>
#include "global.h"
#include "osspec.h"


#ifndef __WIN32__

#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#define DIR_SLASH '/'
#define DIR_SLASH_OTHER '\\'

#else

#include <winalleg.h>
#define DIR_SLASH '\\'
#define DIR_SLASH_OTHER '/'

#endif




/* private variables */
#ifndef __WIN32__
static struct passwd *userinfo;
#endif



/* public functions */

/* 
 * init_os_specifics()
 * OS Specific initialization
 */
void init_os_specifics()
{
#ifndef __WIN32__
    int i;
    char tmp[1024];
    char subdirs[][32] = {
        { "" },              /* $HOME/.$GAME_UNIXNAME/ */
        { "images" },        /* $HOME/.$GAME_UNIXNAME/images */
        { "samples" },       /* etc. */
        { "musics" },
        { "screenshots" },
        { "config" },
        { "themes" },
        { "levels" },
        { "quests"}
    }; /* TODO: quest '.sav' directory? */



    /* retrieving user data */
    if(NULL == (userinfo = getpwuid(getuid())))
        fprintf(stderr, "WARNING: couldn't obtain information about your user. User-specific data may not work.\n");



    /* creating sub-directories */
    for(i=0; i<sizeof(subdirs)/32; i++) {
        home_filepath(tmp, subdirs[i], sizeof(tmp));
        mkdir(tmp, 0755);
    }
#endif
}




/*
 * filepath_exists()
 * Returns TRUE if the given file exists
 * or FALSE otherwise
 */
int filepath_exists(char *filepath)
{
    return exists(filepath);
}



/*
 * directory_exists()
 * Returns TRUE if the given directory exists
 * or FALSE otherwise
 */
int directory_exists(char *dirpath)
{
    return file_exists(dirpath, FA_DIREC | FA_HIDDEN | FA_RDONLY, NULL);
}




/*
 * absolute_filepath()
 * Converts a relative filepath into an
 * absolute filepath.
 */
void absolute_filepath(char *dest, char *relativefp, size_t dest_size)
{
    char *p;

    if(is_relative_filename(relativefp)) {
        get_executable_name(dest, dest_size);
        replace_filename(dest, dest, relativefp, dest_size);
    }
    else
        strcpy(dest, relativefp); /* relativefp is already an absolute filepath */

    for(p=dest; *p; p++) { *p = (*p==DIR_SLASH_OTHER) ? DIR_SLASH : *p; }
    canonicalize_filename(dest, dest, dest_size);
}



/*
 * home_filepath()
 * Similar to absolute_filepath(), but this routine considers
 * the $HOME/.$GAME_UNIXNAME/ directory instead
 */
void home_filepath(char *dest, char *relativefp, size_t dest_size)
{
#ifndef __WIN32__

    char *p;

    if(userinfo) {
        sprintf(dest, "%s/.%s/%s", userinfo->pw_dir, GAME_UNIXNAME, relativefp);
        for(p=dest; *p; p++) { *p = (*p==DIR_SLASH_OTHER) ? DIR_SLASH : *p; }
        canonicalize_filename(dest, dest, dest_size);
    }
    else
        absolute_filepath(dest, relativefp, dest_size);

#else

    absolute_filepath(dest, relativefp, dest_size);

#endif
}




/*
 * resource_filepath()
 * Similar to absolute_filepath() and home_filepath(), but this routine
 * searches the specified file both in the home directory and in the
 * game directory
 */
void resource_filepath(char *dest, char *relativefp, size_t dest_size, int resfp_mode)
{
    switch(resfp_mode) {
        /* I'll read the file */
        case RESFP_READ:
        {
            home_filepath(dest, relativefp, dest_size);
            if(!filepath_exists(dest) && !directory_exists(dest))
                absolute_filepath(dest, relativefp, dest_size);
            break;
        }

        /* I'll write to the file */
        case RESFP_WRITE:
        {
            FILE *fp;
            struct al_ffblk info;
            absolute_filepath(dest, relativefp, dest_size);

            if(al_findfirst(dest, &info, FA_HIDDEN) == 0) {
                /* the file exists AND it's NOT read-only */
                al_findclose(&info);
            }
            else {

                /* the file does not exist OR it is read-only */
                if(!exists(dest)) {

                    /* it doesn't exist */
                    if(NULL != (fp = fopen(dest, "w"))) {
                        /* is it writable? */
                        fclose(fp);
                        delete_file(dest);
                    }
                    else {
                        /* it's not writable */
                        home_filepath(dest, relativefp, dest_size);
                    }

                }
                else {
                    /* the file exists, but it's read-only */
                    home_filepath(dest, relativefp, dest_size);
                }

            }

            break;
        }

        /* Unknown mode */
        default:
        {
            fprintf(stderr, "resource_filepath(): invalid resfp_mode (%d)", resfp_mode);
            break;
        }
    }
}



/*
 * create_process()
 * Creates a new process;
 *     path is the absolute path to the executable file
 *     argc is argument count
 *     argv[] contains the command line arguments
 *
 * NOTE: argv[0] also contains the absolute path of the executable
 */
void create_process(char *path, int argc, char *argv[])
{
#ifndef __WIN32__
    pid_t pid;

    argv[argc] = NULL;
    if(0 == (pid=fork())) /* if(child process) */
        execv(path, argv);
#else
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char cmd[10240]="";
    int i, is_file;

    for(i=0; i<argc; i++) {
        is_file = exists(argv[i]); /* TODO: test folders with spaces (must test program.exe AND level.lev) */
        if(is_file) strcat(cmd, "\"");
        strcat(cmd, argv[i]);
        strcat(cmd, is_file ? "\" " : " ");
    }
    cmd[strlen(cmd)-1] = '\0'; /* erase the last blank space */

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if(!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        MessageBox(NULL, "Couldn't CreateProcess()", "Error", MB_OK);
        MessageBox(NULL, cmd, "Command Line", MB_OK);
        exit(1);
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
#endif
}


/*
 * basename()
 * Finds out the filename portion of a completely specified file path
 */
char *basename(char *path)
{
    return get_filename(path);
}
