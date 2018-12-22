#ifndef FINAL_MODULE_SEGMENTER_INCLUDE_API_SEGMENTER_CORE_HPP_INCLUDED
#define FINAL_MODULE_SEGMENTER_INCLUDE_API_SEGMENTER_CORE_HPP_INCLUDED
#include <array>
#include <vector>
#include <tuple>
#include <cstdint>
#include <utility>

namespace api {
namespace segmenter {

// image definitions using standard libraries

using std_point_type = std::pair<int,int>;
using std_image_size_type = std::pair<int,int>;
using std_image_data_type = std::vector<std::array<std::uint8_t,4>>;
using std_image_float_data_type = std::vector<float>;
using std_image_type = std::tuple<std_image_size_type, std_image_data_type>;
using std_image_float_type = std::tuple<std_image_size_type, std_image_float_data_type>;
using std_keypoints_type = std::vector<std::vector<std::pair<int,int>>>;

} // namespace segmenter
} // namespace api

#endif // FINAL_MODULE_SEGMENTER_INCLUDE_API_SEGMENTER_CORE_HPP_INCLUDED
