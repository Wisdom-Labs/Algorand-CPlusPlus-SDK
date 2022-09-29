# where the cpputest includes and *.a are located
# The defaults are what CI uses
#
# For osx, the brew path will look something like /usr/local/Cellar/cpputest/3.8
CPPUTEST_HOME ?= /home/ubuntu/cpputest

ifeq ($(DOCKER_RUN),1)
TARGET_PLATFORM:=$(shell uname -m)-linux-gnu/
endif

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CURRENT_DIR := $(dir $(MKFILE_PATH))

LD_LIBRARIES += -L$(CPPUTEST_HOME)/$(TARGET_PLATFORM)lib -lCppUTest -lCppUTestExt

TEST_SRC_FILES += \
  $(TEST_COMMON_SRC_DIR)/AllTests.cpp

VERTICES_EXTRA_INC_PATHS += \
  -I$(CPPUTEST_HOME)/include \
  -I$(PROJECT_DIR)/src \
  -I$(TEST_ROOT)/ \
  -I$(TEST_ROOT)/stub_includes \
  -I$(PROJECT_DIR)

CPPUTEST_CPPFLAGS += $(VERTICES_EXTRA_INC_PATHS) \
  -DVERTICES_UNITTEST \
  -DSOURCE_PATH_SIZE=0 \
  -DVTC_LOG_LEVEL=4

# Clang defaults to c++98 but all modern cpp compilers implement most of the features (such as
# variadic macros) introduced in c99 that were later added as part of the c++11 specification
# so we pin the unit tests cpp standard to c++11.
CPPUTEST_CXXFLAGS += \
  -std=c++11

export SILENCE ?= @

export CPPUTEST_USE_EXTENSIONS=Y
export CPPUTEST_USE_MEM_LEAK_DETECTION=Y
export CPPUTEST_USE_GCOV=Y
# Enable branch coverage reporting
export GCOV_ARGS=-b -c

CPPUTEST_WARNINGFLAGS = \
  -Wall \
  -Wextra \
  -Werror \
  -Wshadow \
  -Wswitch-default

CPPUTEST_CFLAGS += \
  -Wbad-function-cast

# These warnings aren't particularly helpful for our use case so we disable them
CPPUTEST_WARNINGFLAGS += \
  -Wno-missing-braces \
  -Wno-missing-field-initializers \
  -Wno-packed \
  -Wno-vla \

CC_VERSION_OUTPUT ="$(shell $(CXX) -v 2>&1)"
CLANG_STR = clang
ifeq ($(findstring $(CLANG_STR),$(CC_VERSION_OUTPUT)),$(CLANG_STR))
COMPILER_SPECIFIC_WARNINGS += \
  -Wno-bad-function-cast \
  -Wno-c++11-extensions \
  -Wno-c++98-compat \
  -Wno-c++98-compat-pedantic \
  -Wno-c11-extensions \
  -Wno-c99-extensions \
  -Wno-covered-switch-default \
  -Wno-documentation \
  -Wno-documentation-unknown-command \
  -Wno-flexible-array-extensions \
  -Wno-gnu-variable-sized-type-not-at-end \
  -Wno-gnu-zero-variadic-macro-arguments \
  -Wno-inconsistent-missing-destructor-override \
  -Wno-keyword-macro \
  -Wno-missing-noreturn \
  -Wno-reserved-id-macro \
  -Wno-shorten-64-to-32 \
  -Wno-vla-extension \
  -Wno-zero-as-null-pointer-constant \
  -Wno-unknown-warning-option \
  -Wno-poison-system-directories
else

# GCC-only warnings
COMPILER_SPECIFIC_WARNINGS += \
  -Wformat-signedness

endif

CPPUTEST_WARNINGFLAGS += $(COMPILER_SPECIFIC_WARNINGS)
CPPUTEST_WARNINGFLAGS += \
  -Werror \
  -fsanitize=address

export CPPUTEST_WARNINGFLAGS

CPPUTEST_LDFLAGS += \
  -fsanitize=address

export CPPUTEST_LDFLAGS

RESULT_DIR=$(BUILD_DIR)/$(COMPONENT_NAME)

export TEST_TARGET=$(RESULT_DIR)/$(COMPONENT_NAME)_tests
export CPPUTEST_OBJS_DIR=$(RESULT_DIR)/objs
export CPPUTEST_LIB_DIR=$(RESULT_DIR)/lib

COV_INCLUDE_FILES = $(notdir $(SRC_FILES))
COV_INCLUDE_ARG = $(patsubst %.c, --include *%.c, $(COV_INCLUDE_FILES))

# run MakefileWorker.mk with the variables defined here
include ${CURRENT_DIR}/MakefileWorker.mk