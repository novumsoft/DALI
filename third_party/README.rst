NVIDIA DALI third party dependencies
====================================
This part of the repository contains extra dependencies required to build DALI, consisting mostly of externally hosted subrepositories:

+-----------------+---------------------+---------------------+
| Repository      | Version             | License             |
+=================+=====================+=====================+
| |benchmark|_    | |benchmarkver|_     | |benchmarklic|_     |
+-----------------+---------------------+---------------------+
| |preprocessor|_ | |preprocessorver|_  | |preprocessorlic|_  |
+-----------------+---------------------+---------------------+
| |cocoapi|_      | |cocoapiver|_       | |cocoapilic|_       |
+-----------------+---------------------+---------------------+
| |cutlass|_      | |cutlassver|_       | |cutlasslic|_       |
+-----------------+---------------------+---------------------+
| |dlpack|_       | |dlpackver|_        | |dlpacklic|_        |
+-----------------+---------------------+---------------------+
| |ffts|_         | |fftsver|_          | |fftslic|_          |
+-----------------+---------------------+---------------------+
| |googletest|_   | |googletestver|_    | |googletestlic|_    |
+-----------------+---------------------+---------------------+
| |libcudacxx|_   | |libcudacxxver|_    | |libcudacxxlic|_    |
+-----------------+---------------------+---------------------+
| |pybind11|_     | |pybind11ver|_      | |pybind11lic|_      |
+-----------------+---------------------+---------------------+
| |rapidjson|_    | |rapidjsonver|_     | |rapidjsonlic|_     |
+-----------------+---------------------+---------------------+

.. |benchmark| replace:: Google Benchmark
.. _benchmark: https://github.com/google/benchmark
.. |benchmarkver| replace:: 1.7.0
.. _benchmarkver: https://github.com/google/benchmark/releases/tag/v1.7.0
.. |benchmarklic| replace:: Apache License 2.0
.. _benchmarklic: https://github.com/google/benchmark/blob/master/LICENSE

.. |preprocessor| replace:: Boost Preprocessor
.. _preprocessor: https://github.com/boostorg/preprocessor
.. |preprocessorver| replace:: 1.80.0
.. _preprocessorver: https://github.com/boostorg/preprocessor/releases/tag/boost-1.78.0
.. |preprocessorlic| replace:: Boost Software License 1.0
.. _preprocessorlic: https://github.com/boostorg/boost/blob/master/LICENSE_1_0.txt

.. |cocoapi| replace:: COCO API
.. _cocoapi: https://github.com/cocodataset/cocoapi
.. |cocoapiver| replace:: Top-of-tree (Feb 20, 2020)
.. _cocoapiver: https://github.com/cocodataset/cocoapi/tree/8c9bcc3cf640524c4c20a9c40e89cb6a2f2fa0e9
.. |cocoapilic| replace:: BSD 2-Clause License
.. _cocoapilic: https://github.com/cocodataset/cocoapi/blob/master/license.txt

.. |cutlass| replace:: CUTLASS
.. _cutlass: https://github.com/NVIDIA/cutlass
.. |cutlassver| replace:: 2.9.1
.. _cutlassver: https://github.com/NVIDIA/cutlass/releases/tag/v2.9.1
.. |cutlasslic| replace:: BSD 3-Clause License
.. _cutlasslic: https://github.com/NVIDIA/cutlass/blob/master/LICENSE.txt

.. |dlpack| replace:: DLPack
.. _dlpack: https://github.com/dmlc/dlpack
.. |dlpackver| replace:: 0.7
.. _dlpackver: https://github.com/dmlc/dlpack/releases/tag/v0.7
.. |dlpacklic| replace:: Apache License 2.0
.. _dlpacklic: https://github.com/dmlc/dlpack/blob/main/LICENSE

.. |ffts| replace:: FFTS
.. _ffts: https://github.com/JanuszL/ffts
.. |fftsver| replace:: Custom fork top-of-tree (Jan 23, 2020)
.. _fftsver: https://github.com/JanuszL/ffts/tree/c9a9f61a60505751cac385ed062ce2720bdf07d4
.. |fftslic| replace:: BSD 3-Clause License
.. _fftslic: https://github.com/JanuszL/ffts/blob/master/COPYRIGHT

.. |googletest| replace:: GoogleTest
.. _googletest: https://github.com/google/googletest
.. |googletestver| replace:: 1.12.1
.. _googletestver: https://github.com/google/googletest/releases/tag/release-1.12.1
.. |googletestlic| replace:: BSD 3-Clause License
.. _googletestlic: https://github.com/google/googletest/blob/master/LICENSE

.. |libcudacxx| replace:: libcu++
.. _libcudacxx: https://github.com/mzient/libcudacxx.git
.. |libcudacxxver| replace:: Custom fork (Aug 30, 2021)
.. _libcudacxxver: https://github.com/mzient/libcudacxx/tree/863f11a16cced8b7aacfc639dacb419843a300e8
.. |libcudacxxlic| replace:: Apache License v2.0 with LLVM Exceptions
.. _libcudacxxlic: https://github.com/mzient/libcudacxx/blob/main/LICENSE.TXT

.. |pybind11| replace:: pybind11
.. _pybind11: https://github.com/pybind/pybind11
.. |pybind11ver| replace:: 2.10.0
.. _pybind11ver: https://github.com/pybind/pybind11/releases/tag/v2.10.0
.. |pybind11lic| replace:: BSD 3-Clause License
.. _pybind11lic: https://github.com/pybind/pybind11/blob/master/LICENSE

.. |rapidjson| replace:: RapidJSON
.. _rapidjson: https://github.com/Tencent/rapidjson
.. |rapidjsonver| replace:: Top-of-tree (Aug 23, 2022)
.. _rapidjsonver: https://github.com/Tencent/rapidjson/commit/06d58b9e848c650114556a23294d0b6440078c61
.. |rapidjsonlic| replace:: MIT License, BSD 3-Clause License, JSON License
.. _rapidjsonlic: https://github.com/Tencent/rapidjson/blob/master/license.txt
