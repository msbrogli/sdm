#!/usr/bin/env python

import ctypes

class InitializedError(Exception):
    pass

class NotInitializedError(Exception):
    pass

_libsdm = ctypes.cdll.LoadLibrary('libsdm.so')
_libsdm.bs_alloc.restype = ctypes.POINTER(ctypes.c_void_p)
_libsdm.bs_init_random.restype = ctypes.POINTER(ctypes.c_void_p)
_libsdm.bs_init_zero.restype = ctypes.POINTER(ctypes.c_void_p)

_dimension = ctypes.c_int.in_dll(_libsdm, 'bs_dimension')
_radius = ctypes.c_int.in_dll(_libsdm, 'sdm_radius')
_sample = ctypes.c_int.in_dll(_libsdm, 'sdm_sample')

_libsdm.bs_initialize()
_libsdm.hl_initialize()

def get_dimension():
    return _dimension.value

def set_dimension(dimension):
    _dimension.value = dimension

class SDM(object):
    def __init__(self):
        self.initialized = False

    def initialize(self):
        if self.initialized:
            raise InitializedError
        _libsdm.sdm_initialize()
        self.initialized = True

    def free(self):
        if not self.initialized:
            raise NotInitializedError
        _libsdm.sdm_free()
        self.initialized = False

    def write(address, data, radius=None):
        if not self.initialized:
            raise NotInitializedError
        pass

    def read(address, radius=None):
        if not self.initialized:
            raise NotInitializedError
        pass


class Bitstring(object):
    def __init__(self, zero=False):
        self._libsdm = _libsdm
        self._bitstring = _libsdm.bs_alloc()
        if zero:
            _libsdm.bs_init_zero(self._bitstring)
        else:
            _libsdm.bs_init_random(self._bitstring)

    def __del__(self):
        self._libsdm.bs_free(self._bitstring)

    def bit(self, bit):
        return self._libsdm.bs_bit(self._bitstring, bit)

    def bitsign(self, bit):
        return self._libsdm.bs_bitsign(self._bitstring, bit)

    def bitset(self, bit):
        self._libsdm.bs_bitset(self._bitstring, bit)

    def bitclear(self, bit):
        self._libsdm.bs_bitclear(self._bitstring, bit)

    def distance(self, other):
        return self._libsdm.bs_distance(self._bitstring, other._bitstring)

    def _print(self):
        self._libsdm.bs_print(self._bitstring)

    def __str__(self):
        return ''.join([ str(self.bit(k)) for k in range(get_dimension()) ])

