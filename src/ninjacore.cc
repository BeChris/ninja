#include <Python.h>
#include "version.h"

/* Defined in ninja.cc */
int main(int argc, char** argv);

/* Defined in ninja_test.cc */
int test_main(int argc, char** argv);

/* Defined in util.cc */
int GetProcessorCount();

static PyObject *
ninjacore_build(PyObject *self, PyObject *args, PyObject *keywds)
{
    (void)self;

    // Accepted keywords
    static const char *kwlist[] = {"dir", "file", "jobs", "verbose", "target", "stats", "explain", "keepdepfile", "keeprsp", "nostatcache", NULL};

    char *dir = NULL;
    char *file = NULL;
    unsigned int jobs = GetProcessorCount() + 1;
    int verbose = 0;
    char *target = NULL;
    int stats = 0;
    int explain = 0;
    int keepdepfile = 0;
    int keeprsp = 0;
    int nostatcache = 0;

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "|$ssIpsbbbbb", (char **)kwlist,
                                         &dir, &file, &jobs, &verbose, &target, &stats, &explain, &keepdepfile, &keeprsp, &nostatcache))
                 return NULL;

#define MAX_RUN_ARGV 20
    int argc = 1;
    char **argv = new char *[MAX_RUN_ARGV];
    int idx = 0;
    argv[idx++] = strdup("ninja");
    if (dir) {
        argv[idx++] = strdup("-C");
        argc++;
        argv[idx++] = strdup(dir);
        argc++;
    }
    if (file) {
        argv[idx++] = strdup("-f");
        argc++;
        argv[idx++] = strdup(file);
        argc++;
    }
    if (jobs > 0) {
        argv[idx++] = strdup("-j");
        argc++;
        char *buffer = (char *)malloc(10 * sizeof(char));
        sprintf(buffer, "%u", jobs);
        argv[idx++] = strdup(buffer);
        free(buffer);
        argc++;
    }
    if (verbose > 0) {
        argv[idx++] = strdup("-v");
        argc++;
    }
    if (target) {
        argv[idx++] = strdup(target);
        argc++;
    }
    if (stats) {
        argv[idx++] = strdup("-d");
        argc++;
        argv[idx++] = strdup("stats");
        argc++;
    }
    if (explain) {
        argv[idx++] = strdup("-d");
        argc++;
        argv[idx++] = strdup("explain");
        argc++;
    }
    if (keepdepfile) {
        argv[idx++] = strdup("-d");
        argc++;
        argv[idx++] = strdup("keepdepfile");
        argc++;
    }
    if (keeprsp) {
        argv[idx++] = strdup("-d");
        argc++;
        argv[idx++] = strdup("keeprsp");
        argc++;
    }
    if (nostatcache) {
        argv[idx++] = strdup("-d");
        argc++;
        argv[idx++] = strdup("nostatcache");
        argc++;
    }

    argv[argc] = NULL;

    exit(main(argc, argv));
}

static PyObject *
ninjacore_selftests(PyObject *self, PyObject *args)
{
    (void)self;

#define MAX_TEST_ARGV 2
    int argc = 1;
    char **argv = new char *[MAX_TEST_ARGV];
    int idx = 0;
    argv[idx++] = strdup("ninja_test");

    argv[argc] = NULL;

    exit(test_main(argc, argv));
}

static PyMethodDef NinjaCoreMethods[] = {
    {"build",  (PyCFunction)ninjacore_build, METH_VARARGS | METH_KEYWORDS,
     "Trigger ninjacore to build necessary files based on rules described in build.ninja file.\n\n"
     "This method is intended to be called with following optionnal keyword parameters:\n"
     " dir (string)          : change to DIR before doing anything else [default=current working dir]\n"
     " file (string)         : specify input build file [default=build.ninja]\n"
     " jobs (integer)        : run N jobs in parallel [default=available CPUs + 1]\n"
     " verbose (boolean)     : show all command lines while building [default=False]\n"
     " target (string)       : target to build [default=default]\n"
     " stats (boolean)       : print operation counts/timing info [default=False]\n"
     " explain (boolean)     : explain what caused a command to execute [default=False]\n"
     " keepdepfile (boolean) : don't delete depfiles after they're read by ninja [default=False]\n"
     " keeprsp (boolean)     : don't delete @response files on success [default=False]\n"
     " nostatcache (boolean) : don't batch stat() calls per directory and cache them [default=False]\n\n"
     "This method calls exit() terminating the Python interpreter and returning a status code:\n"
     " 0   : if everything went OK.\n"
     " > 0 : if something went wrong."
    },
    {"selftests",  (PyCFunction)ninjacore_selftests, METH_NOARGS,
     "Trigger ninjacore self tests.\n\n"
     "This method calls exit() terminating the Python interpreter and returning a status code:\n"
     " 0   : if everything went OK.\n"
     " > 0 : if something went wrong."
    },
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef ninjacore = {
    PyModuleDef_HEAD_INIT,
    "ninjacore",   /* name of module */
    "This module embeds the core part of ninja build system.\n"
    "It enables usage of ninja without the need of running the ninja executable itself.\n\n"
    "For more informations about ninja, consult https://ninja-build.org\n"
    "or its manual in ninja-manual subdirectory.",          /* module documentation */
    -1,       /* size of per-interpreter state of the module,
                    or -1 if the module keeps state in global variables. */
    NinjaCoreMethods

};

PyMODINIT_FUNC PyInit_ninjacore(void) {
    /* Create main "ninjacore" module */
    PyObject *module = PyModule_Create(&ninjacore);
    if (module) {
        /* Add constant "version" string in main "ninjacore" module */
        PyModule_AddStringConstant(module, "version", kNinjaVersion);
    }
    return module;
}
