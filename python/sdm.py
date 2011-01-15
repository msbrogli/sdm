#!/usr/bin/env python

import ctypes

class InitializedError(Exception):
    pass

class NotInitializedError(Exception):
    pass

class hardlocation_struct(ctypes.Structure):
    _fields_ = [ ('address', ctypes.POINTER(ctypes.c_void_p)),
                 ('adder', ctypes.POINTER(ctypes.c_void_p)) ]

_libsdm = ctypes.cdll.LoadLibrary('libsdm.so')

_libsdm.bs_alloc.restype = ctypes.POINTER(ctypes.c_void_p)
_libsdm.bs_init_random.restype = ctypes.POINTER(ctypes.c_void_p)
_libsdm.bs_init_zero.restype = ctypes.POINTER(ctypes.c_void_p)
_libsdm.bs_copy.restype = ctypes.POINTER(ctypes.c_void_p)
_libsdm.bs_average.restype = ctypes.POINTER(ctypes.c_void_p)

_libsdm.hl_alloc.restype = ctypes.POINTER(hardlocation_struct)
_libsdm.hl_init_random.restype = ctypes.POINTER(hardlocation_struct)
_libsdm.hl_read.restype = ctypes.POINTER(ctypes.c_void_p)

_libsdm.sdm_read.restype = ctypes.POINTER(ctypes.c_void_p)
_libsdm.sdm_thread_read.restype = ctypes.POINTER(ctypes.c_void_p)

_dimension = ctypes.c_int.in_dll(_libsdm, 'bs_dimension')
_radius = ctypes.c_int.in_dll(_libsdm, 'sdm_radius')
_sample = ctypes.c_int.in_dll(_libsdm, 'sdm_sample')
_memory = ctypes.POINTER(ctypes.POINTER(hardlocation_struct)).in_dll(_libsdm, 'sdm_memory')
_thread_count = ctypes.c_int.in_dll(_libsdm, 'sdm_thread_count')

_libsdm.bs_initialize()
_libsdm.hl_initialize()

initialized = False

def get_dimension():
    return _dimension.value

def set_dimension(dimension):
    global initialized
    if initialized:
        raise InitializedError
    _dimension.value = dimension

def get_sample():
    return _sample.value

def set_sample(sample):
    global initialized
    if initialized:
        raise InitializedError
    _sample.value = sample

def get_radius():
    return _radius.value

def set_radius(radius, force=False):
    global initialized
    if not force and initialized:
        raise InitializedError
    _radius.value = radius

def get_thread_count():
    return _thread_count.value

def set_thread_count(thread_count, force=False):
    global initialized
    if not force and initialized:
        raise InitializedError
    _thread_count.value = thread_count

def initialize():
    global initialized, _memory
    if initialized:
        raise InitializedError
    _libsdm.sdm_initialize()
    initialized = True

def free():
    global initialized
    if not initialized:
        raise NotInitializedError
    _libsdm.sdm_free()
    initialized = False

def get_memory(index):
    global initialized
    if not initialized:
        raise NotInitializedError
    return Hardlocation(hardlocation=_memory[index], autofree=False)

def get_all_memory():
    global initialized
    if not initialized:
        raise NotInitializedError
    return [ Hardlocation(hardlocation=_memory[i], autofree=False) for i in range(get_sample()) ]

def radius_count(address, radius=None):
    global initialized
    if not initialized:
        raise NotInitializedError
    if radius is None:
        radius = get_radius()
    return _libsdm.sdm_radius_count(address._bitstring, radius)

def radius_count_intersect(addr1, addr2, radius=None):
    global initialized
    if not initialized:
        raise NotInitializedError
    if radius is None:
        radius = get_radius()
    return _libsdm.sdm_radius_count_intersect(addr1._bitstring, addr2._bitstring, radius)

def distance(address):
    global initialized
    if not initialized:
        raise NotInitializedError
    sample = get_sample()
    buf = (ctypes.c_uint*sample)()
    _libsdm.sdm_distance(address._bitstring, buf)
    return [ x for x in buf ]

def write(address, data):
    global initialized
    if not initialized:
        raise NotInitializedError
    return _libsdm.sdm_write(address._bitstring, data._bitstring)

def read(address):
    global initialized
    if not initialized:
        raise NotInitializedError
    return Bitstring(bitstring=_libsdm.sdm_read(address._bitstring))

def thread_radius_count(address, radius=None):
    global initialized
    if not initialized:
        raise NotInitializedError
    if radius is None:
        radius = get_radius()
    return _libsdm.sdm_thread_radius_count(address._bitstring, radius)

def thread_radius_count_intersect(addr1, addr2, radius=None):
    global initialized
    if not initialized:
        raise NotInitializedError
    if radius is None:
        radius = get_radius()
    return _libsdm.sdm_thread_radius_count_intersect(addr1._bitstring, addr2._bitstring, radius)

def thread_distance(address):
    global initialized
    if not initialized:
        raise NotInitializedError
    sample = get_sample()
    buf = (ctypes.c_uint*sample)()
    _libsdm.sdm_thread_distance(address._bitstring, buf)
    return [ x for x in buf ]

def thread_write(address, data):
    global initialized
    if not initialized:
        raise NotInitializedError
    return _libsdm.sdm_thread_write(address._bitstring, data._bitstring)

def thread_read(address):
    global initialized
    if not initialized:
        raise NotInitializedError
    return Bitstring(bitstring=_libsdm.sdm_thread_read(address._bitstring))

class Bitstring(object):
    @classmethod
    def distance(cls, a, b):
        return _libsdm.bs_distance(a._bitstring, b._bitstring)

    @classmethod
    def average(cls, a, b):
        return Bitstring(bitstring=_libsdm.bs_average(a._bitstring, b._bitstring))

    def __init__(self, zero=False, bitstring=None, autofree=True):
        self._libsdm = _libsdm
        self.autofree = autofree
        if bitstring is not None:
            self._bitstring = bitstring
        else:
            self._bitstring = _libsdm.bs_alloc()
            if zero:
                _libsdm.bs_init_zero(self._bitstring)
            else:
                _libsdm.bs_init_random(self._bitstring)

    def __del__(self):
        if self.autofree:
            self._libsdm.bs_free(self._bitstring)

    def bit(self, bit):
        return self._libsdm.bs_bit(self._bitstring, bit)

    def bitsign(self, bit):
        return self._libsdm.bs_bitsign(self._bitstring, bit)

    def bitset(self, bit):
        self._libsdm.bs_bitset(self._bitstring, bit)

    def bitclear(self, bit):
        self._libsdm.bs_bitclear(self._bitstring, bit)
    
    def bitswap(self, bit):
        self._libsdm.bs_bitswap(self._bitstring, bit)

    def bitrandomswap(self, qty):
        self._libsdm.bs_bitrandomswap(self._bitstring, qty)

    def distance_to(self, other):
        return self._libsdm.bs_distance(self._bitstring, other._bitstring)

    def _print(self):
        self._libsdm.bs_print(self._bitstring)

    def _string(self):
        s = ctypes.create_string_buffer(get_dimension())
        self._libsdm.bs_string(self._bitstring, s)
        return s.value

    def copy(self):
        _clone = self._libsdm.bs_copy(self._bitstring)
        return Bitstring(bitstring=_clone)

    def __str__(self):
        return ''.join([ str(self.bit(k)) for k in reversed(range(get_dimension())) ])


class Hardlocation(object):
    def __init__(self, hardlocation=None, autofree=True):
        self._libsdm = _libsdm
        self.autofree = autofree
        if hardlocation is not None:
            self._hardlocation = hardlocation
        else:
            self._hardlocation = self._libsdm.hl_alloc()
            self._libsdm.hl_init_random(self._hardlocation)

    def __del__(self):
        if self.autofree:
            self._libsdm.hl_free(self._hardlocation)

    @property
    def address(self):
        return Bitstring(bitstring=self._hardlocation.contents.address, autofree=False)

    @property
    def adder(self):
        _adder = ctypes.cast(self._hardlocation.contents.adder, ctypes.POINTER(ctypes.c_int8 * get_dimension()))
        return [ x for x in _adder.contents ]

    def write(self, data):
        self._libsdm.hl_write(self._hardlocation, data._bitstring)

    def read(self):
        bitstring = self._libsdm.hl_read(self._hardlocation)
        return Bitstring(bitstring=bitstring)


