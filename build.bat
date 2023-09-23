@echo off
setlocal

set files_to_compile=nemu_sys_win32.c
set files_to_link=build/nemu_resource.res

call rc /nologo /y /fo build/nemu_resource.res nemu_sys_win32_resource.rc

set compiler_flags=-O3 -Wall -nostdlib -o "build/nemu_win32.exe"
set linker_flags=-fuse-ld=lld -Wl,-subsystem:windows

if /i "%1" equ "release" (
	where /q clang || (
		echo Error: 'clang' could not be found.
		echo Please install clang and add it to your PATH environment variable.
		exit /b 1
	)
	call clang %compiler_flags% %files_to_compile% %linker_flags% %files_to_link%
	exit /b %errorlevel%
)

set compiler_flags=/nologo /Zi /W3 /Fe:"build/nemu_win32_debug.exe"
set linker_flags=/WX /incremental:no /opt:icf /opt:ref /subsystem:windows
call cl %compiler_flags% %files_to_compile% /link %linker_flags% %files_to_link%
del *.obj *.res>nul
