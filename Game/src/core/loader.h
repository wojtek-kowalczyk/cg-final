#pragma once

#include "object.h"

#include <string>
#include <memory>

Object loadModel(std::string path, const std::shared_ptr<xMaterial>& overrideMaterial = nullptr);
