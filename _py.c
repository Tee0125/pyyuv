
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "_yuv.h"

static PyObject* open_wrapper(PyObject* self, PyObject* arg);
static PyObject* seek_frame_wrapper(PyObject* self, PyObject* arg);
static PyObject* get_frame_wrapper(PyObject* self, PyObject* arg);
static PyObject* close_wrapper(PyObject* self, PyObject* arg);

static PyMethodDef methods[] =
{
    {"open", open_wrapper, METH_VARARGS, "open"},
    {"seek_frame", seek_frame_wrapper, METH_VARARGS, "seek: frame_no"},
    {"get_frame", get_frame_wrapper, METH_VARARGS, "get_frame"},
    {"close", close_wrapper, METH_VARARGS, "close"},
    {NULL, NULL, 0, NULL}
};

#if PY_VERSION_HEX >= 0x03000000
PyMODINIT_FUNC
PyInit__yuv(void) {
    PyObject* m;
    static PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "_yuv",             /* m_name */
        NULL,               /* m_doc */
        -1,                 /* m_size */
        methods,            /* m_methods */
    };
    m = PyModule_Create(&module_def);
    return m;
}
#else
PyMODINIT_FUNC
init_yuv(void)
{
    PyObject* m = Py_InitModule("_yuv", methods);
}
#endif

static PyObject* open_wrapper(PyObject* self, PyObject* args)
{
    char* filename;
    int width;
    int height;

    yuv_ctx_t* ctx = NULL;

    if (!PyArg_ParseTuple(args, "sii", &filename, &width, &height))
        return NULL;

    ctx = yuv_open(filename, width, height);
    if (!ctx)
        Py_RETURN_NONE;

    return Py_BuildValue("l", ctx);
}

static PyObject* seek_frame_wrapper(PyObject* self, PyObject* args) {
    PyObject* ret;

    yuv_ctx_t* ctx = NULL;
    int frame_no;

    if (!PyArg_ParseTuple(args, "li", &ctx, &frame_no))
        return NULL;

    int error = yuv_seek_frame(ctx, frame_no);

    ret = Py_BuildValue("i", error);
    return ret;
}

static PyObject* get_frame_wrapper(PyObject* self, PyObject* args)
{
    PyObject* ret;
    PyObject* bytes;

    yuv_ctx_t* ctx = NULL;

    uint8_t* frame;

    int width;
    int height;

    if (!PyArg_ParseTuple(args, "l", &ctx))
        return NULL;

    // yuv frame
    frame = yuv_get_frame(ctx);

    if (!frame)
        Py_RETURN_NONE;

    // convert RGB array to python string
    width = yuv_get_width(ctx);
    height = yuv_get_height(ctx);

    bytes = PyBytes_FromStringAndSize((char *)frame, (width*height*3));
    yuv_release_frame(ctx, frame);

    ret = Py_BuildValue("Sii", bytes, width, height);
    Py_XDECREF(bytes);

    return ret;
}

static PyObject* close_wrapper(PyObject* self, PyObject* args)
{
    yuv_ctx_t* ctx = NULL;

    if (!PyArg_ParseTuple(args, "l", &ctx))
        return NULL;

    yuv_close(ctx);

    Py_RETURN_NONE;
}

