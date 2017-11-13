/**
 * C++ interface to a python PropertyNode()
 */

#include "pyprops.hxx"

#include <string>
#include <sstream>
using std::string;
using std::ostringstream;


// These global pointers only need to be looked up once and then
// saved.  This is handled by pyPropsInit() which should be called
// before any other property system functions

static PyObject *pModuleProps = NULL;
static PyObject *pModuleJSON = NULL;
static PyObject *pModuleXML = NULL;


// Constructor

pyPropertyNode::pyPropertyNode()
{
    // printf("pyPropertyNode()\n");
    pObj = NULL;
}

pyPropertyNode::pyPropertyNode(const pyPropertyNode &node)
{
    // printf("visiting pyPropertyNode() copy constructor!\n");
    pObj = node.pObj;
    Py_INCREF(pObj);
}

pyPropertyNode::pyPropertyNode(PyObject *p)
{
    // printf("pyPropertyNode(pObj)\n");
    pObj = p;
}

// Destructor.
pyPropertyNode::~pyPropertyNode() {
    // printf("~pyPropertyNode destructor\n");
    if ( pObj == NULL ) {
	printf("WARNING: calling destructor on null pyPropertyNode\n");
	// Py_DECREF(pObj);
    }
    Py_XDECREF(pObj);
    pObj = NULL;
}

// Assignment operator
pyPropertyNode & pyPropertyNode::operator= (const pyPropertyNode &node) {
    // printf("Visiting pyPropertyNode operator=\n");
    if (this != &node) {
	// protect against invalid self-assignment

	if ( pObj != NULL ) {
	    // 1: decrement current pObj reference because we are losing it
	    // printf("decrementing existing pObj before overwritting it\n");
	    Py_DECREF(pObj);
	}

	// 2: copy new value to pObj
	pObj = node.pObj;

	// 3: increment pObj ref count because we just created another
	// reference to it.
	Py_INCREF(pObj);
    }
    // by convention, always return *this
    return *this;
}

// test if pObj has named child attribute
bool pyPropertyNode::hasChild(const char *name) {
    bool result = false;
    if ( pObj != NULL ) {
        PyObject *pString = PyString_FromString(name);
	PyObject *result_obj = PyObject_GetItem(pObj, pString);
        result = (result_obj != NULL );
        Py_DECREF(pString);
        Py_DECREF(result_obj);
    }
    return result;
}

// Return a pyPropertyNode object that points to the named child
pyPropertyNode pyPropertyNode::getChild(const char *name, bool create)
{
    if ( pObj == NULL ) {
	return pyPropertyNode();
    } else {
        PyObject *pFunc = PyObject_GetAttrString(pModuleProps, "getChild");
        if ( pFunc == NULL || ! PyCallable_Check(pFunc) ) {
            if ( PyErr_Occurred() ) PyErr_Print();
            fprintf(stderr, "Cannot find function 'getChild()'\n");
            return pyPropertyNode();
        }
        PyObject *pName = PyString_FromString(name);
        PyObject *pCreate = PyBool_FromLong((long)create);
        PyObject *pValue
            = PyObject_CallFunctionObjArgs(pFunc, pObj, pName, pCreate, NULL);
        Py_DECREF(pFunc);
        Py_DECREF(pName);
        Py_DECREF(pCreate);
        
	if (pValue == NULL) {
	    if ( PyErr_Occurred() ) PyErr_Print();
	    fprintf(stderr,"Call failed\n");
            return pyPropertyNode();
	}

        // give pValue over to the returned property node
        return pyPropertyNode(pValue);
    }
}

pyPropertyNode pyPropertyNode::getChild(const char *name, int index,
					bool create)
{
    if ( pObj == NULL ) {
	return pyPropertyNode();
    }
    ostringstream ename;
    ename << name << '[' << index << ']';
    // printf("ename = %s\n", ename.str().c_str());
    return getChild(ename.str().c_str(), create);
}

// return true if pObj pointer is NULL
bool pyPropertyNode::isNull() {
    return pObj == NULL;
}    

// return length of attr if it is a list (enumerated)
int pyPropertyNode::getLen(const char *name) {
    printf("getLen not yet ported\n");
    return 0;
    
    if ( pObj != NULL ) {
	PyObject *pValue = PyObject_CallMethod(pObj,
					       (char *)"getLen",
					       (char *)"s",
					       name);
	if ( pValue != NULL ) {
	    int len = PyInt_AsLong(pValue);
	    Py_DECREF(pValue);
	    return len;
	}
    }
    return 0;
}    

// return true if pObj is a list (enumerated)
void pyPropertyNode::setLen(const char *name, int size) {
    printf("setLen() not yet ported\n");
    return;
    
    if ( pObj != NULL ) {
	PyObject *pValue = PyObject_CallMethod(pObj,
					       (char *)"setLen", (char *)"si",
					       name, size);
	if ( pValue != NULL ) {
	    Py_DECREF(pValue);
	}
    }
}    

// return true if pObj is a list (enumerated)
void pyPropertyNode::setLen(const char *name, int size, double init_val) {
    printf("setLen(init) not yet ported\n");
    return;
    
    if ( pObj != NULL ) {
	PyObject *pValue = PyObject_CallMethod(pObj,
					       (char *)"setLen", (char *)"sif",
					       name, size, init_val);
	if ( pValue != NULL ) {
	    Py_DECREF(pValue);
	}
    }
}    

// return true if pObj is a list (enumerated)
vector <string> pyPropertyNode::getChildren(bool expand) {
    vector <string> result;

    printf("getChildren not yet ported\n");
    return result;
    
    if ( pObj != NULL ) {
	PyObject *pList = PyObject_CallMethod(pObj,
					      (char *)"getChildren",
					      (char *)"b", expand);
	if ( pList != NULL ) {
	    if ( PyList_Check(pList) ) {
		int len = PyList_Size(pList);
		for ( int i = 0; i < len; i++ ) {
		    PyObject *pItem = PyList_GetItem(pList, i);
		    // note: PyList_GetItem doesn't give us ownership
		    // of pItem so we should not decref() it.
		    PyObject *pStr = PyObject_Str(pItem);
		    result.push_back( (string)PyString_AsString(pStr) );
		    Py_DECREF(pStr);
		}
	    }
	    Py_DECREF(pList);
	}
    }
    return result;
}    

// return true if pObj/name is leaf
bool pyPropertyNode::isLeaf(const char *name) {
    printf("isLeaf() not yet ported\n");
    return false;
    
    if ( pObj == NULL ) {
	return false;
    }
    PyObject *pValue = PyObject_CallMethod(pObj,
					   (char *)"isLeaf", (char *)"s",
					   name);
    bool result = PyObject_IsTrue(pValue);
    Py_DECREF(pValue);
    return result;
}    

// note: expects the calling layer to Py_DECREF(pAttr)
double pyPropertyNode::PyObject2Double(const char *name, PyObject *pAttr) {
    double result = 0.0;
    if ( pAttr != NULL ) {
	if ( PyFloat_Check(pAttr) ) {
	    result = PyFloat_AsDouble(pAttr);
	} else if ( PyInt_Check(pAttr) ) {
	    result = PyInt_AsLong(pAttr);
	} else if ( PyLong_Check(pAttr) ) {
	    result = PyLong_AsLong(pAttr);
	} else if ( PyString_Check(pAttr) ) {
	    PyObject *pFloat = PyFloat_FromString(pAttr, NULL);
	    if ( pFloat != NULL ) {
		result = PyFloat_AsDouble(pFloat);
		Py_DECREF(pFloat);
	    } else {
		if ( PyErr_Occurred() ) PyErr_Print();
		printf("WARNING: conversion from string to float failed\n");
		PyObject *pStr = PyObject_Str(pAttr);
		char *s = PyString_AsString(pStr);
		printf("  %s='%s'\n", name, s);
		Py_DECREF(pStr);
	    }
	} else {
	    printf("Unknown object type: '%s' ", pObj->ob_type->tp_name);
	    PyObject *pStr = PyObject_Str(pObj);
	    char *s = PyString_AsString(pStr);
	    printf("  %s='%s'\n", name, s);
	    Py_DECREF(pStr);
	}
    }
    return result;
}

// note: expects the calling layer to Py_DECREF(pAttr)
long pyPropertyNode::PyObject2Long(const char *name, PyObject *pAttr) {
    long result = 0;
    if ( pAttr != NULL ) {
	if ( PyLong_Check(pAttr) ) {
	    result = PyLong_AsLong(pAttr);
	} else if ( PyInt_Check(pAttr) ) {
	    result = PyInt_AsLong(pAttr);
	} else if ( PyFloat_Check(pAttr) ) {
	    result = (long)PyFloat_AsDouble(pAttr);
	} else if ( PyString_Check(pAttr) ) {
	    PyObject *pFloat = PyFloat_FromString(pAttr, NULL);
	    if ( pFloat != NULL ) {
		result = PyFloat_AsDouble(pFloat);
		Py_DECREF(pFloat);
	    } else {
		if ( PyErr_Occurred() ) PyErr_Print();
		printf("WARNING: conversion from string to long failed\n");
		PyObject *pStr = PyObject_Str(pAttr);
		char *s = PyString_AsString(pStr);
		printf("  %s='%s'\n", name, s);
		Py_DECREF(pStr);
	    }
	} else {
	    printf("Unknown object type: '%s' ", pAttr->ob_type->tp_name);
	    PyObject *pStr = PyObject_Str(pAttr);
	    char *s = PyString_AsString(pStr);
	    printf("  %s='%s'\n", name, s);
	    Py_DECREF(pStr);
	}
    }
    return result;
}

// value getters
double pyPropertyNode::getDouble(const char *name) {
    double result = 0.0;
    if ( pObj != NULL ) {
        PyObject *pName = PyString_FromString(name);
        PyObject *pResult = PyObject_GetItem(pObj, pName);
        if ( pResult != NULL ) {
            result = PyObject2Double(name, pResult);
            Py_DECREF(pResult);
	} else {
	    printf("WARNING: request non-existent key: %s\n", name);
	}
        Py_DECREF(pName);
    }
    return result;
}

long pyPropertyNode::getLong(const char *name) {
    long result = 0;
    if ( pObj != NULL ) {
        PyObject *pName = PyString_FromString(name);
        PyObject *pResult = PyObject_GetItem(pObj, pName);
        if ( pResult != NULL ) {
            result = PyObject2Long(name, pResult);
            Py_DECREF(pResult);
	} else {
	    printf("WARNING: request non-existent key: %s\n", name);
	}
        Py_DECREF(pName);
    }
    return result;
}

bool pyPropertyNode::getBool(const char *name) {
    bool result = false;
    if ( pObj != NULL ) {
        PyObject *pName = PyString_FromString(name);
        PyObject *pResult = PyObject_GetItem(pObj, pName);
        if ( pResult != NULL ) {
            result = PyObject_IsTrue(pResult);
            Py_DECREF(pResult);
	} else {
	    printf("WARNING: request non-existent key: %s\n", name);
	}
        Py_DECREF(pName);
    }
    return result;
}

string pyPropertyNode::getString(const char *name) {
    printf("getString() not yet ported\n");
    return "";
    
    string result = "";
    if ( pObj != NULL ) {
	// test for normal vs. enumerated request
	char *pos = strchr((char *)name, '[');
	if ( pos == NULL ) {
	    // normal request
	    if ( PyObject_HasAttrString(pObj, name) ) {
		PyObject *pAttr = PyObject_GetAttrString(pObj, name);
		if ( pAttr != NULL ) {
		    PyObject *pStr = PyObject_Str(pAttr);
		    if ( pStr != NULL ) {
			result = (string)PyString_AsString(pStr);
			Py_DECREF(pStr);
		    }
		    Py_DECREF(pAttr);
		}
	    }
	} else {
	    // enumerated request
	    // this is a little goofy, but this code typically only runs
            // on an interactive telnet request, and we don't want to 
            // modify the request string in place.
	    string base = name;
	    size_t basepos = base.find("[");
	    if ( basepos != string::npos ) {
	        base = base.substr(0, basepos);
	    }
	    pos++;
	    int index = atoi(pos);
	    result = getString(base.c_str(), index);
	    // printf("%s %d %s\n", name, index, result.c_str());
	}
    }
    return result;
}

// indexed value getters
double pyPropertyNode::getDouble(const char *name, int index) {
    printf("getDouble(idx) not yet ported\n");
    return 0.0;
    
    double result = 0.0;
    if ( pObj != NULL ) {
	if ( PyObject_HasAttrString(pObj, name) ) {
	    PyObject *pList = PyObject_GetAttrString(pObj, name);
	    if ( pList != NULL ) {
		if ( PyList_Check(pList) ) {
		    if ( index < PyList_Size(pList) ) {
			PyObject *pAttr = PyList_GetItem(pList, index);
			// note: PyList_GetItem doesn't give us ownership
			// of pAttr so we should not decref() it.
			if ( pAttr != NULL ) {
			    result = PyObject2Double(name, pAttr);
			}
		    }
		} else {
		    printf("WARNING: request indexed value of plain node: %s!\n", name);
		}
		Py_DECREF(pList);
	    }
	}
    }
    return result;
}

long pyPropertyNode::getLong(const char *name, int index) {
    printf("getLong(idx) not yet ported\n");
    return 0;
    
    long result = 0;
    if ( pObj != NULL ) {
	if ( PyObject_HasAttrString(pObj, name) ) {
	    PyObject *pList = PyObject_GetAttrString(pObj, name);
	    if ( pList != NULL ) {
		if ( PyList_Check(pList) ) {
		    if ( index < PyList_Size(pList) ) {
			PyObject *pAttr = PyList_GetItem(pList, index);
			// note: PyList_GetItem doesn't give us ownership
			// of pAttr so we should not decref() it.
			if ( pAttr != NULL ) {
			    result = PyObject2Long(name, pAttr);
			}
		    }
		} else {
		    printf("WARNING: request indexed value of plain node: %s!\n", name);
		}
		Py_DECREF(pList);
	    }
	}
    }
    return result;
}

string pyPropertyNode::getString(const char *name, int index) {
    printf("getString(idx) not yet ported\n");
    return "";

    string result = "";
    if ( pObj != NULL ) {
	if ( PyObject_HasAttrString(pObj, name) ) {
	    PyObject *pList = PyObject_GetAttrString(pObj, name);
	    if ( pList != NULL ) {
		if ( PyList_Check(pList) ) {
		    if ( index < PyList_Size(pList) ) {
			PyObject *pAttr = PyList_GetItem(pList, index);
			// note: PyList_GetItem doesn't give us ownership
			// of pAttr so we should not decref() it.
			if ( pAttr != NULL ) {
			    PyObject *pStr = PyObject_Str(pAttr);
			    if ( pStr != NULL ) {
				result = (string)PyString_AsString(pStr);
				Py_DECREF(pStr);
			    }
			}
		    }
		} else {
		    printf("WARNING: request indexed value of plain node: %s!\n", name);
		}
		Py_DECREF(pList);
	    }
	}
    }
    return result;
}

bool pyPropertyNode::getBool(const char *name, int index) {
    printf("getBool(idx) not yet ported\n");
    return "";
    
    bool result = false;
    if ( pObj != NULL ) {
	if ( PyObject_HasAttrString(pObj, name) ) {
	    PyObject *pList = PyObject_GetAttrString(pObj, name);
	    if ( pList != NULL ) {
		if ( PyList_Check(pList) ) {
		    if ( index < PyList_Size(pList) ) {
			PyObject *pAttr = PyList_GetItem(pList, index);
			// note: PyList_GetItem doesn't give us ownership
			// of pAttr so we should not decref() it.
			if ( pAttr != NULL ) {
			    result = PyObject_IsTrue(pAttr);
			}
		    }
		} else {
		    printf("WARNING: request indexed value of plain node: %s!\n", name);
		}
		Py_DECREF(pList);
	    }
	}
    }
    return result;
}

// value setters
bool pyPropertyNode::setDouble( const char *name, double val ) {
    if ( pObj != NULL ) {
        PyObject *pName = PyString_FromString(name);
	PyObject *pVal = PyFloat_FromDouble(val);
	int result = PyObject_SetItem(pObj, pName, pVal);
	Py_DECREF(pVal);
	Py_DECREF(pName);
	return result != -1;
    } else {
	return false;
    }
}

bool pyPropertyNode::setLong( const char *name, long val ) {
    if ( pObj != NULL ) {
        PyObject *pString = PyString_FromString(name);
	PyObject *pLong = PyLong_FromLong(val);
	int result = PyObject_SetItem(pObj, pString, pLong);
	Py_DECREF(pLong);
	Py_DECREF(pString);
	return result != -1;
    } else {
	return false;
    }
}

bool pyPropertyNode::setBool( const char *name, bool val ) {
    if ( pObj != NULL ) {
        PyObject *pString = PyString_FromString(name);
	PyObject *pBool = PyBool_FromLong((long)val);
	int result = PyObject_SetItem(pObj, pString, pBool);
	Py_DECREF(pBool);
	Py_DECREF(pString);
	return result != -1;
    } else {
	return false;
    }
}

bool pyPropertyNode::setString( const char *name, string val ) {
    if ( pObj != NULL ) {
        PyObject *pName = PyString_FromString(name);
	PyObject *pVal = PyString_FromString(val.c_str());
	int result = PyObject_SetItem(pObj, pName, pVal);
	Py_DECREF(pVal);
	Py_DECREF(pName);
	return result != -1;
    } else {
	return false;
    }
}

// indexed value setters
bool pyPropertyNode::setDouble( const char *name, int index, double val ) {
    printf("setDouble(idx) not yet ported\n");
    return false;
    
    if ( pObj != NULL ) {
	PyObject *pList = PyObject_GetAttrString(pObj, name);
	if ( pList != NULL ) {
	    if ( PyList_Check(pList) ) {
		if ( index < PyList_Size(pList) ) {
		    PyObject *pFloat = PyFloat_FromDouble(val);
		    // note setitem() steals the reference so we can't
		    // decrement it
		    PyList_SetItem(pList, index, pFloat );
		} else {
		    // index out of range
		}
	    } else {
		// not a list
	    }
	    Py_DECREF(pList);
	} else {
	    // list lookup failed
	}
    } else {
	return false;
    }
    return true;
}

// This function must be called before any pyPropertyNode usage. It
// imports the python props and props_json/xml modules.
void pyPropsInit() {
    // python property system
    pModuleProps = PyImport_ImportModule("props");
    if (pModuleProps == NULL) {
        if ( PyErr_Occurred() ) PyErr_Print();
        fprintf(stderr, "Failed to load 'props'\n");
    }

    // Json I/O system
    pModuleJSON = PyImport_ImportModule("props_json");
    if (pModuleJSON == NULL) {
        if ( PyErr_Occurred() ) PyErr_Print();
        fprintf(stderr, "Failed to load 'props_json'\n");
    }
}

// This function can be called at exit to properly free resources
// requested by init()
extern void pyPropsCleanup(void) {
    printf("running pyPropsCleanup()\n");
    Py_XDECREF(pModuleProps);
    Py_XDECREF(pModuleXML);
}

// Return a pyPropertyNode object that points to the specified path in
// the property tree.  This is a 'heavier' operation so it is
// recommended to call this function from initialization routines and
// save the result.  Then use the pyPropertyNode for direct read/write
// access in your update routines.
pyPropertyNode pyGetNode(string abs_path, bool create) {
    PyErr_Clear();
    PyObject *pFuncGetNode = PyObject_GetAttrString(pModuleProps, "getNode");
    if ( pFuncGetNode == NULL || ! PyCallable_Check(pFuncGetNode) ) {
	if ( PyErr_Occurred() ) PyErr_Print();
	fprintf(stderr, "Cannot find function 'getNode()'\n");
	return pyPropertyNode();
    }

    // FIXME decref pFuncGetNode
    
    PyObject *pPath = PyString_FromString(abs_path.c_str());
    PyObject *pCreate = PyBool_FromLong(create);
    if (!pPath || !pCreate) {
	Py_XDECREF(pPath);
	Py_XDECREF(pCreate);
	fprintf(stderr, "Cannot convert argument\n");
	return pyPropertyNode();
    }
    PyObject *pValue
	= PyObject_CallFunctionObjArgs(pFuncGetNode, pPath, pCreate, NULL);
    Py_DECREF(pPath);
    Py_DECREF(pCreate);
    if (pValue != NULL) {
	// give pValue over to the returned property node
	/*printf("pyGetNode() success, creating pyPropertyNode\n");
	pyPropertyNode tmp(pValue);
	printf("before return\n");*/
	return pyPropertyNode(pValue);
    } else {
	if ( PyErr_Occurred() ) PyErr_Print();
	printf("Call failed\n");
	return pyPropertyNode();
    }
    return pyPropertyNode();
}

bool readXML(string filename, pyPropertyNode *node) {
    // getNode() function
    PyObject *pFuncLoad = PyObject_GetAttrString(pModuleXML, "load");
    if ( pFuncLoad == NULL || ! PyCallable_Check(pFuncLoad) ) {
	if ( PyErr_Occurred() ) PyErr_Print();
	fprintf(stderr, "Cannot find function 'load()'\n");
	return false;
    }
    PyObject *pPath = PyString_FromString(filename.c_str());
    if (!pPath || !node->pObj) {
	Py_XDECREF(pPath);
	Py_XDECREF(pFuncLoad);
	fprintf(stderr, "Cannot convert argument\n");
	return false;
    }
    PyObject *pValue = PyObject_CallFunctionObjArgs(pFuncLoad, pPath,
						    node->pObj, NULL);
    Py_DECREF(pPath);
    Py_DECREF(pFuncLoad);
    if (pValue != NULL) {
	// give pValue over to the returned property node
	bool result = PyObject_IsTrue(pValue);
	Py_DECREF(pValue);
	return result;
    } else {
	if ( PyErr_Occurred() ) PyErr_Print();
	fprintf(stderr,"Call failed\n");
    }
    return false;
}

bool writeXML(string filename, pyPropertyNode *node) {
    // getNode() function
    PyObject *pFuncSave = PyObject_GetAttrString(pModuleXML, "save");
    if ( pFuncSave == NULL || ! PyCallable_Check(pFuncSave) ) {
	if ( PyErr_Occurred() ) PyErr_Print();
	fprintf(stderr, "Cannot find function 'save()'\n");
	return false;
    }
    PyObject *pPath = PyString_FromString(filename.c_str());
    if (!pPath || !node->pObj) {
	Py_XDECREF(pPath);
	Py_XDECREF(pFuncSave);
	fprintf(stderr, "Cannot convert argument\n");
	return false;
    }
    PyObject *pValue = PyObject_CallFunctionObjArgs(pFuncSave, pPath,
						    node->pObj, NULL);
    Py_DECREF(pPath);
    Py_DECREF(pFuncSave);
    if (pValue != NULL) {
	// give pValue over to the returned property node
	bool result = PyObject_IsTrue(pValue);
	Py_DECREF(pValue);
	return result;
    } else {
	if ( PyErr_Occurred() ) PyErr_Print();
	fprintf(stderr,"Call failed\n");
    }
    return false;
}

bool readJSONtoRoot(string filename) {
    // getNode() function
    PyObject *pFuncLoad = PyObject_GetAttrString(pModuleJSON, "load_to_root");
    if ( pFuncLoad == NULL || ! PyCallable_Check(pFuncLoad) ) {
	if ( PyErr_Occurred() ) PyErr_Print();
	fprintf(stderr, "Cannot find function 'load_to_root()'\n");
	return false;
    }
    PyObject *pPath = PyString_FromString(filename.c_str());
    if ( !pPath ) {
	Py_XDECREF(pPath);
	Py_XDECREF(pFuncLoad);
	fprintf(stderr, "Cannot convert argument\n");
	return false;
    }
    PyObject *pValue = PyObject_CallFunctionObjArgs(pFuncLoad, pPath, NULL);
    Py_DECREF(pPath);
    Py_DECREF(pFuncLoad);
    if (pValue != NULL) {
	// give pValue over to the returned property node
	bool result = PyObject_IsTrue(pValue);
	Py_DECREF(pValue);
	return result;
    } else {
	if ( PyErr_Occurred() ) PyErr_Print();
	fprintf(stderr,"Call failed\n");
    }
    return false;
}

bool writeJSON(string filename, pyPropertyNode *node) {
    // getNode() function
    PyObject *pFuncSave = PyObject_GetAttrString(pModuleJSON, "save");
    if ( pFuncSave == NULL || ! PyCallable_Check(pFuncSave) ) {
	if ( PyErr_Occurred() ) PyErr_Print();
	fprintf(stderr, "Cannot find function 'save()'\n");
	return false;
    }
    PyObject *pPath = PyString_FromString(filename.c_str());
    if (!pPath || !node->pObj) {
	Py_XDECREF(pPath);
	Py_XDECREF(pFuncSave);
	fprintf(stderr, "Cannot convert argument\n");
	return false;
    }
    PyObject *pValue = PyObject_CallFunctionObjArgs(pFuncSave, pPath,
						    node->pObj, NULL);
    Py_DECREF(pPath);
    Py_DECREF(pFuncSave);
    if (pValue != NULL) {
	// give pValue over to the returned property node
	bool result = PyObject_IsTrue(pValue);
	Py_DECREF(pValue);
	return result;
    } else {
	if ( PyErr_Occurred() ) PyErr_Print();
	fprintf(stderr,"Call failed\n");
    }
    return false;
}

// Return a pyPropertyNode object that points to the named child
void pyPropertyNode::pretty_print()
{
    if ( pObj == NULL ) {
	printf("pretty_print(): Null pyPropertyNode()\n");
    } else {
        PyObject *pFunc = PyObject_GetAttrString(pModuleProps, "pretty_print");
        if ( pFunc == NULL || ! PyCallable_Check(pFunc) ) {
            if ( PyErr_Occurred() ) PyErr_Print();
            fprintf(stderr, "Cannot find function 'pretty_print()'\n");
            return;
        }
	PyObject *pValue
            = PyObject_CallFunctionObjArgs(pFunc, pObj, NULL);
        Py_DECREF(pFunc);
	if (pValue != NULL) {
	    Py_DECREF(pValue);
	} else {
	    if ( PyErr_Occurred() ) PyErr_Print();
	    fprintf(stderr,"Call failed\n");
	}
    }
}

