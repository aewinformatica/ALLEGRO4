# ---------------------
# CMakeLists.txt
# ---------------------

# Initializing...
CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
PROJECT(opensonic C)
SET(GAME_BIN opensonic)
SET(LAUNCHER_BIN opensonic_launcher)
SET(GAME_VERSION "0.1.1")
SET(CMAKE_BUILD_TYPE Release)
MESSAGE("Open Sonic version ${GAME_VERSION}")



# We need some libraries...
FIND_LIBRARY(LALLEG NAMES alleg)
IF(NOT LALLEG)
  MESSAGE(FATAL_ERROR "Fatal error: lib Allegro not found! Please read the docs for more information.")
ENDIF(NOT LALLEG)

FIND_LIBRARY(LDUMB NAMES aldmb)
IF(NOT LDUMB)
  MESSAGE(FATAL_ERROR "Fatal error: lib DUMB (with Allegro support) not found! Please read the docs for more information.")
ENDIF(NOT LDUMB)

FIND_LIBRARY(LALPNG NAMES alpng)
IF(NOT LALPNG)
  MESSAGE(FATAL_ERROR "Fatal error: lib AllegroPNG not found! Please read the docs for more information.")
ENDIF(NOT LALPNG)



# source files
SET(
  GAME_SRCS
  src/2xsai.c
  src/actor.c
  src/audio.c
  src/boss.c
  src/brick.c
  src/endofdemo.c
  src/enemy.c
  src/font.c
  src/input.c
  src/intro.c
  src/item.c
  src/level.c
  src/main.c
  src/menu.c
  src/osspec.c
  src/player.c
  src/quest.c
  src/questover.c
  src/scene.c
  src/screenshot.c
  src/sprite.c
  src/util.c
  src/video.c
)

SET(
  LAUNCHER_SRCS
  src/launcher.c
  src/lexgui.c
  src/osspec.c
)



# Linux executable
IF(UNIX)
  SET(GAME_SRCS ${GAME_SRCS} src/iconlin.c)
  SET(LAUNCHER_SRCS ${LAUNCHER_SRCS} src/iconlin.c)
  ADD_EXECUTABLE(${GAME_BIN} ${GAME_SRCS})
  ADD_EXECUTABLE(${LAUNCHER_BIN} ${LAUNCHER_SRCS})
  SET_TARGET_PROPERTIES(${GAME_BIN} PROPERTIES LINK_FLAGS "`allegro-config --libs`")
  TARGET_LINK_LIBRARIES(${GAME_BIN} m aldmb dumb alpng)
  SET_TARGET_PROPERTIES(${LAUNCHER_BIN} PROPERTIES LINK_FLAGS "`allegro-config --libs`")
  SET_TARGET_PROPERTIES(${GAME_BIN} PROPERTIES COMPILE_FLAGS "-Wall -O2")
  SET_TARGET_PROPERTIES(${LAUNCHER_BIN} PROPERTIES COMPILE_FLAGS "-Wall -O2")
ENDIF(UNIX)



# Windows executable
IF(WIN32)

  # MSVC fix
  IF(MSVC)
    SET(
      GAME_SRCS
      ${GAME_SRCS}
      src/2xsai.h
      src/actor.h
      src/audio.h
      src/boss.h
      src/brick.h
      src/endofdemo.h
      src/enemy.h
      src/font.h
      src/global.h
      src/input.h
      src/intro.h
      src/item.h
      src/level.h
      src/menu.h
      src/osspec.h
      src/player.h
      src/quest.h
      src/questover.h
      src/scene.h
      src/screenshot.h
      src/sprite.h
      src/util.h
      src/video.h
    )
    SET(
      LAUNCHER_SRCS
      ${LAUNCHER_SRCS}
      src/global.h
      src/lexgui.h
      src/osspec.h
    )
  ENDIF(MSVC)

  # Executables
  ADD_EXECUTABLE(${GAME_BIN} WIN32 ${GAME_SRCS})
  ADD_EXECUTABLE(${LAUNCHER_BIN} WIN32 ${LAUNCHER_SRCS})

  # Other properties
  IF(MSVC)
    SET_TARGET_PROPERTIES(${GAME_BIN} PROPERTIES COMPILE_FLAGS "/D_CRT_SECURE_NO_DEPRECATE /D__WIN32__")
    SET_TARGET_PROPERTIES(${LAUNCHER_BIN} PROPERTIES COMPILE_FLAGS "/D_CRT_SECURE_NO_DEPRECATE /D__WIN32__")
    TARGET_LINK_LIBRARIES(${GAME_BIN} aldmb dumb alpng alleg)
    TARGET_LINK_LIBRARIES(${LAUNCHER_BIN} alleg)
  ELSE(MSVC)
    SET_TARGET_PROPERTIES(${GAME_BIN} PROPERTIES COMPILE_FLAGS "-Wall -O2 -ansi")
    SET_TARGET_PROPERTIES(${LAUNCHER_BIN} PROPERTIES COMPILE_FLAGS "-Wall -O2")
    TARGET_LINK_LIBRARIES(${GAME_BIN} m aldmb dumb alpng alleg)
    TARGET_LINK_LIBRARIES(${LAUNCHER_BIN} alleg)
  ENDIF(MSVC)
  SET_TARGET_PROPERTIES(${GAME_BIN} PROPERTIES LINK_FLAGS "src/iconwin.res")
  SET_TARGET_PROPERTIES(${LAUNCHER_BIN} PROPERTIES LINK_FLAGS "src/iconwin.res")
ENDIF(WIN32)



# Misc
SET_TARGET_PROPERTIES(${GAME_BIN} PROPERTIES PROJECT_NAME "Open Sonic")
SET_TARGET_PROPERTIES(${LAUNCHER_BIN} PROPERTIES PROJECT_NAME "Open Sonic Launcher")





# Installing on Linux
IF(UNIX)
  SET(GAME_INSTALL_DIR "/usr/share/opensonic")
  SET(GAME_FINALPATH "/usr/bin/opensonic")

  INSTALL(CODE "MESSAGE(\"Installing Open Sonic ${GAME_VERSION}... Make sure you have root privileges.\")")
  INSTALL(TARGETS ${GAME_BIN} ${LAUNCHER_BIN} RUNTIME DESTINATION ${GAME_INSTALL_DIR})
  INSTALL(FILES license.txt readme.html DESTINATION ${GAME_INSTALL_DIR})
  INSTALL(DIRECTORY config gui images levels licenses musics quests samples screenshots themes DESTINATION ${GAME_INSTALL_DIR} PATTERN ".svn" EXCLUDE)

  INSTALL(CODE "MESSAGE(\"Creating symbolic links...\")")
  INSTALL(CODE "EXECUTE_PROCESS(COMMAND \"cmake\" \"-E\" \"create_symlink\" \"${GAME_INSTALL_DIR}/${LAUNCHER_BIN}\" \"${GAME_FINALPATH}\")")

  INSTALL(CODE "MESSAGE(\"Done! Type opensonic to start the game. :)\")")
ENDIF(UNIX)
