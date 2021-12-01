# Compiler flags for language C
CFLAGS = [
	'-I./include']

# Compiler flags for language C++
CXXFLAGS = [
	'-I./include',
	'-DME_DEBUG',
	'-g',
	'-Wall',
	'-Wextra',
	'-std=c++20']

# Compiler flags for language Objective C
OBJCFLAGS = [
	'-I./include']

SOURCES = {
	'/home/edvin/Dokument/nvim++/src/nvimim/Main.cpp': CXXFLAGS,
	'/home/edvin/Dokument/nvim++/src/nvimim/Listener.cpp': CXXFLAGS,
	'/home/edvin/Dokument/nvim++/src/nvim/Nvim.cpp': CXXFLAGS,
	'/home/edvin/Dokument/nvim++/src/nvim/Type.cpp': CXXFLAGS,
	'/home/edvin/Dokument/nvim++/src/nvim/types/Buffer.cpp': CXXFLAGS,
	'/home/edvin/Dokument/nvim++/src/mpack/mpack.c': CFLAGS
}

def Settings(**kwargs):
	key = kwargs['filename']
	if key in SOURCES:
		return { 'flags': SOURCES[key] }
	else:
		if IsCFile(key):
			return { 'flags': CFLAGS }
		elif IsCXXFile(key):
			return { 'flags': CXXFLAGS }
		elif IsOBJCFile(key):
			return { 'flags': OBJCFLAGS }
	return { 'flags': [ ] }

def IsCXXFile(file):
	CXX_FILES = ['.cpp', '.cxx', '.cc', '.hpp', '.hxx', '.hh']
	for ext in CXX_FILES:
		if file.endswith(ext):
			return True
	return False

def IsCFile(file):
	C_FILES = ['.c', '.h']
	for ext in C_FILES:
		if file.endswith(ext):
			return True
	return False

def IsOBJCFile(file):
	OBJC_FILES = ['.h', '.m', '.mm', '.M']
	for ext in OBJC_FILES:
		if file.endswith(ext):
			return True
	return False

