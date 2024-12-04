#include "Viewer2d.h"
#include <gkg-processing-container/Volume_i.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/Raster.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Image.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Transformer.h>
#include <gkg-graphics-X11-core/TransformFitter.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>


#define EPSILON 1e-6


static gkg::FloatColorIntensity gray_level[ 256 ][ 3 ] =
  {
    { 0.000, 0.000, 0.000 },
    { 0.004, 0.004, 0.004 },
    { 0.008, 0.008, 0.008 },
    { 0.012, 0.012, 0.012 },
    { 0.016, 0.016, 0.016 },
    { 0.020, 0.020, 0.020 },
    { 0.024, 0.024, 0.024 },
    { 0.027, 0.027, 0.027 },
    { 0.031, 0.031, 0.031 },
    { 0.035, 0.035, 0.035 },
    { 0.039, 0.039, 0.039 },
    { 0.043, 0.043, 0.043 },
    { 0.047, 0.047, 0.047 },
    { 0.051, 0.051, 0.051 },
    { 0.055, 0.055, 0.055 },
    { 0.059, 0.059, 0.059 },
    { 0.063, 0.063, 0.063 },
    { 0.067, 0.067, 0.067 },
    { 0.071, 0.071, 0.071 },
    { 0.075, 0.075, 0.075 },
    { 0.078, 0.078, 0.078 },
    { 0.082, 0.082, 0.082 },
    { 0.086, 0.086, 0.086 },
    { 0.090, 0.090, 0.090 },
    { 0.094, 0.094, 0.094 },
    { 0.098, 0.098, 0.098 },
    { 0.102, 0.102, 0.102 },
    { 0.106, 0.106, 0.106 },
    { 0.110, 0.110, 0.110 },
    { 0.114, 0.114, 0.114 },
    { 0.118, 0.118, 0.118 },
    { 0.122, 0.122, 0.122 },
    { 0.125, 0.125, 0.125 },
    { 0.129, 0.129, 0.129 },
    { 0.133, 0.133, 0.133 },
    { 0.137, 0.137, 0.137 },
    { 0.141, 0.141, 0.141 },
    { 0.145, 0.145, 0.145 },
    { 0.149, 0.149, 0.149 },
    { 0.153, 0.153, 0.153 },
    { 0.157, 0.157, 0.157 },
    { 0.161, 0.161, 0.161 },
    { 0.165, 0.165, 0.165 },
    { 0.169, 0.169, 0.169 },
    { 0.173, 0.173, 0.173 },
    { 0.176, 0.176, 0.176 },
    { 0.180, 0.180, 0.180 },
    { 0.184, 0.184, 0.184 },
    { 0.188, 0.188, 0.188 },
    { 0.192, 0.192, 0.192 },
    { 0.196, 0.196, 0.196 },
    { 0.200, 0.200, 0.200 },
    { 0.204, 0.204, 0.204 },
    { 0.208, 0.208, 0.208 },
    { 0.212, 0.212, 0.212 },
    { 0.216, 0.216, 0.216 },
    { 0.220, 0.220, 0.220 },
    { 0.224, 0.224, 0.224 },
    { 0.227, 0.227, 0.227 },
    { 0.231, 0.231, 0.231 },
    { 0.235, 0.235, 0.235 },
    { 0.239, 0.239, 0.239 },
    { 0.243, 0.243, 0.243 },
    { 0.247, 0.247, 0.247 },
    { 0.251, 0.251, 0.251 },
    { 0.255, 0.255, 0.255 },
    { 0.259, 0.259, 0.259 },
    { 0.263, 0.263, 0.263 },
    { 0.267, 0.267, 0.267 },
    { 0.271, 0.271, 0.271 },
    { 0.275, 0.275, 0.275 },
    { 0.278, 0.278, 0.278 },
    { 0.282, 0.282, 0.282 },
    { 0.286, 0.286, 0.286 },
    { 0.290, 0.290, 0.290 },
    { 0.294, 0.294, 0.294 },
    { 0.298, 0.298, 0.298 },
    { 0.302, 0.302, 0.302 },
    { 0.306, 0.306, 0.306 },
    { 0.310, 0.310, 0.310 },
    { 0.314, 0.314, 0.314 },
    { 0.318, 0.318, 0.318 },
    { 0.322, 0.322, 0.322 },
    { 0.325, 0.325, 0.325 },
    { 0.329, 0.329, 0.329 },
    { 0.333, 0.333, 0.333 },
    { 0.337, 0.337, 0.337 },
    { 0.341, 0.341, 0.341 },
    { 0.345, 0.345, 0.345 },
    { 0.349, 0.349, 0.349 },
    { 0.353, 0.353, 0.353 },
    { 0.357, 0.357, 0.357 },
    { 0.361, 0.361, 0.361 },
    { 0.365, 0.365, 0.365 },
    { 0.369, 0.369, 0.369 },
    { 0.373, 0.373, 0.373 },
    { 0.376, 0.376, 0.376 },
    { 0.380, 0.380, 0.380 },
    { 0.384, 0.384, 0.384 },
    { 0.388, 0.388, 0.388 },
    { 0.392, 0.392, 0.392 },
    { 0.396, 0.396, 0.396 },
    { 0.400, 0.400, 0.400 },
    { 0.404, 0.404, 0.404 },
    { 0.408, 0.408, 0.408 },
    { 0.412, 0.412, 0.412 },
    { 0.416, 0.416, 0.416 },
    { 0.420, 0.420, 0.420 },
    { 0.424, 0.424, 0.424 },
    { 0.427, 0.427, 0.427 },
    { 0.431, 0.431, 0.431 },
    { 0.435, 0.435, 0.435 },
    { 0.439, 0.439, 0.439 },
    { 0.443, 0.443, 0.443 },
    { 0.447, 0.447, 0.447 },
    { 0.451, 0.451, 0.451 },
    { 0.455, 0.455, 0.455 },
    { 0.459, 0.459, 0.459 },
    { 0.463, 0.463, 0.463 },
    { 0.467, 0.467, 0.467 },
    { 0.471, 0.471, 0.471 },
    { 0.475, 0.475, 0.475 },
    { 0.478, 0.478, 0.478 },
    { 0.482, 0.482, 0.482 },
    { 0.486, 0.486, 0.486 },
    { 0.490, 0.490, 0.490 },
    { 0.494, 0.494, 0.494 },
    { 0.498, 0.498, 0.498 },
    { 0.502, 0.502, 0.502 },
    { 0.506, 0.506, 0.506 },
    { 0.510, 0.510, 0.510 },
    { 0.514, 0.514, 0.514 },
    { 0.518, 0.518, 0.518 },
    { 0.522, 0.522, 0.522 },
    { 0.525, 0.525, 0.525 },
    { 0.529, 0.529, 0.529 },
    { 0.533, 0.533, 0.533 },
    { 0.537, 0.537, 0.537 },
    { 0.541, 0.541, 0.541 },
    { 0.545, 0.545, 0.545 },
    { 0.549, 0.549, 0.549 },
    { 0.553, 0.553, 0.553 },
    { 0.557, 0.557, 0.557 },
    { 0.561, 0.561, 0.561 },
    { 0.565, 0.565, 0.565 },
    { 0.569, 0.569, 0.569 },
    { 0.573, 0.573, 0.573 },
    { 0.576, 0.576, 0.576 },
    { 0.580, 0.580, 0.580 },
    { 0.584, 0.584, 0.584 },
    { 0.588, 0.588, 0.588 },
    { 0.592, 0.592, 0.592 },
    { 0.596, 0.596, 0.596 },
    { 0.600, 0.600, 0.600 },
    { 0.604, 0.604, 0.604 },
    { 0.608, 0.608, 0.608 },
    { 0.612, 0.612, 0.612 },
    { 0.616, 0.616, 0.616 },
    { 0.620, 0.620, 0.620 },
    { 0.624, 0.624, 0.624 },
    { 0.627, 0.627, 0.627 },
    { 0.631, 0.631, 0.631 },
    { 0.635, 0.635, 0.635 },
    { 0.639, 0.639, 0.639 },
    { 0.643, 0.643, 0.643 },
    { 0.647, 0.647, 0.647 },
    { 0.651, 0.651, 0.651 },
    { 0.655, 0.655, 0.655 },
    { 0.659, 0.659, 0.659 },
    { 0.663, 0.663, 0.663 },
    { 0.667, 0.667, 0.667 },
    { 0.671, 0.671, 0.671 },
    { 0.675, 0.675, 0.675 },
    { 0.678, 0.678, 0.678 },
    { 0.682, 0.682, 0.682 },
    { 0.686, 0.686, 0.686 },
    { 0.690, 0.690, 0.690 },
    { 0.694, 0.694, 0.694 },
    { 0.698, 0.698, 0.698 },
    { 0.702, 0.702, 0.702 },
    { 0.706, 0.706, 0.706 },
    { 0.710, 0.710, 0.710 },
    { 0.714, 0.714, 0.714 },
    { 0.718, 0.718, 0.718 },
    { 0.722, 0.722, 0.722 },
    { 0.725, 0.725, 0.725 },
    { 0.729, 0.729, 0.729 },
    { 0.733, 0.733, 0.733 },
    { 0.737, 0.737, 0.737 },
    { 0.741, 0.741, 0.741 },
    { 0.745, 0.745, 0.745 },
    { 0.749, 0.749, 0.749 },
    { 0.753, 0.753, 0.753 },
    { 0.757, 0.757, 0.757 },
    { 0.761, 0.761, 0.761 },
    { 0.765, 0.765, 0.765 },
    { 0.769, 0.769, 0.769 },
    { 0.773, 0.773, 0.773 },
    { 0.776, 0.776, 0.776 },
    { 0.780, 0.780, 0.780 },
    { 0.784, 0.784, 0.784 },
    { 0.788, 0.788, 0.788 },
    { 0.792, 0.792, 0.792 },
    { 0.796, 0.796, 0.796 },
    { 0.800, 0.800, 0.800 },
    { 0.804, 0.804, 0.804 },
    { 0.808, 0.808, 0.808 },
    { 0.812, 0.812, 0.812 },
    { 0.816, 0.816, 0.816 },
    { 0.820, 0.820, 0.820 },
    { 0.824, 0.824, 0.824 },
    { 0.827, 0.827, 0.827 },
    { 0.831, 0.831, 0.831 },
    { 0.835, 0.835, 0.835 },
    { 0.839, 0.839, 0.839 },
    { 0.843, 0.843, 0.843 },
    { 0.847, 0.847, 0.847 },
    { 0.851, 0.851, 0.851 },
    { 0.855, 0.855, 0.855 },
    { 0.859, 0.859, 0.859 },
    { 0.863, 0.863, 0.863 },
    { 0.867, 0.867, 0.867 },
    { 0.871, 0.871, 0.871 },
    { 0.875, 0.875, 0.875 },
    { 0.878, 0.878, 0.878 },
    { 0.882, 0.882, 0.882 },
    { 0.886, 0.886, 0.886 },
    { 0.890, 0.890, 0.890 },
    { 0.894, 0.894, 0.894 },
    { 0.898, 0.898, 0.898 },
    { 0.902, 0.902, 0.902 },
    { 0.906, 0.906, 0.906 },
    { 0.910, 0.910, 0.910 },
    { 0.914, 0.914, 0.914 },
    { 0.918, 0.918, 0.918 },
    { 0.922, 0.922, 0.922 },
    { 0.925, 0.925, 0.925 },
    { 0.929, 0.929, 0.929 },
    { 0.933, 0.933, 0.933 },
    { 0.937, 0.937, 0.937 },
    { 0.941, 0.941, 0.941 },
    { 0.945, 0.945, 0.945 },
    { 0.949, 0.949, 0.949 },
    { 0.953, 0.953, 0.953 },
    { 0.957, 0.957, 0.957 },
    { 0.961, 0.961, 0.961 },
    { 0.965, 0.965, 0.965 },
    { 0.969, 0.969, 0.969 },
    { 0.973, 0.973, 0.973 },
    { 0.976, 0.976, 0.976 },
    { 0.980, 0.980, 0.980 },
    { 0.984, 0.984, 0.984 },
    { 0.988, 0.988, 0.988 },
    { 0.992, 0.992, 0.992 },
    { 0.996, 0.996, 0.996 },
    { 1.000, 1.000, 1.000 }
  };


static gkg::FloatColorIntensity red_temperature[ 256 ][ 3 ] =
  {
    { 0.000, 0.000, 0.000 },
    { 0.004, 0.000, 0.000 },
    { 0.008, 0.000, 0.000 },
    { 0.016, 0.000, 0.000 },
    { 0.020, 0.000, 0.000 },
    { 0.027, 0.000, 0.000 },
    { 0.031, 0.000, 0.000 },
    { 0.039, 0.000, 0.000 },
    { 0.043, 0.000, 0.000 },
    { 0.051, 0.000, 0.000 },
    { 0.055, 0.000, 0.000 },
    { 0.059, 0.000, 0.000 },
    { 0.067, 0.000, 0.000 },
    { 0.071, 0.000, 0.000 },
    { 0.078, 0.000, 0.000 },
    { 0.082, 0.000, 0.000 },
    { 0.090, 0.000, 0.000 },
    { 0.094, 0.000, 0.000 },
    { 0.102, 0.000, 0.000 },
    { 0.106, 0.000, 0.000 },
    { 0.110, 0.000, 0.000 },
    { 0.118, 0.000, 0.000 },
    { 0.122, 0.000, 0.000 },
    { 0.129, 0.000, 0.000 },
    { 0.133, 0.000, 0.000 },
    { 0.141, 0.000, 0.000 },
    { 0.145, 0.000, 0.000 },
    { 0.153, 0.000, 0.000 },
    { 0.157, 0.000, 0.000 },
    { 0.165, 0.000, 0.000 },
    { 0.169, 0.000, 0.000 },
    { 0.173, 0.000, 0.000 },
    { 0.180, 0.000, 0.000 },
    { 0.184, 0.000, 0.000 },
    { 0.192, 0.000, 0.000 },
    { 0.196, 0.000, 0.000 },
    { 0.204, 0.000, 0.000 },
    { 0.208, 0.000, 0.000 },
    { 0.216, 0.000, 0.000 },
    { 0.220, 0.000, 0.000 },
    { 0.224, 0.000, 0.000 },
    { 0.231, 0.000, 0.000 },
    { 0.235, 0.000, 0.000 },
    { 0.243, 0.000, 0.000 },
    { 0.247, 0.000, 0.000 },
    { 0.255, 0.000, 0.000 },
    { 0.259, 0.000, 0.000 },
    { 0.267, 0.000, 0.000 },
    { 0.271, 0.000, 0.000 },
    { 0.275, 0.000, 0.000 },
    { 0.282, 0.000, 0.000 },
    { 0.286, 0.000, 0.000 },
    { 0.294, 0.000, 0.000 },
    { 0.298, 0.000, 0.000 },
    { 0.306, 0.000, 0.000 },
    { 0.310, 0.000, 0.000 },
    { 0.318, 0.000, 0.000 },
    { 0.322, 0.000, 0.000 },
    { 0.329, 0.000, 0.000 },
    { 0.333, 0.000, 0.000 },
    { 0.337, 0.000, 0.000 },
    { 0.345, 0.000, 0.000 },
    { 0.349, 0.000, 0.000 },
    { 0.357, 0.000, 0.000 },
    { 0.361, 0.000, 0.000 },
    { 0.369, 0.000, 0.000 },
    { 0.373, 0.000, 0.000 },
    { 0.380, 0.000, 0.000 },
    { 0.384, 0.000, 0.000 },
    { 0.388, 0.000, 0.000 },
    { 0.396, 0.000, 0.000 },
    { 0.400, 0.000, 0.000 },
    { 0.408, 0.000, 0.000 },
    { 0.412, 0.000, 0.000 },
    { 0.420, 0.000, 0.000 },
    { 0.424, 0.000, 0.000 },
    { 0.431, 0.000, 0.000 },
    { 0.435, 0.000, 0.000 },
    { 0.443, 0.000, 0.000 },
    { 0.447, 0.000, 0.000 },
    { 0.451, 0.000, 0.000 },
    { 0.459, 0.000, 0.000 },
    { 0.463, 0.000, 0.000 },
    { 0.471, 0.000, 0.000 },
    { 0.475, 0.000, 0.000 },
    { 0.482, 0.000, 0.000 },
    { 0.486, 0.000, 0.000 },
    { 0.494, 0.000, 0.000 },
    { 0.498, 0.000, 0.000 },
    { 0.502, 0.000, 0.000 },
    { 0.510, 0.000, 0.000 },
    { 0.514, 0.000, 0.000 },
    { 0.522, 0.000, 0.000 },
    { 0.525, 0.000, 0.000 },
    { 0.533, 0.000, 0.000 },
    { 0.537, 0.000, 0.000 },
    { 0.545, 0.000, 0.000 },
    { 0.549, 0.000, 0.000 },
    { 0.553, 0.000, 0.000 },
    { 0.561, 0.000, 0.000 },
    { 0.565, 0.000, 0.000 },
    { 0.573, 0.000, 0.000 },
    { 0.576, 0.000, 0.000 },
    { 0.584, 0.000, 0.000 },
    { 0.588, 0.000, 0.000 },
    { 0.596, 0.000, 0.000 },
    { 0.600, 0.000, 0.000 },
    { 0.608, 0.000, 0.000 },
    { 0.612, 0.000, 0.000 },
    { 0.616, 0.000, 0.000 },
    { 0.624, 0.000, 0.000 },
    { 0.627, 0.000, 0.000 },
    { 0.635, 0.000, 0.000 },
    { 0.639, 0.000, 0.000 },
    { 0.647, 0.000, 0.000 },
    { 0.651, 0.000, 0.000 },
    { 0.659, 0.000, 0.000 },
    { 0.663, 0.000, 0.000 },
    { 0.667, 0.000, 0.000 },
    { 0.675, 0.000, 0.000 },
    { 0.678, 0.000, 0.000 },
    { 0.686, 0.004, 0.000 },
    { 0.690, 0.012, 0.000 },
    { 0.698, 0.020, 0.000 },
    { 0.702, 0.027, 0.000 },
    { 0.710, 0.035, 0.000 },
    { 0.714, 0.043, 0.000 },
    { 0.722, 0.051, 0.000 },
    { 0.725, 0.059, 0.000 },
    { 0.729, 0.067, 0.000 },
    { 0.737, 0.071, 0.000 },
    { 0.741, 0.078, 0.000 },
    { 0.749, 0.086, 0.000 },
    { 0.753, 0.094, 0.000 },
    { 0.761, 0.102, 0.000 },
    { 0.765, 0.110, 0.000 },
    { 0.773, 0.118, 0.000 },
    { 0.776, 0.125, 0.000 },
    { 0.780, 0.133, 0.000 },
    { 0.788, 0.137, 0.000 },
    { 0.792, 0.145, 0.000 },
    { 0.800, 0.153, 0.000 },
    { 0.804, 0.161, 0.000 },
    { 0.812, 0.169, 0.000 },
    { 0.816, 0.176, 0.000 },
    { 0.824, 0.184, 0.000 },
    { 0.827, 0.192, 0.000 },
    { 0.831, 0.200, 0.000 },
    { 0.839, 0.204, 0.000 },
    { 0.843, 0.212, 0.000 },
    { 0.851, 0.220, 0.000 },
    { 0.855, 0.227, 0.000 },
    { 0.863, 0.235, 0.000 },
    { 0.867, 0.243, 0.000 },
    { 0.875, 0.251, 0.000 },
    { 0.878, 0.259, 0.000 },
    { 0.886, 0.267, 0.000 },
    { 0.890, 0.271, 0.000 },
    { 0.894, 0.278, 0.000 },
    { 0.902, 0.286, 0.000 },
    { 0.906, 0.294, 0.000 },
    { 0.914, 0.302, 0.000 },
    { 0.918, 0.310, 0.000 },
    { 0.925, 0.318, 0.000 },
    { 0.929, 0.325, 0.000 },
    { 0.937, 0.333, 0.000 },
    { 0.941, 0.337, 0.000 },
    { 0.945, 0.345, 0.000 },
    { 0.953, 0.353, 0.000 },
    { 0.957, 0.361, 0.000 },
    { 0.965, 0.369, 0.000 },
    { 0.969, 0.376, 0.000 },
    { 0.976, 0.384, 0.000 },
    { 0.980, 0.392, 0.000 },
    { 0.988, 0.400, 0.000 },
    { 0.992, 0.404, 0.000 },
    { 1.000, 0.412, 0.000 },
    { 1.000, 0.420, 0.000 },
    { 1.000, 0.427, 0.000 },
    { 1.000, 0.435, 0.000 },
    { 1.000, 0.443, 0.000 },
    { 1.000, 0.451, 0.000 },
    { 1.000, 0.459, 0.000 },
    { 1.000, 0.467, 0.000 },
    { 1.000, 0.471, 0.000 },
    { 1.000, 0.478, 0.000 },
    { 1.000, 0.486, 0.000 },
    { 1.000, 0.494, 0.000 },
    { 1.000, 0.502, 0.000 },
    { 1.000, 0.510, 0.000 },
    { 1.000, 0.518, 0.000 },
    { 1.000, 0.525, 0.012 },
    { 1.000, 0.533, 0.027 },
    { 1.000, 0.537, 0.043 },
    { 1.000, 0.545, 0.059 },
    { 1.000, 0.553, 0.075 },
    { 1.000, 0.561, 0.090 },
    { 1.000, 0.569, 0.106 },
    { 1.000, 0.576, 0.122 },
    { 1.000, 0.584, 0.137 },
    { 1.000, 0.592, 0.153 },
    { 1.000, 0.600, 0.169 },
    { 1.000, 0.604, 0.184 },
    { 1.000, 0.612, 0.200 },
    { 1.000, 0.620, 0.212 },
    { 1.000, 0.627, 0.227 },
    { 1.000, 0.635, 0.243 },
    { 1.000, 0.643, 0.259 },
    { 1.000, 0.651, 0.275 },
    { 1.000, 0.659, 0.290 },
    { 1.000, 0.667, 0.306 },
    { 1.000, 0.671, 0.322 },
    { 1.000, 0.678, 0.337 },
    { 1.000, 0.686, 0.353 },
    { 1.000, 0.694, 0.369 },
    { 1.000, 0.702, 0.384 },
    { 1.000, 0.710, 0.400 },
    { 1.000, 0.718, 0.412 },
    { 1.000, 0.725, 0.427 },
    { 1.000, 0.733, 0.443 },
    { 1.000, 0.737, 0.459 },
    { 1.000, 0.745, 0.475 },
    { 1.000, 0.753, 0.490 },
    { 1.000, 0.761, 0.506 },
    { 1.000, 0.769, 0.522 },
    { 1.000, 0.776, 0.537 },
    { 1.000, 0.784, 0.553 },
    { 1.000, 0.792, 0.569 },
    { 1.000, 0.800, 0.584 },
    { 1.000, 0.804, 0.600 },
    { 1.000, 0.812, 0.612 },
    { 1.000, 0.820, 0.627 },
    { 1.000, 0.827, 0.643 },
    { 1.000, 0.835, 0.659 },
    { 1.000, 0.843, 0.675 },
    { 1.000, 0.851, 0.690 },
    { 1.000, 0.859, 0.706 },
    { 1.000, 0.867, 0.722 },
    { 1.000, 0.871, 0.737 },
    { 1.000, 0.878, 0.753 },
    { 1.000, 0.886, 0.769 },
    { 1.000, 0.894, 0.784 },
    { 1.000, 0.902, 0.800 },
    { 1.000, 0.910, 0.812 },
    { 1.000, 0.918, 0.827 },
    { 1.000, 0.925, 0.843 },
    { 1.000, 0.933, 0.859 },
    { 1.000, 0.937, 0.875 },
    { 1.000, 0.945, 0.890 },
    { 1.000, 0.953, 0.906 },
    { 1.000, 0.961, 0.922 },
    { 1.000, 0.969, 0.937 },
    { 1.000, 0.976, 0.953 },
    { 1.000, 0.984, 0.969 },
    { 1.000, 0.992, 0.984 },
    { 1.000, 1.000, 1.000 }
  };


gkg::Viewer2d::Viewer2d()
              : gkg::InputHandler( NULL,
                                   gkg::Session::getInstance().
                                              getDefaultDisplay()->getStyle() )
{

  init();

}


gkg::Viewer2d::Viewer2d( gkg::Style* style )
              : gkg::InputHandler( NULL, style )
{

  init();

}


void gkg::Viewer2d::init()
{

  try
  {

  _font = 
     new gkg::Font( "-*-courier-medium-r-normal--*-120-*-*-*-iso8859-*" );
  if ( _font )
  {

    _font->addReference();

  }
  _annotationColor = new gkg::Color( 1.0, 0.0, 0.0, 1.0f );
  if ( _annotationColor )
  {

    _annotationColor->addReference();

  }

  _patch = new gkg::Patch( NULL );
  this->setBody( _patch );

  reset();

  }
  GKG_CATCH( "gkg::Viewer2d::init()" );

}


gkg::Viewer2d::~Viewer2d()
{

  if ( _font )
  {

    _font->removeReference();

  }
  if ( _annotationColor )
  {

    _annotationColor->removeReference();

  }

}


void gkg::Viewer2d::draw( 
                     gkg::Canvas* canvas,
                     const gkg::GeometryAllocation& geometryAllocation ) const
{

  try
  {

    this->gkg::InputHandler::draw( canvas, geometryAllocation );

    //canvas->pushTransform();
    //gkg::Transformer identity;
    //canvas->setTransformer( identity );

    char tmp[ 64 ];

    sprintf( tmp, "window Avg %g/%g",
             _lowerThresholdAverage + _deltaLowerThresholdAverage,
             _upperThresholdAverage + _deltaUpperThresholdAverage ) ;
    canvas->string( _font, tmp, _annotationColor, 10, 5 );

    sprintf( tmp, "window Cor %g/%g",
             _lowerThresholdActivation + _deltaLowerThresholdActivation,
             _upperThresholdActivation + _deltaUpperThresholdActivation ) ;
    canvas->string( _font, tmp, _annotationColor, 200, 5 );

    sprintf( tmp, "fusion %03d %%",
             _fusionPercentage + _deltaFusion ) ;
    canvas->string( _font, tmp, _annotationColor, 400, 5 );

    sprintf( tmp, "slice  %03d/%03d",
             _currentSlice + _deltaSlice + 1,
             _volumeAverage.getSizeZ() ) ;
    canvas->string( _font, tmp, _annotationColor, 10, 20 );

    sprintf( tmp, "rank   %03d/%03d",
             _currentRank + _deltaRank + 1,
             _volumeAverage.getSizeT() ) ;
    canvas->string( _font, tmp, _annotationColor, 10, 35 );

    //  canvas->popTransform();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Viewer2d< T >::draw( "
             "gkg::Canvas* canvas, "
             "const gkg::GeometryAllocation& geometryAllocation ) const" );

}


gkg::Volume< int16_t >& gkg::Viewer2d::getAverageVolume()
{

  return _volumeAverage;

}


gkg::Volume< float >& gkg::Viewer2d::getActivationVolume()
{

  return _volumeActivation;

}


void gkg::Viewer2d::reset( bool doRedraw )
{

  try
  {
  
    _state = Viewer2d::Idle;
    _currentSlice = 0;
    _deltaSlice = 0;
    _currentRank = 0;
    _deltaRank = 0;
    _lowerThresholdAverage = 0;
    _upperThresholdAverage = 0;
    _lowerThresholdActivation = 0;
    _upperThresholdActivation = 0;
    _deltaLowerThresholdAverage = 0;
    _deltaUpperThresholdAverage = 0;
    _deltaLowerThresholdActivation = 0;
    _deltaUpperThresholdActivation = 0;
    _fusionPercentage = 70;
    _deltaFusion = 0;
    _first = true;
    _multiVolume = false;
    
    _volumeAverage.reallocate();
    _volumeActivation.reallocate();
  
    gkg::Raster* raster = new gkg::Raster( 1, 1 );
    raster->poke( 0, 0, 0.0, 0.0, 0.0, 1.0 );
    gkg::Image* image = new gkg::Image( raster );
    gkg::TransformFitter* transformFitter = new gkg::TransformFitter( image );
    _patch->setBody( transformFitter );
    _patch->reallocate();
    _raster = raster;
    
    if ( doRedraw )
    {
    
      _patch->redraw();
      redraw();
      
    }

  }
  GKG_CATCH( "void gkg::Viewer2d::reset()" );

}


void gkg::Viewer2d::update()
{

  try
  {
  
    _multiVolume = _volumeActivation.hasSameSize( _volumeAverage );
        
    processMinimum();
    processMaximum();
 
    if ( _multiVolume )
    {
    
      _lowerThresholdActivation = _minimumCor;
      _upperThresholdActivation = _maximumCor;
      
    }
 
    if ( _first )
    {
    
      _lowerThresholdAverage = _minimumAvg;
      _upperThresholdAverage = _maximumAvg;
      _currentSlice = _volumeAverage.getSizeZ() / 2;
      gkg::Raster* raster = 
        new gkg::Raster( ( gkg::PixelCoordinate )_volumeAverage.getSizeX(),
                         ( gkg::PixelCoordinate )_volumeAverage.getSizeY() );
      gkg::Image* image = new gkg::Image( raster );
      gkg::TransformFitter* transformFitter = new gkg::TransformFitter( image );
      _patch->setBody( transformFitter );
      _patch->reallocate();
      _raster = raster;
      _first = false;
    
    }

    fillRaster( _currentSlice, _currentRank );    
    _patch->redraw();
    redraw();
 
  }
  GKG_CATCH( "template < class T > "
             "void gkg::Viewer2d< T >::update( "
             "const gkg::Volume< T >& volume )" );

}


void gkg::Viewer2d::press( const gkg::Event& event )
{

  try
  {

    _x0 = event.getPointerX();
    _y0 = event.getPointerY();

    if ( _x0 > 32 && _x0 < 80 && _y0 > 20 && _y0 < 30 )
    {

      _state = Viewer2d::SliceModification;
      //std::cout << "entering SliceModification state" << std::endl;

    }
    else if ( _x0 > 32 && _x0 < 80 && _y0 > 35 && _y0 < 45 )
    {

      _state = Viewer2d::RankModification;
      //std::cout << "entering RankModification state" << std::endl;

    }
    else if ( _x0 > 400 && _x0 < 500 && _y0 > 5 && _y0 < 15 )
    {

      _state = Viewer2d::FusionModification;
      //std::cout << "entering Windowing state" << std::endl;

    }
    else 
    {
    
      if ( event.controlIsDown() ||
           ( _x0 > 200 && _x0 < 390 && _y0 > 5 && _y0 < 15 ) )
      {

        _state = Viewer2d::WindowingActivation;
        //std::cout << "entering Windowing state" << std::endl;

      }
      else
      {

        _state = Viewer2d::WindowingAverage;
        //std::cout << "entering Windowing state" << std::endl;

      }

    }

    this->gkg::InputHandler::press( event );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Viewer2d< T >::press( const gkg::Event& event )" );

}


void gkg::Viewer2d::drag( const gkg::Event& event )
{

  try
  {

    _x2 = event.getPointerX();
    _y2 = event.getPointerY();

    if ( _state == Viewer2d::SliceModification )
    {

      _deltaSlice = ( int32_t )( ( _x2 - _x0 ) / 20.0 );

      if ( _currentSlice + _deltaSlice > _volumeAverage.getSizeZ() - 1 )
      {

        _deltaSlice = _volumeAverage.getSizeZ() - 1 - _currentSlice;

      }
      else if ( _currentSlice + _deltaSlice < 0 )
      {

        _deltaSlice = -_currentSlice;

      }
      fillRaster( _currentSlice + _deltaSlice, _currentRank );

    }
    else if ( _state == Viewer2d::RankModification )
    {

      _deltaRank = ( int32_t )( ( _x2 - _x0 ) / 20.0 );

      if ( _currentRank + _deltaRank > _volumeAverage.getSizeT() - 1 )
      {

        _deltaRank = _volumeAverage.getSizeT() - 1 - _currentRank;

      }
      else if ( _currentRank + _deltaRank < 0 )
      {

        _deltaRank = -_currentRank;

      }
      fillRaster( _currentSlice, _currentRank + _deltaRank );

    }
    else if ( _state == Viewer2d::WindowingAverage )
    {

      double oldLowerThreshold = _lowerThresholdAverage;
      double oldUpperThreshold = _upperThresholdAverage;

      _deltaLowerThresholdAverage = _x2 - _x0;
      _deltaUpperThresholdAverage = _y2 - _y0;

      double newLowerThreshold = getNewThreshold( oldLowerThreshold,
                                                  _deltaLowerThresholdAverage );
      double newUpperThreshold = getNewThreshold( oldUpperThreshold,
                                                  _deltaUpperThresholdAverage );

      if ( newLowerThreshold < newUpperThreshold )
      {

        _lowerThresholdAverage = newLowerThreshold;
        _upperThresholdAverage = newUpperThreshold;

      }
      fillRaster( _currentSlice, _currentRank );
      if ( newLowerThreshold < newUpperThreshold )
      {

        _lowerThresholdAverage = oldLowerThreshold;
        _upperThresholdAverage = oldUpperThreshold;

      }

    }
    else if ( _multiVolume && ( _state == Viewer2d::WindowingActivation ) )
    {

      double oldLowerThreshold = _lowerThresholdActivation;
      double oldUpperThreshold = _upperThresholdActivation;

      _deltaLowerThresholdActivation = _x2 - _x0;
      _deltaUpperThresholdActivation = _y2 - _y0;

      double newLowerThreshold = getNewThreshold( 
                                               oldLowerThreshold,
                                               _deltaLowerThresholdActivation );
      double newUpperThreshold = getNewThreshold( 
                                               oldUpperThreshold,
                                               _deltaUpperThresholdActivation );

      if ( newLowerThreshold < newUpperThreshold )
      {

        _lowerThresholdActivation = newLowerThreshold;
        _upperThresholdActivation = newUpperThreshold;

      }
      fillRaster( _currentSlice, _currentRank );
      if ( newLowerThreshold < newUpperThreshold )
      {

        _lowerThresholdActivation = oldLowerThreshold;
        _upperThresholdActivation = oldUpperThreshold;

      }

    }
    else if ( _state == Viewer2d::FusionModification )
    {

      int32_t oldFusion = _fusionPercentage;
      
      _deltaFusion = ( int32_t )( ( _x2 - _x0 ) / 20.0 );

      if ( _fusionPercentage + _deltaFusion > 100 )
      {

        _deltaFusion = 100 - 1 - _fusionPercentage;

      }
      else if ( _fusionPercentage + _deltaFusion < 0 )
      {

        _deltaFusion = -_fusionPercentage;

      }
      
      _fusionPercentage += _deltaFusion;
      fillRaster( _currentSlice, _currentRank );
      _fusionPercentage = oldFusion;

    }
    _patch->redraw();
    redraw();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Viewer2d< T >::drag( const gkg::Event& event )" );

}


void gkg::Viewer2d::release( const gkg::Event& event )
{

  try
  {

    _x2 = event.getPointerX();
    _y2 = event.getPointerY();

    if ( _state == Viewer2d::SliceModification )
    {

      if ( event.getPointerButton() == gkg::Event::WheelDown )
      {

        _deltaSlice = -1;

        if ( _currentSlice + _deltaSlice > _volumeAverage.getSizeZ() - 1 )
        {

          _deltaSlice = _volumeAverage.getSizeZ() - 1 - _currentSlice;

        }
        else if ( _currentSlice + _deltaSlice < 0 )
        {

          _deltaSlice = -_currentSlice;

        }
        _currentSlice += _deltaSlice;
        _deltaSlice = 0;
        fillRaster( _currentSlice, _currentRank );
        //std::cout << "leaving SliceModification state" << std::endl;

      }
      else if ( event.getPointerButton() == gkg::Event::WheelUp )
      {

        _deltaSlice = +1;

        if ( _currentSlice + _deltaSlice > _volumeAverage.getSizeZ() - 1 )
        {

          _deltaSlice = _volumeAverage.getSizeZ() - 1 - _currentSlice;

        }
        else if ( _currentSlice + _deltaSlice < 0 )
        {

          _deltaSlice = -_currentSlice;

        }
        _currentSlice += _deltaSlice;
        _deltaSlice = 0;
        fillRaster( _currentSlice, _currentRank );
        //std::cout << "leaving SliceModification state" << std::endl;

      }
      else
      {

        _deltaSlice = ( int32_t )( ( _x2 - _x0 ) / 20.0 );

        if ( _currentSlice + _deltaSlice > _volumeAverage.getSizeZ() - 1 )
        {

          _deltaSlice = _volumeAverage.getSizeZ() - 1 - _currentSlice;

        }
        else if ( _currentSlice + _deltaSlice < 0 )
        {

          _deltaSlice = -_currentSlice;

        }
        _currentSlice += _deltaSlice;
        _deltaSlice = 0;
       fillRaster( _currentSlice, _currentRank );
       //std::cout << "leaving SliceModification state" << std::endl;

      }

    }
    else if ( _state == Viewer2d::RankModification )
    {

      if ( event.getPointerButton() == gkg::Event::WheelDown )
      {

        _deltaRank = -1;

        if ( _currentRank + _deltaRank > _volumeAverage.getSizeT() - 1 )
        {

          _deltaRank = _volumeAverage.getSizeT() - 1 - _currentRank;

        }
        else if ( _currentRank + _deltaRank < 0 )
        {

          _deltaRank = -_currentRank;

        }
        _currentRank += _deltaRank;
        _deltaRank = 0;
        fillRaster( _currentSlice, _currentRank );
        //std::cout << "leaving RankModification state" << std::endl;

      }
      else if ( event.getPointerButton() == gkg::Event::WheelUp )
      {

        _deltaRank = +1;

        if ( _currentRank + _deltaRank > _volumeAverage.getSizeT() - 1 )
        {

          _deltaRank = _volumeAverage.getSizeT() - 1 - _currentRank;

        }
        else if ( _currentRank + _deltaRank < 0 )
        {

          _deltaRank = -_currentRank;

        }
        _currentRank += _deltaRank;
        _deltaRank = 0;
        fillRaster( _currentSlice, _currentRank );
        //std::cout << "leaving RankModification state" << std::endl;

      }
      else
      {


        _deltaRank = ( int32_t )( ( _x2 - _x0 ) / 20.0 );

        if ( _currentRank + _deltaRank > _volumeAverage.getSizeT() - 1 )
        {

          _deltaRank = _volumeAverage.getSizeT() - 1 - _currentRank;

        }
        else if ( _currentRank + _deltaRank < 0 )
        {

          _deltaRank = -_currentRank;

        }
        _currentRank += _deltaRank;
        _deltaRank = 0;
        fillRaster( _currentSlice, _currentRank );
        //std::cout << "leaving RankModification state" << std::endl;

      }

    }
    else if ( _state == Viewer2d::WindowingAverage )
    {

      _deltaLowerThresholdAverage = 0;
      _deltaUpperThresholdAverage = 0;

      double newLowerThreshold = getNewThreshold(
                                   _lowerThresholdAverage,
                                   _x2 - _x0 );
      double newUpperThreshold = getNewThreshold(
                                   _upperThresholdAverage,
                                   _y2 - _y0 );

      if ( newLowerThreshold < newUpperThreshold )
      {

        _lowerThresholdAverage = newLowerThreshold;
        _upperThresholdAverage = newUpperThreshold;

      }
      fillRaster( _currentSlice, _currentRank );
      //std::cout << "leaving Windowing state" << std::endl;

    }
    else if ( _multiVolume && ( _state == Viewer2d::WindowingActivation ) )
    {

      _deltaLowerThresholdActivation = 0;
      _deltaUpperThresholdActivation = 0;

      double newLowerThreshold = getNewThreshold(
                                   _lowerThresholdActivation,
                                   _x2 - _x0 );
      double newUpperThreshold = getNewThreshold(
                                   _upperThresholdActivation,
                                   _y2 - _y0 );

      if ( newLowerThreshold < newUpperThreshold )
      {

        _lowerThresholdActivation = newLowerThreshold;
        _upperThresholdActivation = newUpperThreshold;

      }
      fillRaster( _currentSlice, _currentRank );
      //std::cout << "leaving Windowing state" << std::endl;

    }
    else if ( _state == Viewer2d::FusionModification )
    {

      if ( event.getPointerButton() == gkg::Event::WheelDown )
      {

        _deltaFusion = -1;

        if ( _fusionPercentage + _deltaFusion > 100 )
        {

          _deltaFusion = 100 - 1 - _fusionPercentage;

        }
        else if ( _fusionPercentage + _deltaFusion < 0 )
        {

          _deltaFusion = -_fusionPercentage;

        }
        _fusionPercentage += _deltaFusion;
        _deltaFusion = 0;
        fillRaster( _currentSlice, _currentRank );
        //std::cout << "leaving RankModification state" << std::endl;

      }
      else if ( event.getPointerButton() == gkg::Event::WheelUp )
      {

        _deltaFusion = +1;

        if ( _fusionPercentage + _deltaFusion > 100 )
        {

          _deltaFusion = 100 - 1 - _fusionPercentage;

        }
        else if ( _fusionPercentage + _deltaFusion < 0 )
        {

          _deltaFusion = -_fusionPercentage;

        }
        _fusionPercentage += _deltaFusion;
        _deltaFusion = 0;
        fillRaster( _currentSlice, _currentRank );
        //std::cout << "leaving RankModification state" << std::endl;

      }
      else
      {


        _deltaFusion = ( int32_t )( ( _x2 - _x0 ) / 20.0 );

        if ( _fusionPercentage + _deltaFusion > 100 )
        {

          _deltaFusion = 100 - 1 - _fusionPercentage;

        }
        else if ( _fusionPercentage + _deltaFusion < 0 )
        {

          _deltaFusion = -_fusionPercentage;

        }
        _fusionPercentage += _deltaFusion;
        _deltaFusion = 0;
        fillRaster( _currentSlice, _currentRank );
        //std::cout << "leaving RankModification state" << std::endl;

      }

    }

    _patch->redraw();
    _state = Viewer2d::Idle;
    redraw();
    this->gkg::InputHandler::release( event );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Viewer2d< T >::release( const gkg::Event& event )" );

}


double gkg::Viewer2d::getNewThreshold( double oldValue,
                                       double deltaValue ) const
{

  try
  {

    double a = std::log( 3.0 ) / 150.0;

    if ( ( deltaValue <= 100.0 && deltaValue >= 0.0 ) ||
         ( deltaValue < 0.0 && deltaValue >= -100.0 ) )
    {

      return oldValue + 0.2 * deltaValue;

    }

    if ( deltaValue < -100.0 )
    {

      if ( oldValue <= 20.0 )
      {

        return oldValue - 20.0 + 10.0 * ( deltaValue + 100.0 );

      }
      return std::exp( a * ( deltaValue + 100.0 ) +
                       std::log( oldValue - 20.0 ) );

    }

    if ( deltaValue > 100.0 )
    {

      if ( oldValue <= -20.0 )
      {

        return oldValue + 20.0 + 10.0 * ( deltaValue - 100.0 );

      }
      return std::exp( a * ( deltaValue - 100.0 ) +
                       std::log( oldValue + 20.0 ) );

    }

    return oldValue;

  }
  GKG_CATCH( "template < class T > "
             "double gkg::Viewer2d< T >::getNewThreshold( double oldValue, "
             "double deltaValue ) const" );

}


void gkg::Viewer2d::fillRaster( int32_t slice, int32_t rank )
{

  try
  {

    gkg::PixelCoordinate pixelWidth = 
      ( gkg::PixelCoordinate )_volumeAverage.getSizeX();
    gkg::PixelCoordinate pixelHeight = 
      ( gkg::PixelCoordinate )_volumeAverage.getSizeY();

    gkg::PixelCoordinate x, y;
    gkg::FloatColorIntensity r_avg = 0.0;
    gkg::FloatColorIntensity g_avg = 0.0;
    gkg::FloatColorIntensity b_avg = 0.0;
    gkg::FloatColorIntensity a_avg = 1.0;
    gkg::FloatColorIntensity r_cor = 0.0;
    gkg::FloatColorIntensity g_cor = 0.0;
    gkg::FloatColorIntensity b_cor = 0.0;
    gkg::FloatColorIntensity a_cor = 1.0;
    gkg::FloatColorIntensity a1 = 
      (gkg::FloatColorIntensity)_fusionPercentage / 100.0;
    gkg::FloatColorIntensity a2 = 1.0 - a1;
    
    for ( y = 0; y < pixelHeight; y++ )
    {

      for ( x = 0; x < pixelWidth; x++ )
      {

        threshold( (float)_volumeAverage( x, y, slice, rank ), 
	           r_avg, g_avg, b_avg, a_avg,
		   _lowerThresholdAverage, _upperThresholdAverage,
		   gray_level );
		   
        if ( _multiVolume )
	{
	
          threshold( _volumeActivation( x, y, slice, rank ), 
	             r_cor, g_cor, b_cor, a_cor,
		     _lowerThresholdActivation, _upperThresholdActivation,
		     red_temperature );
		     
	  r_avg = a1 * r_avg + a2 * r_cor;
	  g_avg = a1 * g_avg + a2 * g_cor;
	  b_avg = a1 * b_avg + a2 * b_cor;
	  a_avg = a1 * a_avg + a2 * a_cor;
		     
	}
		     
        _raster->poke( x, pixelHeight - y - 1, r_avg, g_avg, b_avg, a_avg );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VolumeModel< T >::fillRaster( "
             "int32_t slice, int32_t rank )" );

}


void gkg::Viewer2d::threshold( const float& inputValue,
                               gkg::FloatColorIntensity& r,
                               gkg::FloatColorIntensity& g,
                               gkg::FloatColorIntensity& b,
                               gkg::FloatColorIntensity& a,
		   	       double lowerThreshold,
		 	       double upperThreshold,
			       gkg::FloatColorIntensity pal[ 256 ][ 3 ] ) const
{

  try
  {
    int32_t idx;

    if ( inputValue < lowerThreshold )
    {

      idx = 0;

    }
    else if ( inputValue > upperThreshold )
    {

      idx = 255;

    }
    else
    {

      if ( gkg::equal( upperThreshold, lowerThreshold, EPSILON ) )
      {

        idx = 0;

      }
      else
      {

        idx = ( int32_t )( 255.0 *
                           ( double )( inputValue - lowerThreshold ) /
                           ( double )( upperThreshold - lowerThreshold ) );
		       
      }

    }

    r = pal[ idx ][ 0 ];
    g = pal[ idx ][ 1 ];
    b = pal[ idx ][ 2 ];
    a = 1.0;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VolumeModel< T >::threshold( const T& inputValue, "
             "gkg::FloatColorIntensity& r, "
             "gkg::FloatColorIntensity& g, "
             "gkg::FloatColorIntensity& b, "
             "gkg::FloatColorIntensity& a ) const" );

}


void gkg::Viewer2d::processMinimum()
{

  try
  {

    gkg::MinimumFilter< gkg::Volume< int16_t >, double > minimumFilterAvg;
    minimumFilterAvg.filter( _volumeAverage, _minimumAvg );
    
    if ( _multiVolume )
    {
    
      gkg::MinimumFilter< gkg::Volume< float >, double > minimumFilterCor;
      minimumFilterCor.filter( _volumeActivation, _minimumCor );
      
    }

  }
  GKG_CATCH( "void gkg::VolumeModel< T >::processMinimum()" );

}


void gkg::Viewer2d::processMaximum()
{

  try
  {

    gkg::MaximumFilter< gkg::Volume< int16_t >, double > maximumFilterAvg;
    maximumFilterAvg.filter( _volumeAverage, _maximumAvg );
    
    if ( _multiVolume )
    {
    
      gkg::MaximumFilter< gkg::Volume< float >, double > maximumFilterCor;
      maximumFilterCor.filter( _volumeActivation, _maximumCor );
      
    }

  }
  GKG_CATCH( "void gkg::VolumeModel< T >::processMaximum()" );

}
