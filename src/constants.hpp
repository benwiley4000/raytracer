#ifndef RAYTRACER_CONSTANTS_HPP
#define RAYTRACER_CONSTANTS_HPP

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

static fs::path scenes_dir = fs::path("..") / fs::path("scenes");
static fs::path models_dir = fs::path("..") / fs::path("models");
static fs::path renders_dir = fs::path("..") / fs::path("renders");

static const float t_threshold = 0.04f;

#endif //RAYTRACER_CONSTANTS_HPP
