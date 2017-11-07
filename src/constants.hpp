#ifndef RAYTRACER_CONSTANTS_HPP
#define RAYTRACER_CONSTANTS_HPP

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

static fs::path scenes_dir = fs::path("..") / fs::path("scenes");
static fs::path models_dir = fs::path("..") / fs::path("models");
static fs::path renders_dir = fs::path("..") / fs::path("renders");

#endif //RAYTRACER_CONSTANTS_HPP
