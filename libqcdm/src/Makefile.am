noinst_LTLIBRARIES = libqcdm.la libqcdm-test.la


libqcdm_la_CPPFLAGS = \
	$(MM_CFLAGS)

libqcdm_la_SOURCES = \
	dm-commands.h \
	nv-items.h \
	com.c \
	com.h \
	commands.c \
	commands.h \
	error.c \
	error.h \
	result.c \
	result.h \
	result-private.h \
	utils.c \
	utils.h

libqcdm_la_LIBADD = \
	$(MM_LIBS)

libqcdm_la_LDFLAGS = -Wl,--version-script=$(srcdir)/libqcdm.ver \
	-version-info "0:0:0"


###########################################
# Test library without symbol versioning
###########################################

libqcdm_test_la_CPPFLAGS = \
	$(MM_CFLAGS)

libqcdm_test_la_SOURCES = \
	utils.c \
	utils.h

libqcdm_test_la_LIBADD = \
	$(MM_LIBS)

