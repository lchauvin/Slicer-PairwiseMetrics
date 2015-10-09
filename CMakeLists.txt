cmake_minimum_required(VERSION 2.8.9)

project(Slicer-PairwiseMetrics)

#-----------------------------------------------------------------------------
# Extension meta-information
set(EXTENSION_HOMEPAGE "https://github.com/lchauvin/Slicer-PairwiseMetrics")
set(EXTENSION_CATEGORY "Quantification")
set(EXTENSION_CONTRIBUTORS "Laurent Chauvin (BWH), Sonia Pujol (BWH)")
set(EXTENSION_DESCRIPTION "This extension compute pairwise metrics such as Dice coefficient, Generale Hausdorff distance and Average Minimum Distance using multiple threads")
set(EXTENSION_ICONURL "https://raw.github.com/lchauvin/Slicer-PairwiseMetrics/master/Slicer-PairwiseMetrics.png")
set(EXTENSION_SCREENSHOTURLS "http://www.example.com/Slicer/Extensions/Slicer-PairwiseMetrics/Screenshots/1.png")

#-----------------------------------------------------------------------------
# Extension dependencies
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})

#-----------------------------------------------------------------------------
# Extension modules
add_subdirectory(PairwiseMetrics)
## NEXT_MODULE

#-----------------------------------------------------------------------------
include(${Slicer_EXTENSION_CPACK})
